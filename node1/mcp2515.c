#include "mcp2515.h"
#include "spi.h"

uint8_t MCP2515_Read(uint8_t address) {
    uint8_t data;

    SPI_Select(2);

    SPI_WriteByte(0x03);
    SPI_WriteByte(address);

    data = SPI_ReadByte();

    SPI_Deselect(2);

    return data;
}

void MCP2515_Write(uint8_t address, uint8_t data) {
    SPI_Select(2);

    SPI_WriteByte(0x02);

    SPI_WriteByte(address);
    SPI_WriteByte(data);

    SPI_Deselect(2);
}

void MCP2515_RTS(uint8_t buffer) {
    SPI_Select(2);

    SPI_WriteByte(0x80 | (buffer & 0x07)); //bestemmer de tre siste bitsene i command byten: 1000 0nnn

    SPI_Deselect(2);
}

uint8_t MCP2515_ReadStatus(void) {
    uint8_t status;

    SPI_Select(2);

    SPI_WriteByte(0xA0);

    status = SPI_ReadByte();

    SPI_Deselect(2);

    return status;
}

void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
    SPI_Select(2);

    SPI_WriteByte(0x05);

    SPI_WriteByte(address);

    SPI_WriteByte(mask);

    SPI_WriteByte(data);

    SPI_Deselect(2);
}

void MCP2515_Reset(void) {
    SPI_Select(2);

    SPI_WriteByte(0xC0);

    SPI_Deselect(2);
}

