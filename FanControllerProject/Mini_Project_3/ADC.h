/*
 * ADC.h
 *
 *  Created on: 6 Oct 2022
 *      Author: MoAziz
 */

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	AREF0,AVCC,RESERVED,INTERNAL
}ADC_ReferenceVolatge;

typedef enum
{
	F_CPU_2=1,F_CPU_4,F_CPU_8,F_CPU_16,F_CPU_32,F_CPU_64,F_CPU_128
}ADC_Prescaler;

typedef struct
{
	ADC_ReferenceVolatge ref_volt;
	ADC_Prescaler prescaler;
}ADC_ConfigType;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ADC_MAXIMUM_VALUE  		  1023
#define ADC_REF_VOLT_VALUE 		  5
#define ADC_INTERNAL_VOLT_VALUE   2.56

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType * Config_Ptr);

uint16 ADC_readChannel(uint8 ch_num);

#endif /* ADC_H_ */
