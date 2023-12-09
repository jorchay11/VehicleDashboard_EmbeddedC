/*
 * ProjectButtons.c
 *
 *  Created on: Nov 26, 2020
 *      Author: 7jorchay
 */

#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "ProjectButtons.h"

volatile int Open_Door_Btn_pressed = 0;			//global debounce variables
volatile int NewRTC_Info_Btn_pressed = 0;
volatile int WatchdogReset_Btn_pressed = 0;

//initializes two pins connected to toggle switch for blinkers, acts like a button
//no debounce required, 3.6 and 3,7
void Blinkers_Toggle_Switch_init()
{
    P3->SEL0 &= ~BIT6;          //P3.6 set to GPIO
    P3->SEL1 &= ~BIT6;
    P3->DIR &= ~BIT6;           //P3.6 direction set to input
    P3->REN |= BIT6;            //activating the internal resistor
    P3->OUT |= BIT6;            //declaring the internal resistor as pull-up

    P3->SEL0 &= ~BIT7;          //P3.7 set to GPIO
    P3->SEL1 &= ~BIT7;
    P3->DIR &= ~BIT7;           //P3.7 direction set to input
    P3->REN |= BIT7;            //activating the internal resistor
    P3->OUT |= BIT7;            //declaring the internal resistor as pull-up
}

//initialize door ajar button on 3.5
void openDoor_Btn_init()
{
    P3->SEL0 &= ~BIT5;          //P3.5 set to GPIO
    P3->SEL1 &= ~BIT5;
    P3->DIR &= ~BIT5;           //P3.5 direction set to input
    P3->REN |= BIT5;            //activating the internal resistor
    P3->OUT |= BIT5;            //declaring the internal resistor as pull-up
    P3->IES |= BIT5;            //interrupt edge select on falling edge
    P3->IE |= BIT5;             //Interrupt enable
    P3->IFG &= ~BIT5;           //Clears the flag bit
}

//initialize simple LED output for right blinker on 8.3
void Left_Blinker()
{
    P8->SEL0 &= ~BIT3;
    P8->SEL1 &= ~BIT3;          //P8.3 direction set to output
    P8->DIR |= BIT3;            //P8.3 direction set to output
    P8->OUT &= ~BIT3;           //P8.3 set to off or as 0
}

//initialize simple LED output for right blinker on 8.4
void Right_Blinker()
{
    P8->SEL0 &= ~BIT4;
    P8->SEL1 &= ~BIT4;          //P8.4 direction set to output
    P8->DIR |= BIT4;            //P8.4 direction set to output
    P8->OUT &= ~BIT4;           //P8.4 set to off or as 0
}

//initialize simple LED output for proximity warning, 8.2
void LED_closeProximity()
{
    P8->SEL0 &= ~BIT2;
    P8->SEL1 &= ~BIT2;          //P8.2 direction set to output
    P8->DIR |= BIT2;            //P8.2 direction set to output
    P8->OUT &= ~BIT2;           //P8.2 set to off or as 0

    /*                  For TIMER A intensity if you get there
     P8->SEL0 |= BIT2;                             //P5.6 set to TA2.1
     P8->SEL1 &= ~BIT2;
     P8->DIR |= BIT2;                              //P5.6 Set to Output

     TIMER_A3->EX0 |= 0x0003; //Expand the flag timer to run slower to encapsulate a 1ms interrupt

     TIMER_A3->CTL = 0x02D6; //Timer to interrupt every 1 ms to set checking flags.
     TIMER_A3->CCR[0] = LED_Period;
     TIMER_A3->CCTL[2] = 0x00E0;

     TIMER_A3->CCR[2] = duty_cycle;                //PWM duty cycle
     */
}

//initialize watchdog reset button on 3.2
void NewRTC_Info_Btn() {
	P3->SEL0 &= ~BIT2;

	P3->SEL1 &= ~BIT2;

	P3->DIR &= ~BIT2;                   //Set as input

	P3->REN |= BIT2;                   //enable resistor

	P3->OUT |= BIT2;                   //set as pull up

	P3->IES |= BIT2;                 //Set to trigger interrupt from high to low

	P3->IFG &= ~BIT2;                   //Clears interrupt flag

	P3->IE |= BIT2;                     //Enables interrupt
}

//initialize watchdog reset button on 3.3
void WatchdogReset_Btn() {
	P3->SEL0 &= ~BIT3;

	P3->SEL1 &= ~BIT3;

	P3->DIR &= ~BIT3;                   //Set as input

	P3->REN |= BIT3;                   //enable resistor

	P3->OUT |= BIT3;                   //set as pull up

	P3->IES |= BIT3;                 //Set to trigger interrupt from high to low

	P3->IFG &= ~BIT3;                   //Clears interrupt flag

	P3->IE |= BIT3;                     //Enables interrupt
}

//debounce for New_RTC_info button
int Debounce_NewRTC_Info_Btn() {
	static uint16_t State_1 = 0;                       //Current debounce status

	State_1 = (State_1 << 1) | (P3IN & 0x04) >> 2 | 0xf800; //checks pin 3.2, don

	if (State_1 == 0xfc00) {
		NewRTC_Info_Btn_pressed = 1;
		return 1;                                             //after 0 level is
															  //stable for 10 consecutive calls
	} else
		return 0;
}

//Door ajar button debounce
int Debounce_DoorOpen_Btn()
{
    static uint16_t State_1 = 0;                       //Current debounce status

    State_1 = (State_1 << 1) | (P3IN & 0x20) >> 5 | 0xf800; //checks pin 3.5, don

    if (State_1 == 0xfc00)
    {
        Open_Door_Btn_pressed = 1;
        return 1;                                             //after 0 level is
                                                              //stable for 10 consecutive calls
    }
    else
        return 0;
}

//debounces watchdog button, returns 1 if confirmed, along with the gloabl debounce flag
int Debounce_WatchodgReset_Btn() {
	static uint16_t State_1 = 0;                       //Current debounce status

	State_1 = (State_1 << 1) | (P3IN & BIT3 ) >> 3 | 0xf800; //checks pin 3.0,

	if (State_1 == 0xfc00) {
		WatchdogReset_Btn_pressed = 1;
		return 1;                                             //after 0 level is
															  //stable for 10 consecutive calls
	} else
		return 0;
}


