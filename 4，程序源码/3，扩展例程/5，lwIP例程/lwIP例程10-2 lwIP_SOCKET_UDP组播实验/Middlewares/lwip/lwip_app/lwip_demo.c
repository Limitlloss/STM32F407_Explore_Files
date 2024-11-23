/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP组播 实验
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
#include "semphr.h"
#include <stdint.h>
#include <stdio.h>
#include <lwip/sockets.h>
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip_demo.h"


/* socket信息 */
struct link_socjet_info
{
    struct sockaddr_in client_addr; /* 网络地址信息 */
    socklen_t client_addr_len;      /* 网络地址信息长度 */
    int optval;                     /* 为存放选项值 */
    int sfd;                        /* socket控制块 */
    ip_mreq multicast_mreq;         /* 组播控制块 */
    
    struct
    {
        uint8_t *buf;               /* 缓冲空间 */
        uint32_t size;              /* 缓冲空间大小 */
    } send;                         /* 发送缓冲 */
    
    struct
    {
        uint8_t *buf;               /* 缓冲空间 */
        uint32_t size;              /* 缓冲空间大小 */
    } recv;                         /* 接收缓冲 */
};

/* 多播信息 */
struct ip_mreq_t
{
    struct ip_mreq mreq;            /* 多播信息控制块 */
    socklen_t mreq_len;             /* 多播信息长度 */
};

#define LWIP_SEND_THREAD_PRIO       (tskIDLE_PRIORITY + 3) /* 发送数据线程优先级 */
void lwip_send_thread(void *pvParameters);
/* 接收数据缓冲区 */
static uint8_t g_lwip_demo_recvbuf[1024];
static uint8_t g_lwip_demo_sendbuf[] = {"ALIENTEK DATA\r\n"}; 

/* 多播 IP 地址 */
#define GROUP_IP "224.0.1.0"

/**
 * @brief       测试代码
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    struct link_socjet_info *socket_info;
    struct ip_mreq_t *mreq_info;
    
    socket_info = mem_malloc(sizeof(struct link_socjet_info));
    mreq_info = mem_malloc(sizeof(struct ip_mreq_t));
    
    socket_info->sfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (socket_info->sfd < 0)
    {
        printf("socket failed!\n");
    }
    
    socket_info->client_addr.sin_family = AF_INET;
    socket_info->client_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* 待与 socket 绑定的本地网络接口 IP */   
    socket_info->client_addr.sin_port = htons(9999);                /* 待与 socket 绑定的本地端口号 */
    socket_info->client_addr_len = sizeof(socket_info->client_addr);
    
    /* 设置接收和发送缓冲区 */
    socket_info->recv.buf = g_lwip_demo_recvbuf;
    socket_info->recv.size = sizeof(g_lwip_demo_recvbuf);
    socket_info->send.buf = g_lwip_demo_sendbuf;
    socket_info->send.size = sizeof(g_lwip_demo_sendbuf);
    
    /* 将 Socket 与本地某网络接口绑定 */
    int ret = bind(socket_info->sfd, (struct sockaddr*)&socket_info->client_addr, socket_info->client_addr_len);
    
    if (ret < 0)
    {
        printf(" bind error!\n ");
    }

    mreq_info->mreq.imr_multiaddr.s_addr = inet_addr(GROUP_IP);     /* 多播组 IP 地址设置 */
    mreq_info->mreq.imr_interface.s_addr = htonl(INADDR_ANY);       /* 待加入多播组的 IP 地址 */
    mreq_info->mreq_len = sizeof(struct ip_mreq);

    /* 添加多播组成员（该语句之前，socket 只与 某单播IP地址相关联 执行该语句后 将与多播地址相关联） */
    ret = setsockopt(socket_info->sfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq_info->mreq,mreq_info->mreq_len);
    
    if (ret < 0)
    {
        printf("setsockopt failed !");
    }
    else
    {
        printf("setsockopt success\n");
    }
    
    int length = 0;
    struct sockaddr_in sender;
    int sender_len = sizeof(sender);
    
    sys_thread_new("lwip_send_thread", lwip_send_thread, (void *)socket_info, 512, LWIP_SEND_THREAD_PRIO );
    
    while(1)
    {
        length = recvfrom(socket_info->sfd,socket_info->recv.buf,socket_info->recv.size,0,(struct sockaddr*)&sender,(socklen_t *)&sender_len);
        socket_info->recv.buf[length]='\0';
        printf("%s %d : %s\n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port), socket_info->recv.buf);
        vTaskDelay(10);
    }
    
    setsockopt(socket_info->sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq_info->mreq, mreq_info->mreq_len);
    closesocket(socket_info->sfd);
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入struct link_socjet_info结构体
 * @retval      无
 */
void lwip_send_thread(void *pvParameters)
{
    struct link_socjet_info *socket_info = pvParameters;
    socket_info->client_addr.sin_addr.s_addr = inet_addr(GROUP_IP); /* 组播ip */
    
    while (1)
    {
        /* 数据广播 */
        sendto(socket_info->sfd, socket_info->send.buf, socket_info->send.size + 1, 0, (struct sockaddr*)&socket_info->client_addr,socket_info->client_addr_len);
        vTaskDelay(1000);
    }
}