/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-12-31
 * @brief       ͨ�ö�ʱ��PWM��� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"

int main(void)
{
    uint16_t ledrpwmval = 0;
    uint8_t dir = 1;

    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(84, 115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    gtim_timx_pwm_chy_init(500 - 1, 84 - 1);/* 1Mhz�ļ���Ƶ��,2Khz��PWM. */

    while (1)
    {
        delay_ms(10);

        if (dir)ledrpwmval++;
        else ledrpwmval--;

        if (ledrpwmval > 300)dir = 0;
        if (ledrpwmval == 0)dir = 1;

        GTIM_TIMX_PWM_CHY_CCRX = ledrpwmval;
    }
}






