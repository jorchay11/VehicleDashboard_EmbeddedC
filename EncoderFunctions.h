/*
 * EncoderFunctions.h
 *
 *  Created on: Nov 26, 2020
 *      Author: 7jorchay
 */

#ifndef ENCODERFUNCTIONS_H_
#define ENCODERFUNCTIONS_H_

extern volatile int Encoder_Btn_pressed;
extern volatile int Encoder_Btn_interruptFlag;

extern volatile int CCW_turns;                         //variable for CCW pulses
extern volatile int CW_turns;                          //variable for CW pulses
extern volatile int total_turns; //number of turns in either direction and magnitude of difference
extern volatile int direction;                //if total is CW, = 0, if CCW, = 1

extern volatile int DT;                               //DT variable, starts high
extern volatile int CLK;                             //CLK variable, starts high

extern volatile int Encoder_Btn_count;
extern volatile int cursor_moved;
void Encoder_init(void);

void Encoder_Btn_init(void);
void Encoder_CLK_init(void);
void Encoder_DT_init(void);


void reset_Encoder_Counts(void);

void control_Cursor(void);

int Debounce_EncoderButton(void);

void reset_Encoder_Counts(void);

void PORT1_IRQHandler(void);


#endif /* ENCODERFUNCTIONS_H_ */
