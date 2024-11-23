/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP SOCKET TCPClient ʵ��
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


/* ����Զ��IP��ַ */
//#define DEST_IP_ADDR0               192
//#define DEST_IP_ADDR1               168
//#define DEST_IP_ADDR2                 1
//#define DEST_IP_ADDR3               167

/* ��Ҫ�Լ�����Զ��IP��ַ */
#define IP_ADDR   "192.168.1.37"

#define LWIP_DEMO_RX_BUFSIZE         100                        /* ���������ݳ��� */
#define LWIP_DEMO_PORT               8080                       /* ���ӵı��ض˿ں� */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 )    /* ���������߳����ȼ� */
/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 

/* ������������ */
uint8_t g_lwip_demo_sendbuf[] = "ALIENTEK DATA \r\n";
/* ���ݷ��ͱ�־λ */
uint8_t g_lwip_send_flag;
int g_sock = -1;
int g_lwip_connect_state = 0;
static void lwip_send_thread(void *arg);

extern QueueHandle_t g_display_queue;     /* ��ʾ��Ϣ���о�� */

/**
 * @brief       ���������߳�
 * @param       ��
 * @retval      ��
 */
void lwip_data_send(void)
{
    sys_thread_new("lwip_send_thread", lwip_send_thread, NULL, 512, LWIP_SEND_THREAD_PRIO );
}

/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    struct sockaddr_in atk_client_addr;
    err_t err;
    int recv_data_len;
    BaseType_t lwip_err;
    char *tbuf;
    
    lwip_data_send();                                           /* �������������߳� */
    
    while (1)
    {
sock_start:
        g_lwip_connect_state = 0;
        atk_client_addr.sin_family = AF_INET;                   /* ��ʾIPv4����Э�� */
        atk_client_addr.sin_port = htons(LWIP_DEMO_PORT);       /* �˿ں� */
        atk_client_addr.sin_addr.s_addr = inet_addr(IP_ADDR);   /* Զ��IP��ַ */
        g_sock = socket(AF_INET, SOCK_STREAM, 0);                 /* �ɿ������������������TCPЭ�� */
        memset(&(atk_client_addr.sin_zero), 0, sizeof(atk_client_addr.sin_zero));
        
        tbuf = mymalloc(SRAMIN, 200); /* �����ڴ� */
        sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* �ͻ��˶˿ں� */
        lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
        
        /* ����Զ��IP��ַ */
        err = connect(g_sock, (struct sockaddr *)&atk_client_addr, sizeof(struct sockaddr));
      
        if (err == -1)
        {
            printf("����ʧ��\r\n");
            g_sock = -1;
            closesocket(g_sock);
            myfree(SRAMIN, tbuf);
            vTaskDelay(10);
            goto sock_start;
        }

        printf("���ӳɹ�\r\n");
        lcd_show_string(5, 90, 200, 16, 16, "State:Connection Successful", BLUE);
        g_lwip_connect_state = 1;
        
        while (1)
        {
            recv_data_len = recv(g_sock,g_lwip_demo_recvbuf,
                                 LWIP_DEMO_RX_BUFSIZE,0);
            if (recv_data_len <= 0 )
            {
                closesocket(g_sock);
                g_sock = -1;
                lcd_fill(5, 89, lcddev.width,110, WHITE);
                lcd_show_string(5, 90, 200, 16, 16, "State:Disconnect", BLUE);
                myfree(SRAMIN, tbuf);
                goto sock_start;
            }
            
            /* ���յ����� */ 
            lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
            
            if (lwip_err == errQUEUE_FULL)
            {
                printf("����Key_Queue���������ݷ���ʧ��!\r\n");
            }
            
            vTaskDelay(10);
        }
    }
}

/**
 * @brief       ���������̺߳���
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;
    
    err_t err;
    
    while (1)
    {
        while (1)
        {
            if(((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) && (g_lwip_connect_state == 1)) /* ������Ҫ���� */
            {
                err = write(g_sock, g_lwip_demo_sendbuf, sizeof(g_lwip_demo_sendbuf));
                
                if (err < 0)
                {
                    break;
                }
                
                g_lwip_send_flag &= ~LWIP_SEND_DATA;
            }
            
            vTaskDelay(10);
        }
        
        closesocket(g_sock);
    }
}
