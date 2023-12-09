/*
 * RTC.h
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */

#ifndef RTC_H_
#define RTC_H_

extern char *Message1;

void Read_RTC();

void Write_New_Time_RTC(unsigned char* New_Time_Array);

void Write_New_Date_RTC(unsigned char* New_Time_Array);

void Read_RTC_Temperature_Print_toLCD();

void print_Temperature_toLCD(unsigned char* Temperature_Array);

void Read_RTC_Print_Date_toLCD();

void print_Date_LCD(unsigned char* Time_Date_Array);

void Read_RTC_Print_Time_toLCD();

void print_Time_LCD(unsigned char* Time_Date_Array);

#endif /* RTC_H_ */
