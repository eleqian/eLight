#include "hal.h"
#include "key.h"
#include "ntc.h"
#include "light.h"

#define LIGHT_DIM_DIV 24 /* PWM->10k/1uf-220k-FB-10k<-Rsense */
#define LIGHT_DUTY_MAX 256

/* unit: 10mv */
#define LIGHT_VREF 10 /* 100mV */
#define LIGHT_VCC_FULL 420 /* 4.2v */
#define LIGHT_VCC_LOW 330 /* 3.3v */
#define LIGHT_VCC_EMPTY 290 /* 2.9v */
#define LIGHT_VCC_TH 10 /* 0.1v */

/* unit: 10ms */
#define LIGHT_T_TASK TASK_MS2TICK(10)
#define LIGHT_MS2CNT(t) ((t) / 10)
#define LIGHT_TRACK_VCC_CNT LIGHT_MS2CNT(500)
#define LIGHT_LOW_POWER_CNT LIGHT_MS2CNT(2000)
#define LIGHT_SHOW_POWER_CNT LIGHT_MS2CNT(1500)
#define LIGHT_TEMP_CHECK_CNT LIGHT_MS2CNT(1000)

#define LIGHT_TEMP_HIGH 50

typedef enum {
    LIGHT_OFF,
    LIGHT_LOCK,
    LIGHT_ON,
    LIGHT_FLASH,
    LIGHT_SOS,
    LIGHT_BAT,
    LIGHT_MODE_MAX
} light_mode_e;

typedef enum {
    POWER_NORMAL,
    POWER_LOW,
    POWER_EMPTY,
    POWER_MAX
} power_level_e;

/* unit: 10mv */
static u16 ROM light_bat_cap[] = {368, 374, 377, 379, 382, 387, 392, 398, 406, 420};
#define LIGHT_BAT_CAP_CNT (ARRAY_SIZE(light_bat_cap))

/* Rsense=0.25, Imax=350mA */
static u8 ROM light_level_gamma[] = {
    25, 27, 29, 31, 33, 36, 38, 41, 44, 47, 51, 54, 58, 63, 67, 72, 78,
    83, 89, 96, 103, 110, 119, 127, 137, 147, 157, 169, 181, 194, 209, 224
};
#define LIGHT_LEVEL_CNT (ARRAY_SIZE(light_level_gamma))
#define LIGHT_LEVEL_MAX (LIGHT_LEVEL_CNT - 1)

static u8 ROM light_flash_pattern[] = {1, 1};
#define LIGHT_FLASH_PATTERN_CNT (ARRAY_SIZE(light_flash_pattern))
static u8 ROM light_flash_scale[] = {LIGHT_MS2CNT(60), LIGHT_MS2CNT(250)};
#define LIGHT_FLASH_SCALE_CNT (ARRAY_SIZE(light_flash_scale))

static u8 ROM light_sos_pattern[] = {1, 1, 1, 1, 1, 2, 3, 2, 3, 2, 3, 2, 1, 1, 1, 1, 1, 6};
#define LIGHT_SOS_PATTERN_CNT (ARRAY_SIZE(light_sos_pattern))
static u8 ROM light_sos_scale[] = {LIGHT_MS2CNT(120), LIGHT_MS2CNT(500)};
#define LIGHT_SOS_SCALE_CNT (ARRAY_SIZE(light_sos_scale))

static u8 ROM light_low_power_pattern[] = {1, 1, 1, 1, 80};
#define LIGHT_LOW_POWER_PATTEN_CNT (ARRAY_SIZE(light_low_power_pattern))
#define LIGHT_LOW_POWER_TIME_SCALE LIGHT_MS2CNT(120)

typedef struct {
    power_level_e power_level;
    light_mode_e mode;
    led_e led;
    BOOL level_is_inc;
    u8 focus_level;
#if CONFIG_LED_WIDE_EN
    u8 wide_level;
#endif
#if CONFIG_NTC_EN
    u8 temp_cnt;
#endif
    u8 flash_freq;
    u8 sos_freq;
    u8 track_vcc_cnt;
    u8 cur_level;
    u8 pattern_index;
    u8 pattern_num;
    u8 pattern_scale;
    u8 ROM *pattern_tbl;
    u16 wait_time;
} light_s;

