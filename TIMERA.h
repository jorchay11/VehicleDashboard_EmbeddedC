/*
 * TIMERA.h
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */

#ifndef TIMERA_H_
#define TIMERA_H_



//////////////////////////////////////////
/*
 * PWM Modes
 */
//////////////////////////////////////////
void TIMERA0_PWM_init(uint16_t period);

void TIMERA0_1_PWM_Init(uint16_t duty1); //use for blinkers

void TIMERA0_2_PWM_Init(uint16_t duty2);



//void TimerA0_1_PWM_init(float dutyCycle);


//////////////////////////////////////////////////////////////////////////////
/*
 *                              Timer Mode
 */
/////////////////////////////////////////////////////////////////////////////
/*
 *                           Initializations
 */
////////
void TIMERA1_0_oneMSInterval_init();

void TIMERA1_0_Timer_withInterrupts(); //unused

void TIMERA3_0_HalfSecondInterval_init();

void TIMERA3_1_oneMSInterval_init();

////////////////////////////////////////////////////////////////////////////
/*
 *                           Handlers
 */
////////////////////////////////////////////////////////////////////////////
void TA0_0_IRQHandler(void);

void TA1_0_IRQHandler(void);

void TA2_0_IRQHandler(void);

void TA3_0_IRQHandler(void);

#endif /* TIMERA_H_ */
