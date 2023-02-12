#include "hal.h"
#include "key.h"

/* unit: 50ms */
#define KEY_T_TASK TASK_MS2TICK(50)
#define KEY_T_SHORT_PRESS 1
#define KEY_T_LONG_PRESS 10
#define KEY_T_VERY_LONG_PRESS 160
#define KEY_T_REPEAT_INTERVAL 1
#define KEY_T_MULTI_INTERVAL 5

typedef struct {
    u8 press_cnt;
    u8 release_cnt;
    u8 repeat_cnt;
    u8 multi_cnt;
    key_event_e event;
} key_s;

static key_s key;
static tick_t key_tick;

void key_init(void)
{
    memset(&key, 0, sizeof(key));
    TASK_SET_DELAY(key_tick, 0);
}

void key_task(void)
{
    if (!TASK_CHECK_READY(key_tick)) {
        return;
    }
    TASK_SET_DELAY(key_tick, KEY_T_TASK);

    if (hal_key_pressed()) {
        key.release_cnt = 0;
        key.press_cnt++;
        if (key.press_cnt == KEY_T_VERY_LONG_PRESS) {
            hal_soft_reset();
        } else if (key.press_cnt == KEY_T_LONG_PRESS) {
            key.event = KEY_EVENT_LONG;
        } else if (key.press_cnt > KEY_T_LONG_PRESS) {
            key.repeat_cnt++;
            if (key.repeat_cnt >= KEY_T_REPEAT_INTERVAL) {
                key.repeat_cnt = 0;
                key.event = KEY_EVENT_LONG_REPEAT;
            }
        } else if (key.press_cnt == 1 && key.event == KEY_EVENT_NONE) {
            key.event = KEY_EVENT_PRESS;
        }
    } else {
        if (key.press_cnt > KEY_T_SHORT_PRESS && key.press_cnt < KEY_T_LONG_PRESS) {
            key.multi_cnt++;
            key.release_cnt = 0;
            if (key.multi_cnt == 1) {
                key.event = KEY_EVENT_SHORT;
            } else if (key.multi_cnt == 2) {
                key.event = KEY_EVENT_SHORT_2;
            } else if (key.multi_cnt == 3) {
                key.event = KEY_EVENT_SHORT_3;
            } else if (key.multi_cnt == 4) {
                key.event = KEY_EVENT_SHORT_4;
            }
        }

        key.press_cnt = 0;

        if (key.multi_cnt > 0) {
            key.release_cnt++;
            if (key.release_cnt > KEY_T_MULTI_INTERVAL) {
                key.release_cnt = 0;
                key.multi_cnt = 0;
            }
        }
    }
}

key_event_e key_get_event(void)
{
    key_event_e event = key.event;
    key.event = KEY_EVENT_NONE;
    return event;
}
