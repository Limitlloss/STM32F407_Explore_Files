/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP NTP ʵ��
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
#include <stdint.h>
#include "lwip_demo.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "lwip/tcpip.h"
#include <stdio.h>
#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/dns.h"
#include "MALLOC/malloc.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"
#include <netdb.h>


#define NTPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 4 )                       /* �������ȼ� */
#define NTP_TIMESTAMP_DELTA  2208988800UL
const char g_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
NPTformat g_ntpformat;                                                        /* NT���ݰ��ṹ�� */
DateTime g_nowdate;                                                           /* ʱ��ṹ�� */
uint8_t g_ntp_message[48];                                                    /* �������ݰ��Ļ����� */  
uint8_t g_ntp_demo_recvbuf[NTP_DEMO_RX_BUFSIZE];                              /* NTP�������ݻ����� */

uint8_t g_lwip_time_buf[100];

/**
 *@brief     ��������ʱ��
 *@param     seconds�UUTC �����׼ʱ��
 *@retval    ��
*/
void lwip_calc_date_time(unsigned long long time)
{
   unsigned int Pass4year;
    int hours_per_year;
    if (time <= 0)
    {
        time = 0;
    }

    g_nowdate.second = (int)(time % 60);   /* ȡ��ʱ�� */
    time /= 60;

    g_nowdate.minute = (int)(time % 60);   /* ȡ����ʱ�� */
    time /= 60;
    
    g_nowdate.hour = (int)(time % 24);     /* Сʱ�� */

    Pass4year = ((unsigned int)time / (1461L * 24L));/* ȡ��ȥ���ٸ����꣬ÿ������ 1461*24 Сʱ */

    g_nowdate.year = (Pass4year << 2) + 1970;    /* ������� */

    time %= 1461 * 24;     /* ������ʣ�µ�Сʱ�� */

    for (;;)               /* У������Ӱ�����ݣ�����һ����ʣ�µ�Сʱ�� */
    {
        hours_per_year = 365 * 24;         /* һ���Сʱ�� */

        if ((g_nowdate.year & 3) == 0) /* �ж����� */
        {

            hours_per_year += 24;          /* �����꣬һ�����24Сʱ����һ�� */
        }

        if (time < hours_per_year)
        {
            break;
        }

        g_nowdate.year++;
        time -= hours_per_year;
    }

    time /= 24;   /* һ����ʣ�µ����� */

    time++;       /* �ٶ�Ϊ���� */

    if ((g_nowdate.year & 3) == 0)      /* У��������������·ݣ����� */
    {
        if (time > 60)
        {
            time--;
        }
        else
        {
            if (time == 60)
            {
                g_nowdate.month = 1;
                g_nowdate.day = 29;
                return ;
            }
        }
    }

    for (g_nowdate.month = 0; g_days[g_nowdate.month] < time; g_nowdate.month++)   /* �������� */
    {
        time -= g_days[g_nowdate.month];
    }

    g_nowdate.day = (int)(time);

    return;

}

/**
 *@brief     ��NTP��������ȡʱ��
 *@param     buf����Ż���
 *@param     idx��������������ʼλ��
 *@retval    ��
*/
void lwip_get_seconds_from_ntp_server(uint8_t *buf, uint16_t idx)
{
    unsigned long long atk_seconds = 0; 
    uint8_t i = 0;

    for (i = 0; i < 4; i++)  /* ��ȡ40~43λ������ */
    {
        atk_seconds = (atk_seconds << 8) | buf[idx + i]; /* ��40~43λת��16������ת��ʮ���� */
    }

    atk_seconds -= NTP_TIMESTAMP_DELTA;/* ��ȥ��ȥ1900-1970��ʱ��2208988800�룩 */
    lwip_calc_date_time(atk_seconds);       /* ��UTCʱ��������� */
}

/**
 *@brief     ��ʼ��NTP Client��Ϣ
 *@param     ��
 *@retval    ��
*/
void lwip_ntp_client_init(void)
{
    uint8_t flag;

    g_ntpformat.leap = 0;           /* leap indicator */
    g_ntpformat.version = 3;        /* version number */
    g_ntpformat.mode = 3;           /* mode */
    g_ntpformat.stratum = 0;        /* stratum */
    g_ntpformat.poll = 0;           /* poll interval */
    g_ntpformat.precision = 0;      /* precision */
    g_ntpformat.rootdelay = 0;      /* root delay */
    g_ntpformat.rootdisp = 0;       /* root dispersion */
    g_ntpformat.refid = 0;          /* reference ID */
    g_ntpformat.reftime = 0;        /* reference time */
    g_ntpformat.org = 0;            /* origin timestamp */
    g_ntpformat.rec = 0;            /* receive timestamp */
    g_ntpformat.xmt = 0;            /* transmit timestamp */

    flag = (g_ntpformat.version << 3) + g_ntpformat.mode; /* one byte Flag */
    memcpy(g_ntp_message, (void const *)(&flag), 1);
}

