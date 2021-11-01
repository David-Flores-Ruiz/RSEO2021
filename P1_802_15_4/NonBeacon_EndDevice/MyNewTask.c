/*
 * MyNewTask.c
 *
 *  Created on: 26 oct 2021
 *      Author: 52354
 */

//a. Include “MyNewTask.h”
#include "MyNewTask.h"


//b. Declare required variables and prototypes
osaEventId_t mMyEvents;
osaEventId_t mMyEvents1;		// EQ2
/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;
tmrTimerID_t myTimerID1 = gTmrInvalidTimerID_c;
/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;
osaTaskId_t gMyTaskHandler_ID1;
/* Local variable to store the current state of the LEDs */
static uint8_t ledsState = 0;
static uint8_t g_Counter = 3;	// EQ2
static uint8_t msg[] = { 'C','o','u','n','t','e','r',':','x','\0'};		// EQ2
//static uint8_t msg[] = "Counter: #";
static instanceId_t   macInstance;
static nwkToMcpsMessage_t *mpPacket;	// Data packet for sending UART
static uint8_t mMsduHandle;				// Data packet identifier

static panDescriptor_t mCoordInfo;		// Info to send the packet
static uint16_t assocShortAddress1;			// Short address nodo1
static uint16_t assocShortAddress2;			// Short address nodo2
static uint16_t assocShortAddress3;			// Short address nodo3
static uint16_t assocShortAddress4;			// Short address nodo4
static uint16_t assocShortAddress5;			// Short address nodo5

//f. Define the callback that will be called each time the timer expires
/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

static void myTaskTimerCallback1(void *param)
{
	OSA_EventSet(mMyEvents1, gTaskEvent2_c);
}


//d. Define the new Task function that will handle the events and trigger an Interval Timer.
/* Main custom task */
void My_Task(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();
	while(1)
	{
		OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c,
				&customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}
		/* Depending on the received event */
		switch(customEvent){
		case gMyNewTaskEvent1_c:
			TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
					1000, /* Timer's Timeout */
					myTaskTimerCallback, /* pointer to
myTaskTimerCallback function */
					NULL
			);
			TurnOffLeds(); /* Ensure all LEDs are turned off */
			break;
		case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */
			if(!ledsState) {
				TurnOnLeds();
				ledsState = 1;
			}
			else {
				TurnOffLeds();
				ledsState = 0;
			}
			break;
		case gMyNewTaskEvent3_c: /* Event to stop the timer */
			ledsState = 0;
			TurnOffLeds();
			TMR_StopTimer(myTimerID);
			break;
		default:
			break;
		}
	}
}

