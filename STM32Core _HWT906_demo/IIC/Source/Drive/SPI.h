#ifndef _SPI_H
#define _SPI_H
#include "stm32f10x_spi.h"
#include "IO.h"
unsigned char SPIReadWriteByte(unsigned char TxData);
void SPIWriteOneBytes(SPin PinCs,unsigned char ucAddr,unsigned char ucData);
void SPIWriteBytes(SPin PinCs,unsigned char ucAddr,unsigned char ucLength,unsigned char ucData[]);
void SPIReadBytes(SPin PinCs,unsigned char ucAddr,unsigned char ucLength,unsigned char ucData[]);
void SPIWrite(SPin PinCs,unsigned char ucAddr,unsigned char ucData);
unsigned char SPIRead(SPin PinCs,unsigned char ucAddr);
void SPI_Initial(unsigned short usCPOL,unsigned short usCPHA);

#endif


