
#include "sam.h"
#include "timer.h"

volatile uint32_t timer_flag = 0;
 
void timer_init(uint32_t ms) {
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;                    // klokke til TC0 (ID 27)
 
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 // 84 MHz / 2 = 42 MHz
                              | TC_CMR_WAVE
                              | TC_CMR_WAVSEL_UP_RC;       // tell til RC, start på nytt
    TC0->TC_CHANNEL[0].TC_RC  = 42000 * ms;               // 42000 tick = 1 ms
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;              // avbrudd når telleren når RC
 
    NVIC_EnableIRQ(TC0_IRQn);                             // la CPU-en ta imot avbruddet
 
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // start telleren
}
 
void TC0_Handler(void) {
    TC0->TC_CHANNEL[0].TC_SR;   // lese SR nullstiller avbruddet
    timer_flag = 1;
}
