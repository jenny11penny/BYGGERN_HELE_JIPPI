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

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    motor_init();
    solenoid_init();

    uart_init(84000000, 9600);
    printf("Hello Solenoid\n\r");

    can_init((CanInit){.reg = CAN_BR_BRP(41) | CAN_BR_PROPAG(5) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(3) | CAN_BR_SJW(0)}, 0);
    pwm_init();
    ir_init();
    encoder_init();

    //Oppstart -> Kjør rolig mot midten
    motor_set(40);                                    // mot høyre
    for (volatile int i = 0; i < 10000000; i++);      // ca. 1-2 sekunder
    motor_set(0);
    for (volatile int i = 0; i < 1000000; i++);       // la vogna roe seg
    encoder_reset();

    controller_init();    

    CanMsg m;
    int8_t x = 0;
    uint8_t last_button = 0;
    int print_count = 0;

    int score = 0;
    
    while (1)
    {
        
        if (can_rx(&m) && m.id == 0x10) {
            x = (int8_t)m.byte[0];
            int8_t y = (int8_t)m.byte[1];
            uint8_t button = m.byte[2];

            if (y > -5 && y < 5) {
                y = 0;
            }
    
            pwm_set_us(1500 - y * 6);

            if (button && !last_button) {
                solenoid_kick();
            }
            last_button = button;
        } 
        
        if (controller_ready()) {
            controller_update(x);
            solenoid_update();

            print_count ++;
            if (print_count >= 20) {
                print_count = 0;
            }

            if (ir_goal()) { 
                score++;
                printf("Mål! Poeng: %d\n\r", score);

                CanMsg goal = {.id = 0x20, .length = 1, .byte = {1}};
                can_tx(goal);
            }
        }
    }
}