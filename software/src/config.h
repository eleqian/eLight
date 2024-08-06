#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 适配微笑鲨867A，带聚光灯和泛光灯 */
#define BOARD_TYPE_867A 0
/* 适配微笑鲨SD-1006，单LED，带NTC温控 */
#define BOARD_TYPE_SD1006 1
/* 适配10W主灯，侧灯支持1.5W白光和0.5W红光，带NTC温控 */
#define BOARD_TYPE_PRO 2
/* 通用双LED控制板（聚光+泛光），带NTC温控 */
#define BOARD_TYPE_CORE 3

/* 选择硬件 */
#define CONFIG_BOARD_TYPE BOARD_TYPE_PRO

/* 主时钟频率，注意在ISP软件下载时选择相同值 */
#if CONFIG_BOARD_TYPE == BOARD_TYPE_PRO
#define SYS_CLK_HZ 16000000UL
#else
#define SYS_CLK_HZ 4000000UL
#endif

/* 任务执行频率 */
#define TASK_HZ 100

/* ADC位数 */
#define CONFIG_ADC_RES 10

/* LED类型 */
#define CONFIG_LED_FOCUS 0x1
#define CONFIG_LED_WIDE 0x2
#define CONFIG_LED_RED 0x4

/* NTC接法 */
#define NTC_NONE 0
#define NTC_TO_GND 1
#define NTC_TO_VCC 2

/*
根据硬件设置的参数：
CONFIG_LED_EN: 选择使能的LED
CONFIG_FOCUS_LV_MAX: 聚光灯常亮最大亮度，0~32
CONFIG_FOCUS_FLASH_LV_MAX: 聚光灯闪烁最大亮度，0~32
CONFIG_WIDE_LV_MAX: 泛光灯最大亮度，0~32，当存在时才需要设置
CONFIG_RED_LV_MAX: 红光灯最大亮度，0~32，当存在时才需要设置
CONFIG_DIM_HIGH_RES: 是否使用宽范围调光（2%~100%），普通范围（10%~100%）
CONFIG_NTC_EN: 是否存在NTC测温，0-无NTC，1-NTC接GND，2-NTC接VCC
*/
#if CONFIG_BOARD_TYPE == BOARD_TYPE_867A
#define CONFIG_LED_EN (CONFIG_LED_FOCUS | CONFIG_LED_WIDE)
#define CONFIG_FOCUS_LV_MAX 31
#define CONFIG_FOCUS_FLASH_LV_MAX 32
#define CONFIG_WIDE_LV_MAX 32
#define CONFIG_DIM_HIGH_RES 0
#define CONFIG_NTC_EN NTC_NONE
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_SD1006
#define CONFIG_LED_EN (CONFIG_LED_FOCUS)
#define CONFIG_FOCUS_LV_MAX 32
#define CONFIG_FOCUS_FLASH_LV_MAX 32
#define CONFIG_DIM_HIGH_RES 0
#define CONFIG_NTC_EN NTC_TO_VCC
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_PRO
#define CONFIG_LED_EN (CONFIG_LED_FOCUS | CONFIG_LED_WIDE | CONFIG_LED_RED)
#define CONFIG_FOCUS_LV_MAX 28 /* 1.8A */
#define CONFIG_FOCUS_FLASH_LV_MAX 32 /* 3A */
#define CONFIG_WIDE_LV_MAX 17 /* 0.5A */
#define CONFIG_RED_LV_MAX 14 /* 0.35A */
#define CONFIG_DIM_HIGH_RES 1
#define CONFIG_NTC_EN NTC_TO_GND
#elif CONFIG_BOARD_TYPE == BOARD_TYPE_CORE
#define CONFIG_LED_EN (CONFIG_LED_FOCUS | CONFIG_LED_WIDE)
#define CONFIG_FOCUS_LV_MAX 31
#define CONFIG_FOCUS_FLASH_LV_MAX 32
#define CONFIG_WIDE_LV_MAX 32
#define CONFIG_DIM_HIGH_RES 0
#define CONFIG_NTC_EN NTC_TO_GND
#else
#error Unsupported hardware!
#endif

#if CONFIG_DIM_HIGH_RES
#define LIGHT_DIM_DIV 4 /* PWM->10k/1uf-20k-FB-10k<-OP_out */
#define LIGHT_DUTY_MAX 1024UL
#define LIGHT_VFB 60UL /* 600mV, unit: 10mv */
typedef u16 light_duty_t;
#else
#define LIGHT_DIM_DIV 24 /* PWM->10k/1uf-220k-FB-10k<-Rsense */
#define LIGHT_DUTY_MAX 256
#define LIGHT_VFB 10 /* 100mV, unit: 10mv */
typedef u8 light_duty_t;
#endif

#if CONFIG_NTC_EN
/* 配置NTC的B值，例如3950/4250等 */
#define CONFIG_NTC_B 4250
/* 温度保护值，℃ */
#define CONFIG_TEMP_HIGH 50
#endif

#endif // !__CONFIG_H__