static light_s light;
static tick_t light_tick;

void light_init(void)
{
    memset(&light, 0, sizeof(light));
    TASK_SET_DELAY(light_tick, 0);
}

static void light_update_power_level(u16 vcc)
{
    switch (light.power_level) {
        case POWER_NORMAL:
            if (vcc < LIGHT_VCC_EMPTY) {
                light.power_level = POWER_EMPTY;
            } else if (vcc < LIGHT_VCC_LOW) {
                light.power_level = POWER_LOW;
            }
            break;
        case POWER_LOW:
            if (vcc < LIGHT_VCC_EMPTY) {
                light.power_level = POWER_EMPTY;
            } else if (vcc > LIGHT_VCC_LOW + LIGHT_VCC_TH) {
                light.power_level = POWER_NORMAL;
            }
            break;
        case POWER_EMPTY:
            if (vcc > LIGHT_VCC_LOW + LIGHT_VCC_TH) {
                light.power_level = POWER_NORMAL;
            } else if (vcc > LIGHT_VCC_EMPTY + LIGHT_VCC_TH) {
                light.power_level = POWER_LOW;
            }
            break;
        default:
            break;
    }
}

static u8 light_level2duty(u16 vcc, u8 level)
{
    u8 duty = (LIGHT_VREF * LIGHT_DIM_DIV * LIGHT_DUTY_MAX - LIGHT_VREF * (LIGHT_DIM_DIV - 1) * level) / vcc;

    return duty;
}

static void light_apply_level(u8 level)
{
    u16 vcc = hal_get_vcc();
    u8 duty = light_level2duty(vcc, light_level_gamma[level]);
    hal_pwm_set_duty(duty);
    light_update_power_level(vcc);
    light.cur_level = level;
}

static u8 light_get_level(void)
{
#if CONFIG_LED_WIDE_EN
    return (light.led == LED_FOCUS) ? light.focus_level : light.wide_level;
#else
    return light.focus_level;
#endif
}

static void light_set_level(u8 level)
{
    if (light.led == LED_FOCUS) {
        if (light.focus_level != level) {
            light.focus_level = level;
            light_apply_level(level);
        }
    } else {
#if CONFIG_LED_WIDE_EN
        if (light.wide_level != level) {
            light.wide_level = level;
            light_apply_level(level);
        }
#endif
    }
}

static u8 light_vbat2cap(u16 vbat)
{
    u8 i;

    for (i = 0; i < LIGHT_BAT_CAP_CNT; i++) {
        if (vbat < light_bat_cap[i]) {
            break;
        }
    }

    return i;
}

static u8 light_vbat_time(void)
{
    u16 vcc = hal_get_vcc();
    u8 cap = light_vbat2cap(vcc);

    light_update_power_level(vcc);

    if (cap == 0) {
        return 1;
    } else {
        return cap * (LIGHT_SHOW_POWER_CNT / LIGHT_BAT_CAP_CNT);
    }
}

static void light_pattern_update(void)
{
    if (light.pattern_tbl == NULL) {
        return;
    }

    if (light.wait_time > 0) {
        light.wait_time--;
    } else {
        light.pattern_index++;
        if (light.pattern_index >= light.pattern_num) {
            light.pattern_index = 0;
        }
        light.wait_time = light.pattern_tbl[light.pattern_index] * light.pattern_scale;
        hal_led_en((light.pattern_index & 0x1) ? LED_NONE : light.led);
    }
}

static void light_set_pattern(u8 ROM *tbl, u8 num, u8 scale)
{
    light.pattern_tbl = tbl;
    light.pattern_num = num;
    light.pattern_scale = scale;
    light.pattern_index = 0;
    light.wait_time = light.pattern_tbl[0] * light.pattern_scale;
    hal_led_en(light.led);
}

