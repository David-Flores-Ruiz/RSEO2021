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
static uint8_t g_Counter = 0;	// EQ2
static uint8_t msg[] = { 'C','o','u','n','t','e','r',':','x','\0'};		// EQ2
//static uint8_t msg[] = "Counter: #";
static instanceId_t   macInstance;
static nwkToMcpsMessage_t *mpPacket;	// Data packet for sending UART
static uint8_t mMsduHandle;				// Data packet identifier

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
			TurnOffLeds(); /* Ensure all LEDs are turned off */
			break;

		case gTaskEvent2_c: /* Event called from myTaskTimerCallback1 */
			TurnOffLeds();
			Led1On();		// LED
            TurnOffLeds();
            Led2On();		// LED ROJO
            TurnOffLeds();
            Led3On();		// LED AZUL
            TurnOffLeds();
            Led4On();		// LED VERDE
            TurnOffLeds();
            TurnOnLeds();	// LED BLANCO
			TurnOffLeds();
			switch (g_Counter) {
				case 0:
		            Led2On();		// LED ROJO
					break;
				case 1:
		            Led4On();		// LED VERDE
					break;
				case 2:
		            Led3On();		// LED AZUL
					break;
				case 3:
					TurnOnLeds();	// LED BLANCO
					break;
				default:
					break;
			}
			// EQ2 - Envia paquete en el aire  de tipo "Counter:x"
			msg[8] = '0' + g_Counter;
			mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + 1);

			if(mpPacket != NULL)
			{
				mpPacket->msgType = gMcpsDataReq_c;
				mpPacket->msgData.dataReq.pMsdu = (uint8_t*)&msg;

		        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
		        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
		        mpPacket->msgData.dataReq.msduLength = sizeof(msg);

//		        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, &maMyAddress, 8);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
//
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, (void*)&mDeviceShortAddress, 2);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, (void*)&mShortAddress, 2);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, (void*)&mPanId, 2);
//		        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, (void*)&mPanId, 2);

//				mpPacket->msgData.dataReq.dstAddr = 0x0000;
//				mpPacket->msgData.dataReq.srcAddr = 0x0001;
//				mpPacket->msgData.dataReq.dstPanId = 0x0808;
//				mpPacket->msgData.dataReq.srcPanId = 0x0808;

//				mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
//				mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
//				mpPacket->msgData.dataReq.msduLength = sizeof(msg);

		        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
		        mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;

				mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
				mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

		        /* Bind to MAC layer */
		        macInstance = BindToMAC( (instanceId_t)0 );

		        /* Send the Data to the MCPS */
		        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);

		        /* If the data wasn't send over the air because there are too many pending packets,
		        or new data has beed received, try to send it later   */

//				(void)NWK_MCPS_SapHandler(mpPacket,0);
			}
			g_Counter++;	// EQ2 - Cada 3s incrementa el contador
			if(g_Counter > 3)
			{
				g_Counter = 0;	// Reiniciamos el contador
			}
			break;

		case gTaskEvent3_c: /* Event to stop the timer */
			TurnOffLeds();
			TMR_StopTimer(myTimerID1);
			break;

		case gTaskEvent4_c: /* Event for SW3 presionado */
			TurnOffLeds();
			Led4On();		// LED VERDE
			g_Counter = 1;		// EQ2: SW3 presionado: Counter = 1

        	MyTaskTimer_Stop1();   /* STOP Timer 3 seconds */
        	MyTaskTimer_Start1();	/*Reiniciar el timer de 3 segundos*/


			// Code to send package

			break;
		case gTaskEvent5_c: /* Event for SW4 presionado */
			TurnOffLeds();
			Led3On();		// LED AZUL
			g_Counter = 2;		// EQ2: SW4 presionado: Counter = 2

        	MyTaskTimer_Stop1();   /* STOP Timer 3 seconds */
        	MyTaskTimer_Start1();	/*Reiniciar el timer de 3 segundos*/

			// Code to send package

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



