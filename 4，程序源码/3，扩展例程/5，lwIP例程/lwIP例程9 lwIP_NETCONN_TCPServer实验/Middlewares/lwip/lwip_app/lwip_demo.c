/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP Netconn TCPServer 实验
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


/* TCPServer 不需要设置远程IP地址 */
//#define DEST_IP_ADDR0               192
//#define DEST_IP_ADDR1               168
//#define DEST_IP_ADDR2                 1
//#define DEST_IP_ADDR3               167

#define LWIP_DEMO_RX_BUFSIZE         2000  /* 最大接收数据长度 */
#define LWIP_DEMO_PORT               8080  /* 连接的本地端口号 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* 发送数据内容 */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA \r\n";
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
    static struct netconn *tcp_serverconn = NULL; /* TCP SERVER网络连接结构体 */
    uint32_t  data_len = 0;
    struct    pbuf *q;
    err_t     err,recv_err;
    uint8_t   remot_addr[4];
    struct netconn *newconn;
    static    ip_addr_t ipaddr;
    static    u16_t  port;
    BaseType_t lwip_err;
    char *tbuf;
    
    tbuf = mymalloc(SRAMIN, 200); /* 申请内存 */
    sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
    lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
    
    /* 第一步：创建一个TCP控制块 */
    tcp_serverconn = netconn_new(NETCONN_TCP);                      /* 创建一个TCP链接 */
    /* 第二步：绑定TCP控制块、本地IP地址和端口号 */
    netconn_bind(tcp_serverconn,IP_ADDR_ANY,LWIP_DEMO_PORT);        /* 绑定端口 8080号端口 */
    /* 第三步：监听 */
    netconn_listen(tcp_serverconn);                                 /* 进入监听模式 */
    tcp_serverconn->recv_timeout = 10;                              /* 禁止阻塞线程 等待10ms */
    
    while (1) 
    {
        /* 第四步：接收连接请求 */
        err = netconn_accept(tcp_serverconn,&newconn);              /* 接收连接请求 */
        if (err == ERR_OK) newconn->recv_timeout = 10;

        if (err == ERR_OK)                                          /* 处理新连接的数据 */
        { 
            struct netbuf *recvbuf;
            netconn_getaddr(newconn,&ipaddr,&port,0);               /* 获取远端IP地址和端口号 */
            
            remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
            remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
            remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
            remot_addr[0] = (uint8_t)(ipaddr.addr);
            printf("主机%d.%d.%d.%d连接上服务器,主机端口号为:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
            lcd_show_string(5, 90, 200, 16, 16, "State:Connection Successful", BLUE);
            
            while (1)
            {
                if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) /* 有数据要发送 */
                {
                    err = netconn_write(newconn ,g_lwip_demo_sendbuf,strlen((char*)g_lwip_demo_sendbuf),NETCONN_COPY); /* 发送g_lwip_demo_sendbuf中的数据 */

                    if(err != ERR_OK)
                    {
                        printf("发送失败\r\n");
                    }

                    g_lwip_send_flag &= ~LWIP_SEND_DATA;
                }
                
                if ((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)           /* 接收到数据 */
                { 
                    taskENTER_CRITICAL();                                           /* 进入临界区 */
                    memset(g_lwip_demo_recvbuf,0,LWIP_DEMO_RX_BUFSIZE);               /* 数据接收缓冲区清零 */

                    for (q = recvbuf->p;q != NULL;q = q->next)                       /* 遍历完整个pbuf链表 */
                    {
                        /* 判断要拷贝到LWIP_DEMO_RX_BUFSIZE中的数据是否大于LWIP_DEMO_RX_BUFSIZE的剩余空间，如果大于 */
                        /* 的话就只拷贝LWIP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
                        if(q->len > (LWIP_DEMO_RX_BUFSIZE-data_len))
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,(LWIP_DEMO_RX_BUFSIZE - data_len));/* 拷贝数据 */
                        }
                        else
                        {
                            memcpy(g_lwip_demo_recvbuf + data_len,q->payload,q->len);
                        }
                        
                        data_len += q->len;

                        if(data_len > LWIP_DEMO_RX_BUFSIZE)
                        {
                            break;   /*超出TCP客户端接收数组,跳出*/
                        }
                    }

                    taskEXIT_CRITICAL();                                /* 退出临界区 */
                    data_len = 0;                                       /* 复制完成后data_len要清零 */
                    
                    lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
                    
                    if (lwip_err == errQUEUE_FULL)
                    {
                        printf("队列Key_Queue已满，数据发送失败!\r\n");
                    }
                    
                    netbuf_delete(recvbuf);
                }
                else if (recv_err == ERR_CLSD)                           /* 关闭连接 */
                {
                    netconn_close(newconn);
                    netconn_delete(newconn);
                    printf("主机:%d.%d.%d.%d断开与服务器的连接\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
                    lcd_fill(5, 89, lcddev.width,110, WHITE);
                    lcd_show_string(5, 90, 200, 16, 16, "State:Disconnect", BLUE);
                    myfree(SRAMIN, tbuf);
                    break;
                }
            }
        }
    }
}
