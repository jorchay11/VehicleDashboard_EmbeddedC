/*
 * RTC_ProjectFunctions.c
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */

#include "RTC_ProjectFunctions.h"
#include "I2C_ProjectFunctions.h"
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "ST7735.h"
#include "Project.h"
#include "SysTick_Library.h"

//char *Message1 = "WARNING\0";


void Read_RTC(unsigned char* Time_Date_Array) //Can only use this if global works.
{                         //functions to print will no longer accept a parameter
    I2C1_init();
    I2C1_burstRead(slaveAddrRTC, 0, 7, Time_Date_Array);
}

//writes 3 bytes of a new time to RTC using I2C to correspinding bytes
void Write_New_Time_RTC(unsigned char* New_Time_Array) {

	I2C1_init();

	I2C1_burstWrite(slaveAddrRTC, 0, 3, New_Time_Array);
}

//writes a new date to RTC with 4 bytes to corresponding bytes
void Write_New_Date_RTC(unsigned char* New_Date_Array) {

	I2C1_init();

	I2C1_burstWrite(slaveAddrRTC, 3, 4, New_Date_Array);
}

//reads the RTC date and calls function to print to LCD
void Read_RTC_Print_Date_toLCD()
{
    I2C1_init();

    unsigned char RTC_Date_Time[7];

    I2C1_burstRead(slaveAddrRTC, 0, 7, RTC_Date_Time);

    print_Date_LCD(RTC_Date_Time);
}

//reads RTC time bytes andcalls a function to print to LCD
void Read_RTC_Print_Time_toLCD()
{
    I2C1_init();

    unsigned char RTC_Date_Time[7]; 

    I2C1_burstRead(slaveAddrRTC, 0, 7, RTC_Date_Time);

    print_Time_LCD(RTC_Date_Time);
}

//Reads RTC temperature bytes and calls function to print to LCD in a readable way
void Read_RTC_Temperature_Print_toLCD()
{
    I2C1_init();

    unsigned char Temp_toPrint[2];        //reading 2 bytes, 1 place for ack bit

    I2C1_burstRead(slaveAddrRTC, 17, 2, Temp_toPrint); //address of temp MSB is 0x11 = 17, 2 bytes

    print_Temperature_toLCD(Temp_toPrint);

}

