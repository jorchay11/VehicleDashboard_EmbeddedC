/*
 * I2C_ProjectFunctions.c
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */





#include "msp.h"
#include "stdio.h"

#include "I2C_ProjectFunctions.h"

#define slaveAddrRTC 0x68


//function only good for one burst of commuication
void I2C1_init(void)
{

    EUSCI_B1->CTLW0 |= 1;               //reset UCB0 to configure

	EUSCI_B1->CTLW0 = 0x0F81; //7-bit slave addr, master, I2C, synch mode, use SMCLK

	//EUSCI_B1->BRW = 30;
	EUSCI_B1->BRW = 120;             //set clock prescalar 12MHz / 120 = 100 kHz

    P6->SEL0 |= 0x30;                   //P6.5 CLK and P6.4 DATA for UCB1

    P6->SEL1 &= ~0x30;

    EUSCI_B1->CTLW0 &= ~1;              //enable UCB0 after configuration

}

/***********************

 * BURST READ (MAZIDI)

 **********************/

int I2C1_burstRead(int slaveAdd, unsigned char memAddr, int byteCount,
                   unsigned char* data)
{

    if (byteCount <= 0)

        return -1;                      //no write was preformed

    EUSCI_B1->I2CSA = slaveAdd;         //setup slave adress

    EUSCI_B1->CTLW0 |= 0x0010;          //enable transmitter

    EUSCI_B1->CTLW0 |= 0x0002;          //generate START and send slave address

    while (!(EUSCI_B1->IFG & 2))
        ;        //wait till ack for transmission

    EUSCI_B1->TXBUF = memAddr;          //send memory address to slave

    while (!(EUSCI_B1->IFG & 2))
        ;        //wait till last transmission

    EUSCI_B1->CTLW0 &= ~0x0010;         //enable reciever

    EUSCI_B1->CTLW0 |= 0x0002;         //generate RESTART and send slave address

    while (EUSCI_B1->CTLW0 & 2)
        ;         //wait till RESTART is finished

    do
    {
        if (byteCount == 1)              //when only one byte of data is left
            EUSCI_B1->CTLW0 |= 0x004; //setup to send STOP after the last byte is recieved

        while (!(EUSCI_B1->IFG & 1))
            ;     //wait till data is recieved

        *data++ = EUSCI_B1->RXBUF;      //read the recieved data

        byteCount--;

    }
    while (byteCount);

    while (EUSCI_B1->CTLW0 & 4)
        ;         //wait until the STOP is sent

    return 0;                           //no error

}

/***********************

 * BURST WRITE (MAZIDI)

 **********************/

int I2C1_burstWrite(int slaveAdd, unsigned char memAddr, int byteCount,
                    unsigned char* data)

{

    if (byteCount <= 0)

        return -1;                      //no write was preformed

    EUSCI_B1->I2CSA = slaveAdd;         //setup slave adress

    EUSCI_B1->CTLW0 |= 0x0010;          //enable transmitter

    EUSCI_B1->CTLW0 |= 0x0002;          //generate START and send slave address

    while (!(EUSCI_B1->IFG & 2))
        ;        //wait till ack for transmission

    EUSCI_B1->TXBUF = memAddr;          //send memory address to slave

    //send data one byte at a time

    do
    {
        while (!(EUSCI_B1->IFG & 2))
            ;     //wait till ready for transmission

        EUSCI_B1->TXBUF = *data++;      //send data to slave

        byteCount--;

    }
    while (byteCount > 0);

    while (!(EUSCI_B1->IFG & 2))
        ;         //wait till last transmission

    EUSCI_B1->CTLW0 |= 0x0004;          //send STOP

    while (EUSCI_B1->CTLW0 & 4)
        ;         //wait until STOP is sent back

    return 0;
}
