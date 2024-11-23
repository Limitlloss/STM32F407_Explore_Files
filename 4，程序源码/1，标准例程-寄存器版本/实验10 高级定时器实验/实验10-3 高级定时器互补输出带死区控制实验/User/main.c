/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-12-31
 * @brief       �߼���ʱ������������������� ʵ��
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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    uint8_t t = 0;

    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(84, 115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */

    atim_timx_cplm_pwm_init(1000 - 1, 168 - 1); /* 1Mhz�ļ���Ƶ�� 1Khz������. */
    atim_timx_cplm_pwm_set(300, 100);           /* ռ�ձ�:7:3, ����ʱ�� 100 * tDTS */

    while (1)
    {
        t++;
        delay_ms(10);

        if (t > 50)                             /* ����LED0��˸, ��ʾ��������״̬ */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}






