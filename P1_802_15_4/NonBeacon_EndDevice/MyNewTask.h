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


//b. Define the EVENTs that the new task will support.
/* Define the available Task's Events */
#define gMyNewTaskEvent1_c (1 << 0)
#define gMyNewTaskEvent2_c (1 << 1)
#define gMyNewTaskEvent3_c (1 << 2)


//c. Define a TASK PRIORITY and TASK SIZE for your new task
#define gMyTaskPriority_c 3
#define gMyTaskStackSize_c 400


//d. Prototype definition
void MyTask_Init(void);
void MyTaskTimer_Start(void);
void MyTaskTimer_Stop(void);










#endif /* MYNEWTASK_H_ */
