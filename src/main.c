/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "BlinkLed.h"

	extern uint32_t __FLASH_segment_start__[];
	extern uint32_t __FLASH_segment_end__[];
	extern uint32_t __SRAM_segment_start__[];
	extern uint32_t __SRAM_segment_end__[];
	extern uint32_t __privileged_functions_start__[];
	extern uint32_t __privileged_functions_end__[];
	extern uint32_t __privileged_data_start__[];
	extern uint32_t __privileged_data_end__[];
	extern uint32_t __privileged_functions_actual_end__[];
	extern uint32_t __privileged_data_actual_end__[];

// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// To demonstrate semihosting, display a message on the standard output
// and another message on the standard error.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// On DEBUG, the uptime in seconds is also displayed on the trace device.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


#include "FreeRTOS.h"
#include "task.h"

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ----------------------------------------------------------------------------
void vApplicationIdleHook(void) {

	//++u64IdleTicksCnt;
}

// A required FreeRTOS function.
// ----------------------------------------------------------------------------
void vApplicationMallocFailedHook(void) {
	//configASSERT( 0 );  // Latch on any failure / error.
	printf("Malloc failed ! !!\n");
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, signed char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */
	//taskDISABLE_INTERRUPTS();
	//for( ;; );
	printf("Holy FUCK! STACK OVERFLOW !!\n");

}

/* Declare an array that will be accessed by the task.  The task should only
be able to read from the array, and not write to it. */
uint8_t cReadOnlyArray[ 512 ] __attribute__((aligned(512)));

uint8_t globalss=5;
const volatile uint32_t *pulSystemPeripheralRegister = ( volatile uint32_t * ) 0xe000e014;
void prvOldStylePrivilegedModeTask(void* pvp)
{
	while(1){


		blink_led_on(0);

		vTaskDelay(500);

		blink_led_off(0);

		vTaskDelay(500);
		printf("Test priviliged:%d %d\n",cReadOnlyArray[0]++);
	}
}




void prvOldStyleUserModeTask(void* pvp){

	volatile uint32_t ulReadData;
	while(1){


		//ulReadData = *pulSystemPeripheralRegister;
		//ulReadData = *ordinaryRAMRegister;

		blink_led_on(1);

		vTaskDelay(1000);

		blink_led_off(1);

		vTaskDelay(500);
		printf("Test unpriviliged old style task\n");

	}
}

static const MemoryRegion_t xAltRegions[ portNUM_CONFIGURABLE_REGIONS ] =
{
    /* Base address    Length    Parameters */
    {(uint32_t*) 0x20000000,      (uint32_t)0x3fffffff-0x20000000,     portMPU_REGION_READ_ONLY },
    { 0,               0,        0 },
    { 0,               0,        0 }
};

void xRegTest1Task(void* pvp){
	while(1){
		//printf("hello !\n");
		//printf("%d\n",ordinary_var);

		//ordinary_var = 66;
		blink_led_on(1);

		vTaskDelay(1000);

		blink_led_off(1);

		vTaskDelay(500);


		//vTaskAllocateMPURegions(NULL,xAltRegions);
		//printf("Test priviliged:%d %d\n",cReadOnlyArray[0]++);



	}
}
#if 1
/* GCC specifics. */
#define mainALIGN_TO( x )				__attribute__((aligned(x)))

/* Define the constants used to allocate the reg test task stacks.  Note that
that stack size is defined in words, not bytes. */
#define mainREG_TEST_STACK_SIZE_WORDS	128
#define mainREG_TEST_STACK_ALIGNMENT	( mainREG_TEST_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

static portSTACK_TYPE xRegTest1Stack[ mainREG_TEST_STACK_SIZE_WORDS ] mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT );
static portSTACK_TYPE xRegTest2Stack[ mainREG_TEST_STACK_SIZE_WORDS ] mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT );

/* Fill in a TaskParameters_t structure per reg test task to define the tasks. */
static const TaskParameters_t xRegTest1Parameters =
{
		xRegTest1Task,						/* pvTaskCode - the function that implements the task. */
	"RegTest1",								/* pcName			*/
	mainREG_TEST_STACK_SIZE_WORDS,			/* usStackDepth		*/
	( void * ) 0x12345678,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
	tskIDLE_PRIORITY ,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
	xRegTest1Stack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
	{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
		/* Base address		Length		Parameters */
        { cReadOnlyArray,				512,			portMPU_REGION_READ_WRITE  },
        { 0x00,				0x00,			0x00 },
		{ 0x00,				0x00,			0x00 }

        //TODO: Looks like length should be at least 32 bytes long
	}
};
#endif

int
main(int argc, char* argv[])
{

  blink_led_init();
  
  /**
   * There are basically three ways to create tasks using MPU:
   * 1) You can use old-fashioned xTaskCreate:
   * 	- if you invoke it as usual
   * 	xTaskCreate(	prvOldStyleUserModeTask,	 //The function that implements the task.
					"Task1",					 //Text name for the task.
					256,						 //Stack depth in words.
					NULL,						 //Task parameters.
					3 ,							 //Priority and mode (user in this case).
					NULL						 //Handle.
				);
		it will create task that has access to it's private stack, full RAM memory and every peripheral register.

	2) You can also invoke old-fashioned xTaskCreate:
	   * 	xTaskCreate(	prvOldStyleUserModeTask,	 //The function that implements the task.
					"Task1",					 //Text name for the task.
					256,						 //Stack depth in words.
					NULL,						 //Task parameters.
					3 | portPRIVILEGE_BIT ,		 //Priority and mode (user in this case).
					NULL						 //Handle.
				);
		with set portPRIVILEGE_BIT which is equal to run task in PRIVILIGED level which gives access to basically everything(system registers etc).

	3) Last but not least, you can use xTaskCreateRestricted which takes TaskParameters_t as argument where you can set three memory regions
	which task will have access to plus peripheral registers.
			 Base address		Length		Parameters
    *   { 0x00,				0x00,			0x00  },
    *   { 0x00,				0x00,			0x00 },
	*	{ 0x00,				0x00,			0x00 }
	*	WARNING: Looks like Length parameter should be at least 32 for memory region to work.
	*
	BY DEFAULT EVERY TASK HAS ACCESS TO PERIPHERAL REGISTERS!
   */




  //xTaskCreateRestricted( &xRegTest1Parameters, NULL );


	/* Create the tasks that are created using the original xTaskCreate() API
	function. */
	xTaskCreate(	prvOldStyleUserModeTask,	 //The function that implements the task.
					"Task1",					 //Text name for the task.
					256,						 //Stack depth in words.
					NULL,						 //Task parameters.
					3 ,							 //Priority and mode (user in this case).
					NULL						 //Handle.
				);
#if 0
	xTaskCreate(	prvOldStylePrivilegedModeTask,	 //The function that implements the task.
					"Task2",						 //Text name for the task.
					256,							 //Stack depth in words.
					NULL,							 //Task parameters.
					( 3 | portPRIVILEGE_BIT ),		 //Priority and mode.
					NULL							 //Handle.
				);
#endif


	vTaskStartScheduler();

	while(1);

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
