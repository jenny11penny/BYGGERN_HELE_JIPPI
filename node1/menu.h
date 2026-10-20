#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void menu_init(const char *options[], uint8_t size);
void menu_update(const char *options[], uint8_t size);
int get_selected(void);
void menu_close(void);
void menu_test(void);

#endif