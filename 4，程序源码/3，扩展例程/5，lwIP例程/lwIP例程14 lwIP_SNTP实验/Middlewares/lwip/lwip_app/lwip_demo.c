/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP SNTP ʵ��
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
 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <time.h>
#include "lwip/opt.h"
#include "lwip/apps/sntp.h"
#include "lwip_demo.h"
#include "lwip/netif.h"
#include "./BSP/RTC/rtc.h"
#include "./BSP/LCD/lcd.h"
#include <netdb.h>


#define HOST_NAME  "ntp1.aliyun.com"    /*������NTP���������� */
uint8_t g_lwip_time_buf[100];

/**
 * @breif       SNTP ��ȡʱ����Ĵ�����
 * @param       sec :  ��ȡ��ʱ���
 * @retval      ��
 */
void sntp_set_system_time(uint32_t sec)
{
    struct tm current_time_val;
    time_t current_time = (time_t)sec;
    
    uint8_t hour, min, secx, ampm;
    uint8_t year, month, date, week;
    uint8_t tbuf[40];
    uint8_t t = 0;
    
#ifdef _MSC_VER
    localtime_s(&current_time_val, &current_time);
#else
    localtime_r(&current_time, &current_time_val);
#endif
    
    printf("����ʱ�䣺%02d-%02d-%d %02d:%02d:%02d\r\n",  
           current_time_val.tm_year + 1900 - 2000, 
           current_time_val.tm_mon + 1,
           current_time_val.tm_mday,
           current_time_val.tm_hour + 8,
           current_time_val.tm_min,
           current_time_val.tm_sec);
    
    sprintf((char*)g_lwip_time_buf,"Beijing time:%02d-%02d-%02d %02d:%02d:%02d", current_time_val.tm_year + 1900 - 2000, 
                                                                               current_time_val.tm_mon + 1,
                                                                               current_time_val.tm_mday,
                                                                               current_time_val.tm_hour + 8,
                                                                               current_time_val.tm_min,
                                                                               current_time_val.tm_sec);
    rtc_set_date(current_time_val.tm_year + 1900 - 2000,current_time_val.tm_mon + 1,current_time_val.tm_mday,current_time_val.tm_wday);
    rtc_set_time(current_time_val.tm_hour + 8,current_time_val.tm_min,current_time_val.tm_sec,0);
    
    while (1)
    {
        t++;

        if ((t % 10) == 0)  /* ÿ100ms����һ����ʾ���� */
        {
            rtc_get_time(&hour, &min, &secx, &ampm);
            sprintf((char *)tbuf, "Time:%02d:%02d:%02d", hour, min, secx);
            lcd_show_string(5, 170, 210, 16, 16, (char*)tbuf, RED);
            rtc_get_date(&year, &month, &date, &week);
            sprintf((char *)tbuf, "Date:20%02d-%02d-%02d", year, month, date);
            lcd_show_string(5, 190, 210, 16, 16, (char*)tbuf, RED);
            sprintf((char *)tbuf, "Week:%d", week);
            lcd_show_string(5, 210, 210, 16, 16, (char*)tbuf, RED);
        }

        vTaskDelay(10);
    }
}

/**
 * @brief  lwip_demo�������
 * @param  ��
 * @retval ��
*/
void lwip_demo(void)
{
    struct hostent *server;
    ip_addr_t sntp_server;
    
    server = gethostbyname((char *)HOST_NAME);                  /* ��aliyun��������ַ���� */
    memcpy(&sntp_server,server->h_addr,server->h_length);       /* �ѽ����õĵ�ַ�����mqtt_ip�������� */
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    sntp_init();
    /* ������ʱ���ĵ�IP��Ϣ */
    sntp_setserver(0, &sntp_server);
    lcd_show_string(5, 170, 210, 16, 16, "wating.....", RED);
}
