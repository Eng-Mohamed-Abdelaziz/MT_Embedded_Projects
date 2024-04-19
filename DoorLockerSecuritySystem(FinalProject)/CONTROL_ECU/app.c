 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: app.c
 * Description: Application file for the CONTROL_ECU
 *******************************************************************************/

#include "DC_Motor.h"
#include "twi.h"
#include "uart.h"
#include "timer1.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      	Modes Definitions                                  *
 *******************************************************************************/

/*Commands to Receive*/
/*Write/Check Pass*/
#define WRITE_PASS			0
#define CHECK_PASS			1
#define OPEN_DOOR			2
#define CHANGE_PASS			3

/*Actuator Used*/
#define BUZZER_ACT			2
#define DC_MOTOR_ACT1		3
#define DC_MOTOR_ACT2		4
#define DC_MOTOR_ACT3		5

/*Door and Buzzer time in secs*/
#define DOOR_UNLOCKING		15
#define DOOR_HOLD			3
#define DOOR_LOCKING		15
#define BUZZER_TIME			60

/*Replies to Send*/
/*Check Status*/
#define FAILURE				0
#define SUCCESS				1
#define COMMAND_DONE		2

#define PASS_SIZE			10
#define ADDRESS_USED		0x0311

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 g_check, g_secs_count=0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void CONTROL_edgeProcessing(void)
{
	g_secs_count++;
}

void receivePassword(uint8 *password){
	uint8 i=0;
	password[i]=UART_recieveByte();
	while (password[i]!=13){
		i++;
		password[i]=UART_recieveByte();
	}
}

void EEPROM_write(uint16 address, uint8 *password){
	uint8 i=0;
	EEPROM_writeByte((address+i),password[i]);
	while(password[i]!=13){
		i++;
		EEPROM_writeByte((address+i),password[i]);
	}
}

void EEPROM_read(uint16 address, uint8 *password){
	uint8 i=0;
	EEPROM_readByte((address+i),password[i]);
	while(password[i]!=13){
		i++;
		EEPROM_readByte((address+i),password[i]);
	}
}

uint8 check_passwords(uint8 *password, uint8 *password_2){
	uint8 i=0,j=0;
	while(password[i]!=13){
		if(password[i]==password_2[i]){
			j++;
		}
		i++;
	}
	if(j==i){
		return SUCCESS;
	}
	else{
		return FAILURE;
	}
}

void main(void){

	uint8 master_state, password[PASS_SIZE], password_2[PASS_SIZE];

	UART_ConfigType CONTROL_UART_Config={BIT_8, PARITY_DISABLED, STOP_ONE, 9600};
	Timer1_ConfigType CONTROL_Timer1_Config={INITIAL_ZERO, 15625, F_CPU_64, CTC_OCR1A};
	TWI_ConfigType CONTROL_TWI_Config={0x01, 400000};

	SET_BIT(SREG, 7);

	Timer1_setCallBack(CONTROL_edgeProcessing);

	DcMotor_Init();
	Buzzer_init();
	UART_init(&CONTROL_UART_Config);
	Timer1_init(&CONTROL_Timer1_Config);
	TWI_init(&CONTROL_TWI_Config);

	while(1){
		master_state=UART_recieveByte();
		switch(master_state){
		case WRITE_PASS:
			receivePassword(password);
			receivePassword(password_2);
			g_check=check_passwords(password, password_2);
			if(g_check==SUCCESS){
				EEPROM_write(ADDRESS_USED, password);
			}
			UART_sendByte(g_check);
			break;
		case CHECK_PASS:
			receivePassword(password);
			EEPROM_read(ADDRESS_USED, password_2);
			g_check=check_passwords(password, password_2);
			UART_sendByte(g_check);
			if(g_check==SUCCESS){
				master_state=UART_recieveByte();
			}
			break;
		case BUZZER_ACT:
			Buzzer_on();
			Timer1_init(&CONTROL_Timer1_Config);
			while(g_secs_count<BUZZER_TIME);
			g_secs_count=0;
			Buzzer_off();
			UART_sendByte(COMMAND_DONE);
			break;
		case DC_MOTOR_ACT1:
			DcMotor_Rotate(CW,100);
			Timer1_init(&CONTROL_Timer1_Config);
			while(g_secs_count<DOOR_UNLOCKING);
			g_secs_count=0;
			UART_sendByte(COMMAND_DONE);
			break;
		case DC_MOTOR_ACT2:
			DcMotor_Rotate(STOP,0);
			Timer1_init(&CONTROL_Timer1_Config);
			while(g_secs_count<DOOR_HOLD);
			g_secs_count=0;
			UART_sendByte(COMMAND_DONE);
			break;
		case DC_MOTOR_ACT3:
			DcMotor_Rotate(A_CW,100);
			Timer1_init(&CONTROL_Timer1_Config);
			while(g_secs_count<DOOR_LOCKING);
			g_secs_count=0;
			DcMotor_Rotate(STOP,0);
			UART_sendByte(COMMAND_DONE);
			break;

		}

	}
}
