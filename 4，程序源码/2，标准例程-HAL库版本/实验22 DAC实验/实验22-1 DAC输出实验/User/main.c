/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       DAC输出 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/ADC/adc.h"


extern DAC_HandleTypeDef g_dac1_handle;

int main(void)
{
    uint16_t adcx;
    float temp;
    uint8_t t = 0;
    uint16_t dacval = 0;
    uint8_t key;

    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    usmart_dev.init(84);                /* 初始化USMART */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    adc_init();                         /* 初始化ADC3 */
    dac_init(1);                        /* 初始化DAC1_OUT1通道 1:DAC_OUT_1-PA4  2:DAC_OUT_2-PA5 */

    lcd_show_string(30,  50, 200, 16, 16, "STM32F103", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "WK_UP:+  KEY1:-", RED);
    
    lcd_show_string(30, 130, 200, 16, 16, "DAC VAL:", BLUE);
    lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
    lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);

    while (1)
    {
        t++;
        key = key_scan(0);  /* 按键扫描 */

        if (key == WKUP_PRES)
        {
            if (dacval < 4000)dacval += 200;

            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval);   /* 输出增大200 */
        }
        else if (key == KEY1_PRES)
        {
            if (dacval > 200)dacval -= 200;
            else dacval = 0;

            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval);   /* 输出减少200 */
        }

        if (t == 10 || key == KEY1_PRES || key == WKUP_PRES)        /* WKUP/KEY1按下了,或者定时时间到了 */
        {
            adcx = HAL_DAC_GetValue(&g_dac1_handle, DAC_CHANNEL_1); /* 读取前面设置DAC1_OUT1的值 */
            lcd_show_xnum(94, 130, adcx, 4, 16, 0, BLUE);           /* 显示DAC寄存器值 */
            
            temp = (float)adcx * (3.3 / 4096);                      /* 得到DAC电压值 */
            adcx = temp;
            lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);           /* 显示电压值整数部分 */
            
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 150, temp, 3, 16, 0X80, BLUE);       /* 显示电压值的小数部分 */
            
            adcx = adc_get_result_average(ADC_ADCX_CHY, 20);        /* 得到ADC1通道5的转换结果 */
            temp = (float)adcx * (3.3 / 4096);                      /* 得到ADC电压值(adc是12bit的) */
            adcx = temp;
            lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);           /* 显示电压值整数部分 */
            
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 170, temp, 3, 16, 0X80, BLUE);       /* 显示电压值的小数部分 */
            
            LED0_TOGGLE();  /* LED0闪烁 */
            t = 0;
        }

        delay_ms(10);
    }
}


