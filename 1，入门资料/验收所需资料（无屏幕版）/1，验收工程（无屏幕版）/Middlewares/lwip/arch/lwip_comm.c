/**
 ****************************************************************************************************
 * @file        lwip_comm.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-12-02
 * @brief       LWIP配置驱动代码
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
 * 修改说明
 * V1.0 20211202
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#include "lwip_comm.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h"
#include "lwip/timeouts.h"
#include "lwip/tcpip.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include <stdio.h>


__lwip_dev g_lwipdev;                   /* lwip控制结构体 */
struct netif g_lwip_netif;              /* 定义一个全局的网络接口 */

#if LWIP_DHCP
uint32_t g_dhcp_fine_timer = 0;         /* DHCP精细处理计时器 */
uint32_t g_dhcp_coarse_timer = 0;       /* DHCP粗糙处理计时器 */
#endif


/**
 * @breif       lwip 默认IP设置
 * @param       lwipx  : lwip控制结构体指针
 * @retval      无
 */
void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
    /* 默认远端IP为:192.168.1.134 */
    lwipx->remoteip[0] = 192;
    lwipx->remoteip[1] = 168;
    lwipx->remoteip[2] = 1;
    lwipx->remoteip[3] = 111;
    
    /* MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID) */
    lwipx->mac[0] = 0xB8;
    lwipx->mac[1] = 0xAE;
    lwipx->mac[2] = 0x1D;
    lwipx->mac[3] = 0x00;
    lwipx->mac[4] = 0x01;
    lwipx->mac[5] = 0x00;
    
    /* 默认本地IP为:192.168.1.30 */
    lwipx->ip[0] = 192;
    lwipx->ip[1] = 168;
    lwipx->ip[2] = 1;
    lwipx->ip[3] = 30;
    /* 默认子网掩码:255.255.255.0 */
    lwipx->netmask[0] = 255;
    lwipx->netmask[1] = 255;
    lwipx->netmask[2] = 255;
    lwipx->netmask[3] = 0;
    
    /* 默认网关:192.168.1.1 */
    lwipx->gateway[0] = 192;
    lwipx->gateway[1] = 168;
    lwipx->gateway[2] = 1;
    lwipx->gateway[3] = 1;
    lwipx->dhcpstatus = 0; /* 没有DHCP */
}

/**
 * @breif       LWIP初始化(LWIP启动的时候使用)
 * @param       无
 * @retval      0,成功
 *              1,内存错误
 *              2,以太网芯片初始化失败
 *              3,网卡添加失败.
 */
