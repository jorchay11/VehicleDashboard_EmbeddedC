

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/*
 *                  INCLUDES
 */
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "math.h"
#include "SysTick_Library.h"
#include "ST7735.h"
#include "TIMERA.h"
#include "Clocks.h"
#include "Menus.h"
#include "EncoderFunctions.h"
#include "ProjectButtons.h"
#include "RTC_ProjectFunctions.h"
#include "I2C_ProjectFunctions.h"
#include "Stepper.h"
#include "MAX219.h"
#include "Hall_Effect.h"
#include "HC-SR04.h"
#include "Photocell.h"
#include "Songs.h"
#include "EEPROM.h"
#include "Project.h"

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/*
 *                  GLOBAL VARIABLES
 */
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
volatile int timerA3_0_interruptFlag = 0;//gloabl interrupt flag for timerA3_0, 0 instance can only time
volatile int timerA1_0_interruptFlag = 0;//global interrupt flag for TIMERA1_0

volatile int Timer32_interruptFlag = 0;	//global interrupt flag for TIMER32, one second timer

volatile int leftBlinker_Switch_held = 0;//flag set when toggle switch position checked corresponding to left blinker
volatile int rightBlinker_Switch_held = 0;//flag set when toggle switch position checked corresponding to right blinker
//these are used to set the toggle flag in TIMERA3 interrupt
volatile int Open_Door_Btn_interruptFlag = 0;//global flag for open dorr button pin
volatile int Open_Door_Btn_heldDown = 0;//continuall reevaluted in while loop to determine if the open door button is still held down

volatile int NewRTC_Info_Btn_interruptFlag = 0;	//global flag for New_RTC_info button
volatile int Enter_Date_Time_Btn_count = 0;	//Count for how many times the New_RTC_info button has been pressed before reset

volatile int WatchdogReset_Btn_interruptFlag = 0;//global flag for when the watchdog button is pressed

volatile uint8_t New_RTC_Time_Hour_tens = 0;//global variables holding the desired new values for time to send to RTC
volatile uint8_t New_RTC_Time_Hour_ones = 0;		//one digit at a time
volatile uint8_t New_RTC_Time_Minute_tens = 0;
volatile uint8_t New_RTC_Time_Minute_ones = 0;

volatile uint8_t New_RTC_Date_Day_ones = 0;	//global variables holding byte data for desired new RTC Date
volatile uint8_t New_RTC_Date_Date_tens = 0;
volatile uint8_t New_RTC_Date_Date_ones = 0;
volatile uint8_t New_RTC_Date_Month_tens = 0;
volatile uint8_t New_RTC_Date_Month_ones = 0;
volatile uint8_t New_RTC_Date_Year_tens = 0;
volatile uint8_t New_RTC_Date_Year_ones = 0;


unsigned char Time_toSend[4] = { 0, 0, 0, 0 };//global array, initialized with zeros, set bytes to new desired time data
unsigned char Date_toSend[5] = { 0, 0, 0, 0, 0 };//global array, initialized with zeros, set bytes to new desired date data
volatile int alarm_count = 0;//global alarm count to determine which byte is written or read to EEPROM
volatile int alarm_entered = 0;	//global flag for whether an alarm was sent to EEPROM

