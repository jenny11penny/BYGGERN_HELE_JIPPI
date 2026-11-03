#include "encoder.h"
#include "sam.h"
#include <stdio.h>

void encoder_init(void) {
    PMC->PMC_PCER0 = 1 << ID_PIOC;

    PMC->PMC_PCER1 = (1 << (ID_TC6-32)); //aktiverer klokken til kanal 0 i TC2 (1 << 1)

    PIOC->PIO_PDR = PIO_PC25 | PIO_PC26;
    PIOC->PIO_ABSR |= PIO_PC25 | PIO_PC26;

    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS; //deaktivere klokken mens vi iniataliserer

    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN; //aktivere Quadrate decoder

    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0; //velg klokke XC0

    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; //enable klokken og software trigger command
}

int32_t encoder_read(void) {
    int32_t pos = (int32_t)TC2->TC_CHANNEL[0].TC_CV;
    if (pos < 0) {
        pos = 0;
    }
    return pos;
}

void encoder_reset(void)
{
    // SWTRG nullstiller telleren til 0
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

