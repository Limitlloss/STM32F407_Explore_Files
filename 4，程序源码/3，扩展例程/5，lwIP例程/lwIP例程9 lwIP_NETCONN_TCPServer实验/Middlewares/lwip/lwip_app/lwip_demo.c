/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP Netconn TCPServer ʵ��
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
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include <lwip/sockets.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip_demo.h"


/* TCPServer ����Ҫ����Զ��IP��ַ */
//#define DEST_IP_ADDR0               192
//#define DEST_IP_ADDR1               168
//#define DEST_IP_ADDR2                 1
//#define DEST_IP_ADDR3               167

#define LWIP_DEMO_RX_BUFSIZE         2000  /* ���������ݳ��� */
#define LWIP_DEMO_PORT               8080  /* ���ӵı��ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA \r\n";
/* ���ݷ��ͱ�־λ */
uint8_t g_lwip_send_flag;

extern QueueHandle_t g_display_queue;     /* ��ʾ��Ϣ���о�� */

/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    static struct netconn *tcp_serverconn = NULL; /* TCP SERVER�������ӽṹ�� */
    uint32_t  data_len = 0;
    struct    pbuf *q;
    err_t     err,recv_err;
    uint8_t   remot_addr[4];
    struct netconn *newconn;
    static    ip_addr_t ipaddr;
    static    u16_t  port;
    BaseType_t lwip_err;
    char *tbuf;
    
    tbuf = mymalloc(SRAMIN, 200); /* �����ڴ� */
    sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* �ͻ��˶˿ں� */
    lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
    
    /* ��һ��������һ��TCP���ƿ� */
    tcp_serverconn = netconn_new(NETCONN_TCP);                      /* ����һ��TCP���� */
    /* �ڶ�������TCP���ƿ顢����IP��ַ�Ͷ˿ں� */
    netconn_bind(tcp_serverconn,IP_ADDR_ANY,LWIP_DEMO_PORT);        /* �󶨶˿� 8080�Ŷ˿� */
    /* ������������ */
    netconn_listen(tcp_serverconn);                                 /* �������ģʽ */
    tcp_serverconn->recv_timeout = 10;                              /* ��ֹ�����߳� �ȴ�10ms */
    
    while (1) 
    {
        /* ���Ĳ��������������� */
        err = netconn_accept(tcp_serverconn,&newconn);              /* ������������ */
        if (err == ERR_OK) newconn->recv_timeout = 10;

        if (err == ERR_OK)                                          /* ���������ӵ����� */
        { 
            struct netbuf *recvbuf;
            netconn_getaddr(newconn,&ipaddr,&port,0);               /* ��ȡԶ��IP��ַ�Ͷ˿ں� */
            
            remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
            remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
            remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
            remot_addr[0] = (uint8_t)(ipaddr.addr);
            printf("����%d.%d.%d.%d�����Ϸ�����,�����˿ں�Ϊ:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
            lcd_show_string(5, 90, 200, 16, 16, "State:Connection Successful", BLUE);
            
            while (1)
            {
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) /* ������Ҫ���� */
                {
                    err = netconn_write(newconn ,g_lwip_demo_sendbuf,strlen((char*)g_lwip_demo_sendbuf),NETCONN_COPY); /* ����g_lwip_demo_sendbuf�е����� */

                    if(err != ERR_OK)
                    {
                        printf("����ʧ��\r\n");
                    }

                    g_lwip_send_flag &= ~LWIP_SEND_DATA;
                }
                
                if ((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)           /* ���յ����� */
                { 
                    taskENTER_CRITICAL();                                           /* �����ٽ��� */
                    memset(g_lwip_demo_recvbuf,0,LWIP_DEMO_RX_BUFSIZE);               /* ���ݽ��ջ��������� */

                    for (q = recvbuf->p;q != NULL;q = q->next)                       /* ����������pbuf���� */
                    {
                        /* �ж�Ҫ������LWIP_DEMO_RX_BUFSIZE�е������Ƿ����LWIP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
                        /* �Ļ���ֻ����LWIP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                        if(q->len > (LWIP_DEMO_RX_BUFSIZE-data_len))
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,(LWIP_DEMO_RX_BUFSIZE - data_len));/* �������� */
                        }
                        else
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,q->len);
                        }
                        
                        data_len += q->len;

                        if(data_len > LWIP_DEMO_RX_BUFSIZE)
                        {
                            break;   /*����TCP�ͻ��˽�������,����*/
                        }
                    }

                    taskEXIT_CRITICAL();                                /* �˳��ٽ��� */
                    data_len = 0;                                       /* ������ɺ�data_lenҪ���� */
                    
                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("����Key_Queue���������ݷ���ʧ��!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);
                }
                else if (recv_err == ERR_CLSD)                           /* �ر����� */
                {
                    netconn_close(newconn);
                    netconn_delete(newconn);
                    printf("����:%d.%d.%d.%d�Ͽ��������������\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
                    lcd_fill(5, 89, lcddev.width,110, WHITE);
                    lcd_show_string(5, 90, 200, 16, 16, "State:Disconnect", BLUE);
                    myfree(SRAMIN, tbuf);
                    break;
                }
            }
        }
    }
}
