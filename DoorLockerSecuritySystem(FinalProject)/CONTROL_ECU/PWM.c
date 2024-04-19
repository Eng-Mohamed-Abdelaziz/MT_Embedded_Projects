 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: PWM.c
 * Description: PWM Source file
 *******************************************************************************/

#include <avr/io.h>
#include "gpio.h"
#include "PWM.h"

void PWM_Timer0_Start(uint8 duty_cycle){
	uint8 PWM_duty_cycle;
	TCNT0 = 0;
	PWM_duty_cycle=(uint8)(((float)duty_cycle/100.0)*255);
	OCR0=PWM_duty_cycle;
	GPIO_setupPinDirection(PWM_PORT_ID, PWM_PIN_ID, PIN_OUTPUT);
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
