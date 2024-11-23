/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       emWin�������ʵ��
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/SPI/spi.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/fonts.h"
#include "uc-os3_demo.h"


int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��, 168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    tp_dev.init();                      /* ��������ʼ�� */
    sram_init();                        /* SRAM��ʼ�� */
    
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                /* ��ʼ���ⲿSRAM�ڴ�� */
    my_mem_init(SRAMCCM);               /* ��ʼ���ڲ�CCM�ڴ�� */

    exfuns_init();                      /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);            /* ����SD�� */
    f_mount(fs[1], "1:", 1);            /* ����FLASH. */

    while(fonts_init())                 /* ��ʼ���ֿ� */
    {
        lcd_show_string(30,70,200,16,16,"Font Error!", RED);

        while (sd_init())               /* ��ⲻ��SD�� */
        {
            lcd_show_string(30,90,200,16,16,"SD Card Failed!", RED);
            delay_ms(200);
            lcd_fill(30,90,200+30,70+16,WHITE);
            delay_ms(200);
        }
        
        fonts_update_font(30,90,16,(uint8_t *)"0:", RED);
        delay_ms(2000);
        lcd_clear(WHITE);
        break;
    }
    
    uc_os3_demo();                      /* �������񣬲�ִ��emwin���� */
    
    while(1);

}
