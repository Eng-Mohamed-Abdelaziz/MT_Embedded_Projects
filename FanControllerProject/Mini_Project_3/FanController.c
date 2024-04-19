/*
 * APP.c
 *
 *  Created on: 8 Oct 2022
 *      Author: MoAziz
 */
#include "ADC.h"
#include "LCD.h"
#include "DC_Motor.h"
#include "LM35.h"

uint8 g_Motor_state;

void Fan_ON(uint8 speed){
	DcMotor_Rotate(CW,speed);
	g_Motor_state=1;
}

void Fan_OFF(){
	DcMotor_Rotate(STOP,0);
	g_Motor_state=0;
}

void main(void){
	uint8 temp=0;

	ADC_ConfigType ADC_Config={INTERNAL,F_CPU_8};

	DcMotor_Init();
	LCD_init();
	ADC_init(&ADC_Config);

	while(1){
		temp=LM35_getTemperature();
		/*Motor Actuation*/
		if(temp<30){
			Fan_OFF();
		}
		else if(temp<60){
			Fan_ON(25);
		}
		else if(temp<90){
			Fan_ON(50);
		}
		else if(temp<120){
			Fan_ON(75);
		}
		else if(temp>=120){
			Fan_ON(100);
		}
		else{}
		/*LCD Actuation First Line*/
		LCD_displayStringRowColumn("FAN is ",0,3);
		if(g_Motor_state){
			LCD_displayString("ON ");
		}
		else{
			LCD_displayString("OFF");
		}
		/*LCD Actuation Second Line*/
		LCD_moveCursor(1,3);
		LCD_displayString("Temp = ");
		if(temp<=100){
			LCD_displayString(" ");
			LCD_integerToString(temp);
		}
		else{
			LCD_integerToString(temp);
		}
		LCD_displayString("C ");
	}
}
