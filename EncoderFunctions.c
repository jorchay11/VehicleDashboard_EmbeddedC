/*
 * EncoderFunctions.c
 *
 *  Created on: Nov 26, 2020
 *      Author: 7jorchay
 */




#include "msp.h"
#include <stdio.h>
#include <math.h>
#include "EncoderFunctions.h"
#include "ST7735.h"
#include "menus.h"
#include "Project.h"
#include "RTC_ProjectFunctions.h"

volatile int Encoder_Btn_pressed = 0;
volatile int Encoder_Btn_interruptFlag = 0;         //interrupt flag variables
volatile int CCW_turns = 0;                         //variable for CCW pulses
volatile int CW_turns = 0;                          //variable for CW pulses
volatile int total_turns = 100; //number of turns in either direction and magnitude of difference
volatile int direction = 3;                   //if total is CW, = 0, if CCW, = 1

volatile int DT = 1;                                //DT variable, starts high
volatile int CLK = 1;                               //CLK variable, starts high

volatile int cursor_moved = 0;//global variable for whether the cursor has responded to 
//a change in encoder turns so that it does not happen repeatedly

//initialize components of the encoder
void Encoder_init(void)
{
    Encoder_Btn_init();
    Encoder_CLK_init();
    Encoder_DT_init();
}

//initialize the encoder switch as an input, same as a button
void Encoder_Btn_init(void)
{
    P1->SEL0 &= ~BIT5;          //P1.5 set to GPIO
    P1->SEL1 &= ~BIT5;
    P1->DIR &= ~BIT5;           //P1.5 direction set to input
    P1->REN |= BIT5;            //activating the internal resistor
    P1->OUT |= BIT5;            //declaring the internal resistor as pull-up
    P1->IES |= BIT5;           //interrupt edge select on falling edge
    P1->IE |= BIT5;            //Interrupt enable
    P1->IFG &= ~BIT5;          //Clears the flag bit

}

//initializes encoder CLK line as input like a button 
//handled by comparing the line to the DT line in the Port 1 handler
void Encoder_CLK_init(void)
{
    P1->SEL0 &= ~BIT7;          //P1.7 set to GPIO
    P1->SEL1 &= ~BIT7;
    P1->DIR &= ~BIT7;           //P1.7 direction set to input
    P1->REN |= BIT7;            //activating the internal resistor
    P1->OUT |= BIT7;            //declaring the internal resistor as pull-up
    P1->IES |= BIT7;           //interrupt edge select on falling edge
    P1->IE |= BIT7;            //Interrupt enable
    P1->IFG &= ~BIT7;          //Clears the flag bit
}

//initializes the DT line, no pull up resistor
//handled by comparing to CLK line 
void Encoder_DT_init(void)      //data line whether the knob was last turned
{
    P1->SEL0 &= ~BIT6;          //P1.6 set to GPIO
    P1->SEL1 &= ~BIT6;
    P1->DIR &= ~BIT6;           //P1.6 direction set to input
    P1->REN |= BIT6;            //activating the internal resistor
    P1->OUT |= BIT6;           //declaring the internal resistor as pull-up
}

//debounce for the encoder button 
int Debounce_EncoderButton()
{
    static uint16_t State_1 = 0;                       //Current debounce status

    State_1 = (State_1 << 1) | (P1IN & 0x20) >> 5 | 0xf800; //checks pin 1.5, don

    if (State_1 == 0xfc00)
    {
        Encoder_Btn_pressed = 1;
        return 1;                                             //after 0 level is
                                                              //stable for 10 consecutive calls
    }
    else
        return 0;
}

//resets all encoder counts: turns, and cursor moved flag 
void reset_Encoder_Counts() {

	CW_turns = 0;                       //reset variable values
	CCW_turns = 0;
	total_turns = 0;
	cursor_moved = 0;
}