static void light_disable_pattern(void)
{
    light.pattern_tbl = NULL;
}

static BOOL light_is_pattern_en(void)
{
    return (light.pattern_tbl != NULL);
}

static void light_set_mode(light_mode_e mode)
{
    light.mode = mode;
    light_disable_pattern();

    switch (mode) {
        case LIGHT_OFF:
            light.wait_time = LIGHT_LOW_POWER_CNT;
            hal_led_en(LED_NONE);
            break;
        case LIGHT_ON:
            hal_led_en(light.led);
            break;
        case LIGHT_FLASH:
            light_apply_level(LIGHT_LEVEL_MAX);
            light_set_pattern(light_flash_pattern, LIGHT_FLASH_PATTERN_CNT, light_flash_scale[light.flash_freq]);
            break;
        case LIGHT_SOS:
            light_apply_level(LIGHT_LEVEL_MAX);
            light_set_pattern(light_sos_pattern, LIGHT_SOS_PATTERN_CNT, light_sos_scale[light.sos_freq]);
            break;
        case LIGHT_BAT:
            light.led = LED_INDICATE;
            light.wait_time = light_vbat_time();
            light_apply_level(0);
            hal_led_en(light.led);
            break;
        case LIGHT_LOCK:
            light.wait_time = LIGHT_LOW_POWER_CNT;
            light_set_level(0);
            hal_led_en(LED_NONE);
            break;
        default:
            break;
    }
}

static void light_off_proc(key_event_e key)
{
    switch (key) {
        case KEY_EVENT_SHORT:
            light.led = LED_FOCUS;
            light_apply_level(light.focus_level);
            light_set_mode(LIGHT_ON);
            return;
        case KEY_EVENT_SHORT_2:
            light.level_is_inc = TRUE;
            light_set_level(LIGHT_LEVEL_MAX);
            light_set_mode(LIGHT_ON);
            return;
        case KEY_EVENT_LONG:
            light_set_mode(LIGHT_BAT);
            return;
        default:
            break;
    }

    if (light.wait_time > 0) {
        light.wait_time--;
    } else {
        light.wait_time = LIGHT_LOW_POWER_CNT;
        hal_enter_low_power();
    }
}

static void light_lock_proc(key_event_e key)
{
    hal_led_en(LED_NONE);

    switch (key) {
        case KEY_EVENT_PRESS:
            light.wait_time = LIGHT_LOW_POWER_CNT;
            hal_led_en(LED_INDICATE);
            break;
        case KEY_EVENT_SHORT_4:
            light_set_mode(LIGHT_OFF);
            return;
        default:
            break;
    }

    if (light.wait_time > 0) {
        light.wait_time--;
    } else {
        light.wait_time = LIGHT_LOW_POWER_CNT;
        hal_enter_low_power();
    }
}

static void light_on_proc(key_event_e key)
{
    switch (key) {
        case KEY_EVENT_SHORT:
            light_set_mode(LIGHT_OFF);
            return;
        case KEY_EVENT_SHORT_2:
#if CONFIG_LED_WIDE_EN
            light.led = LED_WIDE;
            light_apply_level(light.wide_level);
            light_set_mode(LIGHT_ON);
#else
            light.level_is_inc = TRUE;
            light_set_level(LIGHT_LEVEL_MAX);
#endif
            break;
        case KEY_EVENT_SHORT_3:
            light_set_mode(LIGHT_FLASH);
            return;
        case KEY_EVENT_LONG:
            light.level_is_inc = !light.level_is_inc;
        case KEY_EVENT_LONG_REPEAT: {
            u8 level = light_get_level();
            if (light.level_is_inc) {
                level = (level < LIGHT_LEVEL_MAX) ? level + 1 : level;
            } else {
                level = (level > 0) ? level - 1 : level;
            }
            light_set_level(level);
            break;
        }
        default:
            break;
    }

    if (light.power_level == POWER_EMPTY) {
        light_set_mode(LIGHT_OFF);
    } else if (light.power_level == POWER_LOW) {
        if (!light_is_pattern_en()) {
            light_set_pattern(light_low_power_pattern, LIGHT_LOW_POWER_PATTEN_CNT, LIGHT_LOW_POWER_TIME_SCALE);
        }
    } else if (light_is_pattern_en()) {
        hal_led_en(light.led);
        light_disable_pattern();
    }
}

