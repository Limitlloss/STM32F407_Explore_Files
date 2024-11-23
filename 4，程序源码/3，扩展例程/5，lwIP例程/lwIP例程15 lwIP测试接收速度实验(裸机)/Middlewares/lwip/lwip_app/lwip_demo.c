/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-04-01
 * @brief       lwIP lwipperf ʵ��
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
 
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <lwip/sockets.h>
#include "./BSP/LCD/lcd.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/apps/lwiperf.h"
#include "lwip_demo.h"
#include "lwip_comm.h"


uint8_t g_info_buf[30];

/* ����״̬ */
const char *report_type_str[] = 
{
    "TCP_DONE_SERVER",             /* LWIPERF_TCP_DONE_SERVER,*/
    "TCP_DONE_CLIENT",             /* LWIPERF_TCP_DONE_CLIENT,*/
    "TCP_ABORTED_LOCAL",           /* LWIPERF_TCP_ABORTED_LOCAL, */
    "TCP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_TCP_ABORTED_LOCAL_DATAERROR, */
    "TCP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_TCP_ABORTED_LOCAL_TXERROR, */
    "TCP_ABORTED_REMOTE",          /* LWIPERF_TCP_ABORTED_REMOTE, */
    "UDP_STARTED",                 /* LWIPERF_UDP_STARTED, */
    "UDP_DONE",                    /* LWIPERF_UDP_DONE, */
    "UDP_ABORTED_LOCAL",           /* LWIPERF_UDP_ABORTED_LOCAL, */
    "UDP_ABORTED_REMOTE"           /* LWIPERF_UDP_ABORTED_REMOTE */
};

/* �����Խ����Ժ����ô˺������˺�������������Խ�� */
static void lwiperf_report(void *arg,
                           enum lwiperf_report_type report_type,
                           const ip_addr_t *local_addr,
                           u16_t local_port,
                           const ip_addr_t *remote_addr,
                           u16_t remote_port,
                           u32_t bytes_transferred,
                           u32_t ms_duration,
                           u32_t bandwidth_kbitpsec)
{
    printf("-------------------------------------------------\r\n");
  
    if (((int)report_type < (sizeof(report_type_str)/sizeof(report_type_str[0]))) && local_addr && remote_addr)
    {
        printf(" %s \r\n", report_type_str[report_type]);
        printf(" Local address : %u.%u.%u.%u ", ((u8_t *)local_addr)[0], ((u8_t *)local_addr)[1],
               ((u8_t *)local_addr)[2], ((u8_t *)local_addr)[3]);
        printf(" Port %d \r\n", local_port);
        printf(" Remote address : %u.%u.%u.%u ", ((u8_t *)remote_addr)[0], ((u8_t *)remote_addr)[1],
               ((u8_t *)remote_addr)[2], ((u8_t *)remote_addr)[3]);
        printf(" Port %d \r\n", remote_port);
        printf(" Bytes Transferred %d \r\n", (int)bytes_transferred);
        printf(" Duration (ms) %d \r\n", (int)ms_duration);
        printf(" Bandwidth (kbitpsec) %d \r\n", (int)bandwidth_kbitpsec);
        
        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%u.%u.%u.%u",((u8_t *)local_addr)[0], ((u8_t *)local_addr)[1],
               ((u8_t *)local_addr)[2], ((u8_t *)local_addr)[3]);           /* ��ʾ��̬IP��ַ */
        lcd_show_string(130, 170, 200, 16, 16, (char *)g_info_buf, RED);
        
        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%d",local_port);                           /* ��ʾ���ض˿� */
        lcd_show_string(130, 190, 200, 16, 16, (char *)g_info_buf, RED);
        
        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%u.%u.%u.%u",((u8_t *)remote_addr)[0], ((u8_t *)remote_addr)[1],
               ((u8_t *)remote_addr)[2], ((u8_t *)remote_addr)[3]);         /* ��ʾԶ��IP��ַ */
        lcd_show_string(130, 210, 200, 16, 16, (char *)g_info_buf, RED);

        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%d",remote_port);              /* ��ʾԶ��IP�˿� */
        lcd_show_string(130, 230, 200, 16, 16, (char *)g_info_buf, RED);

        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%d",(int)bytes_transferred/1024);   /* ת���ٶ� */
        lcd_show_string(130, 250, 200, 16, 16, (char *)g_info_buf, RED);

        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%d",(int)ms_duration);         /* ����ʱ�� */
        lcd_show_string(130, 270, 200, 16, 16, (char *)g_info_buf, RED);

        memset(g_info_buf,0,sizeof(g_info_buf));
        sprintf((char*)g_info_buf,"%d",(int)bandwidth_kbitpsec);  /* ���� */
        lcd_show_string(130, 290, 200, 16, 16, (char *)g_info_buf, RED);
    }
    else
    {
        printf(" IPERF Report error\r\n");
    }
}

/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    if(lwiperf_start_tcp_server_default(lwiperf_report,NULL))
    {
        printf("\r\n************************************************\r\n");
        printf(" IPERF Server example\r\n");
        printf("************************************************\r\n");
        printf(" IPv4 Address     : %u.%u.%u.%u\r\n", g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);
        printf(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", g_lwipdev.netmask[0],g_lwipdev.netmask[1],g_lwipdev.netmask[2],g_lwipdev.netmask[3]);
        printf(" IPv4 Gateway     : %u.%u.%u.%u\r\n", g_lwipdev.gateway[0], g_lwipdev.gateway[1],g_lwipdev.gateway[2],g_lwipdev.gateway[3]);
        printf("************************************************\r\n");
    }
    else
    {
        printf("IPERF initialization failed!\r\n");
    }
    while(1)
    {
        lwip_periodic_handle();                                             /* LWIP��ѯ���� */
    }
}
