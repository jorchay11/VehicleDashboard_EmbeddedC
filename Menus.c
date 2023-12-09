/*
 * Menus.c
 *
 *  Created on: Nov 25, 2020
 *      Author: 7jorchay
 */



#include <stdio.h>
#include <stdint.h>
#include "Menus.h"
#include "msp.h"
#include "ST7735.h"
#include "RTC_ProjectFunctions.h"
#include "EncoderFunctions.h"
#include "EEPROM.h"
#include "Project.h"

volatile int menu = 0;
//unsigned char CustomString_ToPrint[20];
//int i = 0;


//Menus
//top left corner = 0,0
//bottom right corner = for font 4, it's 100, 120
//middle x = 32 (size 4)
//middle y = 76 (size 4)
//
//int delay = 1000 * 16;        //delay for splash screen stuff

//Startup menu for LCD
void Startup_Menu()
{
	ST7735_FillScreen(0);				//set screen to black 
	Read_RTC_Print_Date_toLCD();		//read and print date to LCD
	Read_RTC_Print_Time_toLCD();		//read and print Time 

	Read_RTC_Temperature_Print_toLCD();	//read and print the temperature 

    char string_toPrint[20];
    uint16_t start_PositionX = 15;
    uint16_t start_PositionY = 110;

    int i = 0;

	//prints Settings highlighted in White 
    string_toPrint[0] = 'S';
    string_toPrint[1] = 'e';
    string_toPrint[2] = 't';
    string_toPrint[3] = 't';
    string_toPrint[4] = 'i';
    string_toPrint[5] = 'n';
    string_toPrint[6] = 'g';
    string_toPrint[7] = 's';

	uint16_t text_Color = 0;        	//
    uint16_t bg_Color = 0xFFFF;          //


        for (i = 0; i <= 7; i++)
    {
        ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
                        text_Color, bg_Color, 2);
        start_PositionX += 12;
    }
}



//prints the options of the interface menu 
void Interface_Options_Menu()
{
	//Output_Clear();
    ST7735_FillScreen(0xFFFF);
    char string_toPrint[20];
    uint16_t start_PositionX = 15;
    uint16_t start_PositionY = 52;

    int i = 0;

    string_toPrint[0] = 'S';
    string_toPrint[1] = 'e';
    string_toPrint[2] = 't';
    string_toPrint[3] = ' ';
    string_toPrint[4] = 'T';
    string_toPrint[5] = 'i';
    string_toPrint[6] = 'm';
    string_toPrint[7] = 'e';
	string_toPrint[8] = NULL;

	//uint16_t text_Color = 0; //white, highlighted will be blue, maybe light blue
    uint16_t bg_Color = 0xFFFF;         //white
    //int delay = 1000 * 16;        //delay for splash screen stuff

    for (i = 0; string_toPrint[i] != NULL; i++)
    {
        ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
		ST7735_BLUE, bg_Color, 2);
        start_PositionX += 12;
    }

	start_PositionY += 17;
    start_PositionX = 15;

    string_toPrint[0] = 'S';
    string_toPrint[1] = 'e';
    string_toPrint[2] = 't';
    string_toPrint[3] = ' ';
    string_toPrint[4] = 'D';
    string_toPrint[5] = 'a';
    string_toPrint[6] = 't';
    string_toPrint[7] = 'e';
	string_toPrint[8] = NULL;

    for (i = 0; string_toPrint[i] != NULL; i++)
    {
        ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
				0x04E0, bg_Color, 2);
        start_PositionX += 12;
    }

	start_PositionY += 17;
	start_PositionX = 30;


	string_toPrint[0] = 'A';
	string_toPrint[1] = 'l';
	string_toPrint[2] = 'a';
	string_toPrint[3] = 'r';
	string_toPrint[4] = 'm';
	string_toPrint[5] = 's';
	string_toPrint[6] = NULL;

	for (i = 0; string_toPrint[i] != NULL; i++) {
		ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
		ST7735_RED, bg_Color, 2);
		start_PositionX += 12;
	}

}

//prints a time format in all zeros, but will reprint with the new value entered when a new digit is being changed 
void Set_Time_Menu() {

	ST7735_FillScreen(0xFFFF);
	uint16_t start_PositionX = 20;
	uint16_t start_PositionY = 60;
	uint16_t text_Color = 0xF800;            //blue
	uint16_t bg_Color = 0xFFFF;              //white

	char New_Time[6];

	New_Time[0] = New_RTC_Time_Hour_tens;
	New_Time[1] = New_RTC_Time_Hour_ones;
	New_Time[2] = New_RTC_Time_Minute_tens;
	New_Time[3] = New_RTC_Time_Minute_ones;

	int i = 0;

	for (i = 0; i < 4; i++) {
		ST7735_DrawChar(start_PositionX, start_PositionY, New_Time[i] + '0',
				text_Color, bg_Color, 2);

		start_PositionX += 12;

		if (i == 1) {
			ST7735_DrawChar(start_PositionX, start_PositionY, ':', text_Color,
					bg_Color, 2);
			start_PositionX += 12;
		}
	}
}

