 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: MeasureDistance.c
 * Description: Application file
 *******************************************************************************/
#include "Ultrasonic.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void){
	uint16 distance;

	SREG=(1<<7);

	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance=    cm");
	while(1){
		distance=Ultrasonic_readDistance();
		LCD_moveCursor(0,9);
		LCD_intgerToString(distance);
		LCD_displayCharacter(' ');
		_delay_ms(750);
	}
}
