#include "hal.h"
#include "key.h"
#include "light.h"

void main(void)
{
    hal_init();
    key_init();
    light_init();

    while (1) {
        key_task();
        light_task();
        hal_wdt_feed();
        hal_enter_idle();
    }
}
