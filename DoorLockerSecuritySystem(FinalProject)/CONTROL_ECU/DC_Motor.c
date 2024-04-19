 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: DC_Motor.c
 * Description: DC Motor Source file
 *******************************************************************************/

#include "DC_Motor.h"
#include "gpio.h"
#include "PWM.h"

void DcMotor_Init(void){
	GPIO_setupPinDirection(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, PIN_OUTPUT);

	GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	if(state==CW){
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}
	else if(state==A_CW){
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_HIGH);
	}
	else if(state==STOP){
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}
	else{}
	PWM_Timer0_Start(speed);
}
