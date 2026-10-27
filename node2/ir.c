#include "sam.h"
#include "ir.h"

#define TERSKEL 1000

void ir_init(void) {
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; //skru på klokke til ADC

    ADC->ADC_MR = ADC_MR_FREERUN_ON //ADC måler hele tiden av seg selv
                | ADC_MR_PRESCAL(41); //formel ADC_klokke = MCK / ((41+1)*2) = 1MHz, når MCK = 85MHz
    
    ADC->ADC_CHER = ADC_CHER_CH7; //SLÅ PÅ KANAL 7 (A0 på Arduino)
    ADC->ADC_CR = ADC_CR_START; //Begynner analog-to-digital conversion
}

uint16_t ir_read(void) {
    uint32_t sum = 0;

    for (int i = 0; i < 16; i++) {
        while (!(ADC->ADC_ISR & ADC_ISR_EOC7)) {} //venter på ny måling
        sum += ADC->ADC_CDR[7]; //0-4095/12 bit), 0 er 0V og 4095 er 3,3V
    }

    return sum/16; //tar gjennomsnitt av 16 målinger for å fjerne støy
    //flere gir mindre stæy, men tar lengre tid. 2-erpotens gjør at det raskt å dele på for prosessoren
}

uint8_t ir_blocked(void) { //returner 1=brutt, 0 = ikke brutt
    return ir_read() < TERSKEL;
}


uint8_t ir_goal(void) {
    //static gjør at verdien huskes mellom hver gang funksjonen kalles
    static uint8_t ready = 1; //1 = klar til å telle mål, 0 = ikke klar
    static uint16_t clear = 0; //teller hvor mange målinger på rad strålen har vært fri

    if (ir_blocked()) { //leser om strålen er brutt
        clear = 0; //
        if (ready) { //klar til å telle?
            ready = 0; //ikke klar lenger
            return 1; //nytt mål
        }
    }

    else {
        if (clear < 1000) {
            clear++;
        }
        else {
            ready = 1; //klar for nytt mål
        }
    }
    return 0;
}