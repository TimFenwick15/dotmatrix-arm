#ifndef _I2C_H
#define _I2C_H

bool I2C_vInit(void);
void I2C_vWriteMemory(uint8_t u8MemoryAddress, uint8_t* pu8Buffer, uint8_t u8Length);
void I2C_vReadMemory(uint8_t u8MemoryAddress, uint8_t* pu8Buffer, uint8_t u8Length);

#endif // _I2C_H
