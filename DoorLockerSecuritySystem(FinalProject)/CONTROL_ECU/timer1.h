 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: timer1.h
 * Description: Timer 1 Header file
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MODE_NORMAL			0
#define MODE_COMPARE		1
#define MODE_ICU			2

#define OVF_PORT_ID			PORTB_ID
#define	OVF_PIN_ID			PIN1_ID

#define COMP_A_PORT_ID		PORTD_ID
#define	COMP_A_PIN_ID		PIN4_ID

#define ICU_PORT_ID			PORTD_ID
#define	ICU_PIN_ID			PIN6_ID

#define INITIAL_ZERO		0
#define NOT_COMPARE_VALUE	0

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}Timer1_Prescaler;

typedef enum {
	NORMAL, PWM_PHASE_8_BIT, PWM_PHASE_9_BIT, PWM_PHASE_10_BIT, CTC_OCR1A, FAST_PWM_8_BIT,
	FAST_PWM_9_BIT, FAST_PWM_10_BIT,PWM_PHASE_AND_FREQ_ICR1, PWM_PHASE_AND_FREQ_OCR1A,
	PWM_PHASE_ICR1, PWM_PHASE_OCR1A, CTC_ICR1, RESERVED, FAST_PWM_ICR1, FAST_PWM_OCR1A
}Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void);

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
