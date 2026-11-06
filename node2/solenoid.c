#include "solenoid.h"
#include "sam.h"

#define SOL_PIN PIO_PB26 //Arduino pinne 22
#define TICKS 30 //30 ms


static int ticks_left = 0;

void solenoid_init(void) {
    PMC->PMC_PCER0 = (1 << ID_PIOB); //skru på PIOB

    PIOB->PIO_SODR = SOL_PIN; //Set Output Data. Når pinne = høy så er rele av
    PIOB->PIO_PER = SOL_PIN; //Skru på PIO kontroll på solenoide pin
    PIOB->PIO_OER = SOL_PIN; //Output Enable Register
}

void solenoid_kick(void) {
    if (ticks_left == 0) {
        PIOB->PIO_CODR = SOL_PIN; //Clear Output Data, sett pinne lav = rele på
        ticks_left = TICKS;
    }
}

void solenoid_update(void) {
    if (ticks_left > 0) {
        ticks_left--;
        if (ticks_left == 0) {
            PIOB->PIO_SODR = SOL_PIN; //høy = rele av
        }
    }
}