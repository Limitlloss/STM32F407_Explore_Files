/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET CPServer������ʵ��
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
#include "lwip_demo.h"
#include "lwip/netdb.h"
#include <lwip/sockets.h>
#include <string.h>
#include <stdlib.h>


static const char *send_data = "link Success\n";

/* �ͻ��˵���Ϣ */
struct client_info
{
    int socket_num;                 /* socket�ŵ����� */
    struct sockaddr_in ip_addr;     /* socket�ͻ��˵�IP��ַ */
    int sockaddr_len;               /* socketaddr�ĳ��� */
};

/* �ͻ��˵�������Ϣ */
struct client_task_info
{
    UBaseType_t client_task_pro;    /* �ͻ����������ȼ� */
    uint16_t client_task_stk;       /* �ͻ����������ȼ� */
    TaskHandle_t * client_handler;  /* �ͻ���������ƿ� */
    char *client_name;              /* �ͻ����������� */
    char *client_num;               /* �ͻ����������� */
};

/* socket��Ϣ */
struct link_socjet_info
{
    int sock_listen;                /* ���� */
    int sock_connect;               /* ���� */
    struct sockaddr_in listen_addr; /* ������ַ */
    struct sockaddr_in connect_addr;/* ���ӵ�ַ */
};

/**
 * @brief       �ͻ��˵�������
 * @param       pvParameters : �������ӿͻ��˵���Ϣ
 * @retval      ��
 */
void lwip_client_thread_entry(void *param)
{
    struct client_info* client = param;
    /* ĳ���ͻ������� */
    printf("Client[%d]%s:%d is connect server\r\n", client->socket_num, inet_ntoa(client->ip_addr.sin_addr),ntohs(client->ip_addr.sin_port));
    /* ��ͻ��˷������ӳɹ���Ϣ */
    send(client->socket_num, (const void* )send_data, strlen(send_data), 0);
    
    while (1)
    {
        char str[2048];
        memset(str, 0, sizeof(str));
        int bytes = recv(client->socket_num, str, sizeof(str), 0);
        
        /* ��ȡ�ر����ӵ����� */
        if (bytes <= 0)
        {
            mem_free(client);
            closesocket(client->socket_num);
            break;
        }
        
        printf("[%d]%s:%d=>%s...\r\n", client->socket_num, inet_ntoa(client->ip_addr.sin_addr),ntohs(client->ip_addr.sin_port), str);
        
        send((int )client->socket_num, (const void * )str, (size_t )strlen(str), 0);
    }
    
    printf("[%d]%s:%d is disconnect...\r\n", client->socket_num, inet_ntoa(client->ip_addr.sin_addr),ntohs(client->ip_addr.sin_port));
    
    vTaskDelete(NULL); /* ɾ�������� */
}

/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    struct client_info *client_fo;
    struct client_task_info *client_task_fo;
    struct link_socjet_info *socket_link_info;
    int sin_size = sizeof(struct sockaddr_in);
    char client_name[10] = "cli";
    char client_num[10];
    
    /* socket���ӽṹ�������ڴ� */
    socket_link_info = mem_malloc(sizeof(struct link_socjet_info));
    
    /* ���ÿͻ���������Ϣ */
    client_task_fo = mem_malloc(sizeof(struct client_task_info));
    client_task_fo->client_handler = NULL;
    client_task_fo->client_task_pro = 5;
    client_task_fo->client_task_stk = 512;
    
    /* ����socket���� */
    if ((socket_link_info->sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket error\r\n");
        return;
    }

    /* ��ʼ�����ӵķ���˵�ַ */
    socket_link_info->listen_addr.sin_family = AF_INET;
    socket_link_info->listen_addr.sin_port = htons(8088);
    socket_link_info->listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(socket_link_info->listen_addr.sin_zero), 0, sizeof(socket_link_info->listen_addr.sin_zero));

    /* ��socket�����ӵķ���˵�ַ��Ϣ */
    if (bind(socket_link_info->sock_listen, (struct sockaddr * )&socket_link_info->listen_addr, sizeof(struct sockaddr)) < 0)
    {
        printf("Bind fail!\r\n");
        goto __exit;
    }

    /* �����ͻ��˵����� */
    listen(socket_link_info->sock_listen, 4);
    printf("begin listing...\r\n");

    while (1)
    {
        /* ����ͻ������� */
        socket_link_info->sock_connect = accept(socket_link_info->sock_listen, (struct sockaddr* )&socket_link_info->connect_addr, (socklen_t* )&sin_size);
        
        if (socket_link_info->sock_connect == -1)
        {
            printf("no socket,waitting others socket disconnect.\r\n");
            continue;
        }
        
        lwip_itoa((char *)socket_link_info->sock_connect, (size_t)client_num, 10);
        strcat(client_name, client_num);
        client_task_fo->client_name = client_name;
        client_task_fo->client_num = client_num;
        
        /* ��ʼ�����ӿͻ�����Ϣ */
        client_fo = mem_malloc(sizeof(struct client_info));
        client_fo->socket_num = socket_link_info->sock_connect;
        memcpy(&client_fo->ip_addr, &socket_link_info->connect_addr, sizeof(struct sockaddr_in));
        client_fo->sockaddr_len = sin_size;
        
        /* �������ӵĿͻ������� */
        xTaskCreate((TaskFunction_t )lwip_client_thread_entry,
                    (const char *   )client_task_fo->client_name,
                    (uint16_t       )client_task_fo->client_task_stk,
                    (void *         )(void*) client_fo,
                    (UBaseType_t    )client_task_fo->client_task_pro ++ ,
                    (TaskHandle_t * )&client_task_fo->client_handler);
        
        if (client_task_fo->client_handler == NULL)
        {

            printf("no memery for thread %s startup failed!\r\n",client_task_fo->client_name);
            mem_free(client_fo);
            continue;
        }
        else
        {
            printf("thread %s success!\r\n", client_task_fo->client_name);
        }
    }
    
__exit: 
    printf("listener failed\r\n");
    /* �ر����socket */
    closesocket(socket_link_info->sock_listen);
    vTaskDelete(NULL); /* ɾ�������� */
}