//prints date to LCD using data read from RTC
//converts read bytes to character values to print
void print_Date_LCD(unsigned char* Time_Date_Array)                          ///
{

    unsigned char Date_Array[4] = { Time_Date_Array[5], Time_Date_Array[4],
                                    Time_Date_Array[6], NULL };
	unsigned char Day_ofWeek[11];

	//char *Day_ofWeek;

	uint16_t start_PositionX = 45;
	uint16_t start_PositionY = 40;

    uint16_t text_Color = 0xF81F;       //Purple
    uint16_t bg_Color = 0;              //black

    uint8_t num_toPrint_ones;
    uint8_t num_toPrint_tens;

    int i = 0;

	if (Time_Date_Array[3] == 0x01) {
		Day_ofWeek[0] = 'S';
		Day_ofWeek[1] = 'u';
		Day_ofWeek[2] = 'n';
		Day_ofWeek[3] = 'd';
		Day_ofWeek[4] = 'a';
		Day_ofWeek[5] = 'y';
		Day_ofWeek[7] = ',';
		Day_ofWeek[6] = NULL;
		text_Color = ST7735_YELLOW; //yellow
	}

	else if (Time_Date_Array[3] == 0x02) {
		Day_ofWeek[0] = 'M';
		Day_ofWeek[1] = 'o';
		Day_ofWeek[2] = 'n';
		Day_ofWeek[3] = 'd';
		Day_ofWeek[4] = 'a';
		Day_ofWeek[5] = 'y';
		Day_ofWeek[6] = ',';
		Day_ofWeek[7] = NULL;
		text_Color = 0x0015;        //dull red
	}

	else if (Time_Date_Array[3] == 0x03) {
		Day_ofWeek[0] = 'T';
		Day_ofWeek[1] = 'u';
		Day_ofWeek[2] = 'e';
		Day_ofWeek[3] = 's';
		Day_ofWeek[4] = 'd';
		Day_ofWeek[5] = 'a';
		Day_ofWeek[6] = 'y';
		Day_ofWeek[7] = ',';
		Day_ofWeek[8] = NULL;
		text_Color = 0x07EA;        //lime green
	}

	else if (Time_Date_Array[3] == 0x04) {
		Day_ofWeek[0] = 'W';
		Day_ofWeek[1] = 'e';
		Day_ofWeek[2] = 'd';
		Day_ofWeek[3] = 'n';
		Day_ofWeek[4] = 'e';
		Day_ofWeek[5] = 's';
		Day_ofWeek[6] = 'd';
		Day_ofWeek[7] = 'a';
		Day_ofWeek[8] = 'y';
		Day_ofWeek[9] = ',';
		Day_ofWeek[10] = NULL;
		text_Color = ST7735_RED;
	}

	else if (Time_Date_Array[3] == 0x05) {
		Day_ofWeek[0] = 'T';
		Day_ofWeek[1] = 'h';
		Day_ofWeek[2] = 'u';
		Day_ofWeek[3] = 'r';
		Day_ofWeek[4] = 's';
		Day_ofWeek[5] = 'd';
		Day_ofWeek[6] = 'a';
		Day_ofWeek[7] = 'y';
		Day_ofWeek[8] = ',';
		Day_ofWeek[9] = NULL;
		text_Color = 0xF800;
	}

	else if (Time_Date_Array[3] == 0x06) {
		Day_ofWeek[0] = 'F';
		Day_ofWeek[1] = 'r';
		Day_ofWeek[2] = 'i';
		Day_ofWeek[3] = 'd';
		Day_ofWeek[4] = 'a';
		Day_ofWeek[5] = 'y';
		Day_ofWeek[6] = ',';
		Day_ofWeek[7] = NULL;
		text_Color = ST7735_BLUE;
	}

	else if (Time_Date_Array[3] == 0x07) {
		Day_ofWeek[0] = 'S';
		Day_ofWeek[1] = 'a';
		Day_ofWeek[2] = 't';
		Day_ofWeek[3] = 'u';
		Day_ofWeek[4] = 'r';
		Day_ofWeek[5] = 'd';
		Day_ofWeek[6] = 'a';
		Day_ofWeek[7] = 'y';
		Day_ofWeek[8] = ',';
		Day_ofWeek[9] = NULL;
		text_Color = ST7735_CYAN;
	}


	for (i = 0; Day_ofWeek[i] != NULL; i++) {
		ST7735_DrawChar(start_PositionX, 30, Day_ofWeek[i], text_Color,
				bg_Color, 1);

		start_PositionX += 6;
	}


    text_Color = 0xF81F;       //Purple

	//start_PositionY += 10;
    start_PositionX = 40;

    for (i = 0; Date_Array[i] != NULL; i++)
    {
        num_toPrint_tens = ((Date_Array[i] & 0xF0) >> 4) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_tens,
                        text_Color, bg_Color, 1);

        start_PositionX += 6;

        num_toPrint_ones = (Date_Array[i] & 0x0F) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_ones,
                        text_Color, bg_Color, 1);

        start_PositionX += 6;

        if (i != 2)
        {
            ST7735_DrawChar(start_PositionX, start_PositionY, '/', text_Color,
                            bg_Color, 1);
            start_PositionX += 6;
        }
    }

}

