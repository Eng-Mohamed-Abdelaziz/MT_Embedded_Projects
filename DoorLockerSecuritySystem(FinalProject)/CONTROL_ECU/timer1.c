 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: timer1.c
 * Description: Timer 1 Source file
 *******************************************************************************/

#include "timer1.h"
#include "common_macros.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*Global variable to hold the Timer Mode*/
static uint8 g_timer_mode;
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


ISR(TIMER1_COMPA_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


ISR(TIMER1_CAPT_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr){
	/*Timer initialization
	 * Mode Selection
	 * Prescaler Assignment
	 * Zeroing Clock
	 */
	TCCR1A=(((Config_Ptr->mode)&0b0011)<<WGM10);
	TCCR1B=(((Config_Ptr->mode)&0b1100)<<WGM12) | ((Config_Ptr->prescaler)<<CS10);
	TCNT1 = Config_Ptr->initial_value;

	/*Initializing PWM-dependant bits*/
	switch(Config_Ptr->mode){
		case CTC_OCR1A:
		case CTC_ICR1:
			TCCR1A|= (1<<FOC1A) | (1<<COM1A1) | (1<<COM1A0);	/*Set OC1A, Non-PWM*/
			break;
		case FAST_PWM_8_BIT:
		case FAST_PWM_9_BIT:
		case FAST_PWM_10_BIT:
		case FAST_PWM_OCR1A:
		case FAST_PWM_ICR1:
			SET_BIT(TCCR1A, COM1A1);							/*Non-inverting mode, FAST PWM*/
			break;
		case PWM_PHASE_8_BIT:
		case PWM_PHASE_9_BIT:
		case PWM_PHASE_10_BIT:
		case PWM_PHASE_AND_FREQ_OCR1A:
		case PWM_PHASE_OCR1A:
		case PWM_PHASE_AND_FREQ_ICR1:
		case PWM_PHASE_ICR1:
			TCCR1A|= (1<<COM1A1) | (1<<COM1A0);					/*Set OC1A, Phase Correct or Phase and Frequency Correct PWM*/
			break;
		default:
			break;
	}

	/*Initializing mode-dependant bits*/
	switch(Config_Ptr->mode){
		case NORMAL:
			GPIO_setupPinDirection(OVF_PORT_ID, OVF_PIN_ID, PIN_OUTPUT);
			SET_BIT(TIMSK,TOIE1);
			g_timer_mode=MODE_NORMAL;
			break;
		case PWM_PHASE_8_BIT:
		case PWM_PHASE_9_BIT:
		case PWM_PHASE_10_BIT:
		case CTC_OCR1A:
		case FAST_PWM_8_BIT:
		case FAST_PWM_9_BIT:
		case FAST_PWM_10_BIT:
		case PWM_PHASE_AND_FREQ_OCR1A:
		case PWM_PHASE_OCR1A:
		case FAST_PWM_OCR1A:
			GPIO_setupPinDirection(COMP_A_PORT_ID, COMP_A_PIN_ID, PIN_OUTPUT);
			OCR1A=Config_Ptr->compare_value;
			SET_BIT(TIMSK,OCIE1A);
			g_timer_mode=MODE_COMPARE;
			break;
		case PWM_PHASE_AND_FREQ_ICR1:
		case PWM_PHASE_ICR1:
		case CTC_ICR1:
		case FAST_PWM_ICR1:
			GPIO_setupPinDirection(ICU_PORT_ID, ICU_PIN_ID, PIN_OUTPUT);
			ICR1=Config_Ptr->compare_value;
			SET_BIT(TCCR1B, ICES1);
			SET_BIT(TIMSK, TICIE1);
			g_timer_mode=MODE_ICU;
			break;
		default:
			break;
	}
}

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void){
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	/*Clear Timer Registers*/
	switch(g_timer_mode){
		case MODE_NORMAL:
			CLEAR_BIT(TIMSK,TOIE1);
			break;
		case MODE_COMPARE:
			OCR1A=0;
			CLEAR_BIT(TIMSK,OCIE1A);
			break;
		case MODE_ICU:
			ICR1 = 0;
			CLEAR_BIT(TIMSK,TICIE1);
			break;
	}
}

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