volatile int TOGGLE_Left_Blinker_flag = 0;//global flag set when corresponding left blinker pin from toggle switch is read low
volatile int TOGGLE_Right_Blinker_flag = 0;	//global flag set when corresponding right blinker pin from toggle switch is read low
volatile int BLINKERS_OFF_flag = 0;	//global flag for when toggle switch is not set either direction
//volatile int alarms_printed;

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/*
 *              FUNCTION PROTOTYPES in .h
 */
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void main()     //Project_main
{
	int i;
	Watchdog_halt();				//halts watchdog

	Project_init();			//initializedcomponents and timers for the project

	Enable_Interrupts();			//enable interrupts for project

	Startup_Menu();			//calls a function to display startup menu on reset


    while (1)
    {
//read inputs

		if (alarm_count == 5) {		//if alarm count is equal to 5, reset it 
			alarm_count = 0;		//only 5 alarms are stored
		}

		ADC_Reading();						//read ADC value from photoresistor
		change_TIMERA0_1_dutyCycle();//change the LED backlight brightness depending on ADC value

		leftBlinker_Switch_held = (!(P3IN & BIT6 ));//read tollge switch value for blinkers
		rightBlinker_Switch_held = (!(P3IN & BIT7 ));

		Open_Door_Btn_heldDown = (!(P3IN & 0x20));//sets global value to the state of the button for door ajar

		for (i = 0; i < 15; i++) {
			Debounce_DoorOpen_Btn();	//global debounce for door open button
			Debounce_EncoderButton(); //global debounce flag for encoder button
			Debounce_NewRTC_Info_Btn();	//global debounce flag for button that enters new RTC data
			Debounce_WatchodgReset_Btn();//global debounce flag for watchdog reset button button;
		}

//respond to inputs
		if (Open_Door_Btn_interruptFlag == 1) //interrupt occurs for door ajar button
				{
			if (Open_Door_Btn_pressed == 1)       //confirms debounce successful
					{
				DoorAjar_Btn_HeldDown();		//display door ajar on MAX219
				Open_Door_Btn_pressed = 0;              //reset debounce flag
				Open_Door_Btn_interruptFlag = 0;//reset interrupt flag once button is no longer held down 
			}
			MAX219_SPI_init_Decode();//reset MAX219 to decode mode for speed display 
		}

		if (Encoder_Btn_interruptFlag == 1) {//interrupt occurs on encoder button pin
			if (Encoder_Btn_pressed == 1) {			//confirm encodr debounce
				MENU();						//the encoder button controls MENUS
			}
		}

		if (NewRTC_Info_Btn_interruptFlag == 1) {//interrupt occurs on button for entering new RTC information
			if (NewRTC_Info_Btn_pressed == 1) {			//confirm debounce

				if ((menu == 2) || (menu == 3)) {//will only have an effect if menu is currently 
					Enter_Date_Time_Btn_count++;		//change time or date 
														//increment button count to control the digit changed 
					NewRTC_Info_Btn_interruptFlag = 0;	//clear flags
					NewRTC_Info_Btn_pressed = 0;
					reset_Encoder_Counts();	//reset encoder counts to begin counting again
				}
			}
		}

		if (WatchdogReset_Btn_interruptFlag == 1) {	//interrupt occurs on watchdog interrut flag 
			if (WatchdogReset_Btn_pressed == 1) {		//debounce confirmed 
				Watchdog_init();//initialize watchdog reset, 4 second timeout
				WatchdogReset_Btn_interruptFlag = 0;//technically never reached, but reset anyway in case 
				WatchdogReset_Btn_pressed = 0;//i'd like to be able to interrupt reset

			}
		}

		CONTROL_BLINKERS();	//every half second, the timer will set a flag for the blinker 
							//corresponding to toggle switch checked above 
		if (Timer32_interruptFlag == 1) {//intrval interrupt for every second 
			RPMs = measure_RPMs(Revolution_count);		//measure RPMs 

			Timer32_interruptFlag = 0;					//reset flag
			Revolution_count = 0;//reset revolution counts for the next second calculation 

			Current_Speed_mph = measure_Speed(RPMs,	//calculate speed from RPMs
			CIRCUMFERENCE_6ftDiameter);

			if (Current_Speed_mph < 100) {				//display speed
				MAX219_SPI_TransferData(0x05, Current_Speed_mph / 10);//tens place is int speed / 10
				MAX219_SPI_TransferData(0x04, Current_Speed_mph % 10);//ones place is remainder of speed / 10
				MAX219_SPI_TransferData(0x06, 0);//if less than 100, hundreds place must be zero
			}

			else if (Current_Speed_mph >= 100) {	//display greater than 100
				MAX219_SPI_TransferData(0x05, (Current_Speed_mph / 10) % 10);
				MAX219_SPI_TransferData(0x04, Current_Speed_mph % 10);
				MAX219_SPI_TransferData(0x06, 1);//just print one in hundres place, speed will not go above 199
			}
			speed_Dif = (Current_Speed_mph - lastSpeed);//before setting speedometer position
														//calculate the difference of last and current speed
			if (RPMs == 0) {				//if the RPMs calculated are zero
				if (RPMs_areZero == 0) {//if this is not a repeated zero calculation 

					//TIMER_A2->CCR[2] = 6000;
					//SysTick_delay_ms(10);
					Rotate_Stepper1_Backward_s_Steps(120);//turn speedometer to zero position 
					RPMs = 0;										//reset RPMs
					RPMs_areZero = 1;//set RPMs are zero flag so this is not repeated continually while spped is zero
				}

			}

			else if (RPMs > 2) {
				RPMs_areZero = 0;		//reset RPMs_are_zero when they are not
				if (firstTime_done == 0) {//if it is not the first time speed is being sent to speedmoeter
					firstTime_done = 1;				//set the first time flag

					//TachometerSpeed(RPMs);
					//TIMER_A2->CCR[2] = 3000;
					//SysTick_delay_ms(10);
					Rotate_Stepper1_Forward_s_Steps(Current_Speed_mph);	//set analog speed by moving motor number of mph in steps 

					RPMs = 0;						//reset RPMs after using it

					if (Current_Speed_mph > 85) {//if greater than 85, end alarm 
						Read_RTC_Store_Alarm_EEPROM(alarm_count);
						alarm_count++;					//increment alarm count 
					}
				} else {
					if (speed_Dif == 0) {//if the same speed is measured twice ina row
						RPMs = 0;						//do nothing, reset RPMs
					} else if (speed_Dif > 0) {	//if the current speed is greater than last
//						TachometerSpeed(RPMs);
//						//TIMER_A2->CCR[2] = 2500;
//						SysTick_delay_ms(10);
						Rotate_Stepper1_Forward_s_Steps(speed_Dif);	//stepper moves forward the number of steps difference 

						RPMs = 0;					//reset RPMs after using 

						if ((speed_Dif + lastSpeed) > 85) {	//now if this calculation is greater than 85
							Read_RTC_Store_Alarm_EEPROM(alarm_count);//send speed alarm
							alarm_count++;						
						}
					} else if (speed_Dif < 0) {	//if the current speed is less than the last

//						TachometerSpeed(RPMs);
//						SysTick_delay_ms(10);
						Rotate_Stepper1_Backward_s_Steps(abs(speed_Dif));//rotate speedometer number of steps backward 
						RPMs = 0;//equal to the absolute value of speed difference
					}
				}
			}

			lastSpeed = Current_Speed_mph;//set last speed equal to current speed for next iteration 
			//TIMER_A2->CCR[2] = 0;
		}


//menu display control
		switch (menu) {

		case 0: {

			if (timerA3_0_interruptFlag == 1) {	//do this if startup menu is running and a half second has elasped 

				Read_RTC_Print_Time_toLCD();		//read and print the time 
				Read_RTC_Temperature_Print_toLCD();	//read and print the temperature in Fahrenheit

				PROXIMITY();		//measure distance from rear of the vehicle
				timerA3_0_interruptFlag = 0;//reset half second interval flag
			}

			break;
		}
		case 1: {						//menu 1: interface menu 
			control_Cursor();//control cursor using menu and encoder turn count conditions
			break;
		}
		case 2: {						//menu 2: change time menu
			control_Cursor();//control cursor using menu and encoder turn count conditions
			break;
		}
		case 3: {						//menu 3: change date menu 
			control_Cursor();			//control cursor for this menu 
			break;
		}
		case 4: {						//menu 4: alarm menu
			break;			//no cursor, an encoder press resets to startup menu
		}

		default: {
			break;
		}

		}



    }
}

