/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP�鲥 ʵ��
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
    ip_mreq multicast_mreq;         /* �鲥���ƿ� */
    
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

/* �ಥ��Ϣ */
struct ip_mreq_t
{
    struct ip_mreq mreq;            /* �ಥ��Ϣ���ƿ� */
    socklen_t mreq_len;             /* �ಥ��Ϣ���� */
};

#define LWIP_SEND_THREAD_PRIO       (tskIDLE_PRIORITY + 3) /* ���������߳����ȼ� */
void lwip_send_thread(void *pvParameters);
/* �������ݻ����� */
static uint8_t g_lwip_demo_recvbuf[1024];
static uint8_t g_lwip_demo_sendbuf[] = {"ALIENTEK DATA\r\n"}; 

/* �ಥ IP ��ַ */
#define GROUP_IP "224.0.1.0"

/**
 * @brief       ���Դ���
 * @param       ��
 * @retval      ��
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
    socket_info->client_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* ���� socket �󶨵ı�������ӿ� IP */   
    socket_info->client_addr.sin_port = htons(9999);                /* ���� socket �󶨵ı��ض˿ں� */
    socket_info->client_addr_len = sizeof(socket_info->client_addr);
    
    /* ���ý��պͷ��ͻ����� */
    socket_info->recv.buf = g_lwip_demo_recvbuf;
    socket_info->recv.size = sizeof(g_lwip_demo_recvbuf);
    socket_info->send.buf = g_lwip_demo_sendbuf;
    socket_info->send.size = sizeof(g_lwip_demo_sendbuf);
    
    /* �� Socket �뱾��ĳ����ӿڰ� */
    int ret = bind(socket_info->sfd, (struct sockaddr*)&socket_info->client_addr, socket_info->client_addr_len);
    
    if (ret < 0)
    {
        printf(" bind error!\n ");
    }

    mreq_info->mreq.imr_multiaddr.s_addr = inet_addr(GROUP_IP);     /* �ಥ�� IP ��ַ���� */
    mreq_info->mreq.imr_interface.s_addr = htonl(INADDR_ANY);       /* ������ಥ��� IP ��ַ */
    mreq_info->mreq_len = sizeof(struct ip_mreq);

    /* ��Ӷಥ���Ա�������֮ǰ��socket ֻ�� ĳ����IP��ַ����� ִ�и����� ����ಥ��ַ������� */
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
 * @brief       ���������̺߳���
 * @param       pvParameters : ����struct link_socjet_info�ṹ��
 * @retval      ��
 */
void lwip_send_thread(void *pvParameters)
{
    struct link_socjet_info *socket_info = pvParameters;
    socket_info->client_addr.sin_addr.s_addr = inet_addr(GROUP_IP); /* �鲥ip */
    
    while (1)
    {
        /* ���ݹ㲥 */
        sendto(socket_info->sfd, socket_info->send.buf, socket_info->send.size + 1, 0, (struct sockaddr*)&socket_info->client_addr,socket_info->client_addr_len);
        vTaskDelay(1000);
    }
}