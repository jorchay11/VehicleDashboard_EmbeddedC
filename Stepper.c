/*
 * Stepper.c
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "Stepper.h"
#include "SysTick_Library.h"


void Stepper1_Motor1_Pin1(void)
{
    P4->SEL0 &= ~BIT0;          //Set to GPIO Pin 2.3, IN1of motor
    P4->SEL1 &= ~BIT0;
    P4->DIR |= BIT0;            //Set to Output
    P4->OUT &= ~BIT0;           //turn off on initialize
}

void Stepper1_Motor1_Pin2(void)
{
    P4->SEL0 &= ~BIT1;          //Set to GPIO Pin 2.4, IN2
    P4->SEL1 &= ~BIT1;
    P4->DIR |= BIT1;            //Set to Output
    P4->OUT &= ~BIT1;           //turn off on initialize

}

void Stepper1_Motor2_Pin1(void)
{
    P4->SEL0 &= ~BIT2;          //Set to GPIO Pin 2.5, IN3
    P4->SEL1 &= ~BIT2;
    P4->DIR |= BIT2;            //Set to Output
    P4->OUT &= ~BIT2;           //turn off on initialize
}

void Stepper1_Motor2_Pin2(void)
{
    P4->SEL0 &= ~BIT3;          //Set to GPIO Pin 2.6, IN4
    P4->SEL1 &= ~BIT3;
    P4->DIR |= BIT3;            //Set to Output
    P4->OUT &= ~BIT3;           //turn off on initialize
}

//initialize speedometer stepper motor by calling all initializations for 4 pins
void Stepper1_Motor()
{
    Stepper1_Motor1_Pin1();
    Stepper1_Motor1_Pin2();
    Stepper1_Motor2_Pin1();
    Stepper1_Motor2_Pin2();
    Rotate_Stepper1_Forward_s_Steps(120);

    Rotate_Stepper1_Backward_s_Steps(120);
}

//turns the GPIO pins of motor on/off in  predetermined sequence using macros defined in .h
void Rotate_Stepper1_Forward_s_Steps(int s)
{
	STEPPER_PINS_OFF
	//macro to turn stepper pins off before
	;

    int i;
    for (i = s; i > 0; i--)                //each loop = 1 step, loops 's' times
    {
        STEP1; //each macro is predefined as a fraction of a step, there are four, shown in Report
		SysTick_delay_ms(2);                         //delay a little between
        STEP2;
		SysTick_delay_ms(2);
        STEP3;
		SysTick_delay_ms(2);
        STEP4;
		SysTick_delay_ms(2);
    }
    STEPPER_PINS_OFF
    ;
}

//reverses forward step rotation, rotation motor backward s steps
void Rotate_Stepper1_Backward_s_Steps(int s)
{
	STEPPER_PINS_OFF
	;

    int i;
    for (i = s; i > 0; i--)                //each loop = 1 step, loops 's' times
    {
        STEP4;                                  //reverse steps to go backward
		SysTick_delay_ms(2);
        STEP3;
		SysTick_delay_ms(2);
        STEP2;
		SysTick_delay_ms(2);
        STEP1;
		SysTick_delay_ms(2);
    }
	STEPPER_PINS_OFF
	//turn pins off after rotation
    ;
}

//initialization of stepper motor pins
//GPIO outputs
void Stepper2_Motor1_Pin1(void)
{
    P4->SEL0 &= ~BIT0;          //Set to GPIO Pin 2.3, IN1of motor
    P4->SEL1 &= ~BIT0;
    P4->DIR |= BIT0;            //Set to Output
    P4->OUT &= ~BIT0;           //turn off on initialize
}

void Stepper2_Motor1_Pin2(void)
{
    P4->SEL0 &= ~BIT1;          //Set to GPIO Pin 2.4, IN2
    P4->SEL1 &= ~BIT1;
    P4->DIR |= BIT1;            //Set to Output
    P4->OUT &= ~BIT1;           //turn off on initialize

}

void Stepper2_Motor2_Pin1(void)
{
    P4->SEL0 &= ~BIT2;          //Set to GPIO Pin 2.5, IN3
    P4->SEL1 &= ~BIT2;
    P4->DIR |= BIT2;            //Set to Output
    P4->OUT &= ~BIT2;           //turn off on initialize
}

void Stepper2_Motor2_Pin2(void)
{
    P4->SEL0 &= ~BIT3;          //Set to GPIO Pin 2.6, IN4
    P4->SEL1 &= ~BIT3;
    P4->DIR |= BIT3;            //Set to Output
    P4->OUT &= ~BIT3;           //turn off on initialize
}

///////////////////////////////
/*
 * 		Initialization of the test stepper for speed checking
 */
