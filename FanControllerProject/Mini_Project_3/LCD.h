/*
 * LCD.h
 *
 *  Created on: 5 Oct 2022
 *      Author: MoAziz
 */

#ifndef LCD_H_
#define LCD_H_

#define LCD_DATA_BITS_MODE 8

#if((LCD_DATA_BITS_MODE!=4)&&(LCD_DATA_BITS_MODE!=8))
#error "Bits mode has to be 4 or 8"
#endif

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/*LCD Ports and Pins ID*/
#define LCD_RS_PORT_ID 				PORTD_ID
#define LCD_RS_PIN_ID 				PIN0_ID

#define LCD_E_PORT_ID 				PORTD_ID
#define LCD_E_PIN_ID 				PIN2_ID

#define LCD_DATA_ID 				PORTC_ID

#if (LCD_DATA_BITS_MODE == 4)

#define LCD_DB4_PIN_ID                 PIN3_ID
#define LCD_DB5_PIN_ID                 PIN4_ID
#define LCD_DB6_PIN_ID                 PIN5_ID
#define LCD_DB7_PIN_ID                 PIN6_ID

#endif

/* LCD Commands */
#define LCD_CLEAR_SCREEN                     0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void LCD_init(void);

void LCD_sendCommand(uint8 command);

void LCD_displayCharacter(uint8 data);

void LCD_displayString (char *Str);

void LCD_moveCursor(uint8 row, uint8 col);

void LCD_displayStringRowColumn(char *Str, uint8 row, uint8 col);

void LCD_integerToString(int num);

void LCD_clearScreen(void);

#endif /* LCD_H_ */