void Project_init()
{
    Clock_Init48MHz();                  //MCLK 48 MHz | SMCLK 12MHz
    AClock_Divide4();                   //Divides ACLK by 4 for interval timers

    SysTickInit_NoInterrupts();         //SysTick for delays

	//TIMERA1_0_oneMSInterval_init();      //1 ms period timer         | TIMERA1_0
    TIMERA3_0_HalfSecondInterval_init(); //1 second interval timer   | TIMERA3_0

    openDoor_Btn_init();                //door latch button           | 3.5 GPIO
	Blinkers_Toggle_Switch_init(); //blinker switch                    | 3.6, 3.7 GPIO
	NewRTC_Info_Btn();

	Encoder_init();          //encoder                   | 1.5, 1.6, 1.7  | GPIO

    Left_Blinker();                       //left blinker              | 8.3 GPIO
    Right_Blinker();                      //right blinker             | 8.4 GPIO
    LED_closeProximity();                 //object too close LED      | 8.2 GPIO

    TimerA0_1_PWM_init();       //Backlight LED                | 2.4 | TIMERA0_1
    ST7735_InitR(INITR_REDTAB);      //ST7735                    | 9.2, 9.3 GPIO
                                     //                          | 9.4, 9.5, 9.7  | CS GPIO, SCL, SDA

    Stepper1_Motor();  //Stepper Motor 1           | 4.0, 4.1, 4.2, 4.3   | GPIO

	//Tachometer_Init();

	SPI_init();                              //MAX219     | 2.1, 2.3 | CLK, SIMO
	MAX219_SPI_init_Decode();				//			  | 2.0	     | CS


	Hall_Effect_Out_init();             //hall effect output pin    | 5.2 | GPIO

	HCSR04_init();						//proximity sensor  | 7.7, 7.6 |

	Speaker_12MHz(0, 0); 				//speaker	| 7.7	| TIMERA_


	//Tachometer_Init();
	ADC_pin_init();								
	ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

	Timer32_init();								//interval for each second 
	WatchdogReset_Btn();					//initialize watchdogbutton | P3.3

	//not initialized because of I2C: RTC and EEPROM  | 6.4, 6.5  |
}

