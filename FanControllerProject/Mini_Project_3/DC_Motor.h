/*
 * DC_Motor.h
 *
 *  Created on: 8 Oct 2022
 *      Author: MoAziz
 */

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
#define DC_MOTOR_PIN1_ID			PIN0_ID
#define DC_MOTOR_PORT2_ID			PORTB_ID
#define DC_MOTOR_PIN2_ID			PIN1_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
