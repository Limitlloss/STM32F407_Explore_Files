/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP�㲥 ʵ��
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


/* socket��Ϣ */
struct link_socjet_info
{
    struct sockaddr_in client_addr; /* �����ַ��Ϣ */
    socklen_t client_addr_len;      /* �����ַ��Ϣ���� */
    int optval;                     /* Ϊ���ѡ��ֵ */
    int sfd;                        /* socket���ƿ� */
    
    struct
    {
        uint8_t *buf;               /* ����ռ� */
        uint32_t size;              /* ����ռ��С */
    } send;                         /* ���ͻ��� */
    
    struct
    {
        uint8_t *buf;               /* ����ռ� */
        uint32_t size;              /* ����ռ��С */
    } recv;                         /* ���ջ��� */
};

#define LWIP_SEND_THREAD_PRIO       (tskIDLE_PRIORITY + 3) /* ���������߳����ȼ� */
/* �������ݻ����� */
static uint8_t g_lwip_demo_recvbuf[1024]; 
static uint8_t g_lwip_demo_sendbuf[] = {"ALIENTEK DATA\r\n"}; 
void lwip_recv_thread(void *pvParameters);

/* UDP�㲥���� */
void lwip_demo(void)
{
    struct link_socjet_info *socket_info;
    socket_info = mem_malloc(sizeof(struct link_socjet_info));
    socket_info->optval = 1;
    
    /* ����socket UDPͨ�� */
    socket_info->sfd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(socket_info->sfd, SOL_SOCKET, SO_BROADCAST, &socket_info->optval, sizeof(socket_info->optval));
    socket_info->client_addr.sin_family = AF_INET;
    socket_info->client_addr.sin_port = htons(9090);
    socket_info->client_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    socket_info->client_addr_len = sizeof(struct sockaddr_in);
    
    /* ���ý��պͷ��ͻ����� */
    socket_info->recv.buf = g_lwip_demo_recvbuf;
    socket_info->recv.size = sizeof(g_lwip_demo_recvbuf);
    socket_info->send.buf = g_lwip_demo_sendbuf;
    socket_info->send.size = sizeof(g_lwip_demo_sendbuf);
    
    sys_thread_new("lwip_send_thread", lwip_recv_thread, (void *)socket_info, 512, LWIP_SEND_THREAD_PRIO );
    
    while (1)
    {
        /* ���͹㲥���� */
        sendto(socket_info->sfd, socket_info->send.buf, socket_info->send.size, 0, 
              (struct sockaddr *)&socket_info->client_addr, socket_info->client_addr_len);
        vTaskDelay(1000);
    }
}

/**
 * @brief       ���������̺߳���
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
