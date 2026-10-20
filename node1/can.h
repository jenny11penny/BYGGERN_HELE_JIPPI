#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#define CANSTAT   0x0E
#define CANCTRL   0x0F
#define CANINTF   0x2C
#define TXB0SIDH  0x31
#define TXB0SIDL  0x32
#define TXB0DLC   0x35
#define TXB0D0    0x36
#define RXB0CTRL  0x60
#define RXB0SIDH  0x61
#define RXB0SIDL  0x62
#define RXB0DLC   0x65
#define RXB0D0    0x66

#define CNF3  0x28
#define CNF2  0x29
#define CNF1  0x2A


typedef struct {
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
} CAN_message;

void CAN_Init(void);
void CAN_SendMessage(CAN_message *msg);
uint8_t CAN_Receive(CAN_message *msg);



#endif