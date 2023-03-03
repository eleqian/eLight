#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 主时钟频率 */
#define SYS_CLK_HZ 4000000UL

/* 任务执行频率 */
#define TASK_HZ 100

/* 以下硬件只能选择一种 */
/* 适配微笑鲨867A，带聚光灯和泛光灯 */
#define CONFIG_BOARD_867A 1
/* 适配微笑鲨SD-1006，兼容其他硬件改装，单灯，带NTC温控 */
#define CONFIG_BOARD_SD1006 0

/*
根据硬件设置的参数：
CONFIG_NTC_EN:       是否存在NTC测温
CONFIG_LED_WIDE_EN:  是否存在泛光灯
CONFIG_FOCUS_LV_MAX: 聚光灯最大亮度，0~32
CONFIG_WIDE_LV_MAX:  泛光灯最大亮度，0~32，当存在时才需要设置
*/
#if CONFIG_BOARD_867A
#define CONFIG_NTC_EN 0
#define CONFIG_LED_WIDE_EN 1
#define CONFIG_FOCUS_LV_MAX 31
#define CONFIG_WIDE_LV_MAX 32
#elif CONFIG_BOARD_SD1006
#define CONFIG_NTC_EN 1
#define CONFIG_LED_WIDE_EN 0
#define CONFIG_FOCUS_LV_MAX 32
#else
#error Unsupported hardware!
#endif

#if CONFIG_NTC_EN
/* 配置NTC的B值 */
//#define CONFIG_NTC_B 3950
#define CONFIG_NTC_B 4250
/* 温度保护值，℃ */
#define CONFIG_TEMP_HIGH 50
#endif

#endif // !__CONFIG_H__
