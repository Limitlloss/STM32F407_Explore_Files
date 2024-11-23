/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP Netconn TCPClient ʵ��
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


/* ���������д��ȷ��Զ��IP��ַ */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 1
#define DEST_IP_ADDR3               111

#define LWIP_DEMO_RX_BUFSIZE         2000  /* ���������ݳ��� */
#define LWIP_DEMO_PORT               8080  /* ���ӵı��ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA\r\n";
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
    static struct netconn *tcp_clientconn = NULL; /* TCP CLIENT�������ӽṹ�� */
    uint32_t data_len = 0;
    struct pbuf *q;
    err_t err,recv_err;
    ip4_addr_t server_ipaddr,loca_ipaddr;
    static uint16_t server_port,loca_port;
    BaseType_t lwip_err;
    char *tbuf;
    
    server_port = LWIP_DEMO_PORT;
    IP4_ADDR(&server_ipaddr,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);   /* ����Ŀ��IP��ַ */
    
    tbuf = mymalloc(SRAMIN, 200); /* �����ڴ� */
    sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* �ͻ��˶˿ں� */
    lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
    
    while (1) 
    {
        tcp_clientconn = netconn_new(NETCONN_TCP);                                      /* ����һ��TCP���� */
        err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);               /* ���ӷ����� */
      
        if (err != ERR_OK)
        {
            printf("����ʧ��\r\n");
            netconn_delete(tcp_clientconn);                                             /* ����ֵ������ERR_OK,ɾ��tcp_clientconn���� */
        }
        else if (err == ERR_OK)                                                         /* ���������ӵ����� */
        { 
            struct netbuf *recvbuf;
            tcp_clientconn->recv_timeout = 10;
            netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1);                  /* ��ȡ����IP����IP��ַ�Ͷ˿ں� */
            printf("�����Ϸ�����%d.%d.%d.%d,�����˿ں�Ϊ:%d\r\n",DEST_IP_ADDR0,DEST_IP_ADDR1, DEST_IP_ADDR2,DEST_IP_ADDR3,loca_port);
            lcd_show_string(5, 90, 200, 16, 16, "State:Connection Successful", BLUE);
            
            while (1)
            {
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA)                 /* ������Ҫ���� */
                {
                    err = netconn_write(tcp_clientconn ,g_lwip_demo_sendbuf,strlen((char*)g_lwip_demo_sendbuf),NETCONN_COPY); /* ����tcp_server_sentbuf�е����� */
                  
                    if (err != ERR_OK)
                    {
                        printf("����ʧ��\r\n");
                    }
                    
                    g_lwip_send_flag &= ~LWIP_SEND_DATA;
                }
                  
                if ((recv_err = netconn_recv(tcp_clientconn,&recvbuf)) == ERR_OK)            /* ���յ����� */
                {
                    taskENTER_CRITICAL();                                                    /* �����ٽ��� */
                    memset(g_lwip_demo_recvbuf,0,LWIP_DEMO_RX_BUFSIZE);                      /* ���ݽ��ջ��������� */

                    for (q = recvbuf->p;q != NULL;q = q->next)                               /* ����������pbuf���� */
                    {
                        /* �ж�Ҫ������TCP_CLIENT_RX_BUFSIZE�е������Ƿ����TCP_CLIENT_RX_BUFSIZE��ʣ��ռ䣬������� */
                        /* �Ļ���ֻ����TCP_CLIENT_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                        if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) 
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,(LWIP_DEMO_RX_BUFSIZE - data_len));/* �������� */
                        }
                        else 
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,q->len);
                        }
                        
                        data_len += q->len;
                        
                        if (data_len > LWIP_DEMO_RX_BUFSIZE) 
                        {
                            break;                  /* ����TCP�ͻ��˽�������,���� */
                        }
                    }
                    
                    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
                    data_len = 0;                   /* ������ɺ�data_lenҪ���� */
                    
                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("����Key_Queue���������ݷ���ʧ��!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);
                }
                else if (recv_err == ERR_CLSD)       /* �ر����� */
                {
                    netconn_close(tcp_clientconn);
                    netconn_delete(tcp_clientconn);
                    printf("������%d.%d.%d.%d�Ͽ�����\r\n",DEST_IP_ADDR0,DEST_IP_ADDR1, DEST_IP_ADDR2,DEST_IP_ADDR3);
                    lcd_fill(5, 89, lcddev.width,110, WHITE);
                    lcd_show_string(5, 90, 200, 16, 16, "State:Disconnect", BLUE);
                    myfree(SRAMIN, tbuf);
                    break;
                }
            }
        }
    }
}