//////////////////////////////
void TESTStepper_Motor()
{
    TESTStepper_Motor1_Pin1();
    TESTStepper_Motor1_Pin2();
    TESTStepper_Motor2_Pin1();
    TESTStepper_Motor2_Pin2();
}

void TESTStepper_Motor1_Pin1(void)
{
    P9->SEL0 &= ~BIT0;          //Set to GPIO Pin 2.3, IN1of motor
    P9->SEL1 &= ~BIT0;
    P9->DIR |= BIT0;            //Set to Output
    P9->OUT &= ~BIT0;           //turn off on initialize
}

void TESTStepper_Motor1_Pin2(void)
{
    P9->SEL0 &= ~BIT1;          //Set to GPIO Pin 2.4, IN2
    P9->SEL1 &= ~BIT1;
    P9->DIR |= BIT1;            //Set to Output
    P9->OUT &= ~BIT1;           //turn off on initialize

}

void TESTStepper_Motor2_Pin1(void)
{
    P9->SEL0 &= ~BIT2;          //Set to GPIO Pin 2.5, IN3
    P9->SEL1 &= ~BIT2;
    P9->DIR |= BIT2;            //Set to Output
    P9->OUT &= ~BIT2;           //turn off on initialize
}

void TESTStepper_Motor2_Pin2(void)
{
    P9->SEL0 &= ~BIT3;          //Set to GPIO Pin 2.6, IN4
    P9->SEL1 &= ~BIT3;
    P9->DIR |= BIT3;            //Set to Output
    P9->OUT &= ~BIT3;           //turn off on initialize
}

void TEST_Stepper_Rotate(int s)
{

    int i;
    for (i = s; i > 0; i--)                //each loop = 1 step, loops 's' times
    {
        P9->OUT &= ~0x0F;
    TESTSTEP1; //each macro is predefined as a fraction of a step, there are four, shown in Report
        SysTick_delay_ms(10);                          //delay a little between
        //TESTSTEP2;
        //    SysTick_delay_us(200);
        P9->OUT &= ~0x0F;

    TESTSTEP3;
        SysTick_delay_ms(10);
        //TESTSTEP4;
        //    SysTick_delay_us(200);
        P9->OUT &= ~0x0F;
        TESTSTEP5;
        SysTick_delay_ms(10);
        //     TESTSTEP6;
        //    SysTick_delay_us(200);
        P9->OUT &= ~0x0F;
        TESTSTEP7;
        SysTick_delay_ms(10);
        //     TESTSTEP8;
        //    SysTick_delay_us(200);
    }
        
}

//initialiizes tachometer
//unused because I did not have time to find an unused timerA that did not affect other components
void Tachometer_Init(void) {
	P5->SEL0 |= BIT7;
	P5->SEL1 &= ~BIT7;
	P5->DIR |= BIT7;

	TIMER_A2->CTL = 0x0294;
	TIMER_A2->CCR[0] = 60000;
	TIMER_A2->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;
	TIMER_A2->CCR[2] = 6000;
	SysTick_delay_ms(10);
}

//sets servo position depending on the RPMs calculated
void TachometerSpeed(int rotations) {
	int duty = rotations / 20;
	TIMER_A2->CCR[2] = 6000 - ((duty * 6000) / 35);
}



