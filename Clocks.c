/*
 * Clocks.c
 *
 *  Created on: Nov 25, 2020
 *      Author: 7jorchay
 */
#include <stdio.h>
#include <stdint.h>
#include "msp.h"
#include "Clocks.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*
 *                  MCLK | SMCLK
 */
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void Clock_Init48MHz(void)
{
    // Configure Flash wait-state to 1 for both banks 0 & 1
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
    FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
    FLCTL_BANK1_RDCTL_WAIT_1;

    //Configure HFXT to use 48MHz crystal, source to MCLK & HSMCLK*
    PJ->SEL0 |= BIT2 | BIT3;               // Configure PJ.2/3 for HFXT function
    PJ->SEL1 &= ~(BIT2 | BIT3 );
    CS->KEY = CS_KEY_VAL;                // Unlock CS module for register access
    CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
    while (CS->IFG & CS_IFG_HFXTIFG)
        CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

    /* Select MCLK & HSMCLK = HFXT, no divider */
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK |
    CS_CTL1_DIVM_MASK |
    CS_CTL1_SELS_MASK |
    CS_CTL1_DIVHS_MASK) |
    CS_CTL1_SELM__HFXTCLK |
    CS_CTL1_SELS__HFXTCLK;

    CS->CTL1 = CS->CTL1 | CS_CTL1_DIVS_2; // change the SMCLK clock speed to 12 MHz.

    CS->KEY = 0;                      // Lock CS module from unintended accesses
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*
 *                  Watchdog
 */
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//initializes watchdog timer for watchdog mode, sending program into infinite while loop.
//after 4 seconds, the timer will time out and reset the program
void Watchdog_init()
{
    WDT_A->CTL = WDT_A_CTL_PW | 0x0024;          //00100100
                                                 //1s at 32.768 KHz
                                                 //ACLK, watchdog mode
	while (1) {
		;
	}
}

//function can reset the watchdog count value before program resets
void Watchdog_resetCount()
{
    CS->KEY = CS_KEY_VAL;                                   //password needed

    WDT_A->CTL = WDT_A_CTL_PW | WDTCNTCL | 0x0024;       //resets watchdog count

    CS->KEY = 0;
}

//halts the watchdog timer
void Watchdog_halt()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*
 *                  ACLK
 */
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//didives the internal ACLK by 4 to reach longer intervals at 48MHz
void AClock_Divide4()
{
    CS->KEY = CS_KEY_VAL;                          //open register with password
    CS->CTL1 |= BIT(25) | BIT9;                     //bit25 for ACLK / 4
                                                    //bit9 for REFOCLK
    CS->KEY = 0;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*
 *                  TIMER32
 */
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//initializes TIMER32 for a one second interval with interrupts
void Timer32_init(void)
{
	TIMER32_1->LOAD = (48000000) - 1;      //change this, is one second now
    TIMER32_1->CONTROL = 0b11100010;         //source to a different clock?
	NVIC_EnableIRQ(T32_INT1_IRQn);//might want to move this to enable interrupts
}