void DoorAjar_Btn_HeldDown(void)
{

	MAX219_SPI_init_NoDecode();	//set MAX219 to nodecode mode to diplay custom letters
	while (Open_Door_Btn_heldDown == 1)  	//while the button is depressed
            {
                Open_Door_Btn_heldDown = (!(P3IN & 0x20));      //reevaluate
		Print_DoorAjar_MAX219();					//print door Ajar digitally 
            }
	blank_MAX219();	//blank the screen (this was for testing mainly, but still works because it is reinitialized in decode mode)
}

void MEASURE_SPEED() {//not used on accident, but could replace the entirety of the speedometer setting
	if (Timer32_interruptFlag == 1) {
		RPMs = measure_RPMs(Revolution_count);

		Timer32_interruptFlag = 0;
		Revolution_count = 0;

		Current_Speed_mph = measure_Speed(RPMs,
		CIRCUMFERENCE_6ftDiameter);

		speed_Dif = (Current_Speed_mph - lastSpeed);

		if (RPMs == 0) {
			if (RPMs_areZero == 0) {
				Rotate_Stepper1_Backward_s_Steps(120);
				RPMs = 0;
				RPMs_areZero = 1;
			}

		}

		else if (RPMs > 2) {
			RPMs_areZero = 0;
			if (firstTime_done == 0) {
				firstTime_done = 1;

				Rotate_Stepper1_Forward_s_Steps(Current_Speed_mph);
				RPMs = 0;

				if (Current_Speed_mph > 85) {
					Read_RTC_Store_Alarm_EEPROM(alarm_count);
					alarm_count++;
				}
			} else {
				if (speed_Dif == 0) {
					RPMs = 0;
				} else if (speed_Dif > 0) {
					Rotate_Stepper1_Forward_s_Steps(speed_Dif);
					RPMs = 0;

					if (speed_Dif > 85) {
						Read_RTC_Store_Alarm_EEPROM(alarm_count);
						alarm_count++;
					}
				} else if (speed_Dif < 0) {
					Rotate_Stepper1_Backward_s_Steps(abs(speed_Dif));
					RPMs = 0;
				}
			}
		}

		lastSpeed = Current_Speed_mph;

	}
}

void CONTROL_BLINKERS() {
	if (TOGGLE_Left_Blinker_flag == 1) {//if the left blinker flag was set in TA3 handler because of the toggle switch evaluation
		TOGGLE_Left_Blinker;						//macro to toggle the LED 
		TOGGLE_Left_Blinker_flag = 0;				//reset flag
	}

	else if (TOGGLE_Right_Blinker_flag == 1) {	//repeat for right blinker LED
		TOGGLE_Right_Blinker;
		TOGGLE_Right_Blinker_flag = 0;
	}

	else if (BLINKERS_OFF_flag == 1) {//if neither toggle position is detected, blinker off flag is set
		BLINKERS_OFF;							//macro to turn off both LEDs
		BLINKERS_OFF_flag = 0;						//reset for next iteration
	}
}

