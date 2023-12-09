/*
 * ProjectButtons.h
 *
 *  Created on: Nov 26, 2020
 *      Author: 7jorchay
 */

#ifndef PROJECTBUTTONS_H_
#define PROJECTBUTTONS_H_

extern volatile int Open_Door_Btn_pressed;
extern volatile int NewRTC_Info_Btn_pressed;
extern volatile int WatchdogReset_Btn_pressed;

void Blinkers_Toggle_Switch_init(void);

void Left_Blinker(void);                         //left blinker
void Right_Blinker(void);                        //right blinker
void LED_closeProximity();

void openDoor_Btn_init(void);
int Debounce_DoorOpen_Btn();

void NewRTC_Info_Btn();
int Debounce_NewRTC_Info_Btn();

void WatchdogReset_Btn();
int Debounce_WatchodgReset_Btn();


#endif /* PROJECTBUTTONS_H_ */
