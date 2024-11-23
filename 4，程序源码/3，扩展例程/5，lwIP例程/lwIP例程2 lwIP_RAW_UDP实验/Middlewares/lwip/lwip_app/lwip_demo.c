/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP RAW接口UDP实验
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
 
#include <stdint.h>
#include <stdio.h>
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip_demo.h"
#include "lwip_comm.h"
#include "stdio.h"
#include "string.h"


#define LWIP_DEMO_RX_BUFSIZE         2000  /* 定义udp最大接收数据长度 */
#define LWIP_DEMO_PORT               8080  /* 定义udp连接的本地端口号 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* 发送数据内容 */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA\r\n";
/* 数据发送标志位 */
uint8_t g_lwip_send_flag;

static void lwip_udp_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void lwip_udp_senddata(struct udp_pcb *upcb);
void lwip_udp_connection_close(struct udp_pcb *upcb);

/**
 * @brief       设置远端IP地址
 * @param       无
 * @retval      无
 */
void lwip_udp_set_remoteip(void)
{
    char *tbuf;
    uint16_t xoff;
    uint8_t key;
    lcd_clear(WHITE);
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "UDP Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "Remote IP Set", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:+  KEY2:-", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:OK", g_point_color);
    tbuf = mymalloc(SRAMIN, 100); /* 申请内存 */

    if (tbuf == NULL)return;

    /* 前三个IP保持和DHCP得到的IP一致 */
    g_lwipdev.remoteip[0] = g_lwipdev.ip[0];
    g_lwipdev.remoteip[1] = g_lwipdev.ip[1];
    g_lwipdev.remoteip[2] = g_lwipdev.ip[2];
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2]); /* 远端IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    xoff = strlen((char *)tbuf) * 8 + 5;
    lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0, g_point_color);

    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)break;
        else if (key)
        {
            if (key == KEY0_PRES)g_lwipdev.remoteip[3] ++; /* IP增加 */

            if (key == KEY2_PRES)g_lwipdev.remoteip[3] --; /* IP减少 */

            lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0X80, g_point_color); /* 显示新IP */
        }
    }

    myfree(SRAMIN, tbuf);
}

/**
 * @brief       UDP服务器回调函数
 * @param       arg ：传入参数
 * @param       upcb：UDP控制块
 * @param       p   : 网络数据包
 * @param       addr：IP地址
 * @param       port：端口号
 * @retval      无
 */
