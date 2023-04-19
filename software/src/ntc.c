#include "base.h"
#include "hal.h"
#include "ntc.h"

#if CONFIG_NTC_EN

/* NTC温度表起始温度 */
#define NTC_TABLE_T_FIRST 25

/* NTC温度对应的ADC值查找表，25~80℃，间隔1℃ */
static ROM u16 ntc_adc_tbl[] = {
#if (CONFIG_NTC_B == 4250)
    512, 524, 536, 548, 560, 571, 583, 594, 606, 617, 628, 639, 649, 659, 670, 680, 689, 699, 708,
    718, 726, 735, 744, 752, 760, 768, 776, 783, 790, 798, 804, 811, 818, 824, 830, 836, 842, 847,
    853, 858, 863, 868, 873, 877, 882, 886, 891, 895, 899, 902, 906, 910, 913, 916, 920, 923,
#elif (CONFIG_NTC_B == 3950)
    512, 523, 534, 545, 556, 567, 578, 589, 599, 610, 620, 630, 640, 650, 659, 669, 678, 687, 696,
    704, 713, 721, 729, 737, 745, 753, 760, 768, 775, 782, 788, 795, 801, 807, 813, 819, 825, 831,
    836, 841, 846, 851, 856, 861, 865, 870, 874, 878, 882, 886, 890, 894, 898, 901, 904, 908,
#else
#error Not suppored NTC!
#endif
};

/* 二分法查NTC温度表 */
static u8 ntc_search_table(u16 adc_val)
{
    u8 s_idx = 0;
    u8 e_idx = ARRAY_SIZE(ntc_adc_tbl) - 1;
    u8 idx;
    u16 val;

    // 二分法查找
    while (s_idx + 1 < e_idx) {
        idx = (s_idx + e_idx) >> 1;
        val = ntc_adc_tbl[idx];
        if (adc_val < val) {
            e_idx = idx;
        } else if (adc_val > val) {
            s_idx = idx;
        } else {
            return idx;
        }
    }

    return NTC_TABLE_T_FIRST + s_idx;
}

static u16 ntc_adc_conv(void)
{
    u16 val = 0;
    u8 i;

    /* 读取8次数据取平均值 */
    for (i = 0; i < 8; i++) {
        val += hal_adc_conv(ADC_NTC);
    }
    val >>= 3;

    return val;
}

/* 获取温度 */
u8 ntc_get_temp(void)
{
    u16 adc_val = ntc_adc_conv();

#if CONFIG_NTC_EN == NTC_TO_GND
    adc_val = (1u << CONFIG_ADC_RES) - adc_val;
#endif

    return ntc_search_table(adc_val);
}

#endif // !CONFIG_NTC_EN
