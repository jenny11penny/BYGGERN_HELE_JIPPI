#include "can.h"
#include "mcp2515.h"

void CAN_Init(void) {
    MCP2515_Reset();

    MCP2515_Write(CNF1, 0X03);
    MCP2515_Write(CNF2, 0XA5);
    MCP2515_Write(CNF3, 0X03);

    MCP2515_BitModify(RXB0CTRL, 0x60, 0x60); //RXB0CTRL, ta i mot alle meldinger
    MCP2515_BitModify(CANCTRL, 0xE0, 0X00); //normal mode, 0x40 - loopback mode


}

void CAN_SendMessage(CAN_message *msg) {
    MCP2515_Write(TXB0SIDH, msg->id >> 3); //8 største bitene
    MCP2515_Write(TXB0SIDL, (msg->id & 0x07) << 5); //3 minste
    MCP2515_Write(TXB0DLC, msg->length); //skriv lengden på dataen

    for (uint8_t i = 0; i < msg->length; i++) {
        MCP2515_Write(TXB0D0 + i, msg->data[i]); 
    }

    MCP2515_RTS(0x01); //request to send


}

uint8_t CAN_Receive(CAN_message *msg) {
    if ((MCP2515_Read(CANINTF) & 0x01) == 0) { //sjekk om RX0IF er 0 i CANINTF 
        return 0; //ingen melding
    }

    msg->id = (MCP2515_Read(RXB0SIDH) << 3) | (MCP2515_Read(RXB0SIDL) >> 5); //får en 11 bit ID
    msg->length = MCP2515_Read(RXB0DLC) & 0X0F;

    for (uint8_t i = 0; i < msg->length; i++) {
        msg->data[i] = MCP2515_Read(RXB0D0 + i); //legge inn data
    }

    MCP2515_BitModify(CANINTF, 0x01, 0x00); //nullstill RX0IF
    return 1;

}
