 /******************************************************************************
 * Author: Mohamed Abdelaziz
 * Module: Ultrasonic
 * File Name: Ultrasonic.h
 * Description: Header file for the Ultrasonic driver
 *******************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define TRIGGER_PORT_ID		PORTB_ID
#define TRIGGER_PIN_ID		PIN5_ID


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Ultrasonic:
 * 1. Initialize ICU Driver.
 * 2. Setup ICU callback.
 * 3. Setup direction for trigger pin.
 */
void Ultrasonic_init(void);

/*
 * Description :
 * Send Trigger pulse to Ultrasonic.
 */
void Ultrasonic_Trigger(void);

/*
 * Description :
 * Reads distance using Ultrasonic:
 * 1. Send Trigger pulse to Ultrasonic.
 * 2. Starts measurements from ICU.
 */
uint16 Ultrasonic_readDistance(void);

/*
 * Description :
 * Edge processing of Ultrasonic:
 * 1. Callback of ICU.
 * 2. Calculate pulse time.
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
