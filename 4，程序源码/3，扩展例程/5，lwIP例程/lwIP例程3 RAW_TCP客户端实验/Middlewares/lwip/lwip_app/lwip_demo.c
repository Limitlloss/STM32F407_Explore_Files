/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-8-01
 * @brief       lwIP RAW TCPClient 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
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
char *g_lwip_demo_sendbuf = "ALIENTEK DATA";

/* TCP Client 测试全局状态标记变量
 * bit7:0,没有数据要发送;1,有数据要发送
 * bit6:0,没有收到数据;1,收到数据了.
 * bit5:0,没有连接上服务器;1,连接上服务器了.
 * bit4~0:保留 */
uint8_t g_lwip_send_flag;

/* tcp服务器连接状态 */
enum tcp_client_states
{
    ES_TCPCLIENT_NONE = 0,          /* 没有连接 */
    ES_TCPCLIENT_CONNECTED,         /* 连接到服务器了 */
    ES_TCPCLIENT_CLOSING,           /* 关闭连接 */
};

/* LWIP回调函数使用的结构体 */
struct tcp_client_struct
{
    uint8_t state;                  /* 当前连接状 */
    struct tcp_pcb *pcb;            /* 指向当前的pcb */
    struct pbuf *p;                 /* 指向接收/或传输的pbuf */
};  

void lwip_tcp_client_set_remoteip(void);
err_t lwip_tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t lwip_tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err);
err_t lwip_tcp_client_usersent(struct tcp_pcb *tpcb);
void lwip_tcp_client_error(void *arg,err_t err);
err_t lwip_tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
err_t lwip_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void lwip_tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es);
void lwip_tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es );

/**
 * @brief       设置远端IP地址
 * @param       无
 * @retval      无
 */
void lwip_tcp_client_set_remoteip(void)
{
    char *tbuf;
    uint16_t xoff;
    uint8_t key;
    lcd_clear(WHITE);
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCP Client Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "Remote IP Set", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:+  KEY2:-", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:OK", g_point_color);
    tbuf = mymalloc(SRAMIN, 100); /* 申请内存 */

    if (tbuf == NULL)return;

    /* 前三个IP保持和DHCP得到的IP一致 */
    g_lwipdev.remoteip[0] = g_lwipdev.ip[0];
    g_lwipdev.remoteip[1] = g_lwipdev.ip[1];
//    g_lwipdev.remoteip[2] = g_lwipdev.ip[2];
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2]); /* 远端IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    xoff = strlen((char *)tbuf) * 8 + 30;
    lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0, g_point_color);

    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)break;
        else if (key)
        {
            if (key == KEY0_PRES)g_lwipdev.remoteip[3]++; /* IP增加 */

            if (key == KEY2_PRES)g_lwipdev.remoteip[3]--; /* IP减少 */

            lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0X80, g_point_color); /* 显示新IP */
        }
    }

    myfree(SRAMIN, tbuf);
}

/**
 * @brief       lwip_demo程序入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    struct tcp_pcb *tcppcb;   /* 定义一个TCP客户端控制块 */
    ip_addr_t rmtipaddr;      /* 远端ip地址 */

    char *tbuf;
    uint8_t key;
    uint8_t res = 0;
    uint8_t t = 0;

    lwip_tcp_client_set_remoteip();  /* 先选择IP */
    lcd_clear(WHITE);                /* 清屏 */
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCP Client Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:Send data", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:Quit", g_point_color);
    tbuf = mymalloc(SRAMIN, 200); /* 申请内存 */

    if (tbuf == NULL)return ;     /* 内存申请失败了,直接退出 */

    sprintf((char *)tbuf, "Local IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* 服务器IP */
    lcd_show_string(5, 130, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.%d", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]); /* 远端IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote Port:%d", LWIP_DEMO_PORT); /* 客户端端口号 */
    lcd_show_string(5, 170, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
    tcppcb = tcp_new();  /* 创建一个新的pcb */

    if (tcppcb)           /* 创建成功 */
    {
        IP4_ADDR(&rmtipaddr, g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]);
        tcp_connect(tcppcb, &rmtipaddr, LWIP_DEMO_PORT, lwip_tcp_client_connected); /* 连接到目的地址的指定端口上,当连接成功后回调lwip_tcp_client_connected()函数 */
    }
    else res = 1;

    while (res == 0)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)break;

        if (key == KEY0_PRES)                       /* KEY0按下了,发送数据 */
        {
            lwip_tcp_client_usersent(tcppcb);       /* 发送数据 */
        }

        if (g_lwip_send_flag & 1 << 6)              /* 是否收到数据 */
        {
            lcd_fill(5, 230, lcddev.width - 1, lcddev.height - 1, WHITE);                                                       /* 清上一次数据 */
            lcd_show_string(5, 230, lcddev.width - 30, lcddev.height - 230, 16, (char *)g_lwip_demo_recvbuf, g_point_color);    /* 显示接收到的数据 */
            g_lwip_send_flag &= ~(1 << 6);          /* 标记数据已经被处理了 */
        }

        if (g_lwip_send_flag & 1 << 5)              /* 是否连接上 */
        {
            lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "STATUS:Connected   ", g_point_color);          /* 提示消息 */
            g_point_color = RED;
            lcd_show_string(5, 210, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", g_point_color);                /* 提示消息 */
            g_point_color = BLUE;
        }
        else if ((g_lwip_send_flag & 1 << 5) == 0)
        {
            lcd_show_string(5, 190, 190, 16, 16, "STATUS:Disconnected", g_point_color);
            lcd_fill(5, 210, lcddev.width - 1, lcddev.height - 1, WHITE);       /* 清屏 */
        }

        lwip_periodic_handle();                                                 /* LWIP轮询任务 */
        
        delay_ms(2);
        t++;

        if (t == 200)
        {
            if ((g_lwip_send_flag & 1 << 5) == 0)                               /* 未连接上,则尝试重连 */
            {
                lwip_tcp_client_connection_close(tcppcb, 0);                    /* 关闭连接 */
                tcppcb = tcp_new();                                             /* 创建一个新的pcb */

                if (tcppcb)                                                     /* 创建成功 */
                {
                    tcp_connect(tcppcb, &rmtipaddr, LWIP_DEMO_PORT, lwip_tcp_client_connected); /* 连接到目的地址的指定端口上,当连接成功后回调lwip_tcp_client_connected()函数 */
                }
            }

            t = 0;
            LED0_TOGGLE();
        }
    }
    
    lwip_tcp_client_connection_close(tcppcb, 0);    /* 关闭TCP Client连接 */
    lcd_clear(WHITE);                               /* 清屏 */
    g_point_color = DARKBLUE;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCPclient Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
    myfree(SRAMIN, tbuf);
}

