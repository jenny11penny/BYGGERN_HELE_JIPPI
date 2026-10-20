#define F_CPU 4915200UL
#include "OLED.h"
#include "spi.h"
#include "fonts.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#define OLED_DC PB2

static void OLED_DC_Init(void) {
    DDRB |= (1 << OLED_DC); 
}

static void OLED_Command(uint8_t cmd) {
    PORTB &= ~(1 << OLED_DC); //DC=lav -> kommando
    SPI_Select(1);
    SPI_WriteByte(cmd);
    SPI_Deselect(1);
}

static void OLED_Data(uint8_t data) {
    PORTB |= (1 << OLED_DC);    // DC=høy -> data
    SPI_Select(1);
    SPI_WriteByte(data);
    SPI_Deselect(1);
}

void OLED_Init(void) { //Fant ikke init fil, så brukte ChatGPT 
    OLED_DC_Init();

    OLED_Command(0xAE); //Display av

    OLED_Command(0xD5); OLED_Command(0x80); // klokkefrekvens
    OLED_Command(0xA8); OLED_Command(0x3F); // multiplex ratio (64-1)
    OLED_Command(0xD3); OLED_Command(0x00); // display offset
    OLED_Command(0x40);                     // start linje 0

    OLED_Command(0x20); OLED_Command(0x02); // page adresseringsmodus
    OLED_Command(0xA1);                     // segment remap
    OLED_Command(0xC8);                     // COM scan retning

    OLED_Command(0xDA); OLED_Command(0x12); // COM pins config
    OLED_Command(0x81); OLED_Command(0xCF); // kontrast 0xCF = 207
    OLED_Command(0xD9); OLED_Command(0x22); // pre-charge periode
    OLED_Command(0xDB); OLED_Command(0x40); // VCOMH nivå

    OLED_Command(0xA4); // vis RAM-innhold (ikke "all pixels on")
    OLED_Command(0xA6); // normal (ikke invertert) visning

    OLED_Command(0xAF); // display på

    _delay_ms(100);
    OLED_Clear();
    OLED_Home();
}

void OLED_GotoLine(uint8_t line) {
    OLED_Command(0xB0 | (line & 0x07));  // side-adresse (0-7)
}

void OLED_GotoColumn(uint8_t column) {
    OLED_Command(0x00 | (column & 0x0F));        // kolonne, lav nibble
    OLED_Command(0x10 | ((column >> 4) & 0x0F)); // kolonne, høy nibble
}

void OLED_Pos(uint8_t row, uint8_t column) {
    OLED_GotoLine(row);
    OLED_GotoColumn(column);
}

void OLED_Home(void) {
    OLED_Pos(0, 0);
}

void OLED_ClearLine(uint8_t line) {
    OLED_GotoLine(line);
    OLED_GotoColumn(0);
    for (uint8_t col = 0; col < 128; col++) {
        OLED_Data(0x00);
    }
}

void OLED_Clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_ClearLine(page);
        }
        OLED_Home();
}

void OLED_WriteChar(char c) {
    if (c < ' ' || c > '~') {
        c = ' '; // ukjent tegn -> tomt felt
    }
    uint8_t index = c - ' ';

    for (uint8_t i = 0; i < 5; i++) {
        OLED_Data(pgm_read_byte(&font5[index][i]));
    }
    OLED_Data(0x00); // ett tomt kolonne mellom tegn
}

void OLED_Print(const char *str) {
    while (*str) {
        OLED_WriteChar(*str);
        str++;
    }
}