uint8_t lwip_comm_init(void)
{
    uint8_t retry = 0;
    struct netif *netif_init_flag;              /* 调用netif_add()函数时的返回值,用于判断网络初始化是否成功 */
    ip_addr_t ipaddr;                           /* ip地址 */
    ip_addr_t netmask;                          /* 子网掩码 */
    ip_addr_t gw;                               /* 默认网关 */

    if (ethernet_mem_malloc())return 1;         /* 内存申请失败*/

    lwip_comm_default_ip_set(&g_lwipdev);       /* 设置默认IP等信息 */

    while (ethernet_init())                     /* 初始化以太网芯片,如果失败的话就重试5次 */
    {
        retry++;

        if (retry > 5)
        {
            retry = 0;                          /* 以太网芯片初始化失败 */
            return 3;
        }
    }

    lwip_init();                                /* 初始化LWIP内核 */

#if LWIP_DHCP                                   /* 使用动态IP */
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else   /* 使用静态IP */
    IP4_ADDR(&ipaddr, g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
    IP4_ADDR(&netmask, g_lwipdev.netmask[0], g_lwipdev.netmask[1], g_lwipdev.netmask[2], g_lwipdev.netmask[3]);
    IP4_ADDR(&gw, g_lwipdev.gateway[0], g_lwipdev.gateway[1], g_lwipdev.gateway[2], g_lwipdev.gateway[3]);
    printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n", g_lwipdev.mac[0], g_lwipdev.mac[1], g_lwipdev.mac[2], g_lwipdev.mac[3], g_lwipdev.mac[4], g_lwipdev.mac[5]);
    printf("静态IP地址........................%d.%d.%d.%d\r\n", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
    printf("子网掩码..........................%d.%d.%d.%d\r\n", g_lwipdev.netmask[0], g_lwipdev.netmask[1], g_lwipdev.netmask[2], g_lwipdev.netmask[3]);
    printf("默认网关..........................%d.%d.%d.%d\r\n", g_lwipdev.gateway[0], g_lwipdev.gateway[1], g_lwipdev.gateway[2], g_lwipdev.gateway[3]);
#endif  /* 向网卡列表中添加一个网口 */
    netif_init_flag = netif_add(&g_lwip_netif, (const ip_addr_t *)&ipaddr, (const ip_addr_t *)&netmask, (const ip_addr_t *)&gw, NULL, &ethernetif_init, &ethernet_input);


    if (netif_init_flag == NULL)
    {
        return 4;                             /* 网卡添加失败 */
    }
    else                                      /* 网口添加成功后,设置netif为默认值,并且打开netif网口 */
    {
        netif_set_default(&g_lwip_netif);     /* 设置netif为默认网口 */

        if (netif_is_link_up(&g_lwip_netif))
        {
            netif_set_up(&g_lwip_netif);      /* 打开netif网口 */
        }
        else
        {
            netif_set_down(&g_lwip_netif);
        }
    }

#if LWIP_DHCP                               /* 如果使用DHCP的话 */
    g_lwipdev.dhcpstatus = 0;                 /* DHCP标记为0 */
    dhcp_start(&g_lwip_netif);                /* 开启DHCP服务 */
#endif
    return 0;                               /* 操作OK. */
}

/**
 * @breif       当接收到数据后调用
 * @param       无
 * @retval      无
 */
void lwip_pkt_handle(void)
{
    /* 从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 */
    ethernetif_input(&g_lwip_netif);
}

/**
 * @breif       LWIP轮询任务
 * @param       无
 * @retval      无
 */
void lwip_periodic_handle(void)
{
    sys_check_timeouts();
#if LWIP_DHCP       /* 如果使用DHCP */

    /* 每500ms调用一次dhcp_fine_tmr() */
    if (sys_now() - g_dhcp_fine_timer >= DHCP_FINE_TIMER_MSECS)    /* DHCP_FINE_TIMER_MSECS(500) lwip_localtime心跳 T3 毫秒单位 */
    {
        g_dhcp_fine_timer =  sys_now();
        dhcp_fine_tmr();

        if ((g_lwipdev.dhcpstatus != 2) && (g_lwipdev.dhcpstatus != 0XFF))
        {
            lwip_dhcp_process_handle();  /* DHCP处理 */
        }
    }

    /* 每60s执行一次DHCP粗糙处理 */
    if (sys_now() - g_dhcp_coarse_timer >= DHCP_COARSE_TIMER_MSECS)
    {
        g_dhcp_coarse_timer =  sys_now();
        dhcp_coarse_tmr();
    }
#endif
}



/* 如果使能DHCP */
#if LWIP_DHCP

struct dhcp g_dhcp;

/**
 * @breif       DHCP处理任务
 * @param       无
 * @retval      无
 */
void lwip_dhcp_process_handle(void)
{
    uint32_t ip = 0, netmask = 0, gw = 0;

    switch (g_lwipdev.dhcpstatus)
    {
        case 0:     /* 开启DHCP */
            dhcp_start(&g_lwip_netif);
            g_lwipdev.dhcpstatus = 1;             /* 等待通过DHCP获取到的地址 */
            printf("正在查找DHCP服务器,请稍等...........\r\n");
            break;

        case 1:     /* 等待获取到IP地址 */
        {
            ip = g_lwip_netif.ip_addr.addr;       /* 读取新IP地址 */
            netmask = g_lwip_netif.netmask.addr;  /* 读取子网掩码 */
            gw = g_lwip_netif.gw.addr;            /* 读取默认网关 */

            if (ip != 0)    /*正确获取到IP地址的时候*/
            {
                g_lwipdev.dhcpstatus = 2;         /* DHCP成功 */
                printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n", g_lwipdev.mac[0], g_lwipdev.mac[1], g_lwipdev.mac[2], g_lwipdev.mac[3], g_lwipdev.mac[4], g_lwipdev.mac[5]);
                /* 解析出通过DHCP获取到的IP地址 */
                g_lwipdev.ip[3] = (uint8_t)(ip >> 24);
                g_lwipdev.ip[2] = (uint8_t)(ip >> 16);
                g_lwipdev.ip[1] = (uint8_t)(ip >> 8);
                g_lwipdev.ip[0] = (uint8_t)(ip);
                printf("通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
                /* 解析通过DHCP获取到的子网掩码地址 */
                g_lwipdev.netmask[3] = (uint8_t)(netmask >> 24);
                g_lwipdev.netmask[2] = (uint8_t)(netmask >> 16);
                g_lwipdev.netmask[1] = (uint8_t)(netmask >> 8);
                g_lwipdev.netmask[0] = (uint8_t)(netmask);
                printf("通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n", g_lwipdev.netmask[0], g_lwipdev.netmask[1], g_lwipdev.netmask[2], g_lwipdev.netmask[3]);
                /* 解析出通过DHCP获取到的默认网关 */
                g_lwipdev.gateway[3] = (uint8_t)(gw >> 24);
                g_lwipdev.gateway[2] = (uint8_t)(gw >> 16);
                g_lwipdev.gateway[1] = (uint8_t)(gw >> 8);
                g_lwipdev.gateway[0] = (uint8_t)(gw);
                printf("通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n", g_lwipdev.gateway[0], g_lwipdev.gateway[1], g_lwipdev.gateway[2], g_lwipdev.gateway[3]);
            }
            else if (g_dhcp.tries > LWIP_MAX_DHCP_TRIES)    /* 通过DHCP服务获取IP地址失败,且超过最大尝试次数 */
            {
                g_lwipdev.dhcpstatus = 0XFF;  /* DHCP超时失败. */
                /* 使用静态IP地址 */
                IP4_ADDR(&(g_lwip_netif.ip_addr), g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
                IP4_ADDR(&(g_lwip_netif.netmask), g_lwipdev.netmask[0], g_lwipdev.netmask[1], g_lwipdev.netmask[2], g_lwipdev.netmask[3]);
                IP4_ADDR(&(g_lwip_netif.gw), g_lwipdev.gateway[0], g_lwipdev.gateway[1], g_lwipdev.gateway[2], g_lwipdev.gateway[3]);
                printf("DHCP服务超时,使用静态IP地址!\r\n");
                printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n", g_lwipdev.mac[0], g_lwipdev.mac[1], g_lwipdev.mac[2], g_lwipdev.mac[3], g_lwipdev.mac[4], g_lwipdev.mac[5]);
                printf("静态IP地址........................%d.%d.%d.%d\r\n", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]);
                printf("子网掩码..........................%d.%d.%d.%d\r\n", g_lwipdev.netmask[0], g_lwipdev.netmask[1], g_lwipdev.netmask[2], g_lwipdev.netmask[3]);
                printf("默认网关..........................%d.%d.%d.%d\r\n", g_lwipdev.gateway[0], g_lwipdev.gateway[1], g_lwipdev.gateway[2], g_lwipdev.gateway[3]);
            }
        }
        break;

        default :
            break;
    }
}
#endif
