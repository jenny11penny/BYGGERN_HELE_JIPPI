#include "menu.h"
#include "OLED.h"
#include "adc.h"
#include <util/delay.h>

static uint8_t menu_active = 1;
int i = 2;

void menu_init(const char *options[], uint8_t size) {
    menu_active = 1;
    i = 2;
    
    OLED_Clear();
    OLED_Print("Heisann");

    //char *options[] = {"JA", "NEI"};
    //int antall = 2;
    OLED_Pos(2,0);

    OLED_Print(">");

    int selected = 0;

    for(int i = 0; i < size; i ++) {
        OLED_Pos(i+2,10);
        OLED_Print(options[i]); 
    }
}
void menu_update(const char *options[], uint8_t size) {

    if (!menu_active) {
        return;
    }

    Direction dir = ADC_read_dir();

    if (dir == DOWN && i < (size + 1)) {
        //selected ++;
        OLED_Pos(i,0);
        OLED_Print(" ");
        i ++;
        OLED_Pos(i, 0);
        OLED_Print(">");
    }
    else if (dir == UP && i > 2) {
        OLED_Pos(i,0);
        OLED_Print(" ");
        i --;
        OLED_Pos(i, 0);
        OLED_Print(">");
    }
}

void menu_close(void) {
    menu_active = 0;
    OLED_Clear();
}

int get_selected(void) {
    return i-2;
}

void menu_test(void)
{
    const char *options[] = {"JA", "NEI", "KANSKJE"};
    uint8_t size = sizeof(options) / sizeof(options[0]);

    // Setter opp meny og joystick
    menu_init(options, size);
    joystick_btn_init();

    while (1)
    {
        if (!joystick_btn_pressed())
        {
            menu_update(options, size);
            _delay_ms(200);
        }
        else
        {
            menu_close();

            int selected = get_selected();

            OLED_Clear();
            OLED_Pos(0, 0);

            switch (selected)
            {
                case 0:
                    OLED_Print("valg 1");
                    break;

                case 1:
                    OLED_Print("valg 2");
                    break;

                case 2:
                    OLED_Print("valg 3");
                    break;
            }
        }
    }
}
