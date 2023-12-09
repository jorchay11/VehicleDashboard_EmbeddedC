/*
 * EEPROM.c
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */


#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "EEPROM.h"
#include "I2C_ProjectFunctions.h"
#include "ST7735.h"
#include "Project.h"

volatile int Alarm_position = 0;

//not used
void store_Date_and_Time_EEPROM_withType(unsigned char* Time_Date, int count,
		int type)
{
	if (type == 0)			//if type is zero it is a collision
			{
		;
	}

	if (type == 1) {
		count += 5;
	}

    uint8_t start_memAddr = (count * 8); //move to next "drawer", addresses are 7 bytes long so multiply by 8 to jump to next
                                         //will write to 0, 8, 16, 24, 32
    I2C1_init();

    I2C1_burstWrite(slaveAddrA0, start_memAddr, 7, Time_Date);
}

//reads the RTC time and stores it in the EEPROM
//address stored determined by count
void Read_RTC_Store_Alarm_EEPROM(int count) {
	unsigned char Time_Date_toSend_EEPROM[7];

	I2C1_init();

	I2C1_burstRead(slaveAddrRTC, 0, 7, Time_Date_toSend_EEPROM); //read all 7 values from RTC

	store_Date_and_Time_EEPROM(Time_Date_toSend_EEPROM, count); //send to store in EEPROM, button count decides which address it writes to
}

void store_Date_and_Time_EEPROM(unsigned char* Time_Date, int Alarm_count) {

	
	uint8_t start_memAddr = (Alarm_count * 8); //move to next "drawer", addresses are 7 bytes long so multiply by 8 to jump to next
	//will write to 0, 8, 16, 24, 32
	I2C1_init();

	I2C1_burstWrite(slaveAddrA0, start_memAddr, 7, Time_Date);
}
/////////////////////////////////////////////////////////////////////////////////////

//not used
void read_Date_and_Time_EEPROM_withType(int count, int type)	//edit
{
    unsigned char Time_Date_toRead[7];       //create array to store read values

    uint8_t start_memAddr = (count * 8);

    I2C1_init();

    I2C1_burstRead(slaveAddrA0, start_memAddr, 7, Time_Date_toRead); //put watch expression here

    print_Alarm_LCD(Time_Date_toRead, count);
}

//reads the times that alarms have been stored into the EEPROM
//reads 5 alarms
void read_Date_and_Time_EEPROM(int Alarm_count) {
	unsigned char Time_Date_toRead[7];       //create array to store read values

	uint8_t start_memAddr = (Alarm_count * 8);

	I2C1_init();

	I2C1_burstRead(slaveAddrA0, start_memAddr, 7, Time_Date_toRead); //put watch expression here

	print_Alarm_LCD(Time_Date_toRead, Alarm_count);
}

//prints alarms read from the EEPROM to the LCD in readable decimal numbers and format
void print_Alarm_LCD(unsigned char* Time_Date_Array, int Alarm_count)
{
    //uint8_t year = Time_Date_Array[6];     local variable to hold the year byte
    //uint8_t month = Time_Date_Array[5];    local variable to hold the month byte
    //uint8_t date = Time_Date_Array[4];     local variable to hold the date byte
    //uint8_t day = Time_Date_Array[3];      local variable to hold the day byte
    //hours = Time_Date_Array[2];
    //minutes = Time_Date_Array[1];
    //seconds = Time_Date_Array[0];

	uint16_t start_PositionY;
	uint16_t start_PositionX;

	if (Alarm_count == 0) {
		start_PositionY = 30;
		start_PositionX = 20;
	}
	if (Alarm_count == 1) {
		start_PositionY = 42;
		start_PositionX = 20;
	}
	if (Alarm_count == 2) {
		start_PositionY = 54;
		start_PositionX = 20;
	}
	if (Alarm_count == 3) {
		start_PositionY = 66;
		start_PositionX = 20;
	}
	if (Alarm_count == 4) {
		start_PositionY = 78;
		start_PositionX = 20;
	}

    uint16_t text_Color = 0xF81F;           //Purple
    uint16_t bg_Color = 0;                  //black

    uint8_t num_toPrint_ones;               //variable printed to LCD ones place
    uint8_t num_toPrint_tens;               //variable printed to LCD tens place

    int i = 0;

    unsigned char Date_Array[5] = { Time_Date_Array[5], Time_Date_Array[4],
                                    Time_Date_Array[2], Time_Date_Array[1],
                                    Time_Date_Array[0] }; //fills array with desired values for lab

    for (i = 0; i < 5; i++)
    {

		num_toPrint_tens = ((Date_Array[i] & 0xF0) >> 4) + '0'; //convert to decimal and print character value

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_tens,
                        text_Color, bg_Color, 1);

        start_PositionX += 6;

        num_toPrint_ones = (Date_Array[i] & 0x0F) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_ones,
                        text_Color, bg_Color, 1);

        start_PositionX += 6;

		if (i != 4) //print a semicolon after each number print until the last one
        {
            ST7735_DrawChar(start_PositionX, start_PositionY, ':', text_Color,
                            bg_Color, 1);
            start_PositionX += 6;
        }
    }

}
