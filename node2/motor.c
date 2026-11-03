#include "motor.h"
#include "sam.h"

void motor_init(void) {
    PMC->PMC_PCER0 = (1 << ID_PIOB) | (1 << ID_PIOC); //skru på piob, pioc og pwm
    PMC->PMC_PCER1 = (1 << (ID_PWM - 32));

    PIOB->PIO_PUDR = PIO_PB12;
    PIOC->PIO_PUDR = PIO_PC23;

    PIOB->PIO_CODR = PIO_PB12;
    PIOC->PIO_CODR = PIO_PC23;

    PIOC->PIO_PER = PIO_PC23; 
    PIOC->PIO_OER = PIO_PC23;

    PIOB->PIO_PER = PIO_PB12;
    PIOB->PIO_OER = PIO_PB12;

    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_MCK | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[0].PWM_CPRD = 4200;
    PWM->PWM_CH_NUM[0].PWM_CDTY = 0;

    PWM->PWM_ENA = PWM_ENA_CHID0;

    PIOB->PIO_ABSR |= PIO_PB12;
    PIOB->PIO_PDR = PIO_PB12;
}

void motor_set(int8_t speed) {
    if (speed > -5 && speed < 5) {
        speed = 0;
    }
    if (speed < 0) {
        PIOC->PIO_CODR = PIO_PC23; //settes lav
        speed = -speed;
    }
    else {
        PIOC->PIO_SODR = PIO_PC23; //settes høy
    }

    if (speed > 100) {
        speed = 100;
    }

    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = speed * (4200/100);
}