static void lwip_udp_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t data_len = 0;
    struct pbuf *q;

    if (p != NULL) /* 接收到不为空的数据时 */
    {
        memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE); /* 数据接收缓冲区清零 */

        for (q = p; q != NULL; q = q->next) /* 遍历完整个pbuf链表 */
        {
            /* 判断要拷贝到LWIP_DEMO_RX_BUFSIZE中的数据是否大于LWIP_DEMO_RX_BUFSIZE的剩余空间，如果大于 */
            /* 的话就只拷贝LWIP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
            if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* 拷贝数据 */
            else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

            data_len += q->len;

            if (data_len > LWIP_DEMO_RX_BUFSIZE) break; /* 超出UDP客户端接收数组,跳出 */
        }

        upcb->remote_ip = *addr;                                        /* 记录远程主机的IP地址 */
        upcb->remote_port = port;                                       /* 记录远程主机的端口号 */
        g_lwipdev.remoteip[0] = upcb->remote_ip.addr & 0xff;              /* IADDR4 */
        g_lwipdev.remoteip[1] = (upcb->remote_ip.addr >> 8) & 0xff;       /* IADDR3 */
        g_lwipdev.remoteip[2] = (upcb->remote_ip.addr >> 16) & 0xff;      /* IADDR2 */
        g_lwipdev.remoteip[3] = (upcb->remote_ip.addr >> 24) & 0xff;      /* IADDR1 */
        g_lwip_send_flag |= 1 << 6;                                       /* 标记接收到数据了 */
        pbuf_free(p);                                                   /* 释放内存 */
    }
    else
    {
        udp_disconnect(upcb);
        lcd_clear(WHITE);           /* 清屏 */
        lcd_show_string(30, 30, 200, 16, 16, "STM32", g_point_color);
        lcd_show_string(30, 50, 200, 16, 16, "UDP Test", g_point_color);
        lcd_show_string(30, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
        lcd_show_string(30, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
        lcd_show_string(30, 190, 210, 16, 16, "Connect break!", g_point_color);
    }
}

/**
 * @brief       lwip_democ程序入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    err_t err;
    struct udp_pcb *udppcb;      /* 定义一个UDP服务器控制块 */
    ip_addr_t rmtipaddr;         /* 远端ip地址 */

    char *tbuf;
    uint8_t key;
    uint8_t res = 0;
    uint8_t t = 0;

    lwip_udp_set_remoteip();/* 先选择IP */
    lcd_clear(WHITE);    /* 清屏 */
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "UDP Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:Send data", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:Exit", g_point_color);
    tbuf = mymalloc(SRAMIN, 200); /* 申请内存 */

    if (tbuf == NULL)return ;     /* 内存申请失败了,直接退出 */

    sprintf((char *)tbuf, "Local IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* 服务器IP */
    lcd_show_string(5, 130, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.%d", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]); /* 远端IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
    lcd_show_string(5, 170, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    udppcb = udp_new();

    if (udppcb) /* 创建成功 */
    {
        IP4_ADDR(&rmtipaddr, g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]);
        err = udp_connect(udppcb, &rmtipaddr, LWIP_DEMO_PORT);      /* UDP客户端连接到指定IP地址和端口号的服务器 */

        if (err == ERR_OK)
        {
            err = udp_bind(udppcb, IP_ADDR_ANY, LWIP_DEMO_PORT);    /* 绑定本地IP地址与端口号 */

            if (err == ERR_OK)  /* 绑定完成 */
            {
                udp_recv(udppcb,lwip_udp_callback, NULL);           /* 注册接收回调函数 */
                g_point_color = BLUE;
                lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", g_point_color); /* 提示消息 */
                g_point_color = MAGENTA;
            }
            else res = 1;
        }
        else res = 1;
    }
    else res = 1;

    while (res == 0)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)break;

        if (key == KEY0_PRES) /* KEY0按下了,发送数据 */
        {
            lwip_udp_senddata(udppcb);
        }

        if (g_lwip_send_flag & 1 << 6) /* 是否收到数据 */
        {
            lcd_fill(5, 230, lcddev.width - 1, lcddev.height - 1, WHITE); /* 清上一次数据 */
            /* 显示接收到的数据 */
            lcd_show_string(6, 230, lcddev.width - 2, lcddev.height - 230, 16, (char *)g_lwip_demo_recvbuf, g_point_color); 
            g_lwip_send_flag &= ~(1 << 6); /* 标记数据已经被处理了 */
            
        }

        lwip_periodic_handle(); /* LWIP轮询任务 */
        delay_ms(2);
        t++;

        if (t == 200)
        {
            t = 0;
            LED0_TOGGLE();
        }
    }

    g_point_color = DARKBLUE;
    lwip_udp_connection_close(udppcb);
    myfree(SRAMIN, tbuf);
}

/**
 * @brief       UDP服务器发送数据
 * @param       upcb: UDP控制块
 * @retval      无
 */
void lwip_udp_senddata(struct udp_pcb *upcb)
{
    struct pbuf *ptr;
    ptr = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)g_lwip_demo_sendbuf), PBUF_POOL); /* 申请内存 */

    if (ptr)
    {
        pbuf_take(ptr, (char *)g_lwip_demo_sendbuf, strlen((char *)g_lwip_demo_sendbuf)); /* 将g_lwip_demo_sendbuf中的数据打包进pbuf结构中 */
        udp_send(upcb, ptr);    /* udp发送数据 */
        pbuf_free(ptr);         /* 释放内存 */
    }
}

/**
 * @brief       关闭tcp连接
 * @param       upcb: UDP控制块
 * @retval      无
 */
void lwip_udp_connection_close(struct udp_pcb *upcb)
{
    udp_disconnect(upcb);
    udp_remove(upcb);               /* 断开UDP连接 */
    g_lwip_send_flag &= ~(1 << 5);    /* 标记连接断开 */
    lcd_clear(WHITE);               /* 清屏 */
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "UDP Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
}
