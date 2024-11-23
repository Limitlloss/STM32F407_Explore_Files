/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP SOCKET TCPClient 实验
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


/* 设置远程IP地址 */
//#define DEST_IP_ADDR0               192
//#define DEST_IP_ADDR1               168
//#define DEST_IP_ADDR2                 1
//#define DEST_IP_ADDR3               167

/* 需要自己设置远程IP地址 */
#define IP_ADDR   "192.168.1.37"

#define LWIP_DEMO_RX_BUFSIZE         100                        /* 最大接收数据长度 */
#define LWIP_DEMO_PORT               8080                       /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 )    /* 发送数据线程优先级 */
/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 

/* 发送数据内容 */
uint8_t g_lwip_demo_sendbuf[] = "ALIENTEK DATA \r\n";
/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
int g_sock = -1;
int g_lwip_connect_state = 0;
static void lwip_send_thread(void *arg);

extern QueueHandle_t g_display_queue;     /* 显示消息队列句柄 */

/**
 * @brief       发送数据线程
 * @param       无
 * @retval      无
 */
void lwip_data_send(void)
{
    sys_thread_new("lwip_send_thread", lwip_send_thread, NULL, 512, LWIP_SEND_THREAD_PRIO );
}

/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    struct sockaddr_in atk_client_addr;
    err_t err;
    int recv_data_len;
    BaseType_t lwip_err;
    char *tbuf;
    
    lwip_data_send();                                           /* 创建发送数据线程 */
    
    while (1)
    {
sock_start:
        g_lwip_connect_state = 0;
        atk_client_addr.sin_family = AF_INET;                   /* 表示IPv4网络协议 */
        atk_client_addr.sin_port = htons(LWIP_DEMO_PORT);       /* 端口号 */
        atk_client_addr.sin_addr.s_addr = inet_addr(IP_ADDR);   /* 远程IP地址 */
        g_sock = socket(AF_INET, SOCK_STREAM, 0);                 /* 可靠数据流交付服务既是TCP协议 */
        memset(&(atk_client_addr.sin_zero), 0, sizeof(atk_client_addr.sin_zero));
        
        tbuf = mymalloc(SRAMIN, 200); /* 申请内存 */
        sprintf((char *)tbuf, "Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
        lcd_show_string(5, 150, 200, 16, 16, tbuf, BLUE);
        
        /* 连接远程IP地址 */
        err = connect(g_sock, (struct sockaddr *)&atk_client_addr, sizeof(struct sockaddr));
      
        if (err == -1)
        {
            printf("连接失败\r\n");
            g_sock = -1;
            closesocket(g_sock);
            myfree(SRAMIN, tbuf);
            vTaskDelay(10);
            goto sock_start;
        }

        printf("连接成功\r\n");
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
            
            /* 接收的数据 */ 
            lwip_err = xQueueSend(g_display_queue,&g_lwip_demo_recvbuf,0);
            
            if (lwip_err == errQUEUE_FULL)
            {
                printf("队列Key_Queue已满，数据发送失败!\r\n");
            }
            
            vTaskDelay(10);
        }
    }
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;
    
    err_t err;
    
    while (1)
    {
        while (1)
        {
            if(((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) && (g_lwip_connect_state == 1)) /* 有数据要发送 */
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