void PROXIMITY() {

	measure_Period();//measure the period of timerA capture for proximity sensor

	if (object_DistanceFromSensor < 15) {//if the distance measured is less than 15 inches

		while (object_DistanceFromSensor < 15) {//repeat this while the object is sensed

			if (alarm_entered == 0) {//only want to enter one alarm each time, so this evaluates a flag for that
				Read_RTC_Store_Alarm_EEPROM(alarm_count);//store alarm in EEPROM
				alarm_count++;								//increment count
				alarm_entered = 1;			//set the flag after entering alarm
				}

			P8->OUT |= BIT2;//blue LED on 8.2 is lit while an object is near the rear

			displayParkingWarning();//displays a visual warning on screen each second 
			Bells(5);						//audible alarm from piezo speaker
			Output_Clear();									//clear output 

			measure_Period();			//reevaluate the distance fromthe object
				}

		alarm_entered = 0;//when the object is no longer sensed within 15, reset the alarm flag
		Startup_Menu();							//go back to the startup menu 

		} else
		P8->OUT &= ~BIT2;								//else, turn the LED off
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////





///////////////////////////////////////////////////
///////////////////////////////////////////////////
/*
 *              Interrupt Handlers 
 */
///////////////////////////////////////////////////
//////////////////////////////////////////////////
void Enable_Interrupts(void)
{
	NVIC_EnableIRQ(PORT1_IRQn);	//enables interrupt register for Port 1 inputs
	NVIC_EnableIRQ(PORT3_IRQn);	//enables interrupt register for Port 3 inputs
	NVIC_EnableIRQ(PORT5_IRQn);	//enables interrupt register for Port 5 input 
	NVIC_EnableIRQ(TA3_0_IRQn); //Enable TimerA3 interrupt for half second interval
    //NVIC_EnableIRQ(TA1_0_IRQn);   //for 1ms interval

    __enable_irq();                 //enable global interrupts
}

void PORT5_IRQHandler(void)
{
	if (P5->IFG & BIT2)                  //Checks for P5.5 flag bit
    {
		Revolution_count++;	//increments global variable for revolutions of a wheel 
		//Clears interrupt flag
    }

	P5->IFG = 0;
	// P5->IFG &= ~BIT0;                   //Clears interrupt flag
}


void TA0_0_IRQHandler(void)
{
//TOGGLE_P20;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//clear flag bit
}

void TA1_0_IRQHandler(void)
{
//TOGGLE_P20;
	timerA1_0_interruptFlag = 1;				//sets flag every half second 
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    //clear flag bit
}

void TA2_0_IRQHandler(void)
{
//TOGGLE_P20;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//clear flag bit
}

void TA3_0_IRQHandler(void)
{
    timerA3_0_interruptFlag = 1;            //set gloabl timer interrupt

	if (leftBlinker_Switch_held == 1) {	//sets blinker flags separately for evaluation separate from other calculations
		TOGGLE_Left_Blinker_flag = 1;
	}

	else if (rightBlinker_Switch_held == 1) {
		TOGGLE_Right_Blinker_flag = 1;
	}

	else if ((rightBlinker_Switch_held == 0)
			&& (leftBlinker_Switch_held == 0))
	{
		BLINKERS_OFF_flag = 1;
	}

    TIMER_A3->CCTL[0] &= ~BIT0;
}

void T32_INT1_IRQHandler()
{
	Timer32_interruptFlag = 1;			//sets T32 flag each second 
	TIMER32_1->INTCLR = 1;				//clear the internal register flag 
}

void PORT3_IRQHandler(void)
{
	if ((P3->IFG & BIT5 ) >> 5)     	     	//checks P3.5
    {
        Open_Door_Btn_interruptFlag = 1;
    }
	
	if ((P3->IFG & BIT2 ) >> 2)					//checks P3.2
			{
		NewRTC_Info_Btn_interruptFlag = 1;
	}

	if ((P3->IFG & BIT3 ) >> 3) {				//checks 3.3
		WatchdogReset_Btn_interruptFlag = 1;
	}
	//P3->IFG &= ~BIT5;
	P3->IFG = 0;
}


void PORT1_IRQHandler(void)
{
    if (CLK == 0)                        //if clock was 1 prior to the interrupt
    {
		cursor_moved = 0;
        DT = (P1->IN & BIT6 ) >> 6; //sets current value of encoder lines to these variables
        CLK = (P1->IN & BIT7 ) >> 7;       //shifted so I can compare them later

        if ((CLK == 1) && (DT == 0)) //if lines were not equal and the clk was zero before, turn is CW
        {
            CW_turns++;
        }
        else if (CLK == 1 && DT == 1)                //else, CCW
        {
            CCW_turns++;
        }
    }

    else if (CLK == 1)                   //if clock was 1 prior to the interrupt
    {
		cursor_moved = 0;
        DT = (P1->IN & BIT6 ) >> 6; //sets current value of encoder lines to these variables
        CLK = (P1->IN & BIT7 ) >> 7;       //shifted so I can compare them later

        if (CLK == 0 && DT == 1)     //if clk was 1 prior and they are not equal
            CW_turns++;

        else if ((CLK == 0) && (DT == 0))            //else, CCW
            CCW_turns++;
    }

    if (CLK)             //toggle interrupt select because they only change once
        P1->IES |= BIT7;

    else
        P1->IES &= ~BIT7;

    if (((P1->IFG & BIT5 ) >> 5) == 1) //if interrupt is for an encoder button press
        Encoder_Btn_interruptFlag = 1;

    P1->IFG = 0;                    //reset interrupt register flags on all pins
}



///////////////////////////////////////////////////
//////////////////////////////////////////////////


void main987465(void)	//main for testing proximity	| works
{
    Watchdog_halt();

    Clock_Init48MHz();
    AClock_Divide4();

    TIMERA3_0_HalfSecondInterval_init();

    LED_closeProximity();

    Enable_Interrupts();

	ST7735_InitR(INITR_REDTAB);

	TimerA0_1_PWM_init();

	ADC_pin_init();
	ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

	Enable_Interrupts();

    HCSR04_init();

    Speaker_12MHz(0, 0);

    Startup_Menu();


    while (1)
    {
		ADC_Reading();
		change_TIMERA0_1_dutyCycle();

		if (alarm_count == 5) {
			alarm_count = 0;
		}

        if (timerA3_0_interruptFlag == 1)
        {

            measure_Period();

			Read_RTC_Print_Time_toLCD();
			Read_RTC_Temperature_Print_toLCD();

			if (object_DistanceFromSensor < 15) {

				while (object_DistanceFromSensor < 15) {

					if (alarm_entered == 0) {
					Read_RTC_Store_Alarm_EEPROM(alarm_count);
					alarm_count++;
						alarm_entered = 1;
					}

					P8->OUT |= BIT2;
					
					displayParkingWarning();
					Bells(5);
					Output_Clear();
					
					measure_Period();
				}
				
				alarm_entered = 0;
				Startup_Menu();
				
			} else
                P8->OUT &= ~BIT2;

            timerA3_0_interruptFlag = 0;
        }
    }

}

void main58394756() 	//Servo_Test
{
	int rotation = 0;
	Clock_Init48MHz();
	AClock_Divide4();

	TIMERA3_0_HalfSecondInterval_init();

	Tachometer_Init();

	Enable_Interrupts();

	rotation = 0;

	while (1) {
		if (timerA3_0_interruptFlag == 1) {


			rotation += 50;
			TachometerSpeed(rotation);
			SysTick_delay_ms(10);
			TIMER_A2->CCR[2] = 0;

			timerA3_0_interruptFlag = 0;
		}
	}
}

void main98475(void)   //menus_main | showing time and date and temperature
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Clock_Init48MHz();
	AClock_Divide4();

	TIMERA3_0_HalfSecondInterval_init();

    ST7735_InitR(INITR_REDTAB);

    TimerA0_1_PWM_init();

	ADC_pin_init();
	ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

	Enable_Interrupts();

    Startup_Menu();
    

    while (1)
    {
		ADC_Reading();
		change_TIMERA0_1_dutyCycle();

		if (timerA3_0_interruptFlag == 1) {	//do this if startup menu is running

			Read_RTC_Print_Time_toLCD();
			Read_RTC_Temperature_Print_toLCD();

			timerA3_0_interruptFlag = 0;
		}

    }
}

