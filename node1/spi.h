#ifndef SPI_H
#define SPI_H

#include <stdint.h>


void SPI_Init(void);
void SPI_Select(uint8_t slave);
void SPI_Deselect(uint8_t slave);
uint8_t SPI_WriteByte(uint8_t data);
uint8_t SPI_ReadByte(void);
void SPI_WriteBytes(uint8_t *data, uint8_t len);
void SPI_ReadBytes(uint8_t *buffer, uint8_t len);

void SPI_test(void);

#endif