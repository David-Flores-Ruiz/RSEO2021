/*
 * MyNewTask.h
 *
 *  Created on: 26 oct 2021
 *      Author: 52354
 */

#ifndef MYNEWTASK_H_
#define MYNEWTASK_H_


//a. Include the required dependencies
/* Fwk */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "LED.h"
/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

//b. Define the EVENTs that the new task will support.
/* Define the available Task's Events */
#define gMyNewTaskEvent1_c (1 << 0)
#define gMyNewTaskEvent2_c (1 << 1)
#define gMyNewTaskEvent3_c (1 << 2)

#define gTaskEvent1_c (1 << 0)	// Start timer		// EQ2
#define gTaskEvent2_c (1 << 1)	// Timer 3s
#define gTaskEvent3_c (1 << 2)	// Stop timer
#define gTaskEvent4_c (1 << 3)	// SW3 presionado
#define gTaskEvent5_c (1 << 4)	// SW4 presionado

//c. Define a TASK PRIORITY and TASK SIZE for your new task
#define gMyTaskPriority_c 3
#define gMyTaskStackSize_c 400


//d. Prototype definition
void MyTask_Init(void);
void MyTask_Init1(void);
void MyTaskTimer_Start(void);
void MyTaskTimer_Start1(void);
void MyTaskTimer_Stop(void);
void MyTaskTimer_Stop1(void);









#endif /* MYNEWTASK_H_ */
