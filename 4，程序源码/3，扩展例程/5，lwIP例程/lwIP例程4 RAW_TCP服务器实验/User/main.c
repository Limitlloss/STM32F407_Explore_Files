/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-5-04
 * @brief       lwIP RAW TCPServer ʵ��
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
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "lwip_demo.h"


/**
 * @breif       ����UI
 * @param       mode :  bit0:0,������;1,����ǰ�벿��UI
 *                      bit1:0,������;1,���غ�벿��UI
 * @retval      ��
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    uint8_t buf[30];
    
    if(mode & 1<< 0)
    {
        lcd_show_string(6, 10, 200, 32, 32, "STM32", DARKBLUE);
        lcd_show_string(6, 40, lcddev.width, 24, 24, "lwIP TcpServer Test", DARKBLUE);
        lcd_show_string(6, 70, 200, 16, 16, "ATOM@ALIENTEK", DARKBLUE);
    }
    
    if(mode & 1 << 1)
    {
        lcd_show_string(6, 110, 200, 16, 16, "lwIP Init Successed", MAGENTA);
        
        if(g_lwipdev.dhcpstatus == 2)
        {
            sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);      /* ��ʾ��̬IP��ַ */
        }
        else
        {
            sprintf((char*)buf,"Static IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* ��ӡ��̬IP��ַ */
        }
        
        lcd_show_string(6, 130, 200, 16, 16, (char*)buf, MAGENTA);
        
        speed = ethernet_chip_get_speed();                                                                                  /* �õ����� */
        
        if(speed)
        {
            lcd_show_string(6, 150, 200, 16, 16, "Ethernet Speed:100M", MAGENTA);
        }
        else
        {
            lcd_show_string(6, 150, 200, 16, 16, "Ethernet Speed:10M", MAGENTA);
        }
    }
}

int main(void)
{
    uint8_t key;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    sram_init();                        /* SRAM��ʼ�� */
    
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                /* ��ʼ���ⲿSRAM�ڴ�� */
    my_mem_init(SRAMCCM);               /* ��ʼ���ڲ�CCM�ڴ�� */
    
    lwip_test_ui(1);                    /* ����ǰ�벿��UI */

    lcd_show_string(6, 110, 200, 16, 16, "lwIP Init !!", BLUE);
    
    while (lwip_comm_init() != 0)
    {
        lcd_show_string(6, 110, 200, 16, 16, "lwIP Init failed!!", BLUE);
        delay_ms(500);
        lcd_fill(6, 50, 200 + 30, 50 + 16, WHITE);
        lcd_show_string(6, 110, 200, 16, 16, "Retrying...       ", BLUE);
        delay_ms(500);
        LED1_TOGGLE();
    }

    while (!ethernet_read_phy(PHY_SR))  /* ���MCU��PHYоƬ�Ƿ�ͨ�ųɹ� */
    {
        printf("MCU��PHYоƬͨ��ʧ�ܣ������·����Դ�룡������\r\n");
    }
    
#if LWIP_DHCP
    lcd_show_string(6, 130, 200, 16, 16, "DHCP IP configing... ", BLUE);    /* ��ʼDHCP */
    
    while ((g_lwipdev.dhcpstatus != 2) && (g_lwipdev.dhcpstatus != 0XFF))          /* �ȴ�DHCP��ȡ�ɹ�/��ʱ��� */
    {
        lwip_periodic_handle();
        delay_ms(1000);
    }
#endif
    
    lwip_demo();                /* lwIP������� */
    
    lwip_test_ui(2);            /* ���غ�벿��UI */
    
    while (1)
    {
        lwip_periodic_handle(); /* LWIP��ѯ���� */
        
        key = key_scan(0);
        
        if (key == KEY1_PRES)
        {
            if ((g_lwip_send_flag & 1 << 5)) 
            {
                printf("�����Ѿ�����,�����ظ�����\r\n"); /* ������ӳɹ�,�����κδ��� */
            }
            else 
            {
                lwip_demo();  /* ���Ͽ����Ӻ�,����lwip_demo()���� */
            }
        }
        
        delay_ms(10);
    }
}


