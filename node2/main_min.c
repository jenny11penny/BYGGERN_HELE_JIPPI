#include "sam.h"
#include "pwm.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS;

    pwm_init();
    pwm_set_us(1500);

    while (1) { }
}