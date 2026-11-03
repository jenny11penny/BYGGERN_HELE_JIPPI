#include <stdint.h>

void controller_init(void);

int controller_ready(void);
void SysTick_Handler(void);

void controller_update(int8_t x);