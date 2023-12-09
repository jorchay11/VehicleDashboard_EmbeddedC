/*
 * TIMERA.c
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */

// bit  mode
// 9-8  20    TASSEL, SMCLK=12MHz
// 7-6  11    ID, divide by 8
// 5-4  11    MC, up-down mode
// 2    0     TACLR, no clear
// 1    0     TAIE, no interrupt
// 0          TAIFG
//NVIC_EnableIRQ(TA1_0_IRQn);
//void TA1_0_IRQHandler(void);
#include <stdio.h>
#include <stdint.h>
#include "TIMERA.h"
#include "msp.h"


void TIMERA0_PWM_init(uint16_t period)
{
    TIMER_A0->CCR[0] = period;   // Period is 2*period*8*83.33ns is 1.333*period
    TIMER_A0->CTL = 0x02F0; // SMCLK=12MHz, divide by 8, up-down mode (makes period 2 * period * 8)
}
/*
 void TIMERA0_1_PWM_Init(uint16_t duty1)
{
    P2DIR |= 0x30;                                  // P2.4, P2.5 output
    P2SEL0 |= 0x30;                              // P2.4, P2.5 Timer0A functions
    P2SEL1 &= ~0x30;                              // P2.4, P2.5 Timer0A function
    TIMER_A0->CCTL[0] = 0x0080;                     // CCI0 toggle
    //TA0EX0 = 0x0000;        //    divide by 1
    TIMER_A0->CCTL[1] = 0x0040;                     // CCR1 toggle/reset
    TIMER_A0->CCR[1] = duty1;                 // CCR1 duty cycle is duty1/period

}

void TIMERA0_2_PWM_Init(uint16_t duty2)
{
    P7->DIR |= 0x08;          // 7.3 output
    P7->SEL0 |= 0x08;         // P7.3 Timer0A functions
    P7->SEL1 &= ~0x08;        // P7.3 Timer0A functions

    TIMER_A0->CCTL[2] = 0x0040;      // CCR2 toggle/reset
    TIMER_A0->CCR[2] = duty2;        // CCR2 duty cycle is duty2/period
}
 */
/*
void TimerA0_1_PWM_init(float dutyCycle_percentage)
{                             //Timer A initialization function

    P2->SEL0 |= BIT4;                               //P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;                                //P2.4 Set to Output

    TIMER_A0->CCR[0] = 10000;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCR1 outmode 7
    TIMER_A0->CCR[1] = 10000 * dutyCycle_percentage;            //PWM duty cycle
    TIMER_A0->CTL = 0x0214;                 //SMCLK, Up Mode, Clear TAR to start
}
 */

void TIMERA1_0_Timer_withInterrupts()
{
    P2->SEL0 &= ~BIT0;
    P2->SEL1 &= ~BIT0;  //P2.0 set to GPIO
    P2->DIR |= BIT0;    //P2.0 direction set to output
    P2->OUT &= ~BIT0;   //P2.0 set to off or as 0

    TIMER_A1->CTL = 0x0111;            //0000_0001_1101_0110: ACLK (32KHz),
    TIMER_A1->CCR[0] = 16000 - 1;      //half a second of 32KHz
    TIMER_A1->CCTL[0] |= 10;
}

void TIMERA1_0_oneMSInterval_init()
{
    //note ACLK has to be already divided by 4
    TIMER_A1->CTL = 0x0136;     //ACLK, Up mode, clear TAR, /1, interrupt enable
	TIMER_A1->CCR[0] = 4; //clock is 32768 Hz divided by 4, so 32768 / 4 / 2 = 1 second
    TIMER_A1->CCTL[0] = 0x0810; //outmode not needed, just interval timer, enable interrupts and synchronize

    //4092 = 1 second
    //2046 = 1/2 second
    //4092 / 1000 = 4 = 1 ms
}

void TIMERA3_0_HalfSecondInterval_init()
{
    //note ACLK has to be already divided by 4
    TIMER_A3->CTL = 0x0136;     //ACLK, Up mode, clear TAR, /1, interrupt enable
    TIMER_A3->CCR[0] = 2046; //clock is 32768 Hz divided by 4, so 32768 / 4 / 2 = 1 second
    TIMER_A3->CCTL[0] = 0x0810; //outmode not needed, just interval timer, enable interrupts and synchronize

    //4092 = 1 second
    //2046 = 1/2 second
}

/*
 void TIMERA3_1_oneMSInterval_init()
 {
 //note ACLK has to be already divided by 4
 TIMER_A3->CTL = 0x0136;     //ACLK, Up mode, clear TAR, /1, interrupt enable
 TIMER_A3->CCR[1] = 4; //clock is 32768 Hz divided by 4, so 32768 / 4 / 2 = 1 second
 TIMER_A3->CCTL[1] = 0x0810; //outmode not needed, just interval timer, enable interrupts and synchronize

 //4092 = 1 second
 //2046 = 1/2 second
 //4092 / 1000 = 5 = 1 ms
 }
 */
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/*
 * HANDLERS
 *
 * Each function includes a clear and a toggle onboard LED 2.0 command
 */
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////




