/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-12-30
 * @brief       �������Ź� ʵ��
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
#include "./BSP/WDG/wdg.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(84, 115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    delay_ms(100);                          /* ��ʱ100ms�ٳ�ʼ�����Ź�,LED0�ı仯"�ɼ�" */
    iwdg_init(4, 500);                      /* Ԥ��Ƶ��Ϊ64,����ֵΪ500, ���ʱ��Ϊ1s */
    LED0(0);                                /* ����LED0(���) */

    while (1)
    {
        if (key_scan(0) == WKUP_PRES)       /* ���WK_UP����,��ι�� */
        {
            iwdg_feed();    /* ι�� */
        }

        delay_ms(10);
    }
}
