/**
 * @brief  lwip_demo�������
 * @param  ��
 * @retval ��
*/
void lwip_demo(void)
{
    err_t err;
    static struct netconn *udpconn;
    static struct netbuf  *recvbuf;
    static struct netbuf  *sentbuf;
    ip_addr_t destipaddr;
    uint32_t data_len = 0;
    struct pbuf *q;
    lwip_ntp_client_init();
    /* ��һ��������udp���ƿ� */
    udpconn = netconn_new(NETCONN_UDP);
    /* ������ճ�ʱʱ�� */
    udpconn->recv_timeout = 10;

    if (udpconn != NULL) /* �жϴ������ƿ��ͷųɹ� */
    {
        /* �ڶ������󶨿��ƿ顢����IP�Ͷ˿� */
        err = netconn_bind(udpconn, IP_ADDR_ANY, NTP_DEMO_PORT);
        /* �������� */
        netconn_gethostbyname((char *)(HOST_NAME), &(destipaddr));
        /* �����������ӻ��߽����Ի��� */
        netconn_connect(udpconn, &destipaddr, NTP_DEMO_PORT); /* ���ӵ�Զ������ */

        if (err == ERR_OK) /* ����� */
        {
            while (1)
            {
                sentbuf = netbuf_new();
                netbuf_alloc(sentbuf, 48);
                memcpy(sentbuf->p->payload, (void *)g_ntp_message, sizeof(g_ntp_message));
                err = netconn_send(udpconn, sentbuf); /* ��sentbuf�е����ݷ��ͳ�ȥ */
                if (err != ERR_OK)
                {
                    printf("����ʧ��\r\n");
                    netbuf_delete(sentbuf); /* ɾ��buf */
                }
                netbuf_delete(sentbuf);      /* ɾ��buf */

                /* ���岽���������� */
                netconn_recv(udpconn, &recvbuf);
                vTaskDelay(1000);     /* ��ʱ1s */
                if (recvbuf != NULL)  /* ���յ����� */
                {
                    memset(g_ntp_demo_recvbuf, 0, NTP_DEMO_RX_BUFSIZE); /*���ݽ��ջ��������� */

                    for (q = recvbuf->p; q != NULL; q = q->next) /*����������pbuf���� */
                    {
                        /* �ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
                        /* �Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                        if (q->len > (NTP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_ntp_demo_recvbuf + data_len, q->payload, (NTP_DEMO_RX_BUFSIZE - data_len)); /* �������� */
                        else memcpy(g_ntp_demo_recvbuf + data_len, q->payload, q->len);

                        data_len += q->len;

                        if (data_len > NTP_DEMO_RX_BUFSIZE) break;          /* ����TCP�ͻ��˽�������,���� */
                    }

                    data_len = 0;                                           /* ������ɺ�data_lenҪ���� */
                    lwip_get_seconds_from_ntp_server(g_ntp_demo_recvbuf,40);   /* ��NTP��������ȡʱ�� */
                    printf("����ʱ�䣺%02d-%02d-%02d %02d:%02d:%02d\r\n",  
                           g_nowdate.year, 
                           g_nowdate.month + 1,
                           g_nowdate.day,
                           g_nowdate.hour + 8,
                           g_nowdate.minute,
                           g_nowdate.second);
                    sprintf((char*)g_lwip_time_buf,"Beijing time:%02d-%02d-%02d %02d:%02d:%02d", g_nowdate.year, 
                                                                                               g_nowdate.month + 1,
                                                                                               g_nowdate.day,
                                                                                               g_nowdate.hour + 8,
                                                                                               g_nowdate.minute,
                                                                                               g_nowdate.second);
                    lcd_show_string(5, 170, lcddev.width, 16, 16, (char*)g_lwip_time_buf, RED);
                    
                    netbuf_delete(recvbuf);             /* ɾ��buf */
                }
                else vTaskDelay(5);                     /* ��ʱ5ms */
            }
        }
        else printf("NTP��ʧ��\r\n");
    }
    else printf("NTP���Ӵ���ʧ��\r\n");
}
