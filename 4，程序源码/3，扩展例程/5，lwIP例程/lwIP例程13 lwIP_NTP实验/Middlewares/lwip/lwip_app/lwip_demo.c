/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP NTP 实验
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


#define NTPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 4 )                       /* 定义优先级 */
#define NTP_TIMESTAMP_DELTA  2208988800UL
const char g_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
NPTformat g_ntpformat;                                                        /* NT数据包结构体 */
DateTime g_nowdate;                                                           /* 时间结构体 */
uint8_t g_ntp_message[48];                                                    /* 发送数据包的缓存区 */  
uint8_t g_ntp_demo_recvbuf[NTP_DEMO_RX_BUFSIZE];                              /* NTP接收数据缓冲区 */

uint8_t g_lwip_time_buf[100];

/**
 *@brief     计算日期时间
 *@param     secondsUTC 世界标准时间
 *@retval    无
*/
void lwip_calc_date_time(unsigned long long time)
{
   unsigned int Pass4year;
    int hours_per_year;
    if (time <= 0)
    {
        time = 0;
    }

    g_nowdate.second = (int)(time % 60);   /* 取秒时间 */
    time /= 60;

    g_nowdate.minute = (int)(time % 60);   /* 取分钟时间 */
    time /= 60;
    
    g_nowdate.hour = (int)(time % 24);     /* 小时数 */

    Pass4year = ((unsigned int)time / (1461L * 24L));/* 取过去多少个四年，每四年有 1461*24 小时 */

    g_nowdate.year = (Pass4year << 2) + 1970;    /* 计算年份 */

    time %= 1461 * 24;     /* 四年中剩下的小时数 */

    for (;;)               /* 校正闰年影响的年份，计算一年中剩下的小时数 */
    {
        hours_per_year = 365 * 24;         /* 一年的小时数 */

        if ((g_nowdate.year & 3) == 0) /* 判断闰年 */
        {

            hours_per_year += 24;          /* 是闰年，一年则多24小时，即一天 */
        }

        if (time < hours_per_year)
        {
            break;
        }

        g_nowdate.year++;
        time -= hours_per_year;
    }

    time /= 24;   /* 一年中剩下的天数 */

    time++;       /* 假定为闰年 */

    if ((g_nowdate.year & 3) == 0)      /* 校正闰年的误差，计算月份，日期 */
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

    for (g_nowdate.month = 0; g_days[g_nowdate.month] < time; g_nowdate.month++)   /* 计算月日 */
    {
        time -= g_days[g_nowdate.month];
    }

    g_nowdate.day = (int)(time);

    return;

}

/**
 *@brief     从NTP服务器获取时间
 *@param     buf：存放缓存
 *@param     idx：定义存放数据起始位置
 *@retval    无
*/
void lwip_get_seconds_from_ntp_server(uint8_t *buf, uint16_t idx)
{
    unsigned long long atk_seconds = 0; 
    uint8_t i = 0;

    for (i = 0; i < 4; i++)  /* 获取40~43位的数据 */
    {
        atk_seconds = (atk_seconds << 8) | buf[idx + i]; /* 把40~43位转成16进制再转成十进制 */
    }

    atk_seconds -= NTP_TIMESTAMP_DELTA;/* 减去减去1900-1970的时间差（2208988800秒） */
    lwip_calc_date_time(atk_seconds);       /* 由UTC时间计算日期 */
}

/**
 *@brief     初始化NTP Client信息
 *@param     无
 *@retval    无
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
 * @brief  lwip_demo程序入口
 * @param  无
 * @retval 无
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
    /* 第一步：创建udp控制块 */
    udpconn = netconn_new(NETCONN_UDP);
    /* 定义接收超时时间 */
    udpconn->recv_timeout = 10;

    if (udpconn != NULL) /* 判断创建控制块释放成功 */
    {
        /* 第二步：绑定控制块、本地IP和端口 */
        err = netconn_bind(udpconn, IP_ADDR_ANY, NTP_DEMO_PORT);
        /* 域名解析 */
        netconn_gethostbyname((char *)(HOST_NAME), &(destipaddr));
        /* 第三步：连接或者建立对话框 */
        netconn_connect(udpconn, &destipaddr, NTP_DEMO_PORT); /* 连接到远端主机 */

        if (err == ERR_OK) /* 绑定完成 */
        {
            while (1)
            {
                sentbuf = netbuf_new();
                netbuf_alloc(sentbuf, 48);
                memcpy(sentbuf->p->payload, (void *)g_ntp_message, sizeof(g_ntp_message));
                err = netconn_send(udpconn, sentbuf); /* 将sentbuf中的数据发送出去 */
                if (err != ERR_OK)
                {
                    printf("发送失败\r\n");
                    netbuf_delete(sentbuf); /* 删除buf */
                }
                netbuf_delete(sentbuf);      /* 删除buf */

                /* 第五步：接收数据 */
                netconn_recv(udpconn, &recvbuf);
                vTaskDelay(1000);     /* 延时1s */
                if (recvbuf != NULL)  /* 接收到数据 */
                {
                    memset(g_ntp_demo_recvbuf, 0, NTP_DEMO_RX_BUFSIZE); /*数据接收缓冲区清零 */

                    for (q = recvbuf->p; q != NULL; q = q->next) /*遍历完整个pbuf链表 */
                    {
                        /* 判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于 */
                        /* 的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
                        if (q->len > (NTP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_ntp_demo_recvbuf + data_len, q->payload, (NTP_DEMO_RX_BUFSIZE - data_len)); /* 拷贝数据 */
                        else memcpy(g_ntp_demo_recvbuf + data_len, q->payload, q->len);

                        data_len += q->len;

                        if (data_len > NTP_DEMO_RX_BUFSIZE) break;          /* 超出TCP客户端接收数组,跳出 */
                    }

                    data_len = 0;                                           /* 复制完成后data_len要清零 */
                    lwip_get_seconds_from_ntp_server(g_ntp_demo_recvbuf,40);   /* 从NTP服务器获取时间 */
                    printf("北京时间：%02d-%02d-%02d %02d:%02d:%02d\r\n",  
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
                    
                    netbuf_delete(recvbuf);             /* 删除buf */
                }
                else vTaskDelay(5);                     /* 延时5ms */
            }
        }
        else printf("NTP绑定失败\r\n");
    }
    else printf("NTP连接创建失败\r\n");
}