void main09965()      //main for testing door Ajar  | works
{
    int i = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Clock_Init48MHz();

    SysTickInit_NoInterrupts();

    openDoor_Btn_init();
    SPI_init();

	MAX219_SPI_init_Decode();

    Enable_Interrupts();

    while (1)
    {


        Open_Door_Btn_heldDown = (!(P3IN & 0x20));

        for (i = 0; i < 15; i++)
        {
            Debounce_DoorOpen_Btn();
        }

		if (Open_Door_Btn_interruptFlag == 1)                 //interrupt occurs
				{
			if (Open_Door_Btn_pressed == 1)       //confirms debounce successful
					{
				DoorAjar_Btn_HeldDown();
				Open_Door_Btn_pressed = 0;                 //reset debounce flag
				Open_Door_Btn_interruptFlag = 0;
			}
			MAX219_SPI_init_Decode();
		}

		else


		MAX219_SPI_TransferData(0x04, 0x07);         //display 74
		MAX219_SPI_TransferData(0x04, 0x04);

    }
}

void main96847(void)      //Blinker testing main	| works
{
    Watchdog_halt();

    Clock_Init48MHz();
    AClock_Divide4();

    Blinkers_Toggle_Switch_init();

    Left_Blinker();
    Right_Blinker();

    TIMERA3_0_HalfSecondInterval_init();
    Enable_Interrupts();

    while (1)
    {

        leftBlinker_Switch_held = (!(P3IN & BIT6 ));
        rightBlinker_Switch_held = (!(P3IN & BIT7 ));

        if ((timerA3_0_interruptFlag == 1) && (leftBlinker_Switch_held == 1))
        {
            TOGGLE_Left_Blinker;

            timerA3_0_interruptFlag = 0;
        }

        else if ((timerA3_0_interruptFlag == 1)
                && (rightBlinker_Switch_held == 1))
        {
            TOGGLE_Right_Blinker;

            timerA3_0_interruptFlag = 0;
        }
        else if ((leftBlinker_Switch_held == 0)
                && (rightBlinker_Switch_held == 0))
        {
            BLINKERS_OFF;
        }
    }
}

