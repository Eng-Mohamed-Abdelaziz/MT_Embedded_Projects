 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * File Name: DC_Motor.h
 * Description: DC Motor Header file
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_PORT1_ID			PORTB_ID
#define DC_MOTOR_PIN1_ID			PIN7_ID
#define DC_MOTOR_PORT2_ID			PORTB_ID
#define DC_MOTOR_PIN2_ID			PIN6_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
