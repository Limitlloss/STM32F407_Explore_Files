/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP Netconn TCPClient 实验
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
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include <lwip/sockets.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip_demo.h"


/* 这个必须填写正确，远程IP地址 */
#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2                 1
#define DEST_IP_ADDR3               111

#define LWIP_DEMO_RX_BUFSIZE         2000  /* 最大接收数据长度 */
#define LWIP_DEMO_PORT               8080  /* 连接的本地端口号 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* 发送数据内容 */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA\r\n";
/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
extern QueueHandle_t g_display_queue;     /* 显示消息队列句柄 */

/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    static struct netconn *tcp_clientconn = NULL; /* TCP CLIENT网络连接结构体 */
    uint32_t data_len = 0;
    struct pbuf *q;
    err_t err,recv_err;
    ip4_addr_t server_ipaddr,loca_ipaddr;
    static uint16_t server_port,loca_port;
    BaseType_t lwip_err;
    char *tbuf;
    
    server_port = LWIP_DEMO_PORT;
    IP4_ADDR(&server_ipaddr,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);   /* 构造目的IP地址 */
    
    tbuf = mymalloc(SRAMIN, 200); /* 申请内存 */
    sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
    lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
    
    while (1) 
    {
        tcp_clientconn = netconn_new(NETCONN_TCP);                                      /* 创建一个TCP链接 */
        err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);               /* 连接服务器 */
      
        if (err != ERR_OK)
        {
            printf("接连失败\r\n");
            netconn_delete(tcp_clientconn);                                             /* 返回值不等于ERR_OK,删除tcp_clientconn连接 */
        }
        else if (err == ERR_OK)                                                         /* 处理新连接的数据 */
        { 
            struct netbuf *recvbuf;
            tcp_clientconn->recv_timeout = 10;
            netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1);                  /* 获取本地IP主机IP地址和端口号 */
            printf("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",DEST_IP_ADDR0,DEST_IP_ADDR1, DEST_IP_ADDR2,DEST_IP_ADDR3,loca_port);
            lcd_show_string(5, 90, 200, 16, 16, "State:Connection Successful", BLUE);
            
            while (1)
            {
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA)                 /* 有数据要发送 */
                {
                    err = netconn_write(tcp_clientconn ,g_lwip_demo_sendbuf,strlen((char*)g_lwip_demo_sendbuf),NETCONN_COPY); /* 发送tcp_server_sentbuf中的数据 */
                  
                    if (err != ERR_OK)
                    {
                        printf("发送失败\r\n");
                    }
                    
                    g_lwip_send_flag &= ~LWIP_SEND_DATA;
                }
                  
                if ((recv_err = netconn_recv(tcp_clientconn,&recvbuf)) == ERR_OK)            /* 接收到数据 */
                {
                    taskENTER_CRITICAL();                                                    /* 进入临界区 */
                    memset(g_lwip_demo_recvbuf,0,LWIP_DEMO_RX_BUFSIZE);                      /* 数据接收缓冲区清零 */

                    for (q = recvbuf->p;q != NULL;q = q->next)                               /* 遍历完整个pbuf链表 */
                    {
                        /* 判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于 */
                        /* 的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
                        if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) 
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,(LWIP_DEMO_RX_BUFSIZE - data_len));/* 拷贝数据 */
                        }
                        else 
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,q->len);
                        }
                        
                        data_len += q->len;
                        
                        if (data_len > LWIP_DEMO_RX_BUFSIZE) 
                        {
                            break;                  /* 超出TCP客户端接收数组,跳出 */
                        }
                    }
                    
                    taskEXIT_CRITICAL();            /* 退出临界区 */
                    data_len = 0;                   /* 复制完成后data_len要清零 */
                    
                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("队列Key_Queue已满，数据发送失败!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);
                }
                else if (recv_err == ERR_CLSD)       /* 关闭连接 */
                {
                    netconn_close(tcp_clientconn);
                    netconn_delete(tcp_clientconn);
                    printf("服务器%d.%d.%d.%d断开连接\r\n",DEST_IP_ADDR0,DEST_IP_ADDR1, DEST_IP_ADDR2,DEST_IP_ADDR3);
                    lcd_fill(5, 89, lcddev.width,110, WHITE);
                    lcd_show_string(5, 90, 200, 16, 16, "State:Disconnect", BLUE);
                    myfree(SRAMIN, tbuf);
                    break;
                }
            }
        }
    }
}
