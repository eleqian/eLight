#ifndef __HAL_H__
#define __HAL_H__

#include <intrins.h>
#include "base.h"
#include "config.h"

extern volatile tick_t g_tick;

#define TASK_MS2TICK(t) (((t) + (1000U / TASK_HZ / 2)) / (1000U / TASK_HZ))
#define TASK_CHECK_READY(t) ((s16)((t) - g_tick) <= 0)
#define TASK_SET_DELAY(t, d) do { (t) = g_tick + (d); } while (0)

typedef enum {
    LED_NONE,
    LED_FOCUS,
#if CONFIG_LED_WIDE_EN
    LED_WIDE,
    LED_INDICATE = LED_WIDE,
#else
    LED_INDICATE = LED_FOCUS,
#endif

    LED_MAX
} led_e;

typedef enum {
#if CONFIG_BOARD_867A
    ADC_VLED = 0,
#endif
#if CONFIG_NTC_EN
    ADC_NTC = 4,
#endif
    ADC_VCC = 15,
    ADC_MAX
} adc_ch_e;

void hal_init(void);

BOOL hal_key_pressed(void);
void hal_led_en(led_e led);
void hal_pwm_set_duty(u8 duty);

void hal_adc_en(u8 en);
u16 hal_adc_conv(adc_ch_e ch);
u16 hal_get_vcc(void);

void hal_enter_idle(void);
void hal_enter_low_power(void);
void hal_soft_reset(void);
void hal_wdt_feed(void);

void delay_ms(u8 ms);

#endif
