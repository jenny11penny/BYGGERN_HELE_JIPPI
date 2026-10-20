
#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t x_touch;
    int16_t y_touch;
} Position;


typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} Direction;

void ADC_calibrate(void);
void ADC_clock_init(void);
Position ADC_read_pos(void);
Direction ADC_read_dir(void);
void adc_test(void);

void joystick_btn_init(void);
uint8_t joystick_btn_pressed(void);