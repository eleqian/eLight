#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 主时钟频率 */
#define SYS_CLK_HZ 4000000UL

/* 任务执行频率 */
#define TASK_HZ 100

/* 以下硬件只能选择一种 */
/* 适配微笑鲨867A，带聚光灯和泛光灯 */
#define CONFIG_BOARD_867A 0
/* 适配微笑鲨SD-1006，兼容其他硬件改装，单灯，带NTC温控 */
#define CONFIG_BOARD_SD1006 1

#if CONFIG_BOARD_867A
#define CONFIG_LED_WIDE_EN 1
#define CONFIG_NTC_EN 0
#elif CONFIG_BOARD_SD1006
#define CONFIG_LED_WIDE_EN 0
#define CONFIG_NTC_EN 1
#else
#error Unsupported hardware!
#endif

/* 配置NTC的B值 */
#if CONFIG_NTC_EN
//#define CONFIG_NTC_B 3950
#define CONFIG_NTC_B 4250
#endif

#endif // !__CONFIG_H__
