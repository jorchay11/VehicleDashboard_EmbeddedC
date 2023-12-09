/*
 * Hall_Effect.c
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */


#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "Hall_Effect.h"

volatile int Current_Speed_mph = 0;
volatile int RPMs = 0;
volatile int Revolution_count = 0;
volatile int speed_Dif = 0;
volatile int lastSpeed = 0;
volatile int firstTime_done = 0;
volatile int RPMs_areZero = 0;

volatile int last_SpeedingTicket_Speed = 0;

//initilaize hall effect output pin as input with interrupt enable 
void Hall_Effect_Out_init(void)
{
	P5->SEL0 &= ~BIT2;
	P5->SEL1 &= ~BIT2;
	P5->DIR &= ~BIT2;       //Set as input
	P5->IES |= BIT2;        //Set to trigger interrupt from high to low
	P5->IE |= BIT2;         //Enables interrupt
	P5->IFG &= ~BIT2;       //Clears interrupt flag
}

//function returns the calculated RPMs from revolutions sensed by the hall effect
int measure_RPMs(int Revolution_count) //checks every 10 seconds, SysTick Interrupt == 5
{
    int RPMs;

	RPMs = Revolution_count / 2 * 60; //60 seconds per minute divided by 1 second = 60 intervals / min
									  //^^^ this will have to be changed depending on how long I check for it

    return RPMs;
}

//fucntion returns speed using the RPMs measured and the circumference of the wheel from which we measure 
float measure_Speed(int RPMs, float circumference)
{
    int speed = 0;

    speed = RPMs * circumference * 60; //revs per minute * circumference in miles * 60 (min / hour)
    return speed;
}