void My_Task1(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID1 = TMR_AllocateTimer();
	while(1)
	{
		OSA_EventWait(mMyEvents1, osaEventFlagsAll_c, FALSE, osaWaitForever_c,
				&customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}
		/* Depending on the received event */
		switch(customEvent){
		case gTaskEvent1_c:
			TMR_StartIntervalTimer(myTimerID1, /*myTimerID1*/
					3000, /* Timer's Timeout */
					myTaskTimerCallback1, /* pointer to
myTaskTimerCallback1 function */
					NULL
			);
//			TurnOffLeds(); /* Ensure all LEDs are turned off */
			break;

		case gTaskEvent2_c: /* Event called from myTaskTimerCallback1 */
			g_Counter++;	// EQ2 - Cada 3s incrementa el contador
			if(g_Counter > 3)
			{
				g_Counter = 0;	// Reiniciamos el contador
			}
			TurnOffLeds();
			switch (g_Counter) {
				case 0:
		            Led2On();		// LED ROJO
					break;
				case 1:
					Led3On();		// LED VERDE
					break;
				case 2:
					Led4On();		// LED AZUL
					break;
				case 3:
					TurnOnLeds();	// LED BLANCO
					break;
				default:
					break;
			}
			// EQ2 - Actualizamos el valor del Counter por timer 3s
			msg[8] = '0' + g_Counter;
			// EQ2 - Envia paquete en el aire  de tipo "Counter:x"
			EnviarPaqueteEnElAire();
			break;

		case gTaskEvent3_c: /* Event to stop the timer */
//			TurnOffLeds();
			TMR_StopTimer(myTimerID1);
			break;

		case gTaskEvent4_c: /* Event for SW3 presionado */
			TMR_StopTimer(myTimerID1);	/* STOP Timer 3 seconds */
        	MyTaskTimer_Start1();	/* Reiniciar el timer de 3 segundos */
			TurnOffLeds();
			Led3On();			// LED VERDE
			g_Counter = 1;		// EQ2: SW3 presionado: Counter = 1
			// EQ2 - Actualizamos el valor del Counter por SW3
			msg[8] = '0' + g_Counter;
			// EQ2 - Envia paquete en el aire  de tipo "Counter:x"
			EnviarPaqueteEnElAire();
			break;

		case gTaskEvent5_c: /* Event for SW4 presionado */
			TMR_StopTimer(myTimerID1);	/* STOP Timer 3 seconds */
        	MyTaskTimer_Start1();	/* Reiniciar el timer de 3 segundos */
			TurnOffLeds();
			Led4On();			// LED AZUL
			g_Counter = 2;		// EQ2: SW4 presionado: Counter = 2
			// EQ2 - Actualizamos el valor del Counter por SW4
			msg[8] = '0' + g_Counter;
			// EQ2 - Envia paquete en el aire  de tipo "Counter:x"
			EnviarPaqueteEnElAire();
			break;
		default:
			break;
		}
	}
}


//c. Define OSA Task
/* OSA Task Definition*/
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );
OSA_TASK_DEFINE(My_Task1, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );


//e. Define the function that will be called to send an Event to your new Task.
/* Function to init the task */
void MyTask_Init(void)
{
	mMyEvents = OSA_EventCreate(TRUE);
	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}
void MyTask_Init1(void)
{
	mMyEvents1 = OSA_EventCreate(TRUE);
	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID1 = OSA_TaskCreate(OSA_TASK(My_Task1), NULL);
}

//   Define a function to start the timer
/* Public function to start the timer */
void MyTaskTimer_Start(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent1_c);
}
void MyTaskTimer_Start1(void)
{
	OSA_EventSet(mMyEvents1, gTaskEvent1_c);
}

//g. Define a function to send an event to stop the timer
/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}
void MyTaskTimer_Stop1(void)
{
	OSA_EventSet(mMyEvents1, gTaskEvent3_c);
}

void EnviarPaqueteEnElAire(void)
{
	// EQ2 - Envia paquete en el aire  de tipo "Counter:x"
	if(mpPacket == NULL)
		mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + 1);

	if(mpPacket != NULL)
	{
		mpPacket->msgType = gMcpsDataReq_c;
		mpPacket->msgData.dataReq.pMsdu = (uint8_t*)&msg;	// EQ2 - TX Payload

        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.msduLength = sizeof(msg);

        mCoordInfo.coordAddress = 0xCAFE;	// EQ2 - Cambiar 0xCAFE a 0x0000
        assocShortAddress1 = 0x0001;				// EQ2 - Nodo1
        assocShortAddress2 = 0x0002;				// EQ2 - Nodo2
        assocShortAddress3 = 0x0003;				// EQ2 - Nodo3
        assocShortAddress4 = 0x0004;				// EQ2 - Nodo4
        assocShortAddress5 = 0x0005;				// EQ2 - Nodo5
        mCoordInfo.coordPanId = 0x0202;		// EQ2 - PAN ID 0x0202

        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, &assocShortAddress1, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);

        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
		mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
		mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

        /* Bind to MAC layer */
        macInstance = BindToMAC( (instanceId_t)0 );

        /* Send the Data to the MCPS */
        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);

        /* If the data wasn't send over the air because there are too many pending packets,
        or new data has beed received, try to send it later   */
	}
}