void main583847() { 			//digital speed test

	int i;
	Watchdog_halt();

	Clock_Init48MHz();
	AClock_Divide4();

	ST7735_InitR(INITR_REDTAB);
	TimerA0_1_PWM_init();

	SysTickInit_NoInterrupts(); //must be done right before initializing stepper
	Stepper1_Motor();

	Hall_Effect_Out_init();

	Timer32_init();

	openDoor_Btn_init();
	SPI_init();

	MAX219_SPI_init_Decode();

	ADC_pin_init();
	ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

	Enable_Interrupts();

	while (1) {
		if (alarm_count == 5) {
			alarm_count = 0;
		}

		ADC_Reading();
		change_TIMERA0_1_dutyCycle();

		Open_Door_Btn_heldDown = (!(P3IN & 0x20));

		for (i = 0; i < 15; i++) {
			Debounce_DoorOpen_Btn();
		}

		DoorAjar_Btn_HeldDown();

		if (Timer32_interruptFlag == 1) {
			RPMs = measure_RPMs(Revolution_count);

			Timer32_interruptFlag = 0;
			Revolution_count = 0;

			Current_Speed_mph = measure_Speed(RPMs,
			CIRCUMFERENCE_6ftDiameter);

			speed_Dif = (Current_Speed_mph - lastSpeed);

			if (RPMs == 0) {
				if (RPMs_areZero == 0) {
					Rotate_Stepper1_Backward_s_Steps(120);
					RPMs = 0;
					RPMs_areZero = 1;
				}

			}

			else if (RPMs > 2) {
				RPMs_areZero = 0;
				if (firstTime_done == 0) {
					firstTime_done = 1;

					Rotate_Stepper1_Forward_s_Steps(Current_Speed_mph);
					RPMs = 0;

					if (Current_Speed_mph > 85) {
						Read_RTC_Store_Alarm_EEPROM(alarm_count);
						alarm_count++;
					}
				} else {
					if (speed_Dif == 0) {
						RPMs = 0;
					} else if (speed_Dif > 0) {
						Rotate_Stepper1_Forward_s_Steps(speed_Dif);
						RPMs = 0;

						if (speed_Dif > 85) {
							Read_RTC_Store_Alarm_EEPROM(alarm_count);
							alarm_count++;
						}
					} else if (speed_Dif < 0) {
						Rotate_Stepper1_Backward_s_Steps(abs(speed_Dif));
						RPMs = 0;
					}
				}
			}

			lastSpeed = Current_Speed_mph;

		}
	}
}

void main94857() {	//watchdog test main
	Watchdog_halt();

	Clock_Init48MHz();
	AClock_Divide4();

	int i;

	while (1) {
		for (i = 0; i < 15; i++) {
			//debounce warchdog button
		}
	}
}


void main723454() //Motor_main | works
{
    Watchdog_halt();

    Clock_Init48MHz();
    AClock_Divide4();

	//ST7735_InitR(INITR_REDTAB);
	//TimerA0_1_PWM_init();

    SysTickInit_NoInterrupts(); //must be done right before initializing stepper
    Stepper1_Motor();

    Hall_Effect_Out_init();

	//MAX219_SPI_init_Decode();

    Timer32_init();

	//ADC_pin_init();
	//ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

    Enable_Interrupts();

    while (1)
    {
		if (alarm_count == 5) {
			alarm_count = 0;
		}

		//ADC_Reading();
		//change_TIMERA0_1_dutyCycle();


        if (Timer32_interruptFlag == 1)
        {
            RPMs = measure_RPMs(Revolution_count);

            Timer32_interruptFlag = 0;
            Revolution_count = 0;

            Current_Speed_mph = measure_Speed(RPMs,
            CIRCUMFERENCE_6ftDiameter);

			MAX219_SPI_TransferData(0x04, Current_Speed_mph / 10);  //display 74
			MAX219_SPI_TransferData(0x04, Current_Speed_mph % 10);
            speed_Dif = (Current_Speed_mph - lastSpeed);

            if (RPMs == 0)
            {
                if (RPMs_areZero == 0)
                {
                    Rotate_Stepper1_Backward_s_Steps(120);
                    RPMs = 0;
                    RPMs_areZero = 1;
                }

            }

            else if (RPMs > 2)
            {
                RPMs_areZero = 0;
                if (firstTime_done == 0)
                {
                    firstTime_done = 1;

                    Rotate_Stepper1_Forward_s_Steps(Current_Speed_mph);

                    RPMs = 0;

					if (Current_Speed_mph > 85) {
						Read_RTC_Store_Alarm_EEPROM(alarm_count);
						alarm_count++;
					}
                }
                else
                {
                    if (speed_Dif == 0)
                    {
                        RPMs = 0;
                    }
                    else if (speed_Dif > 0)
                    {
                        Rotate_Stepper1_Forward_s_Steps(speed_Dif);
                        RPMs = 0;

						if (speed_Dif > 85) {
							Read_RTC_Store_Alarm_EEPROM(alarm_count);
							alarm_count++;
						}
                    }
                    else if (speed_Dif < 0)
                    {
						Rotate_Stepper1_Backward_s_Steps(abs(speed_Dif));
                        RPMs = 0;
                    }
                }
            }

            lastSpeed = Current_Speed_mph;

        }
    }
}