static void light_flash_proc(key_event_e key)
{
    switch (key) {
        case KEY_EVENT_SHORT:
            light_set_mode(LIGHT_SOS);
            return;
        case KEY_EVENT_SHORT_4:
            light_set_mode(LIGHT_LOCK);
            return;
        case KEY_EVENT_LONG:
            light.flash_freq = !light.flash_freq;
            light_set_pattern(light_flash_pattern, LIGHT_FLASH_PATTERN_CNT, light_flash_scale[light.flash_freq]);
            break;
        default:
            break;
    }

    if (light.power_level == POWER_EMPTY) {
        light_set_mode(LIGHT_OFF);
    }
}

static void light_sos_proc(key_event_e key)
{
    switch (key) {
        case KEY_EVENT_SHORT:
            light_set_mode(LIGHT_OFF);
            return;
        case KEY_EVENT_SHORT_2:
            light.level_is_inc = TRUE;
            light_set_level(LIGHT_LEVEL_MAX);
            light_set_mode(LIGHT_ON);
            return;
        case KEY_EVENT_LONG:
            light.sos_freq = !light.sos_freq;
            light_set_pattern(light_sos_pattern, LIGHT_SOS_PATTERN_CNT, light_sos_scale[light.sos_freq]);
            break;
        default:
            break;
    }

    if (light.power_level == POWER_EMPTY) {
        light_set_mode(LIGHT_OFF);
    }
}

static void light_bat_proc(key_event_e key)
{
    UNUSED(key);
    light.wait_time--;
    if (light.wait_time == 0) {
        light_set_mode(LIGHT_OFF);
    }
}

static void light_mode_proc(void)
{
    key_event_e key = key_get_event();

    switch (light.mode) {
        case LIGHT_OFF:
            light_off_proc(key);
            break;
        case LIGHT_LOCK:
            light_lock_proc(key);
            break;
        case LIGHT_ON:
            light_on_proc(key);
            break;
        case LIGHT_FLASH:
            light_flash_proc(key);
            break;
        case LIGHT_SOS:
            light_sos_proc(key);
            break;
        case LIGHT_BAT:
            light_bat_proc(key);
            break;
        default:
            break;
    }
}

static void light_temp_protect(void)
{
#if CONFIG_NTC_EN
    u8 temp;

    if (light.mode != LIGHT_ON) {
        light.temp_cnt = 0;
        return;
    }

    if (light.temp_cnt > 0) {
        light.temp_cnt--;
        return;
    }

    light.temp_cnt = LIGHT_TEMP_CHECK_CNT;
    temp = ntc_get_temp();
    if (temp > LIGHT_TEMP_HIGH) {
        u8 level = light_get_level();
        if (level > LIGHT_LEVEL_MAX / 2) {
            level--;
            light_set_level(level);
        }
    }
#endif
}

static void light_track_vcc(void)
{
    if (light.mode == LIGHT_OFF || light.mode == LIGHT_LOCK) {
        light.track_vcc_cnt = 0;
        return;
    }

    if (light.track_vcc_cnt > 0) {
        light.track_vcc_cnt--;
    } else {
        light.track_vcc_cnt = LIGHT_TRACK_VCC_CNT;
        light_apply_level(light.cur_level);
    }
}

void light_task(void)
{
    if (!TASK_CHECK_READY(light_tick)) {
        return;
    }
    TASK_SET_DELAY(light_tick, LIGHT_T_TASK);

    light_track_vcc();
    light_temp_protect();
    light_mode_proc();
    light_pattern_update();
}
