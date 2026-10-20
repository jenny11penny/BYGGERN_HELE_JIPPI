#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_Init(void);
void OLED_Home(void);
void OLED_GotoLine(uint8_t line);
void OLED_GotoColumn(uint8_t column);
void OLED_ClearLine(uint8_t line);
void OLED_Clear(void);
void OLED_Pos(uint8_t row, uint8_t column);
void OLED_WriteChar(char c);
void OLED_Print(const char *str);

//void OLED_Reset(void);

#endif