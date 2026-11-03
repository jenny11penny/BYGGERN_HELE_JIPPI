#include "pi.h"
#include "motor.h"
#include "encoder.h"
#include "sam.h"

#define ENCODER_MAX 2800
#define ENCODER_MIN 0
#define MARGIN (((ENCODER_MAX) - (ENCODER_MIN)) / 20)
#define T 0.01f

#define KP 1.8f
#define KI 0.7f

static volatile int tick = 0;
static float integral = 0;

void SysTick_Handler(void) {
    tick = 1;
}

int controller_ready(void) {
    if (tick) {
        tick = 0;
        return 1;
    }
    return 0;
}

void controller_init(void) {
    SysTick_Config(84000000 / 100);
}

void controller_update(int8_t x) {
    int32_t low = ENCODER_MIN + MARGIN;
    int32_t high = ENCODER_MAX - MARGIN;

    int32_t pos = (int32_t)encoder_read();

    int32_t ref = low + (int32_t)(100 - x) * (high - low) / 200;
    float e = (float)(ref - pos) * 100.0f / (float)(ENCODER_MAX - ENCODER_MIN);

    integral += e * T;

    if (integral > 50.0f / (KI + 0.001f)) integral = 50.0f / (KI + 0.001f);
    if (integral < -50.0f / (KI + 0.001f)) integral = -50.0f / (KI + 0.001f);

    float u = KP * e + KI * integral;

    if (u > 100) u = 100;
    if (u < -100) u = -100;

    motor_set((int8_t)-u);

}