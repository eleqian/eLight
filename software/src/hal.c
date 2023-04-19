#include "stc8g.h"
#include "base.h"
#include "hal.h"

#define TIMER_INIT_TICK (65536UL - (SYS_CLK_HZ / TASK_HZ / 12))
#define WDT_TIME 0 /* ~197ms@4M */
#define CHIP_VREF (*(u16 code *)0x1ff7) /* in mv */

#define LED_ON 1
#define LED_OFF 0

#if CONFIG_BOARD_TYPE == BOARD_TYPE_867A
#define SET_LED_FOCUS(on) P54 = (on)
#define SET_LED_WIDE(on) P55 = (on)
#define SET_DRV_EN(on) P31 = (on)
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_SD1006
#define SET_LED_FOCUS(on) P55 = (on)
#define SET_DRV_EN(on)
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_PRO
#define SET_LED_FOCUS(on) P55 = (on)
#define SET_LED_WIDE(on) P31 = (on)
#define SET_LED_RED(on) do { P30 = !(on); SET_LED_WIDE(on); } while (0)
#define SET_DRV_EN(on)
#else
#error Unsupported hardware!
#endif

#define GET_KEY() (!P32)

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
        i = SYS_CLK_HZ / 10000U;
        while (--i)
            ;
    } while (--ms);
}

static void hal_gpio_init(void)
{
#if CONFIG_BOARD_TYPE == BOARD_TYPE_867A
    /* P3.0: ADC IN; P3.1: OUT; P3.2: Bi-IO; P3.3: OUT */
    P3IE = 0xfe;
    P3M0 = 0x0a;
    P3M1 = 0x01;

    /* P5.4: OUT; P5.5: OUT */
    P5M0 = 0x30;
    P5M1 = 0x00;

    SET_DRV_EN(LED_OFF);
    SET_LED_FOCUS(LED_OFF);
    SET_LED_WIDE(LED_OFF);
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_SD1006
    /* P3.0~2: Bi-IO; P3.3: OUT */
    P3M0 = 0x08;
    P3M1 = 0x00;

    /* P5.4: ADC IN; P5.5: OUT */
    P5IE = 0xef;
    P5M0 = 0x20;
    P5M1 = 0x10;

    SET_DRV_EN(LED_OFF);
    SET_LED_FOCUS(LED_OFF);
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_PRO
    /* P3.0: Bi-IO; P3.1: OUT; P3.2: Bi-IO; P3.3: OUT */
    P3M0 = 0x0a;
    P3M1 = 0x00;

    /* P5.4: ADC IN; P5.5: OUT */
    P5IE = 0xef;
    P5M0 = 0x20;
    P5M1 = 0x10;

    SET_DRV_EN(LED_OFF);
    SET_LED_FOCUS(LED_OFF);
    SET_LED_WIDE(LED_OFF);
    SET_LED_RED(LED_OFF);
#endif
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
    CCON = 0x00;
    CMOD = 0x08; // PCA时钟为系统时钟
    CL = 0x00;
    CH = 0x00;

    // PCA模块1
    CCAPM1 = 0x42; // PWM工作模式
#ifdef CONFIG_DIM_HIGH_RES
    PCA_PWM1 = 0xc0; // 10位PWM
#else
    PCA_PWM1 = 0x00; // 8位PWM
#endif
    CCAP1L = 0x00; // 占空比为100%
    CCAP1H = 0x00;

    CR = 1; // 启动PCA计时器
}

static void hal_adc_init(void)
{
    ADCTIM = 0x3f; // 设置ADC内部时序，单次转换总共45个时钟周期
    ADCCFG = 0x20 | (SYS_CLK_HZ / 2 / 1000000UL - 1); // 设置ADC时钟为1M，结果右对齐
    ADC_CONTR = 0x8f; // 使能ADC模块，并选择第15通道
}

void hal_init(void)
{
    g_tick = 0;

    P_SW2 |= 0x80; /* 使能访问XFR */
    hal_time_base_init();
    hal_pwm_init();
    hal_gpio_init();
    hal_adc_init();

    /* 使能INT0下降沿中断 */
    IT0 = 1;

    /* 使能看门狗，空闲状态不停止 */
    WDT_CONTR = 0x20 | WDT_TIME;

    EA = 1;
}

BOOL hal_key_pressed(void)
{
    return GET_KEY();
}

void hal_led_en(led_e led)
{
    SET_DRV_EN(LED_OFF);
    SET_LED_FOCUS(LED_OFF);
#if (CONFIG_LED_EN & CONFIG_LED_WIDE)
    SET_LED_WIDE(LED_OFF);
#endif
#if (CONFIG_LED_EN & CONFIG_LED_RED)
    SET_LED_RED(LED_OFF);
#endif

    switch (led) {
        case LED_FOCUS:
            SET_LED_FOCUS(LED_ON);
            SET_DRV_EN(LED_ON);
            break;
#if (CONFIG_LED_EN & CONFIG_LED_WIDE)
        case LED_WIDE:
            SET_LED_WIDE(LED_ON);
            SET_DRV_EN(LED_ON);
            break;
#endif
#if (CONFIG_LED_EN & CONFIG_LED_RED)
        case LED_RED:
            SET_LED_RED(LED_ON);
            SET_DRV_EN(LED_ON);
            break;
#endif
        default:
            break;
    }
}

void hal_pwm_set_duty(light_duty_t duty)
{
#if CONFIG_DIM_HIGH_RES
    u16 pwm_value = LIGHT_DUTY_MAX - 1 - duty;

    PCA_PWM1 = (PCA_PWM1 & ~0x30) | (u8)((pwm_value & 0x0300) >> 4);
    CCAP1H = (u8)pwm_value;
#else
    CCAP1H = LIGHT_DUTY_MAX - 1 - duty;
#endif
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
    /* 按键按下时不能进入低功耗模式 */
    if (hal_key_pressed()) {
        return;
    }

    hal_adc_en(FALSE);
    /* pwm固定输出低，避免外部电流功耗 */
    CCAP1H = 0xff;
    CCAP1L = 0xff;
    PCA_PWM1 |= 0x3f;
    EX0 = 1; // 使能INT0中断
    PCON = 0x02; // MCU进入掉电模式
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    EX0 = 0; // 关闭INT0中断
    /* pwm固定输出高，最低亮度 */
    CCAP1H = 0x0;
    CCAP1L = 0x0;
    PCA_PWM1 &= 0xc0;
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
