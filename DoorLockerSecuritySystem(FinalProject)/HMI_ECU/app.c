 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: app.c
 * Description: Application file for the HMI_ECU
 *******************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <util/delay.h>
#include "common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      	Modes Definitions                                  *
 *******************************************************************************/

/*System States*/
#define FIRST_PASS			0
#define	SECOND_PASS			1
#define MAIN_OPTIONS		        2
#define OPEN_DOOR			3
#define CHANGE_PASS			4

/*Commands to Send*/
/*Write/Check Pass*/
#define WRITE_PASS			0
#define CHECK_PASS			1

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

/*Replies to Receive*/
/*Check Status*/
#define FAILURE				0
#define SUCCESS				1
#define COMMAND_DONE		2

#define PASS_SIZE			10

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 g_callback_secs, g_secs_count=0, g_i;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void HMI_edgeProcessing(void)
{
	g_secs_count++;
}

void buzzer_act(uint8 t){
	LCD_clearScreen();
	LCD_displayString("THIEF!!!");
	Timer1_setCallBack(HMI_edgeProcessing);
	while(g_secs_count<BUZZER_TIME*2);
	g_secs_count=0;
}

void sendPassword(uint8 *password){
	uint8 i=0;
	while(password[i]!=13){
		UART_sendByte(password[i]);
		i++;
	}
	UART_sendByte(password[i]);
}

void getPassword(uint8 * password){
	uint8 i=0,key;
	_delay_ms(100);
	key=KEYPAD_getPressedKey();
	while(key!=13){
		password[i]=key;
		i++;
		LCD_displayString("*");
		_delay_ms(500);
		key=KEYPAD_getPressedKey();
	}
	password[i]=key;
}

void motor_act(uint8 act){
	UART_sendByte(act);
	switch(act){
	case DC_MOTOR_ACT1:
		LCD_clearScreen();
		LCD_displayString("Door is");
		LCD_displayStringRowColumn(1,0,"Unlocking");
		Timer1_setCallBack(HMI_edgeProcessing);
		while(g_secs_count<DOOR_UNLOCKING*2);
		g_secs_count=0;
		break;
	case DC_MOTOR_ACT2:
		LCD_clearScreen();
		LCD_displayString("Door is");
		LCD_displayStringRowColumn(1,0,"Unlocked");
		Timer1_setCallBack(HMI_edgeProcessing);
		while(g_secs_count<DOOR_HOLD*2);
		g_secs_count=0;
		break;
	case DC_MOTOR_ACT3:
		LCD_clearScreen();
		LCD_displayString("Door is");
		LCD_displayStringRowColumn(1,0,"Locking");
		Timer1_setCallBack(HMI_edgeProcessing);
		while(g_secs_count<DOOR_LOCKING*2);
		g_secs_count=0;
		break;
	}
}

uint8 enter_opt(){
	return KEYPAD_getPressedKey();
}

void main(void){

	uint8 state_sys=FIRST_PASS, check, state, opt_forsa=0, password[PASS_SIZE];

	UART_ConfigType HMI_UART_Config={BIT_8, PARITY_DISABLED, STOP_ONE, 9600};
	Timer1_ConfigType HMI_Timer1_Config={INITIAL_ZERO, NOT_COMPARE_VALUE, F_CPU_8, NORMAL};

	SET_BIT(SREG, 7);

	LCD_init();
	UART_init(&HMI_UART_Config);
	Timer1_init(&HMI_Timer1_Config);

	while(1){
		switch(state_sys){
		case FIRST_PASS:
			UART_sendByte(WRITE_PASS);
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1,0);
			getPassword(password);
			sendPassword(password);
			state_sys=SECOND_PASS;
			break;
		case SECOND_PASS:
			LCD_clearScreen();
			LCD_displayString("Plz re-enter the");
			LCD_displayStringRowColumn(1, 0, "same pass:");
			getPassword(password);
			sendPassword(password);
			LCD_clearScreen();
			LCD_displayString("Loading...");
			check=UART_recieveByte();
			switch(check){
			case SUCCESS:
				LCD_clearScreen();
				LCD_displayString("Pass success");
				state_sys=MAIN_OPTIONS;
				break;
			case FAILURE:
				LCD_clearScreen();
				LCD_displayString("Pass fail");
				state_sys=FIRST_PASS;
				break;
			default:
				break;
			}
			_delay_ms(250);
			break;
		case MAIN_OPTIONS:
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_displayStringRowColumn(1, 0, "- : Change Pass");
			state=KEYPAD_getPressedKey();
			switch(state){
			case '+':
				state_sys=OPEN_DOOR;
				break;
			case '-':
				state_sys=CHANGE_PASS;
				break;
			default:
				break;
			}
			break;
		case OPEN_DOOR:
				UART_sendByte(CHECK_PASS);
				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1,0);
				getPassword(password);
				sendPassword(password);
			check=UART_recieveByte();
			switch(check){
			case SUCCESS:
				UART_sendByte(DC_MOTOR_ACT1);
				motor_act(DC_MOTOR_ACT1);
				UART_recieveByte();
				UART_sendByte(DC_MOTOR_ACT2);
				motor_act(DC_MOTOR_ACT2);
				UART_recieveByte();
				UART_sendByte(DC_MOTOR_ACT3);
				motor_act(DC_MOTOR_ACT3);
				UART_recieveByte();
				state_sys=MAIN_OPTIONS;
				break;
			case FAILURE:
				opt_forsa++;
				if(opt_forsa==3){
					UART_sendByte(BUZZER_ACT);
					buzzer_act(BUZZER_TIME);
					state_sys=MAIN_OPTIONS;
					UART_recieveByte();
				}
				break;
			default:
				break;
			}
			break;
		case CHANGE_PASS:
			UART_sendByte(CHECK_PASS);
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1,0);
			getPassword(password);
			sendPassword(password);
			check=UART_recieveByte();
			switch(check){
			case SUCCESS:
				state_sys=FIRST_PASS;
				break;
			case FAILURE:
				opt_forsa++;
				if(opt_forsa==3){
					UART_sendByte(BUZZER_ACT);
					LCD_clearScreen();
					LCD_displayString("THIEF!!!");
					state_sys=MAIN_OPTIONS;
					UART_recieveByte();
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}
