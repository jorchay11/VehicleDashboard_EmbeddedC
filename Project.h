/*
 * Project.h
 *
 *  Created on: Nov 26, 2020
 *      Author: 7jorchay
 */

#ifndef PROJECT_H_
#define PROJECT_H_

#define TOGGLE_P20  P2 -> OUT ^= BIT0
#define TOGGLE_Left_Blinker  P8 -> OUT ^= BIT3
#define TOGGLE_Right_Blinker  P8 -> OUT ^= BIT4
#define BLINKERS_OFF P8 -> OUT &= ~(BIT3 | BIT4)

#define CIRCUMFERENCE_6ftDiameter .0072        // 6(ft) * pi
#define CIRCUMFERENCE_3inRad .0002974   //circumference in miles of 3 inch radius wheel

#define slaveAddrRTC 0x68
extern volatile int Port1_interruptFlag;
extern volatile int Enter_Date_Time_Btn_count;

extern volatile uint8_t New_RTC_Time_Hour_tens;
extern volatile uint8_t New_RTC_Time_Hour_ones;
extern volatile uint8_t New_RTC_Time_Minute_tens;
extern volatile uint8_t New_RTC_Time_Minute_ones;

extern unsigned char Time_toSend[4];
extern unsigned char Date_toSend[5];

extern volatile uint8_t New_RTC_Date_Day_ones;
extern volatile uint8_t New_RTC_Date_Date_tens;
extern volatile uint8_t New_RTC_Date_Date_ones;
extern volatile uint8_t New_RTC_Date_Month_tens;
extern volatile uint8_t New_RTC_Date_Month_ones;
extern volatile uint8_t New_RTC_Date_Year_tens;
extern volatile uint8_t New_RTC_Date_Year_ones;

void Clock_Init48MHz(void);
void Enable_Interrupts(void);

void PORT1_IRQHandler(void);
void PORT3_IRQHandler(void);
void PORT5_IRQHandler(void);

void TA3_0_IRQHandler(void);
void TA0_0_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);

void Project_init(void);
void reset_Encoder_Counts(void);
void control_Cursor(void);

void DoorAjar_Btn_HeldDown(void);

void CONTROL_BLINKERS();
void PROXIMITY(void);
void main();

#endif /* PROJECT_H_ */
