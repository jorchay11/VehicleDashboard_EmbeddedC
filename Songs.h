/*
 * Songs.h
 *
 *  Created on: Dec 3, 2020
 *      Author: 7jorchay
 */

#ifndef SONGS_H_
#define SONGS_H_

#include "msp.h"
#include <stdint.h>

//Note frequencies
#define A_2     110
#define A_S2    117
#define B_2     123
#define C_3     131
#define C_S3    139
#define D_3     147
#define D_S3    156
#define E_3     165
#define F_3     175
#define F_S3    185
#define G_3     196
#define G_S3    208
#define A_3     220
#define A_S3    233
#define B_3     247
#define C_4     262
#define C_S4    277
#define D_4     294
#define D_S4    311
#define E_4     330
#define F_4     349
#define F_S4    370
#define G_4     392
#define G_S4    415
#define A_4     440
#define A_S4    466
#define B_4     494
#define C_5     523
#define C_S5    554
#define D_5     587
#define D_S5    622
#define E_5     659
#define F_5     698
#define F_S5    740
#define G_5     784
#define G_S5    831
#define A_5     880
#define A_S5    932
#define B_5     988

#define WHOLE 4
#define HALF 2
#define QUARTER 1

extern uint8_t song;

/********************* Function Prototypes *****************************
 ************************************************************************/
void Speaker(int Note, int on);
void Speaker_3MHz(int Note, int on);
void Speaker_12MHz(int Note, int on);

void Pirates_C(void);
void Despacito(void);
void Bells(int repeats);
/********************* Function Prototypes *****************************
 ************************************************************************/



#endif /* SONGS_H_ */