//prints time to LCD using data read from RTC
//converts read bytes to character values to print
void print_Time_LCD(unsigned char* Time_Date_Array)
{
    //Time_Data_array[0] = seconds
    //Time_Data_array[1] = minues
    //Time_Data_array[2] = hours

//    uint16_t start_PositionX = 40;			//centers time under Date
//    uint16_t start_PositionY = 42;
//    uint16_t text_Color = 0xF800;            //blue
//    uint16_t bg_Color = 0;                   //white

	uint16_t start_PositionX = 20;				//makes time Lower and Bigger
	uint16_t start_PositionY = 60;
    uint16_t text_Color = 0xF800;            //blue
	uint16_t bg_Color = 0;                   //black


    uint8_t num_toPrint_ones;
    uint8_t num_toPrint_tens;

    int i = 0;

    for (i = 2; i >= 0; i--)
    {
        num_toPrint_tens = ((Time_Date_Array[i] & 0xF0) >> 4) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_tens,
				text_Color, bg_Color, 2);

		start_PositionX += 12;

        num_toPrint_ones = (Time_Date_Array[i] & 0x0F) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY, num_toPrint_ones,
				text_Color, bg_Color, 2);

		start_PositionX += 12;

        if (i != 0)
        {
            ST7735_DrawChar(start_PositionX, start_PositionY, ':', text_Color,
					bg_Color, 2);
			start_PositionX += 12;
        }
    }
}

//takes data from RTC and converts it to fahrenheit, then converts to charater value to be printed
//can print 2 or 3 digit temperature
//high cabin temp warning will be displayed
void print_Temperature_toLCD(unsigned char* Temperature_Array)
{
    int temp_tens;                         //local variable for MSB of temp byte
    int temp_ones;          //local variable for LSB of temp byte, .25 precision

    uint16_t start_PositionX = 2;
    uint16_t start_PositionY = 12;
    uint16_t text_Color = ST7735_YELLOW;            //yellow

    uint16_t bg_Color = 0;                          //white

    uint8_t num_toPrint_ones;
    uint8_t num_toPrint_tens;
    uint8_t num_toPrint_hundreds;


    float temp_Celsius;     //variable to combine the bytes into a 10 bit number
    float temp_Fahrenheit;

    temp_tens = Temperature_Array[0] << 2; //ints are 32 bit, we can set it to the MSB of temp and shift it twice left
    temp_ones = Temperature_Array[1] >> 6; //LSB of temp only has data on bits 6 and 7 so we shift 6

    temp_Celsius = (temp_tens | temp_ones) * .25; //now we can or them for a 10 bit number
                                                 //times .25 because 2 least sig bits are decimal places
                                                 //the shift had multiplied our actual temp by 2^2, so we divide
    temp_Fahrenheit = temp_Celsius * 1.8 + 32;


    num_toPrint_ones = (int) temp_Fahrenheit % 10;

    num_toPrint_tens = (((int) temp_Fahrenheit - num_toPrint_ones) / 10) % 10;

    num_toPrint_hundreds = (((int) temp_Fahrenheit
            - (num_toPrint_ones + 10 * num_toPrint_tens)) / 100) % 10;

    //num_toPrint_tens = (((int) temperature_Fahrenheit & 0xF0) >> 4) + '0';

    if (num_toPrint_hundreds == 0)
    {
        ST7735_DrawChar(start_PositionX, start_PositionY,
                        num_toPrint_tens + '0',
                    text_Color, bg_Color, 2);

		start_PositionX += 12;

        //num_toPrint_ones = ((int) temperature & 0x0F) + '0';

        ST7735_DrawChar(start_PositionX, start_PositionY,
                        num_toPrint_ones + '0',
                    text_Color, bg_Color, 2);

		start_PositionX += 12;

    ST7735_DrawChar(start_PositionX, start_PositionY, (char) 247, text_Color,
                    bg_Color, 2);                                //degree symbol

    start_PositionX += 12;

        ST7735_DrawChar(start_PositionX, start_PositionY, 'F', text_Color,
                        bg_Color, 1);
    }

    else
    {
        displayHiCabinTemp();
    }
    
}
