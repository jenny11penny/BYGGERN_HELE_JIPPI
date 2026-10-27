#include "pwm.h"
#include "sam.h"

void pwm_init(void) {
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; //Skru på klokke til PIOB
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Skru på klokke til PWM

    PIOB->PIO_PDR |= PIO_PDR_P13; //LA PWM STYRE PIN 13, pdr = pio disable register
    PIOB->PIO_ABSR |= PIO_ABSR_P13; // absr = peipheral AB select register, periferi B = PWMH1

    PWM->PWM_CLK = PWM_CLK_DIVA(84) | PWM_CLK_PREA(0); //84MHz / 84 = 1MHz  som er 1 tikk = 1us

    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL; //Bruke klokke A og signalet starter høyt(pga PWMH1)
    PWM->PWM_CH_NUM[1].PWM_CPRD = 20000; //periode til 20ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1500; //sette duty cycle til 1,5ms (midten)

    PWM->PWM_ENA = PWM_ENA_CHID1; //SKRU PÅ KANAL 1



}

void pwm_set_us(uint32_t us) {
    if (us < 900) us = 900;
    if (us > 2100) us = 2100;

    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = us; //PWM Channel Duty Cycle Update Register, sette ny duty cycle
}