/**
 * @brief       lwIP TCP连接建立后调用回调函数
 * @param       arg   : 回调函数传入的参数
 * @param       tpcb  : TCP控制块
 * @param       err   : 错误码
 * @retval      返回错误码
 */
err_t lwip_tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    struct tcp_client_struct *es = NULL;

    if (err == ERR_OK)
    {
        es = (struct tcp_client_struct *)mem_malloc(sizeof(struct tcp_client_struct)); /* 申请内存 */

        if (es) /* 内存申请成功 */
        {
            es->state = ES_TCPCLIENT_CONNECTED;         /* 状态为连接成功 */
            es->pcb = tpcb;
            es->p = NULL;
            tcp_arg(tpcb, es);                          /* 使用es更新tpcb的callback_arg */
            tcp_recv(tpcb, lwip_tcp_client_recv);       /* 初始化LwIP的tcp_recv回调功能 */
            tcp_err(tpcb, lwip_tcp_client_error);       /* 初始化tcp_err()回调函数 */
            tcp_sent(tpcb, lwip_tcp_client_sent);       /* 初始化LwIP的tcp_sent回调功能 */
            tcp_poll(tpcb, lwip_tcp_client_poll, 1);    /* 初始化LwIP的tcp_poll回调功能 */
            g_lwip_send_flag |= 1 << 5;                 /* 标记连接到服务器了 */
            err = ERR_OK;
        }
        else
        {
            lwip_tcp_client_connection_close(tpcb, es); /* 关闭连接 */
            err = ERR_MEM;                              /* 返回内存分配错误 */
        }
    }
    else
    {
        lwip_tcp_client_connection_close(tpcb, 0);      /* 关闭连接 */
    }

    return err;
}

/**
 * @brief       lwIP tcp_recv()函数的回调函数
 * @param       arg   : 回调函数传入的参数
 * @param       tpcb  : TCP控制块
 * @param       p     : 网络数据包
 * @param       err   : 错误码
 * @retval      返回错误码
 */
