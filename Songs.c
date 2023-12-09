/*
 * Songs.c
 *
 *  Created on: Dec 3, 2020
 *      Author: 7jorchay
 */




#include "msp.h"
#include <stdint.h>
#include "SysTick_Library.h"
#include "Songs.h"

//Note frequencies in .h

uint8_t song = 0;

void Speaker(int Note, int on) { //on = 2, if you want it off, call Speaker(#, 0)

	int freq = 3000000 / Note;

	P7->SEL0 |= BIT7;                               //Set TimerA

	P7->SEL1 = 0;

	P7->DIR |= BIT7;                                //Set P2.4 for OutPut

	TIMER_A1->CCR[0] = freq;                       //Period based off note input

	TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCR1 reset

	TIMER_A1->CCR[1] = freq / on;                   //PWM duty cycle

	TIMER_A1->CTL = 0x0214;                 //SMCLK, Up Mode, Clear TAR to start

}

void Speaker_3MHz(int Note, int on) {//on = 2, if you want it off, call Speaker(#, 0)

	int freq = 3000000 / Note;

	P7->SEL0 |= BIT7;                               //Set TimerA

	P7->SEL1 = 0;

	P7->DIR |= BIT7;                                //Set P2.4 for OutPut

	TIMER_A1->CCR[0] = freq;                       //Period based off note input

	TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCR1 reset

	TIMER_A1->CCR[1] = freq / on;                   //PWM duty cycle

	TIMER_A1->CTL = 0x0214;                 //SMCLK, Up Mode, Clear TAR to start

}

//initializes speaker for a 12MHz clock
void Speaker_12MHz(int Note, int on) {

	int freq = 12000000 / Note;

	P5->SEL0 |= BIT6;                               //Set TimerA

	P5->SEL1 &= ~BIT6;

	P5->DIR |= BIT6;                                //Set P2.4 for OutPut

	TIMER_A2->CCR[0] = freq;                       //Period based off note input

	TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //CCR1 reset

	TIMER_A2->CCR[1] = freq / on;                   //PWM duty cycle

	TIMER_A2->CTL = 0x0214;                 //SMCLK, Up Mode, Clear TAR to start
}

void Bells(int repeats) {

	int i;

	for (i = repeats; i > 0; i--) {              //For loop allows for repeating

		Speaker_12MHz(D_5, 2);                    //Note frequency D_5 plays

		SysTick_delay_ms(50);               //Delays for 50ms

		Speaker_12MHz(C_5, 2);                    //Plays note frequency C_5

		SysTick_delay_ms(50);               //Delays 50ms
	}
	Speaker_12MHz(0, 0);                       //Speaker off
}

void Pirates_C(void) {

	//FIRST PART
	Speaker(E_4, 2);
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(G_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT2;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(250);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(250);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(G_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT2;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(375);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);
	//END OF PART 1

	//PART 2
	Speaker(E_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(G_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT2;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(250);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(250);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(G_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT2;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(375);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);
	//END OF PART 2

	//PART 3
	Speaker(E_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(G_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT2;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(E_5, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(F_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT3 );
	SysTick_delay_ms(250);
	Speaker(F_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(E_5, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(E_5, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);
	//END OF PART 3

	//PART 4
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(250);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(D_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(E_5, 2);
	P3->OUT = 0;
	P3->OUT |= BIT0;
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(250);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(250);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= BIT3;
	SysTick_delay_ms(125);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(125);

	Speaker(C_5, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 );
	SysTick_delay_ms(125);
	Speaker(A_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(125);
	Speaker(B_4, 2);
	P3->OUT = 0;
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 );
	SysTick_delay_ms(375);
	Speaker(0, 0);
	P3->OUT = 0;
	SysTick_delay_ms(1000);
	//END OF PART 4

}

void Despacito(void) {

	Speaker(D_5, 2);
	P3->OUT |= BIT0;                     //Blue
	SysTick_delay_ms(575);
	Speaker(0, 0);
	P3->OUT = 0;                         //Off
	SysTick_delay_ms(200);
	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(575);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(200);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                         //Off
	SysTick_delay_ms(50);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);

	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);
	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(G_4, 2);
	P3->OUT |= (BIT0 | BIT7 );           //Blue and Red
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);

	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(D_5, 2);
	P3->OUT |= BIT0;                     //Blue
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(A_4, 2);
	P3->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 ); //All
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);

	Speaker(D_5, 2);
	P3->OUT |= BIT0;                    //Blue
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(D_5, 2);
	P3->OUT |= BIT0;                    //Blue
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);

	Speaker(D_5, 2);
	P3->OUT |= BIT0;                    //Blue
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

	Speaker(E_5, 2);
	P3->OUT |= (BIT3 | BIT5 );           //Red and Yellow
	SysTick_delay_ms(145);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(50);
	Speaker(E_5, 2);
	P3->OUT |= (BIT3 | BIT5 );           //Red and Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);
	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(575);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(400);

	Speaker(D_5, 2);
	P3->OUT |= BIT0;                    //Blue
	SysTick_delay_ms(575);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(200);
	Speaker(C_S5, 2);
	P3->OUT |= BIT2;                    //Orange
	SysTick_delay_ms(575);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(200);
	Speaker(B_4, 2);
	P3->OUT |= BIT3;                    //Yellow
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);
	Speaker(F_S4, 2);
	P3->OUT |= (BIT5 | BIT6 | BIT7 );    //Reds
	SysTick_delay_ms(290);
	Speaker(0, 0);
	P3->OUT = 0;                        //Off
	SysTick_delay_ms(100);

}