////////////////////////////////////////////
//component integration main 
void main86938() {

	int i;
	Clock_Init48MHz();
	AClock_Divide4();

	TIMERA3_0_HalfSecondInterval_init();

	ST7735_InitR(INITR_REDTAB);

	TimerA0_1_PWM_init();

	ADC_pin_init();
	ADC_StartConversion_WithInterrupts();   	//Photocell         | 6.1 | ADC

	Encoder_init();

	Blinkers_Toggle_Switch_init();

	Left_Blinker();
	Right_Blinker();

	openDoor_Btn_init();
	NewRTC_Info_Btn();

	SPI_init();

	MAX219_SPI_init_Decode();
	Print_Zeros_MAX219();

	Enable_Interrupts();

	Startup_Menu();

	while (1)
	{
		ADC_Reading();
		change_TIMERA0_1_dutyCycle();


		leftBlinker_Switch_held = (!(P3IN & BIT6 ));
		rightBlinker_Switch_held = (!(P3IN & BIT7 ));

		Open_Door_Btn_heldDown = (!(P3IN & 0x20));

		for (i = 0; i < 15; i++) {
			Debounce_DoorOpen_Btn();
			Debounce_EncoderButton(); //global debounce flag for encoder button;
			Debounce_NewRTC_Info_Btn();

		}

		if (Open_Door_Btn_interruptFlag == 1)                 //interrupt occurs
				{
			if (Open_Door_Btn_pressed == 1)       //confirms debounce successful
					{
				DoorAjar_Btn_HeldDown();
				Open_Door_Btn_pressed = 0;                 //reset debounce flag
				Open_Door_Btn_interruptFlag = 0;
			}
			MAX219_SPI_init_Decode();
		}

		
		
		if (Encoder_Btn_interruptFlag == 1) {
			if (Encoder_Btn_pressed == 1) {
				MENU();
			}
		}

		if (NewRTC_Info_Btn_interruptFlag == 1) {
			if (NewRTC_Info_Btn_pressed == 1) {

				if ((menu == 2 || menu == 3)) {
					Enter_Date_Time_Btn_count++;

					NewRTC_Info_Btn_interruptFlag = 0;
					NewRTC_Info_Btn_pressed = 0;
					reset_Encoder_Counts();
				}
			}
		}


		if (TOGGLE_Left_Blinker_flag == 1) {
			TOGGLE_Left_Blinker;
			TOGGLE_Left_Blinker_flag = 0;
		}

		else if (TOGGLE_Right_Blinker_flag == 1) {
			TOGGLE_Right_Blinker;
			TOGGLE_Right_Blinker_flag = 0;
		}

		else if (BLINKERS_OFF_flag == 1) {
			BLINKERS_OFF;
			BLINKERS_OFF_flag = 0;
		}




		switch (menu) {

		case 0: {

			if (timerA3_0_interruptFlag == 1) {	//do this if startup menu is running

				Read_RTC_Print_Time_toLCD();
				Read_RTC_Temperature_Print_toLCD();

				timerA3_0_interruptFlag = 0;
			}

			break;
		}
		case 1:
		{
			control_Cursor();
			break;
		}
		case 2: {
			control_Cursor();
			break;
		}
		case 3: {
			control_Cursor();
			break;
		}
		case 4: {
			break;
		}

		default:
		{
			break;
		}

	}
			
	}
}


