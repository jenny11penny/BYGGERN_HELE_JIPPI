#define F_CPU 4915200UL
#define BAUD 9600
#define MYUBRR 31

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "sram_test.h"
#include "adc.h"
#include "spi.h"
#include "OLED.h"
#include "menu.h"
#include "io_board.h"
#include "can.h"
#include "mcp2515.h"

void wait_for_button(void) 
{
    while (!joystick_btn_pressed()) {
    }
    while (joystick_btn_pressed()) {
    }
}

void show_lives(uint8_t lives)
{
    char buf[17];
    OLED_Pos(2, 0);
    sprintf(buf, "Liv: %d", lives);
    OLED_Print(buf);
}


void play_game(void)
{
    CAN_message joy;
    CAN_message rx;
    uint8_t lives = 3;
 
    joy.id = 0x10;
    joy.length = 3;

    while (CAN_Receive(&rx)) { //Tømmer gamle meldinger
    }

    show_lives(lives);

 
    while (1) {
        Position pos = ADC_read_pos();
        joy.data[0] = (int8_t)pos.x;
        joy.data[1] = (int8_t)pos.y;
        joy.data[2] = joystick_btn_pressed();
        CAN_SendMessage(&joy);
 
        if (CAN_Receive(&rx) && rx.id == 0x20) {
            lives --;
            show_lives(lives);

            if (lives == 0) {
                break; //game over
            }
            
        }
        _delay_ms(50);
    }
}

int main(void)
{
    USART_Init(MYUBRR);
    fdevopen(uart_putchar, uart_getchar);
    printf("Hei!");

    MCUCR |= (1 << SRE); //aktivere external memomry

    SFIOR |= (1 << XMM2); //


    ADC_clock_init();
    ADC_calibrate();

    SPI_Init();
    OLED_Init();
    OLED_Clear();

    OLED_Pos(0,0);

    joystick_btn_init();
    CAN_Init();

    const char *options[] = {"Glad?: Spill!", "Sur?: Ikke spill!"};
    uint8_t size = sizeof(options) / sizeof(options[0]);

    menu_init(options, size);
 
    while (1) //KI-deklarasjon: menyen er fra claude
    {
        if (!joystick_btn_pressed()) {
            menu_update(options, size);
            _delay_ms(200);
            continue;
        }

        menu_close();
        int selected = get_selected();
        while (joystick_btn_pressed()) {
        } // vent til knappen slippes, så spillet ikke skyter med en gang

        OLED_Clear();
        OLED_Pos(0, 0);

            if (selected == 0) {
            OLED_Print("Spiller...");
            play_game();
            OLED_Clear();
            OLED_Pos(0, 0);
            OLED_Print("GAME OVER");
            OLED_Pos(2,0);
            OLED_Print(" > Hovedmeny");
        } else {
            OLED_Print("Ok, ha det!");
        }

        wait_for_button();
        OLED_Clear();
        menu_init(options, size);

    }
}
