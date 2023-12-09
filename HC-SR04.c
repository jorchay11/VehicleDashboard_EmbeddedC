/*
 * HC-SR04.c
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "HC-SR04.h"

volatile int object_DistanceFromSensor = 0;        //distance variable
volatile float conversion_ToInches = 100.0;          //echo signal to inches


//initializes the Proximity sensors echo and trigger pins  using TIMERA capture and compare
void HCSR04_init(void)
{
	P7->SEL0 |= (BIT6 | BIT7 );
    P7->SEL1 &= ~(BIT6 | BIT7 );
	P7->DIR |= BIT7;								//pulse pin
	P7->DIR &= ~BIT6;								//echo

	TIMER_A1->CTL = 0x02E4;         //SMCLK, CLK /4, Up Mode, Clear TAR to start
    TIMER_A1->CCR[0] = 65535 - 1;                   //87ms period
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_6;     //Toggle/set

	TIMER_A1->CCR[1] = 8;                         //PWM duty cycle 10us toggles
	TIMER_A1->CCTL[2] = 0xC900; //rising and falling edge, CCI2A, SCS, capture mode
	TIMER_A1->EX0 = 0x0001;                       	//Divide by 4
}

//check at an interval
void measure_Period()
{
    unsigned int last, current;

    float period;

    TIMER_A1->CCTL[2] &= ~1;                         //Clears interrupt flag

    while ((TIMER_A1->CCTL[2] & 1) == 0)
        ;
    //Wait until CCIFG is set

    current = TIMER_A1->CCR[2];                         //Save first time stamp

    TIMER_A1->CCTL[2] &= ~1;                            //Clears interrupt flag

    while ((TIMER_A1->CCTL[2] & 1) == 0)
        ;               //Wait until CCIFG is set

    last = current;

    current = TIMER_A1->CCR[2];                         //Save second time stamp

    period = current - last; //Calculating difference between rising and falling edge

    object_DistanceFromSensor = period / conversion_ToInches; //Converting cycle time to inches

    //printf("\nDistance = %.1f\n", object_DistanceFromSensor);
}

