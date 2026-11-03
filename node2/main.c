#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "ir.h"
#include "encoder.h"
#include "motor.h"
#include "pi.h"
#include <stdint.h>
#include "solenoid.h"


/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-MCP2515_Write(CNF1, 0X03);
    MCP2515_Write(CNF2, 0XA5);
    MCP2515_Write(CNF3, 0X03):gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"
void test_pb13_hoy(void) {
    PMC->PMC_PCER0 = (1u << ID_PIOB);
    PIOB->PIO_PER  = (1u << 13);
    PIOB->PIO_OER  = (1u << 13);
    PIOB->PIO_SODR = (1u << 13);
}

// Test fra lab 6: skriver ut joystick-meldinger fra Node 1. Kommer aldri tilbake.
void test_can(void) {
    can_init((CanInit){.reg = CAN_BR_BRP(41) | CAN_BR_PROPAG(5) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(3) | CAN_BR_SJW(0)}, 0);

    CanMsg m;
    while (1) {
        if (can_rx(&m) && m.id == 0x10) {
            printf("x=%d y=%d knapp=%d\n\r",
                   (int8_t)m.byte[0], (int8_t)m.byte[1], m.byte[2]);
        }
    }
}

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    motor_init();
    solenoid_init();

    uart_init(84000000, 9600);
    printf("Hello Solenoid\n\r");

    //can_init((CanInit){.reg = CAN_BR_BRP(41) | CAN_BR_PROPAG(5) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(3) | CAN_BR_SJW(0)}, 0);
    //pwm_init();
    //encoder_init();

    //Oppstart -> Kjør rolig mot midten
    //motor_set(40);                                    // mot høyre
    //for (volatile int i = 0; i < 10000000; i++);      // ca. 1-2 sekunder
    //motor_set(0);
    //for (volatile int i = 0; i < 1000000; i++);       // la vogna roe seg
    //encoder_reset();

    controller_init();    

    //CanMsg m;
    //int8_t x = 0;
    //uint8_t last_button = 0;
    int print_count = 0;
    
    while (1)
    {
        /*
        if (can_rx(&m) && m.id == 0x10) {
            x = (int8_t)m.byte[0];
            int8_t y = (int8_t)m.byte[1];
            uint8_t button = m.byte[2];

            pwm_set_us(1500 - y * 6);
            //printf("x=%d pos = %d\n\r", x, (int)encoder_read());
            if (button && !last_button) {
                solenoid_kick();
            }
            last_button = button;
        } 
        */

        if (controller_ready()) {
            //controller_update(x);
            solenoid_update();

            print_count ++;
            if (print_count >= 200) {
                print_count = 0;
                printf("Klikk!\n\r");
                solenoid_kick();
            }

            /*
            print_count++;
            if (print_count >= 20) {
                print_count = 0;
                printf("x=%d pos=%d\n\r", x, (int)encoder_read());
            }
            */
        }

        //if (ir_goal()) {
            //score++;
            //printf("Mål! Poeng: %lu\n\r", score);
    }
}