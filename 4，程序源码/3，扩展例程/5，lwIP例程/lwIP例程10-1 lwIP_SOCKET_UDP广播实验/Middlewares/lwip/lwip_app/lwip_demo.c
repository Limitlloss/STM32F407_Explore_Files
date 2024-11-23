/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP广播 实验
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

#define LWIP_SEND_THREAD_PRIO       (tskIDLE_PRIORITY + 3) /* 发送数据线程优先级 */
/* 接收数据缓冲区 */
static uint8_t g_lwip_demo_recvbuf[1024]; 
static uint8_t g_lwip_demo_sendbuf[] = {"ALIENTEK DATA\r\n"}; 
void lwip_recv_thread(void *pvParameters);

/* UDP广播测试 */
void lwip_demo(void)
{
    struct link_socjet_info *socket_info;
    socket_info = mem_malloc(sizeof(struct link_socjet_info));
    socket_info->optval = 1;
    
    /* 创建socket UDP通信 */
    socket_info->sfd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(socket_info->sfd, SOL_SOCKET, SO_BROADCAST, &socket_info->optval, sizeof(socket_info->optval));
    socket_info->client_addr.sin_family = AF_INET;
    socket_info->client_addr.sin_port = htons(9090);
    socket_info->client_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    socket_info->client_addr_len = sizeof(struct sockaddr_in);
    
    /* 设置接收和发送缓冲区 */
    socket_info->recv.buf = g_lwip_demo_recvbuf;
    socket_info->recv.size = sizeof(g_lwip_demo_recvbuf);
    socket_info->send.buf = g_lwip_demo_sendbuf;
    socket_info->send.size = sizeof(g_lwip_demo_sendbuf);
    
    sys_thread_new("lwip_send_thread", lwip_recv_thread, (void *)socket_info, 512, LWIP_SEND_THREAD_PRIO );
    
    while (1)
    {
        /* 发送广播数据 */
        sendto(socket_info->sfd, socket_info->send.buf, socket_info->send.size, 0, 
              (struct sockaddr *)&socket_info->client_addr, socket_info->client_addr_len);
        vTaskDelay(1000);
    }
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_recv_thread(void *pvParameters)
{
    struct link_socjet_info *socket_info = pvParameters;
    int recv_len = 0;
    
    while (1)
    {
        recv_len = recv(socket_info->sfd, (void *)socket_info->recv.buf, socket_info->recv.size, 0);
        socket_info->recv.buf[recv_len] = '\0';
        printf("%s",g_lwip_demo_recvbuf);
        vTaskDelay(1);
    }
}