err_t lwip_tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    uint32_t data_len = 0;
    struct pbuf *q;
    struct tcp_client_struct *es;
    err_t ret_err;
    LWIP_ASSERT("arg != NULL", arg != NULL);
    es = (struct tcp_client_struct *)arg;

    if (p == NULL)                                                  /* 如果从服务器接收到空的数据帧就关闭连接 */
    {
        es->state = ES_TCPCLIENT_CLOSING;                           /* 需要关闭TCP 连接了 */
        es->p = p;
        ret_err = ERR_OK;
    }
    else if (err != ERR_OK)                                         /* 当接收到一个非空的数据帧,但是err!=ERR_OK */
    {
        if (p)pbuf_free(p);                                         /* 释放接收pbuf */

        ret_err = err;
    }
    else if (es->state == ES_TCPCLIENT_CONNECTED)                   /* 当处于连接状态时 */
    {
        if (p != NULL)                                              /* 当处于连接状态并且接收到的数据不为空时 */
        {
            memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE);   /* 数据接收缓冲区清零 */

            for (q = p; q != NULL; q = q->next)                     /* 遍历完整个pbuf链表 */
            {
                /* 判断要拷贝到LWIP_DEMO_RX_BUFSIZE中的数据是否大于LWIP_DEMO_RX_BUFSIZE的剩余空间，如果大于 */
                /* 的话就只拷贝LWIP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据 */
                if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* 拷贝数据 */
                else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

                data_len += q->len;

                if (data_len > LWIP_DEMO_RX_BUFSIZE) break; /* 超出TCP客户端接收数组,跳出 */
            }

            g_lwip_send_flag |= 1 << 6;                     /* 标记接收到数据了 */
            tcp_recved(tpcb, p->tot_len);                   /* 用于获取接收数据,通知LWIP可以获取更多数据 */
            pbuf_free(p);                                   /* 释放内存 */
            ret_err = ERR_OK;
        }
    }
    else                                                    /* 接收到数据但是连接已经关闭 */
    {
        tcp_recved(tpcb, p->tot_len);                       /* 用于获取接收数据,通知LWIP可以获取更多数据 */
        es->p = NULL;
        pbuf_free(p);                                       /* 释放内存 */
        ret_err = ERR_OK;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_err函数的回调函数
 * @param       arg   : 回调函数传入的参数
 * @retval      无
 */
void lwip_tcp_client_error(void *arg, err_t err)
{
    /* 这里我们不做任何处理 */
}

/**
 * @brief       LWIP数据发送，用户应用程序调用此函数来发送数据
 * @param       tpcb: TCP控制块
 * @retval      返回值:0，成功；其他，失败
 */
err_t lwip_tcp_client_usersent(struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_client_struct *es;
    es = tpcb->callback_arg;

    if (es != NULL) /* 连接处于空闲可以发送数据 */
    {
        es->p = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)g_lwip_demo_sendbuf), PBUF_POOL); /* 申请内存 */
        pbuf_take(es->p, (char *)g_lwip_demo_sendbuf, strlen((char *)g_lwip_demo_sendbuf)); /* 将tcp_client_sentbuf[]中的数据拷贝到es->p_tx中 */
        lwip_tcp_client_senddata(tpcb, es);                                                 /* 将tcp_client_sentbuf[]里面复制给pbuf的数据发送出去 */
        g_lwip_send_flag &= ~(1 << 7);                                                      /* 清除数据发送标志 */

        if (es->p)pbuf_free(es->p);                                                         /* 释放内存 */

        ret_err = ERR_OK;
    }
    else
    {
        tcp_abort(tpcb);                                                                    /* 终止连接,删除pcb控制块 */
        ret_err = ERR_ABRT;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_poll的回调函数
 * @param       arg : 回调函数传入的参数
 * @param       tpcb: TCP控制块
 * @retval      ERR_OK
 */
err_t lwip_tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_client_struct *es;
    es = (struct tcp_client_struct *)arg;

    if (es->state == ES_TCPCLIENT_CLOSING)              /* 连接断开 */
    {
        lwip_tcp_client_connection_close(tpcb, es);     /* 关闭TCP连接 */
    }

    ret_err = ERR_OK;
    return ret_err;
}

/**
 * @brief       lwIP tcp_sent的回调函数(当从远端主机接收到ACK信号后发送数据)
 * @param       arg : 回调函数传入的参数
 * @param       tpcb: TCP控制块
 * @param       len : 长度
 * @retval      ERR_OK
 */
err_t lwip_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_client_struct *es;
    LWIP_UNUSED_ARG(len);
    es = (struct tcp_client_struct *)arg;

    if (es->p)lwip_tcp_client_senddata(tpcb, es); /* 发送数据 */

    return ERR_OK;
}

/**
 * @brief       用来发送数据
 * @param       tpcb: TCP控制块
 * @param       es  : LWIP回调函数使用的结构体
 * @retval      无
 */
void lwip_tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && es->p && (es->p->len <= tcp_sndbuf(tpcb))) /* 将要发送的数据加入到发送缓冲队列中 */
    {
        ptr = es->p;
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK)
        {
            es->p = ptr->next;             /* 指向下一个pbuf */

            if (es->p)pbuf_ref(es->p);     /* pbuf的ref加一 */

            pbuf_free(ptr);                /* 释放ptr */
        }
        else if (wr_err == ERR_MEM)es->p = ptr;

        tcp_output(tpcb);                  /* 将发送缓冲队列中的数据立即发送出去 */
    }
}

/**
 * @brief       关闭与服务器的连接
 * @param       tpcb: TCP控制块
 * @param       es  : LWIP回调函数使用的结构体
 * @retval      无
 */
void lwip_tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
    /* 移除回调 */
    tcp_abort(tpcb);                        /* 终止连接,删除pcb控制块 */
    tcp_arg(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es)mem_free(es);

    g_lwip_send_flag &= ~(1 << 5);          /* 标记连接断开了 */
}
