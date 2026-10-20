#include "io_board.h"
#include "spi.h"
#include <util/delay.h>
#include <stdio.h>

Buttons IO_Readbuttons(void) {
    Buttons buttons;

    SPI_Select(0); //velger IO_boardet

    SPI_WriteByte(0x04); //send button kommando
    _delay_us(50);

    buttons.right = SPI_ReadByte(); //leser 8 bits = 1 byte
    _delay_us(10);

    buttons.left = SPI_ReadByte();
    _delay_us(10);

    buttons.nav = SPI_ReadByte();
    
    SPI_Deselect(0);

    return buttons;
}

void led_test(uint8_t led, uint8_t on) {
    SPI_Select(0); //velger IO_boardet

    SPI_WriteByte(0x05); //send led kommando
    _delay_us(40);

    SPI_WriteByte(led); //velg led 1
    SPI_WriteByte(on); //1 == på, 0 == av

    SPI_Deselect(0);
}

void buttons_test(void) {
    Buttons buttons;

    while (1) {
        buttons = IO_Readbuttons();

        printf("right: %u, left: %u, nav: %u\r\n", buttons.right, buttons.left, buttons.nav);

        led_test(3, buttons.R5);

        _delay_ms(500);
    }
}

void IO_Info(void) {
    uint8_t data[10];

    SPI_Select(0);

    SPI_WriteByte(0x07);
    _delay_us(40);

    for (uint8_t i = 0; i < 10; i++)
    {
        data[i] = SPI_ReadByte();
        _delay_us(2);
    }

    SPI_Deselect(0);

    for (uint8_t i = 0; i < 10; i++)
    {
        printf("%u: %u (0x%02X)\r\n",
               i, data[i], data[i]);
    }

    printf("\r\n");
}