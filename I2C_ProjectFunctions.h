/*
 * I2C_ProjectFunctions.h
 *
 *  Created on: Nov 19, 2020
 *      Author: 7jorchay
 */

#ifndef I2C_PROJECTFUNCTIONS_H_
#define I2C_PROJECTFUNCTIONS_H_

void I2C1_init();



int I2C1_burstWrite(int slaveAdd, unsigned char memAddr, int byteCount,
                    unsigned char* data);

int I2C1_burstRead(int slaveAdd, unsigned char memAddr, int byteCount,
                   unsigned char* data);


#endif /* I2C_PROJECTFUNCTIONS_H_ */
