/*
 * EEPROM.h
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define slaveAddrA0 0b1010001

extern volatile int alarm_number;

void store_Date_and_Time_EEPROM(unsigned char* Time_Date, int count); //stores  character array to EEPROM using I2C

void read_Date_and_Time_EEPROM(int count); //read charater array from EEPROM using I2C

void print_Alarm_LCD(unsigned char* Time_Date_Array, int Alarm_count); //prints the Alarm read from the EEPROM in the labs desired format
void Read_RTC_Store_Alarm_EEPROM(int count);

#endif /* EEPROM_H_ */
