/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       PWM DAC输出 实验
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
#include "./BSP/PWMDAC/pwmdac.h"
#include "./BSP/ADC/adc.h"
#include "math.h"


int main(void)
{
    uint16_t adcx;
    float temp;
    uint8_t t = 0;
    uint8_t key;
    uint16_t pwmval = 0;

    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(84, 115200);                     /* 串口初始化为115200 */
    usmart_dev.init(84);                        /* 初始化USMART */
    led_init();                                 /* 初始化LED */
    lcd_init();                                 /* 初始化LCD */
    key_init();                                 /* 初始化按键 */
    adc_init();                                 /* 初始化ADC */

    pwmdac_init(256 - 1, 0);                    /* PWM DAC初始化, Fpwm = 84M / 256 = 328.125Khz */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "PWM DAC TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY_UP:+  KEY1:-", RED);

    lcd_show_string(30, 130, 200, 16, 16, "PWM VAL:", BLUE);
    lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
    lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);

    while (1)
    {
        t++;
        key = key_scan(0);  /* 按键扫描 */

        if (key == WKUP_PRES)           /* PWM占空比调高 */
        {
            if (pwmval < 250)           /* 范围限定 */
            {
                pwmval += 10;
            }

            PWMDAC_TIMX_CCRX = pwmval;  /* 输出新的PWM占空比 */
        }
        else if (key == KEY1_PRES)      /* PWM占空比调低 */
        {
            if (pwmval > 10)            /* 范围限定 */
            {
                pwmval -= 10;
            }
            else
            {
                pwmval = 0;
            }
            
            PWMDAC_TIMX_CCRX = pwmval;  /* 输出新的PWM占空比 */
        }

        if (t == 10 || key == KEY1_PRES || key == WKUP_PRES)   /* WKUP / KEY1 按下了, 或者定时时间到了 */
        {
            adcx = PWMDAC_TIMX_CCRX;                            /* PWM DAC 定时器输出比较值 */
            lcd_show_xnum(94, 130, adcx, 3, 16, 0, BLUE);       /* 显示CCRX寄存器值 */

            temp = (float)adcx * (3.3 / 256);                   /* 得到DAC电压值 */
            adcx = temp;
            lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);       /* 显示电压值整数部分 */

            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 150, temp, 3, 16, 0X80, BLUE);   /* 显示电压值的小数部分 */

            adcx = adc_get_result_average(ADC_ADCX_CHY, 10);    /* 得到ADC通道5的转换结果 */
            temp = (float)adcx * (3.3 / 4096);                  /* 得到ADC电压值(adc是12bit的) */
            adcx = temp;
            lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);       /* 显示电压值整数部分 */

            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 170, temp, 3, 16, 0X80, BLUE);   /* 显示电压值的小数部分 */

            LED0_TOGGLE();  /* LED0闪烁 */
            t = 0;
        }

        delay_ms(10);
    }
}

















