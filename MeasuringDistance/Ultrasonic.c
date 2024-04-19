 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * Module: Ultrasonic
 * File Name: Ultrasonic.c
 * Description: Source file for the Ultrasonic driver
 *******************************************************************************/

#include <util/delay.h> /* For the delay functions */
#include "Ultrasonic.h"
#include "icu.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

uint32 g_time;
uint8 g_edgeCount;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Ultrasonic:
 * 1. Initialize ICU Driver.
 * 2. Setup ICU callback.
 * 3. Setup direction for trigger pin.
 */
void Ultrasonic_init(void){
	Icu_ConfigType US_Ptr={F_CPU_8,RISING};
	Icu_init(&US_Ptr);
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT_ID, TRIGGER_PIN_ID, PIN_OUTPUT);
}

/*
 * Description :
 * Send Trigger pulse to Ultrasonic.
 */
void Ultrasonic_Trigger(void){
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * Reads distance using Ultrasonic:
 * 1. Send Trigger pulse to Ultrasonic.
 * 2. Starts measurements from ICU.
 */
uint16 Ultrasonic_readDistance(void){
	uint16 distance=0;
	Ultrasonic_init();
	Ultrasonic_Trigger();
	if(g_edgeCount==2){
		g_edgeCount=0;
		distance=(uint16)((g_time/58.8)+2);
	}
	return distance;
}

/*
 * Description :
 * Edge processing of Ultrasonic:
 * 1. Callback of ICU.
 * 2. Calculate pulse time.
 */
void Ultrasonic_edgeProcessing(void){

	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */

		Icu_clearTimerValue();

		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{

		/* Store the High time value */
		g_time = Icu_getInputCaptureValue();

		Icu_DeInit();
	}
}
