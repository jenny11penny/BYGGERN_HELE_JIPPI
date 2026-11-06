#include <stdio.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "motor.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS;   // skru av watchdog
    motor_init();                 // holder motoren stille

    uart_init(84000000, 9600);
    printf("CAN-test\n\r");

    can_init((CanInit){.reg = CAN_BR_BRP(41) | CAN_BR_PROPAG(5) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(3) | CAN_BR_SJW(0)}, 0);

    CanMsg m;
    while (1) {
        if (can_rx(&m)) {
            // Skriver ut ALLE meldinger, ikke bare 0x10
            printf("id=0x%x len=%d data=%d %d %d\n\r",
                   m.id, m.length, (int8_t)m.byte[0], (int8_t)m.byte[1], m.byte[2]);
        }
    }
}