/*
 * HC-SR04.h
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

extern volatile int object_DistanceFromSensor;        //distance variable
extern volatile float conversion_ToInches;      //echo signal to inches = 148.0;

void HCSR04_init();              //P2.4

void measure_Period();


#endif /* HC_SR04_H_ */
