/*
 * Photocell.h
 *
 *  Created on: Dec 1, 2020
 *      Author: 7jorchay
 */

#ifndef PHOTOCELL_H_
#define PHOTOCELL_H_

extern volatile int TA0_1_dutyCycle;

void Clock_Init48MHz(void);

void ADC_pin_init(void);                //initialize ADC pin 5.0
void ADC_StartConversion_WithInterrupts(void);        //starts ADC conversion

void ADC_Reading(void); //reads value in ADC register and converts to readable value

void TimerA0_1_PWM_init(void);          //LED
void change_TIMERA0_1_dutyCycle(void);

#endif /* PHOTOCELL_H_ */