//this function holds all the conditions for what happens when the encoder knob is turned
//uses global variables for which menu is displayed and how many turns to determine which set of words is highlighted as an option
//if the encoder button is pressed while an option is highlight, the menu will be opened accordingly 
//works by reprinting the menu, then reprinting the desired line with a black background in the same coordinates as the original menu 
//changing individual digits in time and date are displayed and saved into RTC by using switch cases on total_turns % 10
//the currently highlighted and changing digit are set equal to total turns, and encoder values are reset if it goes out of bound for
//the current condition of the digit being set. The New_RTC_info button will also reset the counts and change the highlighted
//digit to the next one, changing the applicable conditions for encoder turn values
//many conditions for digits are dependent on the digit before. These conditions are nested within menu, then New_RTC_info button values


void control_Cursor(void) {

	char string_toPrint[20];
	uint16_t start_PositionX;
	uint16_t start_PositionY;

	uint16_t text_Color = 0xFFFF;           //white number when changing
	uint16_t bg_Color = 0;				//black bg for option being selected 

	int i = 0;

	if (cursor_moved == 0) {//only enters when the condition true after turning encoder is not yet executed 

		cursor_moved = 1;					//set flag variable 

		total_turns = abs(CW_turns - CCW_turns);  //magnitude of pulse direction

		if ((total_turns == 0) && (menu == 1)) {		//highlight Set time 

			Interface_Options_Menu();				//reprint the option menu

		start_PositionX = 15;
		start_PositionY = 52;
			string_toPrint[0] = 'S';//reprint the Set Time option with black background
		string_toPrint[1] = 'e';
		string_toPrint[2] = 't';
		string_toPrint[3] = ' ';
		string_toPrint[4] = 'T';
		string_toPrint[5] = 'i';
		string_toPrint[6] = 'm';
		string_toPrint[7] = 'e';
		string_toPrint[8] = NULL;

		for (i = 0; string_toPrint[i] != NULL; i++) {
			ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
			ST7735_BLUE, bg_Color, 2);
			start_PositionX += 12;
		}
	}

		else if ((total_turns == 1) && (menu == 1)) {
		Interface_Options_Menu();

		start_PositionX = 15;
			start_PositionY = 69;
		string_toPrint[0] = 'S';
		string_toPrint[1] = 'e';
		string_toPrint[2] = 't';
		string_toPrint[3] = ' ';
		string_toPrint[4] = 'D';
		string_toPrint[5] = 'a';
		string_toPrint[6] = 't';
		string_toPrint[7] = 'e';
		string_toPrint[8] = NULL;

		for (i = 0; string_toPrint[i] != NULL; i++) {
			ST7735_DrawChar(start_PositionX, start_PositionY, string_toPrint[i],
					0x04E0, bg_Color, 2);
			start_PositionX += 12;
		}
	}

		else if ((total_turns == 2) && (menu == 1)) {
		Interface_Options_Menu();

			start_PositionY = 86;
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

		///////////////////////////////////////////////////
		//
		//Starts conditions for changing time and date 
		//
		///////////////////////////////////////////////////
		else if (menu == 2) {					//if menu is 2, changing time 
			Set_Time_Menu();

			start_PositionX = 20;					
			start_PositionY = 60;

			if (Enter_Date_Time_Btn_count == 0) {// setting tens place os hours 0 - 2

				switch (total_turns % 10) {

				case 0: {

					New_RTC_Time_Hour_tens = 0x00;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_tens + '0',
						text_Color, bg_Color, 2);
					break;
				}

				case 1: {

					New_RTC_Time_Hour_tens = 0x01;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_tens + '0',
						text_Color, bg_Color, 2);
					break;
				}

				case 2: {

					New_RTC_Time_Hour_tens = 0x02;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_tens + '0',
						text_Color, bg_Color, 2);
					break;
				}

				default:
				{
				reset_Encoder_Counts();
					break;
				}

				}
			}

			else if (Enter_Date_Time_Btn_count == 1) {//setting ones place of hours 0-9 unless tens was 2, then 0 - 3
				start_PositionX = 32;

				if ((New_RTC_Time_Hour_tens == 0)
						|| (New_RTC_Time_Hour_tens == 1)) {

					switch (total_turns % 10) {

					case 0: {

						New_RTC_Time_Hour_ones = 0x00;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						New_RTC_Time_Hour_ones = 0x01;
						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 2: {

						New_RTC_Time_Hour_ones = 0x02;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 3: {
						New_RTC_Time_Hour_ones = 0x03;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 4: {

						New_RTC_Time_Hour_ones = 0x04;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 5: {

						New_RTC_Time_Hour_ones = 0x05;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 6: {

						New_RTC_Time_Hour_ones = 0x06;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 7: {

						New_RTC_Time_Hour_ones = 0x07;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 8: {

						New_RTC_Time_Hour_ones = 0x08;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 9: {

						New_RTC_Time_Hour_ones = 0x09;

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Time_Hour_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}

					}
				}

				if (New_RTC_Time_Hour_tens == 2) {

					switch (total_turns % 10) {

					case 0: {

						New_RTC_Time_Hour_ones = 0x00;

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 1: {

						New_RTC_Time_Hour_ones = 0x01;
						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_ones + '0', text_Color, bg_Color,
							2);
						break;
					}
					case 2: {

						New_RTC_Time_Hour_ones = 0x02;

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 3: {
						New_RTC_Time_Hour_ones = 0x03;

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Hour_ones + '0', text_Color, bg_Color,
							2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}

				}
			}
			}

			else if (Enter_Date_Time_Btn_count == 2)//setting minutes tens 0 - 5
					{
				Time_toSend[2] = (New_RTC_Time_Hour_tens << 4)
						| New_RTC_Time_Hour_ones;

				start_PositionX = 56;

				switch (total_turns % 10) {

				case 0: {

					New_RTC_Time_Minute_tens = 0x00;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 1: {

					New_RTC_Time_Minute_tens = 0x01;
					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 2: {

					New_RTC_Time_Minute_tens = 0x02;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 3: {
					New_RTC_Time_Minute_tens = 0x03;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 4: {

					New_RTC_Time_Minute_tens = 0x04;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 5: {

					New_RTC_Time_Minute_tens = 0x05;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_tens + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				default:
				{
					reset_Encoder_Counts();
					break;
				}
				}

			}

			else if (Enter_Date_Time_Btn_count == 3) {//setting minutes ones 0 - 9
				start_PositionX = 68;

				switch (total_turns % 10) {

				case 0: {

					New_RTC_Time_Minute_ones = 0x00;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 1: {

					New_RTC_Time_Minute_ones = 0x01;
					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 2: {

					New_RTC_Time_Minute_ones = 0x02;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 3: {
					New_RTC_Time_Minute_ones = 0x03;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 4: {

					New_RTC_Time_Minute_ones = 0x04;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 5: {

					New_RTC_Time_Minute_ones = 0x05;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 6: {

					New_RTC_Time_Minute_ones = 0x06;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 7: {

					New_RTC_Time_Minute_ones = 0x07;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				case 8: {

					New_RTC_Time_Minute_ones = 0x08;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}

				case 9: {

					New_RTC_Time_Minute_ones = 0x09;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Time_Minute_ones + '0', text_Color,
							bg_Color,
							2);
					break;
				}
				default:
				{
					reset_Encoder_Counts();
					break;
				}

				}
			}

			else if (Enter_Date_Time_Btn_count == 4) {//here we have all the values loaded into Time_toSend array
																	
				Time_toSend[1] = (New_RTC_Time_Minute_tens << 4)
						| New_RTC_Time_Minute_ones;

				Write_New_Time_RTC(Time_toSend);//load data into RTC using I2C functions
				Enter_Date_Time_Btn_count = 0;//reset New_RTC_info counts as its function is complete
				MENU();	//call menu to go back to startup menu, reading the new time

			}
		}

		else if (menu == 3) {					//changing the date

			Set_Date_Menu();					//reprint menu

			start_PositionX = 5;
			start_PositionY = 60;

			if (Enter_Date_Time_Btn_count == 0) {// setting day of the week 1-7

				New_RTC_Date_Day_ones = total_turns + 1;//can't have a 0th day of the week, since the day is printed using conditions 1-7
														//this digit is therefore the total turns + 1 
				switch (total_turns % 10) {

				case 0: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 1: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 2: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 3: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 4: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 5: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 6: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Day_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				default:
				{
					reset_Encoder_Counts();
					break;
				}

				}
			}

			else if (Enter_Date_Time_Btn_count == 1) {//setting tens place of month, 0-2

				start_PositionX = 29;

				New_RTC_Date_Month_tens = total_turns;

				switch (total_turns % 10) {

				case 0: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_tens + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 1: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_tens + '0', text_Color, bg_Color,
							2);
					break;
				}
				default:
				{
					reset_Encoder_Counts();
					break;
				}
				}

			}

			else if (Enter_Date_Time_Btn_count == 2) {// setting ones place of month, dependent on tens place 

				start_PositionX = 41;

				if (New_RTC_Date_Month_tens == 0) {

					New_RTC_Date_Month_ones = total_turns + 1;

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 2: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 3: {


						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}
					case 4: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}
					case 5: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 6: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}
					case 7: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Month_ones + '0', text_Color, bg_Color,
							2);
						break;
					}

					case 8: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Month_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					default:
					{
						reset_Encoder_Counts();
						break;
					}

					}
				}
				else if (New_RTC_Date_Month_tens == 1) {

					New_RTC_Date_Month_ones = total_turns;

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Month_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Month_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 2: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Month_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}
					}
				}
				
			}

			else if (Enter_Date_Time_Btn_count == 3)//date tens place, dependant on the month chosen 
					{
				start_PositionX = 65;
				New_RTC_Date_Date_tens = total_turns;

				if ((New_RTC_Date_Month_tens == 1)
						|| ((New_RTC_Date_Month_ones != 2))
								&& (New_RTC_Date_Month_tens != 1)) {

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 2: {
						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 3: {
						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}
					}
				}
				else if ((New_RTC_Date_Month_tens != 1)//if the month was February 
						&& (New_RTC_Date_Month_ones == 2)) {

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 2: {
						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_tens + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}
					}
				}

			}

			else if (Enter_Date_Time_Btn_count == 4) {	//ones place of months
				start_PositionX = 77;

				if (New_RTC_Date_Date_tens == 0)
					New_RTC_Date_Date_ones = (total_turns % 10) + 1;
				else
					New_RTC_Date_Date_ones = total_turns % 10;

				if ((New_RTC_Date_Date_tens == 0)
						|| (New_RTC_Date_Date_tens == 1)) {

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 2: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 3: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 4: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 5: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 6: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 7: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 8: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 9: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}
					}
				}

				else if (New_RTC_Date_Date_tens == 2) {

					if (!((New_RTC_Date_Month_tens == 0)
							&& (New_RTC_Date_Month_ones == 2))) {

						switch (total_turns % 10) {

						case 0: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 1: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 2: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 3: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 4: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 5: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 6: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 7: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 8: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 9: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						default:
						{
							New_RTC_Date_Date_ones = 0;
							reset_Encoder_Counts();
							break;
						}
						}
					}
					else if ((New_RTC_Date_Month_tens == 0)
							&& (New_RTC_Date_Month_ones == 2)) {
						switch (total_turns % 10) {

						case 0: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 1: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 2: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 3: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 4: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 5: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 6: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 7: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}
						case 8: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						default:
						{
							reset_Encoder_Counts();
							break;
						}
						}
					}
				}

				else if (New_RTC_Date_Date_tens == 3) {


					if ((New_RTC_Date_Month_tens == 0)
						&& ((New_RTC_Date_Month_ones == 1)
								|| (New_RTC_Date_Month_ones == 3)
								|| (New_RTC_Date_Month_ones == 5)
								|| (New_RTC_Date_Month_ones == 7)
								|| (New_RTC_Date_Month_ones == 8))) {//for date ones place for 31 day months

					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
						default:
					{
						reset_Encoder_Counts();
							break;
					}
						}
					} else if ((New_RTC_Date_Month_tens == 1)
							&& ((New_RTC_Date_Month_ones == 0)
									|| (New_RTC_Date_Month_ones == 2))) {//31 day months

						switch (total_turns % 10) {

						case 0: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						case 1: {

							ST7735_DrawChar(start_PositionX, start_PositionY,
									New_RTC_Date_Date_ones + '0', text_Color,
									bg_Color, 2);
							break;
						}

						default:
						{
							reset_Encoder_Counts();
							break;
						}
						}
					}
				
				}

				else if ((New_RTC_Date_Month_tens == 1)
						&& ((New_RTC_Date_Month_ones == 1)
								&& (New_RTC_Date_Date_tens != 3))) {


					switch (total_turns % 10) {

					case 0: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 1: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 2: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 3: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 4: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 5: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 6: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 7: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					case 8: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}

					case 9: {

						ST7735_DrawChar(start_PositionX, start_PositionY,
								New_RTC_Date_Date_ones + '0', text_Color,
								bg_Color, 2);
						break;
					}
					default:
					{
						reset_Encoder_Counts();
						break;
					}
					}
				}
				else if ((New_RTC_Date_Month_tens == 1)
						&& (New_RTC_Date_Month_ones == 1)) {	//30 day months

					New_RTC_Date_Date_ones = 0;
					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Date_ones + '0', text_Color, bg_Color,
							2);
					reset_Encoder_Counts();
				}
				else {
					New_RTC_Date_Date_ones = 0;

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Date_ones + '0', text_Color, bg_Color,
							2);
					reset_Encoder_Counts();
				}

			}

			else if (Enter_Date_Time_Btn_count == 5) {

				start_PositionX = 99;

				New_RTC_Date_Year_tens = total_turns % 10;

				switch (total_turns % 10) {

				case 0: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}

				case 1: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				case 2: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}

				case 3: {
					
					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}

				case 4: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				case 5: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				case 6: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				case 7: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				case 8: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}

				case 9: {
					
					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_tens + '0', text_Color,
							bg_Color, 2);
					break;
				}
				default:
				{
					reset_Encoder_Counts();
					break;
				}
				}
			}
			
			else if (Enter_Date_Time_Btn_count == 6) {//setting the year ones place. can be 0-9
				start_PositionX = 111;

				New_RTC_Date_Year_ones = total_turns % 10;

				switch (total_turns % 10) {

				case 0: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 1: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 2: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 3: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 4: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 5: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 6: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 7: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				case 8: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}

				case 9: {

					ST7735_DrawChar(start_PositionX, start_PositionY,
							New_RTC_Date_Year_ones + '0', text_Color, bg_Color,
							2);
					break;
				}
				default:
				{
					reset_Encoder_Counts();
					break;
				}
				}
			}
		
			else if (Enter_Date_Time_Btn_count == 7) {//we now have all the data for the date 


				Date_toSend[0] = New_RTC_Date_Day_ones;	//setting the array to data received 


				Date_toSend[1] = (New_RTC_Date_Date_tens << 4)//creating a byte of two bcd numbers to make a 2 digit number readable to RTC 
						| New_RTC_Date_Date_ones;


				Date_toSend[2] = (New_RTC_Date_Month_tens << 4)
						| New_RTC_Date_Month_ones;



				Date_toSend[3] = (New_RTC_Date_Year_tens << 4)
						| New_RTC_Date_Year_ones;

				Write_New_Date_RTC(Date_toSend);//write the date to the correct positions in RTC
				Enter_Date_Time_Btn_count = 0;//reset button count after function is complete
				MENU();						//call menu to go back to startup 

			}

		}

	else
		reset_Encoder_Counts();
	}

}



