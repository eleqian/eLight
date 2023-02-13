#include "base.h"
#include "hal.h"

#define TIMER_INIT_TICK (65536UL - (SYS_CLK_HZ / TASK_HZ / 12))
#define WDT_TIME 0 /* ~197ms@4M */
#define CHIP_VREF (*(u16 code *)0x1ff7) /* in mv */

#define PIN_LED_FOCUS P54
#define PIN_LED_WIDE P55
#define PIN_DRV_EN P31
#define PIN_KEY P32

volatile tick_t g_tick;

void timer0_isr(void) interrupt TIMER0_VECTOR
{
    g_tick++;
}

void int0_isr() interrupt INT0_VECTOR
{
    /* wakeup */
}

void delay_ms(u8 ms)
{
    u16 i;

    do {
        i = SYS_CLK_HZ / 10000;
        while (--i)
            ;
    } while (--ms);
}

static void hal_gpio_init(void)
{
    /* P3.0: ADC IN; P3.1: OUT; P3.2: Bi-IN; P3.3: OUT */
    P3IE = 0xfe;
    P3M0 = 0x0a;
    P3M1 = 0x01;

    /* P5.4: OUT; P5.5: OUT */
    P5M0 = 0x30;
    P5M1 = 0x00;

    PIN_LED_FOCUS = 0;
    PIN_LED_WIDE = 0;
    PIN_DRV_EN = 0;
}

static void hal_time_base_init(void)
{
    /* Timer0: 时基定时器 */
    TMOD = 0x00;
    TL0 = LOW8(TIMER_INIT_TICK);
    TH0 = HIGH8(TIMER_INIT_TICK);
    TR0 = 1; // 启动定时器
    ET0 = 1; // 使能定时器中断
}

static void hal_pwm_init(void)
{
    /* 8位PWM */
    CCON = 0x00;
    CMOD = 0x08; // PCA时钟为系统时钟
    CL = 0x00;
    CH = 0x00;

    // PCA模块1
    CCAPM1 = 0x42; // PWM工作模式
    PCA_PWM1 = 0x00; // 8位PWM
    CCAP1L = 0x00; // 占空比为100%
    CCAP1H = 0x00;

    CR = 1; // 启动PCA计时器
}

static void hal_adc_init(void)
{
    ADCTIM = 0x3f; // 设置ADC内部时序，单次转换总共45个时钟周期
    ADCCFG = 0x21; // 设置ADC时钟为系统时钟/2/2=1M，结果右对齐
    ADC_CONTR = 0x8f; // 使能ADC模块，并选择第15通道
}

void hal_init(void)
{
    g_tick = 0;

    P_SW2 |= 0x80; /* 使能访问XFR */
    hal_gpio_init();
    hal_time_base_init();
    hal_pwm_init();
    hal_adc_init();

    /* 使能INT0下降沿中断 */
    IT0 = 1;

    /* 使能看门狗，空闲状态不停止 */
    WDT_CONTR = 0x20 | WDT_TIME;

    EA = 1;
}

BOOL hal_key_pressed(void)
{
    return !PIN_KEY;
}

void hal_led_en(led_e led)
{
    switch (led) {
        case LED_FOCUS:
            PIN_LED_FOCUS = 1;
            PIN_LED_WIDE = 0;
            PIN_DRV_EN = 1;
            break;
        case LED_WIDE:
            PIN_LED_WIDE = 1;
            PIN_LED_FOCUS = 0;
            PIN_DRV_EN = 1;
            break;
        default:
            PIN_DRV_EN = 0;
            PIN_LED_FOCUS = 0;
            PIN_LED_WIDE = 0;
            break;
    }
}

void hal_pwm_set_duty(u8 duty)
{
    CCAP1H = 255 - duty;
}

void hal_adc_en(BOOL en)
{
    if (en) {
        ADC_CONTR |= 0x80;
    } else {
        ADC_CONTR &= ~0x80;
    }
}

u16 hal_adc_conv(adc_ch_e ch)
{
    u16 val;

    ADC_CONTR = (ADC_CONTR & 0xf0) | ch;
    ADC_CONTR |= 0x40;                          // 启动AD转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));                // 查询ADC完成标志
    ADC_CONTR &= ~0x20;                         // 清完成标志
    val = (ADC_RES << 8) | ADC_RESL;            // 读取ADC结果

    return val;
}

/* unit: 10mV */
u16 hal_get_vcc(void)
{
    u16 val = 0;
    u8 i;

    // 读取8次数据取平均值
    for (i = 0; i < 8; i++) {
        val += hal_adc_conv(ADC_VCC);
    }
    val >>= 3;

    return (u16)(((u32)CHIP_VREF << 10) / 10 / val);
}

void hal_enter_idle(void)
{
    PCON = 0x01; // MCU进入IDLE模式
}

void hal_enter_low_power(void)
{
    hal_adc_en(FALSE);
    EX0 = 1; // 使能INT0中断
    PCON = 0x02; // MCU进入掉电模式
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    EX0 = 0; // 关闭INT0中断
    hal_adc_en(TRUE);
    delay_ms(1);
}

void hal_soft_reset(void)
{
    IAP_CONTR |= 0x60;
    while (1);
}

void hal_wdt_feed(void)
{
    WDT_CONTR = 0x38 | WDT_TIME;
}
