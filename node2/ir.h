#ifndef IR_H
#define IR_H

#include <stdint.h>

void ir_init(void);
uint16_t ir_read(void);     // filtrert verdi, 0-4095
uint8_t ir_blocked(void);   // 1 = strålen er brutt
uint8_t ir_goal(void);

#endif