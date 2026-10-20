#include <stdint.h>

uint8_t MCP2515_Read(uint8_t address);

void MCP2515_Write(uint8_t address, uint8_t data);

void MCP2515_RTS(uint8_t buffer);

uint8_t MCP2515_ReadStatus(void);

void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data);

void MCP2515_Reset(void);

