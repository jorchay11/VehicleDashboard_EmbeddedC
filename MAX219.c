/*
 * MAX219.c
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */



#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "MAX219.h"
#include "SysTick_Library.h"

void SPI_init()
{
    //P2.1
    //P2.3
    //

    EUSCI_A1->CTLW0 = 0x0001;           //disable during config
    EUSCI_A1->CTLW0 = 0xA9C1; //CLK polarity: 11, MSB first, 8 bit, master, 3- pin SPI
                              //Synchronous, SMCLK, no STE
                              //0x1010_1001_1000_0001 ::
	EUSCI_A1->BRW = 4;                  //'divide to 128KHz
    EUSCI_A1->CTLW0 &= ~0x0001;

    P2->SEL1 &= ~(BIT3 | BIT1 );        //2.1  CLK
    P2->SEL0 |= (BIT3 | BIT1 );         //2.3 SIMO
    P2->REN |= (BIT3 | BIT1 );
    P2->OUT |= (BIT3 | BIT1 );

	MAX219_CS_init();					//initialize Chip select pin, GPIO
}

//initializes the CS pin of the MAX219, idles high 
void MAX219_CS_init()
{
    P2->SEL1 &= ~BIT0;
    P2->SEL0 &= ~BIT0;                //GPIO CS 2.0
    P2->DIR |= BIT0;                  //DIR = output
    P2->OUT |= BIT0;                  //Idle high
}

void MAX219_SPI_init_NoDecode()
{
    uint8_t i;

    char seg7_setupSequence_CMD[6] = { NODECODEMODECMD, INTENSITYCMD,
                                       BIT8MODECMD,
                                       DISABLESHUTDOWNCMD, TESTCMD };
    char seg7_setupSequence_DT[6] = { NODECODEMODEDT, INTENSITYDT, BIT8MODEDT,
                                      DISABLESHUTDOWNDT, TESTDT };



    for (i = 0; i < 6; i++)                   //5 commands, last must be nuls???
    {
        CS_LOW;                                        //set CS low (active low)

		SysTick_delay_ms(1);                            //wait 1 millisecond

        while (!(EUSCI_A1->IFG & 2))
            ; //wait for EUSCI flag bit to acknowledge communication is successful

        EUSCI_A1->TXBUF = seg7_setupSequence_CMD[i];            //Send Command

        while (!(EUSCI_A1->IFG & 2))
            ;                            //wait again

        EUSCI_A1->TXBUF = seg7_setupSequence_DT[i];             //Send Data

        while (!(EUSCI_A1->IFG & 2))
            ;                            //wait for flag

		SysTick_delay_ms(1);

        CS_HIGH;                         //set CS after successful data transfer
    }

    CS_LOW;                                                     //?????

	SysTick_delay_ms(1);

    while (!(EUSCI_A1->IFG & 2))
        ;

    EUSCI_A1->TXBUF = 0x0F;                                //Command Decode Mode

    while (!(EUSCI_A1->IFG & 2))
        ;

    EUSCI_A1->TXBUF = 0x00;                                //Data Decode Mode

    while (!(EUSCI_A1->IFG & 2))
        ;

	SysTick_delay_ms(1);

    CS_HIGH;
}

void MAX219_SPI_init_Decode() {
	uint8_t i;

	char seg7_setupSequence_CMD[6] = { DECODEMODECMD, INTENSITYCMD, BIT8MODECMD,
			DISABLESHUTDOWNCMD, TESTCMD };
	char seg7_setupSequence_DT[6] = { DECODEMODEDT, INTENSITYDT, BIT8MODEDT,
			DISABLESHUTDOWNDT, TESTDT };

	for (i = 0; i < 6; i++)                   //5 commands, last must be nuls???
			{
		CS_LOW;                                        //set CS low (active low)

		SysTick_delay_ms(1);                             //wait 1 millisecond

		while (!(EUSCI_A1->IFG & 2))
			; //wait for EUSCI flag bit to acknowledge communication is successful

		EUSCI_A1->TXBUF = seg7_setupSequence_CMD[i];            //Send Command

		while (!(EUSCI_A1->IFG & 2))
			;                            //wait again

		EUSCI_A1->TXBUF = seg7_setupSequence_DT[i];             //Send Data

		while (!(EUSCI_A1->IFG & 2))
			;                            //wait for flag

		SysTick_delay_ms(1);

		CS_HIGH;                         //set CS after successful data transfer
	}

	CS_LOW;                                                     //?????

	SysTick_delay_ms(1);

	while (!(EUSCI_A1->IFG & 2))
		;

	EUSCI_A1->TXBUF = 0x0F;                                //Command Decode Mode

	while (!(EUSCI_A1->IFG & 2))
		;

	EUSCI_A1->TXBUF = 0x00;                                   //Data Decode Mode

	while (!(EUSCI_A1->IFG & 2))
		;

	SysTick_delay_ms(1);

	CS_HIGH;

	Print_Zeros_MAX219();

}

void MAX219_SPI_TransferData(uint8_t cmd, uint8_t data)
{

    CS_LOW;             //set  CS low to initiate communication with this device

	SysTick_delay_ms(1);         //wait 1 ms for good measure

    while (!(EUSCI_A1->IFG & 2))
        ;    //wait

    EUSCI_A1->TXBUF = cmd;          //Send Command

    while (!(EUSCI_A1->IFG & 2))
        ;    //wait

    EUSCI_A1->TXBUF = data;         //Send Data

    while (!(EUSCI_A1->IFG & 2))
        ;    //wait

	SysTick_delay_ms(1);

    CS_HIGH;                        //set CS after communication

}

void Print_DoorAjar_MAX219(void)
{

    uint8_t i;
    uint8_t j = 0;
    
    //fill array with Data commands for no decode mode 
    char seg7_doorAJAr_DT[8] = { MAX219_D_DT, MAX219_o_DT, MAX219_o_DT,
                                 MAX219_r_DT, MAX219_A_DT, MAX219_J_DT,
                                 MAX219_A_DT,
                                 MAX219_r_DT };

	//i go from 8 to 1
	//j goes 0-7
    for (i = 8; i > 0; i--)
    {
        MAX219_SPI_TransferData(i, seg7_doorAJAr_DT[j]);
        j++;
    }
    
}

//function to turn intensity to zero on digits, 
//blanking the MAX219 digits 
void blank_MAX219()
{
    MAX219_SPI_TransferData(0x0C, 0x00);
}

//prints zeros to each character of the segment 
void Print_Zeros_MAX219(void)
{
    uint8_t i;           //counter to sequence through each character on the MAX

    for (i = 1; i < 9; i++)              //for each character

    {
        CS_LOW;

		SysTick_delay_ms(1);         //wait 1 ms for good measure

        while (!(EUSCI_A1->IFG & 2))
            ;    //wait for flag

        EUSCI_A1->TXBUF = i;            //will be 1-8

        while (!(EUSCI_A1->IFG & 2))
            ;    //wait

        EUSCI_A1->TXBUF = 0x00;         //Send zeros

        while (!(EUSCI_A1->IFG & 2))
            ;    //wait

		SysTick_delay_ms(1);

        CS_HIGH;                        //set CS bit

    }
}

