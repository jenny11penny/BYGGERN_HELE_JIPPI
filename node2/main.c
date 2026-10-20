#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"


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

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PMC->PMC_PCER0 = (1u << ID_PIOB);

    PIOB->PIO_PER = (1u << 13);

    PIOB->PIO_OER = (1u << 13);

    PIOB->PIO_SODR = (1u << 13);

    PIOB->PIO_PER = PIO_PB27;
    PIOB->PIO_OER = PIO_PB27;

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    printf("Hello World\n\r");

    can_init((CanInit){.reg = CAN_BR_BRP(41) | CAN_BR_PROPAG(5) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(3) | CAN_BR_SJW(0)}, 0);

    CanMsg m;

    while (1)
    {
        if (can_rx(&m) && m.id == 0x10) {
            printf("x=%d y=%d knapp=%d\n\r",
                   (int8_t)m.byte[0], (int8_t)m.byte[1], m.byte[2]);
        }
    }
    
}