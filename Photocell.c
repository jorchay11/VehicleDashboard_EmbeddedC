/*
 * Photocell.c
 *
 *  Created on: Dec 1, 2020
 *      Author: 7jorchay
 */


#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "Photocell.h"

volatile int TA0_1_dutyCycle = 30000;

void ADC_pin_init(void)
{
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;
}


void ADC_StartConversion_WithInterrupts(void)
{
	ADC14->CTL0 &= ~ADC14_CTL0_ENC;     //disable, no conversions runnning

	ADC14->CTL0 |= 0x04200210; //Predivide 1, ACD14SC bit, Source from timer, no inversion, divide 2, 
							   //single channel single conv, not active, 16 cycle sample and hold, rising edge, core on

	ADC14->CTL1 = 0x00000030;           //14 bit resolution

	ADC14->CTL1 |= 0x0000000;			//in case any other ADC is used

	ADC14->MCTL[0] = 0x0E;              //attaching 14th channel to mem[0]

	ADC14->CTL0 |= ADC14_CTL0_ENC;      //enable, start conversion
}

void ADC_Reading(void)
{

	static volatile uint16_t result;       //local variable for result in MEM[0]
	float V;                                //converted Voltage reading 

	ADC14->CTL0 |= ADC14_CTL0_SC;

    if (ADC14->IFGR0 & BIT0)
    {

		result = ADC14->MEM[0];          	//result equals converted value

		V = ((result * 3.3) / 16384); 		//V conversion from raw ADC readings

		if (V <= .33)       //conditions are inverted. If V is low, duty is high
        {
            TA0_1_dutyCycle = 60000;
        }

        if (V > .33 && V <= 0.66)
        {
            TA0_1_dutyCycle = (9.75 * 6000);
        }

        if (V > 0.66 && V <= 0.99)
        {
            TA0_1_dutyCycle = (9.5 * 6000);
        }

        if (V > 0.99 && V <= 1.32)
        {
            TA0_1_dutyCycle = (8.5 * 6000);
        }

        if (V > 1.32 && V <= 1.65)
        {
            TA0_1_dutyCycle = (7.5 * 6000);
        }

        if (V > 1.65 && V <= 1.98)
        {
            TA0_1_dutyCycle = (5.5 * 6000);
        }

        if (V > 1.98 && V <= 2.31)
        {
            TA0_1_dutyCycle = (3.5 * 6000);
        }

        if (V > 2.31 && V <= 2.64)
        {
            TA0_1_dutyCycle = (1.5 * 6000);
        }

        if (V > 2.64 && V <= 2.97)
        {
            TA0_1_dutyCycle = (1 * 6000);
        }

        if (V > 2.97)
        {
            TA0_1_dutyCycle = (0.5 * 6000);
        }

		ADC14->CTL0 |= ADC14_CTL0_SC; //restart conversion after changing global duty cycle

    }
}


void TimerA0_1_PWM_init(void)
{                //Timer A initialization function
    P2->SEL0 |= BIT4;                               //P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;                                //P2.4 Set to Output

    TIMER_A0->CCR[0] = 60000;                       //20ms period
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCR1 reset
    TIMER_A0->CCR[1] = TA0_1_dutyCycle;                        //PWM duty cycle
    TIMER_A0->CTL = 0x0294;                 //SMCLK, Up Mode, Clear TAR to start
}

void change_TIMERA0_1_dutyCycle()
{
	TIMER_A0->CCR[1] = TA0_1_dutyCycle; //changes TIMERA0.1 PWM, the backlight LED for LCD
}
