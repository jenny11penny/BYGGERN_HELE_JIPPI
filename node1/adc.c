#define F_CPU 4915200UL

#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define ADC_ADDRESS 0x1000

volatile uint8_t *adc_base = (volatile uint8_t *)ADC_ADDRESS;

static uint8_t x_center, y_center, x_touch_center, y_touch_center;

#define X_MIN_DIFF -94
#define X_MAX_DIFF  87
#define Y_MIN_DIFF -91
#define Y_MAX_DIFF  84

void ADC_clock_init(void) {
    DDRD |= (1 << PD5); //Make PD5 output

    TCCR1A = (1 << COM1A0); //Toggle OC1A on compare match

    TCCR1B = (1 << WGM12) | (1 << CS10);

    OCR1A = 1;

    DDRB &= ~(1 << PB0);
    PORTB &= ~(1 << PB0);
}

void ADC_calibrate(void) {
    *adc_base  = 0; 

    while (PINB & (1 << PB0)) {
    }

    while (!(PINB & (1 << PB0))) {
    }

    x_center = *adc_base; //AIN0
    y_center = *adc_base; //AIN1
    x_touch_center = *adc_base; //AIN2
    y_touch_center = *adc_base; //AIN3
}

Position ADC_read_pos(void) {
    Position position;

    *adc_base = 0;

    while (PINB & (1 << PB0)) {}

    while (!(PINB & (1 << PB0))) {}

    uint8_t y = *adc_base; //AIN0
    uint8_t x = *adc_base; //AIN1
    uint8_t x_touch = *adc_base; //AIN2
    uint8_t y_touch = *adc_base; //AIN3

    int16_t x_diff = (int16_t)x - x_center;
    int16_t y_diff = (int16_t)y - y_center;

    int16_t x_pct, y_pct;

     if (x_diff >= 0) {
        x_pct = (x_diff * 100) / X_MAX_DIFF;
    } else {
        x_pct = (x_diff * 100) / (-X_MIN_DIFF);
    }
    position.x = x_pct;   // venstre = +100%, høyre = -100%

    if (y_diff >= 0) {
        y_pct = (y_diff * 100) / Y_MAX_DIFF;
    } else {
        y_pct = (y_diff * 100) / (-Y_MIN_DIFF);
    }
    position.y = y_pct;

    //position.x = (int16_t)x - x_center;
    //position.y = (int16_t)y - y_center;
    position.x_touch = (int16_t)x_touch - x_touch_center;
    position.y_touch = (int16_t)y_touch - y_touch_center;
    
    return position;
}

Direction ADC_read_dir(void) {
    Position position = ADC_read_pos();

    
    if (position.x < -30) {
        return LEFT;
    }
    if (position.x > 30) {
        return RIGHT;
    }
    if (position.y > 30) {
        return UP;
    }
    if (position.y < -30) {
        return DOWN;
    }
    return NEUTRAL;
}

void adc_test(void) {
    Position pos = ADC_read_pos();

    printf("X: %d Y: %d\r\n", pos.x, pos.y);
    //printf("X: %d Y: %d\r\n", pos.x_touch, pos.y_touch);
    //adc_test();
    _delay_ms(500);
}


void joystick_btn_init(void) {
    DDRB &= ~(1 << PB1); //Sett PB1 som input
    PORTB |= (1 << PB1); //Aktivere intern pull-up
}

uint8_t joystick_btn_pressed(void) {
    return !(PINB & (1 << PB1)); //Leser av PB1, hvis lav(knapp trykket) = returner 1, hvis høy = returner 0
}