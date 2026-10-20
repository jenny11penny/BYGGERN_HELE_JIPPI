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

int main(void)
{
    USART_Init(MYUBRR);
    fdevopen(uart_putchar, uart_getchar);
    printf("Hei!");

    MCUCR |= (1 << SRE); //aktivere external memomry

    SFIOR |= (1 << XMM2); //
    
    //volatile uint8_t *adc = (uint8_t *)0x1000;
    //volatile uint8_t *sram = (uint8_t *)0x1800;

    ADC_clock_init();
    ADC_calibrate();

    SPI_Init();
    OLED_Init();
    OLED_Clear();

    OLED_Pos(0,0);

    const char *options[] = {"JA", "NEI", "KANSKJE"};
    uint8_t size = sizeof(options) / sizeof(options[0]);

    menu_init(options, size);

    joystick_btn_init();

    CAN_Init();
    printf("CNF1=0x%02X CNF2=0x%02X CNF3=0x%02X\r\n", MCP2515_Read(CNF1), MCP2515_Read(CNF2), MCP2515_Read(CNF3));

    //printf("CANSTAT: 0x%02X\r\n", MCP2515_Read(CANSTAT) & 0xE0);
 
    // Test 2: send en melding og se at den samme kommer tilbake
    //CAN_message tx = { .id = 0x23, .length = 3, .data = { 1, 2, 3 } };
    //CAN_message rx;

    CAN_message joy;
    joy.id = 0x10;
    joy.length = 3;

 
    
 
    //if (CAN_Receive(&rx)) {
      //  printf("Mottatt id=0x%X len=%d data=%d %d %d\r\n", rx.id, rx.length, rx.data[0], rx.data[1], rx.data[2]);
    //} else {
      //  printf("Ingen melding mottatt\r\n");
    //}
 
    while (1)
    {
        Position pos = ADC_read_pos();

        joy.data[0] = (int8_t)pos.x;
        joy.data[1] = (int8_t)pos.y;
        joy.data[2] = joystick_btn_pressed();

        CAN_SendMessage(&joy);
        _delay_ms(500); //Bytt til 50 når joystick skal styre motor/servo
        //CAN_SendMessage(&tx);
        //printf("Sendt id=0x%X\r\n", tx.id);

        //_delay_ms(100);

        //if (CAN_Receive(&rx)) {
          //  printf("Svar fra Node 2: id=0x%X data=%d %d %d\r\n",
            //       rx.id, rx.data[0], rx.data[1], rx.data[2]);
        //}

        //tx.data[0]++;          // ny verdi hver gang
        //_delay_ms(900);
        
        /*

        if (!joystick_btn_pressed()) {
            menu_update(options, size);
           _delay_ms(200);
        }
        
        else {
            menu_close();
            int selected = get_selected();

            OLED_Pos(0,0);
            OLED_Clear();

            switch(selected) {
                case 0:
                OLED_Print("valg 1"); break;
                case 1:
                OLED_Print("valg 2"); break;
                case 2:
                OLED_Print("valg 3"); break;
            } 
                
        }      */    
    }
}
