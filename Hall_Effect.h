/*
 * Hall_Effect.h
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#ifndef HALL_EFFECT_H_
#define HALL_EFFECT_H_

extern volatile int Current_Speed_mph;
extern volatile int RPMs;
extern volatile int Revolution_count;
extern volatile int speed_Dif;
extern volatile int lastSpeed;
extern volatile int firstTime_done;
extern volatile int RPMs_areZero;
extern volatile int last_SpeedingTicket_Speed;

void Hall_Effect_Out_init(void);

int measure_RPMs(int Revolution_count);
float measure_Speed(int RPMs, float circumference);

#endif /* HALL_EFFECT_H_ */