//same function as the Set time menu 
//reprints with newly set Date digits 
void Set_Date_Menu() {
	ST7735_FillScreen(0xFFFF);
	uint16_t start_PositionX = 5;
	uint16_t start_PositionY = 60;
	uint16_t text_Color = ST7735_MAGENTA;            //
	uint16_t bg_Color = 0xFFFF;              //white

	char New_Date[7];

	New_Date[0] = New_RTC_Date_Day_ones;
	New_Date[1] = New_RTC_Date_Month_tens;
	New_Date[2] = New_RTC_Date_Month_ones;
	New_Date[3] = New_RTC_Date_Date_tens;
	New_Date[4] = New_RTC_Date_Date_ones;
	New_Date[5] = New_RTC_Date_Year_tens;
	New_Date[6] = New_RTC_Date_Year_ones;

	int i = 0;

	for (i = 0; i < 7; i++) {
		ST7735_DrawChar(start_PositionX, start_PositionY, New_Date[i] + '0',
				text_Color, bg_Color, 2);

		text_Color = ST7735_RED;
		start_PositionX += 12;

		if (i == 0) {
			ST7735_DrawChar(start_PositionX, start_PositionY, ' ', text_Color,
					bg_Color, 2);
			start_PositionX += 12;
		}

		if ((i == 2) || (i == 4)) {
			ST7735_DrawChar(start_PositionX, start_PositionY, '/', text_Color,
					bg_Color, 2);
			start_PositionX += 12;
		}
	}


}

//menu display for the alarms 

void Alarms_Menu() {

	//Output_Clear();
	ST7735_FillScreen(0);
	char string_toPrint[20];
	uint16_t start_PositionX = 15;
	uint16_t start_PositionY = 10;
	uint16_t text_Color = 0x0FF; //orange , highlighted will be blue, maybe light blue
	uint16_t bg_Color = 0;

	int i = 0;

//	string_toPrint[0] = 'C';
//	string_toPrint[1] = 'o';
//	string_toPrint[2] = 'l';
//	string_toPrint[3] = 'l';
//	string_toPrint[4] = 'i';
//  string_toPrint[5] = 's';
//	string_toPrint[6] = 'i';
//	string_toPrint[7] = 'o';
//	string_toPrint[8] = 'n';
//	string_toPrint[9] = 's';
//	string_toPrint[10] = NULL;

	string_toPrint[0] = 'A';
	string_toPrint[1] = 'L';
	string_toPrint[2] = 'A';
	string_toPrint[3] = 'R';
	string_toPrint[4] = 'M';
	string_toPrint[5] = 'S';
	string_toPrint[6] = NULL;

	ST7735_FillRect(0, 0, 140, 3, ST7735_YELLOW);

    for (i = 0; string_toPrint[i] != NULL; i++)
    {
        ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
				text_Color, bg_Color, 2);
		start_PositionX += 12;
    }

	ST7735_FillRect(0, 24, 140, 2, ST7735_MAGENTA);

//	start_PositionY += 65;
//	start_PositionX = 15;

//    string_toPrint[0] = 'S';
//    string_toPrint[1] = 'p';
//    string_toPrint[2] = 'e';
//    string_toPrint[3] = 'e';
//    string_toPrint[4] = 'd';
//    string_toPrint[5] = 'i';
//    string_toPrint[6] = 'n';
//    string_toPrint[7] = 'g';
//	string_toPrint[8] = 'T';
//	string_toPrint[9] = 'i';
//	string_toPrint[10] = 'c';
//	string_toPrint[11] = 'k';
//	string_toPrint[12] = 'e';
//	string_toPrint[13] = 't';
//	string_toPrint[14] = 's';
//	string_toPrint[15] = NULL;

//    for (i = 0; string_toPrint[i] != NULL; i++)
//    {
//        ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
//				0xF7E0, bg_Color, 1);
//		start_PositionX += 7;
//
//		if (i == 7) {
//			start_PositionY += 10;
//			start_PositionX = 15;
//		}
//    }

	//ST7735_FillRect(0, 95, 140, 2, ST7735_MAGENTA);

	for (i = 0; i < 5; i++) {
		read_Date_and_Time_EEPROM(i);
	}

	ST7735_FillRect(0, 157, 140, 3, ST7735_YELLOW);
}


//function to decide which menu is next 
//dependent on the current menu and total turns
void MENU() {

	//startup = 0
	//interface options = 1
	//Set time menu = 2
	//set date menu = 3
	//alarms menu = 4
	if (menu == 0) {
		menu = 1;
		Interface_Options_Menu();
	}

	else if ((menu == 1) && (total_turns == 0)) {//interface menu, cursor on Set_
		menu = 2;
		Set_Time_Menu();
	}

	else if ((menu == 1) && (total_turns == 1)) {//interface menu, cursor on Set_
		menu = 3;
		Set_Date_Menu();
	}

	else if ((menu == 1) && (total_turns == 2)) {//interface menu, cursor on Set_
		menu = 4;
		Alarms_Menu();
	}

	else if (menu == 2) {//if menu is greater than 1, function will reset display to startup menu 
		menu = 0;
		//function to set new Time
		Startup_Menu();

	} else if (menu == 3) {
		menu = 0;
		//function to set new Date
		Startup_Menu();

	} else if (menu == 4) {
		menu = 0;
		Startup_Menu();
	}

	reset_Encoder_Counts(); 			//at new menu, reset encoder counts
	Encoder_Btn_pressed = 0;		//reset interrupt and debounce global flags 
	Encoder_Btn_interruptFlag = 0;
}
