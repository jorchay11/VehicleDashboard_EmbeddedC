/*
 * Stepper.h
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#ifndef STEPPER_H_
#define STEPPER_H_

extern volatile int Current_Speed_mph;

#define STEP1   P4->OUT = (BIT1 | BIT3)          //in2 | in4    0x0A
#define STEP2   P4->OUT = (BIT3 | BIT0)          //in4 | in1    0x
#define STEP3   P4->OUT = (BIT0 | BIT2)          //in1 | in3
#define STEP4   P4->OUT = (BIT1 | BIT2)          //in2 | in3
#define STEPPER_PINS_OFF    P4->OUT = 0;

#define TESTSTEP1   P9->OUT = 0x0E          //in1 | in2 | in3
#define TESTSTEP2   P9->OUT = 0x0C          //in1 | in2
#define TESTSTEP3   P9->OUT = 0x0D          //in1 | in2 | in4
#define TESTSTEP4   P9->OUT = 0x09          //in1 | in4
#define TESTSTEP5   P9->OUT = 0x0B          //in1 | in3 | in4
#define TESTSTEP6   P9->OUT = 0x03          //in3 | in4
#define TESTSTEP7   P9->OUT = 0x07          //in2 | in3 | in4
#define TESTSTEP8   P9->OUT = 0x06          //in2 | in3

void Stepper1_Motor1_Pin1();
void Stepper1_Motor1_Pin2();
void Stepper1_Motor2_Pin1();
void Stepper1_Motor2_Pin2();
void Stepper1_Motor();                     //calls all motor pin initializations
void Rotate_Stepper1_Forward_s_Steps(int s);     //runs forward stepper sequence
void Rotate_Stepper1_Backward_s_Steps(int s);   //runs backward stepper sequence


void TESTStepper_Motor(void);
void TESTStepper_Motor1_Pin1(void);
void TESTStepper_Motor1_Pin2(void);
void TESTStepper_Motor2_Pin1(void);
void TESTStepper_Motor2_Pin2(void);

void Tachometer_Init(void);
void TachometerSpeed(int rotations);

void TEST_Stepper_Rotate(int s);

#endif /* STEPPER_H_ */
