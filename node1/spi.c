#include <stdint.h>
#include "spi.h"
#include <avr/io.h>

#define SS1 PB4 //IO slave
#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define SS2 PB3 //OLED slave
#define SS3 PD3//MCP2515 slave

void SPI_Init(void) {
    //MOSI, SCK og begge SS som utganger. 
    DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS1) | (1 << SS2);

    DDRB &= ~(1 << MISO); //MISO som inngang

    DDRD |= (1 << SS3); //Sett SS til MCP2515(PD3) som inngang

    //Alle slaver antiselect (aktiv lav -> sett høy)
    PORTB |= (1 << SS1) | (1 << SS2);
    PORTD |= (1 << SS3);

    //Enable SPI, Master mode, clock = fosc/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_Select(uint8_t slave) {
    if (slave == 0) { //Hvis Slave IO er valgt
        PORTB &= ~(1 << SS1);
    }
    else if (slave == 1) { //Hvis OLED slave er valgt
        PORTB &= ~(1 << SS2);
    }
    else if (slave == 2) {
        PORTD &= ~(1 << SS3); //Hvis Mcp2515 er valgt
    }
}

void SPI_Deselect(uint8_t slave) {
    if (slave == 0) { //SS IO
        PORTB |= (1 << SS1);
    } else if (slave == 1) { //SS OLED
        PORTB |= (1 << SS2);
    }
    else if (slave == 2) { //SS MCP2515
        PORTD |= (1 << SS3);
    }
}

uint8_t SPI_WriteByte(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
    return SPDR;
}

uint8_t SPI_ReadByte(void) {
    return SPI_WriteByte(0xFF);
}

void SPI_WriteBytes(uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        SPI_WriteByte(data[i]);
    }
}

void SPI_ReadBytes(uint8_t *buffer, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        buffer[i] = SPI_ReadByte();
    }
}

void SPI_test(void) {
    SPI_Select(1);
    SPI_WriteByte(0xA5);  
    SPI_Deselect(1);
}