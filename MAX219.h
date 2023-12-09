/*
 * MAX219.h
 *
 *  Created on: Nov 29, 2020
 *      Author: 7jorchay
 */

#ifndef MAX219_H_
#define MAX219_H_


#define DECODEMODECMD           0x09
#define DECODEMODEDT            0xFF

#define NODECODEMODECMD         0x09
#define NODECODEMODEDT          0x00

#define INTENSITYCMD            0x0A
#define INTENSITYDT             0x03

#define BIT8MODECMD             0x0B
#define BIT8MODEDT              0x07

#define DISABLESHUTDOWNCMD      0x0C
#define DISABLESHUTDOWNDT       0x01

#define TESTCMD                 0x0F
#define TESTDT                  0x01

#define CS_LOW                  P2 -> OUT &= ~BIT0
#define CS_HIGH                 P2 -> OUT |= BIT0

//door_ajar values. put in loop going 8 to 0 to print on each character of MAX219
#define MAX219_D_DT             0x3D
#define MAX219_o_DT             0x1D
#define MAX219_r_DT             0x05
#define MAX219_A_DT             0x77
#define MAX219_J_DT             0x38

void SPI_init(void);
void MAX219_CS_init(void);


void MAX219_SPI_init_Decode(void);

void MAX219_SPI_init_NoDecode(void);

void MAX219_CS_init(void);

void MAX219_SPI_TransferData(uint8_t cmd, uint8_t data);

void Print_Zeros_MAX219(void);

void Print_DoorAjar_MAX219(void);

void blank_MAX219(void);



#endif /* MAX219_H_ */
