/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-8-01
 * @brief       lwIP RAW TCPServer ʵ��
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


#define LWIP_DEMO_RX_BUFSIZE         2000   /* �������������ݳ��� */
#define LWIP_DEMO_PORT               8080   /* �������ӵ�Ŀ��/���ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE];  
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA\r\n";

/* TCP Server ����ȫ��״̬��Ǳ���
 * bit7:0,û������Ҫ����;1,������Ҫ����
 * bit6:0,û���յ�����;1,�յ�������.
 * bit5:0,û�пͻ���������;1,�пͻ�����������.
 * bit4~0:���� */
uint8_t g_lwip_send_flag;

/* tcp����������״̬ */
enum tcp_server_states
{
    ES_TCPSERVER_NONE = 0,                  /* ��ʼ�� */
    ES_TCPSERVER_ACCEPTED,                  /* ����״̬ */
    ES_TCPSERVER_CLOSING,                   /* �ر�����״̬ */
};

/* LWIP�ص�����ʹ�õĽṹ�� */
struct tcp_server_struct
{
    uint8_t state;                          /* ��ǰ״̬ */
    struct tcp_pcb *pcb;                    /* ָ��TCP���ƿ� */
    struct pbuf *p;                         /* ָ�����/�����pbuf */
};

err_t lwip_tcp_server_accept(void *arg,struct tcp_pcb *newpcb,err_t err);
err_t lwip_tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void lwip_tcp_server_error(void *arg,err_t err);
err_t lwip_tcp_server_usersent(struct tcp_pcb *tpcb);
err_t lwip_tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
err_t lwip_tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void lwip_tcp_server_senddata(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
void lwip_tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
void lwip_tcp_server_remove_timewait(void);

/**
 * @brief       lwip_demo ����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    err_t err;
    struct tcp_pcb *tcppcbnew;          /* ����һ��TCP���������ƿ� */
    struct tcp_pcb *tcppcbconn;         /* ����һ��TCP���������ƿ� */

    char *tbuf;
    uint8_t key;
    uint8_t res = 0;
    uint8_t t = 0;

    lcd_clear(WHITE);                   /* ���� */
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCP Server Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:Send data", g_point_color);
    tbuf = mymalloc(SRAMIN, 200);       /* �����ڴ� */

    if (tbuf == NULL)return ;           /* �ڴ�����ʧ����,ֱ���˳� */

    sprintf((char *)tbuf, "Server IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* ������IP */
    lcd_show_string(5, 130, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Server Port:%d", LWIP_DEMO_PORT);                                                            /* �������˿ں� */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    tcppcbnew = tcp_new();              /* ����һ���µ�pcb */

    if (tcppcbnew)                      /* �����ɹ� */
    {
        err = tcp_bind(tcppcbnew, IP_ADDR_ANY, LWIP_DEMO_PORT); /* ������IP��ָ���Ķ˿ںŰ���һ��,IP_ADDR_ANYΪ�󶨱������е�IP��ַ */

        if (err == ERR_OK)              /* ����� */
        {
            tcppcbconn = tcp_listen(tcppcbnew);                 /* ����tcppcb�������״̬ */
            tcp_accept(tcppcbconn, lwip_tcp_server_accept);     /* ��ʼ��LWIP��tcp_accept�Ļص����� */
        }
        else res = 1;
    }
    else res = 1;

    g_point_color = BLUE;

    while (res == 0)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)                                   /* KEY0������,�������� */
        {
            lwip_tcp_server_usersent(tcppcbnew);                /* �������� */
        }

        if (g_lwip_send_flag & 1 << 6)                          /* �Ƿ��յ����� */
        {
            lcd_fill(5, 210, lcddev.width - 1, lcddev.height - 1, WHITE);                                                       /* ����һ������ */
            lcd_show_string(5, 210, lcddev.width - 30, lcddev.height - 210, 16, (char *)g_lwip_demo_recvbuf, g_point_color);    /* ��ʾ���յ������� */
            g_lwip_send_flag &= ~(1 << 6);                      /* ��������Ѿ��������� */
        }

        if (g_lwip_send_flag & 1 << 5)                          /* �Ƿ������� */
        {
            sprintf((char *)tbuf, "Client IP:%d.%d.%d.%d", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]); /* �ͻ���IP */
            lcd_show_string(5, 170, 230, 16, 16, tbuf, g_point_color);
            g_point_color = RED;
            lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", g_point_color); /* ��ʾ��Ϣ */
            g_point_color = BLUE;
        }
        else if (g_lwip_send_flag & 1 << 5)
        {
            lcd_fill(5, 170, lcddev.width - 1, lcddev.height - 1, WHITE);   /* ���� */
        }

        lwip_periodic_handle();
        delay_ms(2);
        t++;

        if (t == 200)
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}

/**
 * @brief       lwIP tcp_accept()�Ļص�����
 * @param       arg ������Ĳ���
 * @param       newpcb��TCP���ƿ�
 * @param       err ��������
 * @retval      ���� ret_err
 */
err_t lwip_tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    err_t ret_err;
    struct tcp_server_struct *es;
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    tcp_setprio(newpcb, TCP_PRIO_MIN);                                  /* �����´�����pcb���ȼ� */
    es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct)); /* �����ڴ� */

    if (es != NULL)                                                     /* �ڴ����ɹ� */
    {
        es->state = ES_TCPSERVER_ACCEPTED;                              /* �������� */
        es->pcb = newpcb;
        es->p = NULL;
        
        tcp_arg(newpcb, es);
        tcp_recv(newpcb, lwip_tcp_server_recv);                         /* ��ʼ��tcp_recv()�Ļص����� */
        tcp_err(newpcb, lwip_tcp_server_error);                         /* ��ʼ��tcp_err()�ص����� */
        tcp_poll(newpcb, lwip_tcp_server_poll, 1);                      /* ��ʼ��tcp_poll�ص����� */
        tcp_sent(newpcb, lwip_tcp_server_sent);                         /* ��ʼ�����ͻص����� */

        g_lwip_send_flag |= 1 << 5;                                     /* ����пͻ��������� */
        g_lwipdev.remoteip[0] = newpcb->remote_ip.addr & 0xff;          /* IADDR4 */
        g_lwipdev.remoteip[1] = (newpcb->remote_ip.addr >> 8) & 0xff;   /* IADDR3 */
        g_lwipdev.remoteip[2] = (newpcb->remote_ip.addr >> 16) & 0xff;  /* IADDR2 */
        g_lwipdev.remoteip[3] = (newpcb->remote_ip.addr >> 24) & 0xff;  /* IADDR1 */
        ret_err = ERR_OK;
    }
    else
    {
        ret_err = ERR_MEM;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_recv()�����Ļص�����
 * @param       arg ������Ĳ���
 * @param       tpcb��TCP���ƿ�
 * @param       p   ���������ݰ�
 * @param       err ��������
 * @retval      ���� ret_err
 */
err_t lwip_tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    err_t ret_err;
    uint32_t data_len = 0;
    struct pbuf *q;
    struct tcp_server_struct *es;
    LWIP_ASSERT("arg != NULL", arg != NULL);
    es = (struct tcp_server_struct *)arg;

    if (p == NULL)                                                          /* �ӿͻ��˽��յ������� */
    {
        es->state = ES_TCPSERVER_CLOSING;                                   /* ��Ҫ�ر�TCP ������ */
        es->p = p;
        ret_err = ERR_OK;
    }
    else if (err != ERR_OK)                                                 /* �ӿͻ��˽��յ�һ���ǿ�����,��������ĳ��ԭ��err!=ERR_OK */
    {
        if (p)pbuf_free(p);                                                 /* �ͷŽ���pbuf */

        ret_err = err;
    }
    else if (es->state == ES_TCPSERVER_ACCEPTED)                            /* ��������״̬ */
    {
        if (p != NULL)                                                      /* ����������״̬���ҽ��յ������ݲ�Ϊ��ʱ�����ӡ���� */
        {
            memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE);           /* ���ݽ��ջ��������� */

            for (q = p; q != NULL; q = q->next)                             /* ����������pbuf���� */
            {
                /* �ж�Ҫ������LWIP_DEMO_RX_BUFSIZE�е������Ƿ����LWIP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
                /* �Ļ���ֻ����LWIP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* �������� */
                else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

                data_len += q->len;

                if (data_len > LWIP_DEMO_RX_BUFSIZE) break;                 /* ����TCP�ͻ��˽�������,���� */
            }

            g_lwip_send_flag |= 1 << 6;                                     /* ��ǽ��յ������� */
            g_lwipdev.remoteip[0] = tpcb->remote_ip.addr & 0xff;            /* IADDR4 */
            g_lwipdev.remoteip[1] = (tpcb->remote_ip.addr >> 8) & 0xff;     /* IADDR3 */
            g_lwipdev.remoteip[2] = (tpcb->remote_ip.addr >> 16) & 0xff;    /* IADDR2 */
            g_lwipdev.remoteip[3] = (tpcb->remote_ip.addr >> 24) & 0xff;    /* IADDR1 */
            tcp_recved(tpcb, p->tot_len);                                   /* ���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ�������� */
            pbuf_free(p);                                                   /* �ͷ��ڴ� */
            ret_err = ERR_OK;
        }
    }
    else                                                                    /* �������ر��� */
    {
        tcp_recved(tpcb, p->tot_len);                                       /* ���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ�������� */
        es->p = NULL;
        pbuf_free(p);                                                       /* �ͷ��ڴ� */
        ret_err = ERR_OK;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_err�����Ļص�����
 * @param       arg ������Ĳ���
 * @param       err ��������
 * @retval      ��
 */
void lwip_tcp_server_error(void *arg, err_t err)
{
    LWIP_UNUSED_ARG(err);
    printf("tcp error:%x\r\n", (uint32_t)arg);

    if (arg != NULL)mem_free(arg); /* �ͷ��ڴ� */
}

/**
 * @brief       lwIP���ݷ��ͣ��û�Ӧ�ó�����ô˺�������������
 * @param       tpcb ��TCP���ƿ�
 * @retval      ����ֵ:0���ɹ���������ʧ��
 */
err_t lwip_tcp_server_usersent(struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_server_struct *es;
    es = tpcb->callback_arg;

    if (es != NULL)                                                                         /* ���Ӵ��ڿ��п��Է������� */
    {
        es->p = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)g_lwip_demo_sendbuf), PBUF_POOL); /* �����ڴ� */
        pbuf_take(es->p, (char *)g_lwip_demo_sendbuf, strlen((char *)g_lwip_demo_sendbuf)); /* ��lwip_tcp_server_sentbuf[]�е����ݿ�����es->p_tx�� */
        lwip_tcp_server_senddata(tpcb, es);                                                 /* ��lwip_tcp_server_sentbuf[]���渴�Ƹ�pbuf�����ݷ��ͳ�ȥ */
        g_lwip_send_flag &= ~(1 << 7);                                                      /* ������ݷ��ͱ�־ */

        if (es->p != NULL)pbuf_free(es->p);                                                 /* �ͷ��ڴ� */

        ret_err = ERR_OK;
    }
    else
    {
        tcp_abort(tpcb);                                                                    /* ��ֹ����,ɾ��pcb���ƿ� */
        ret_err = ERR_ABRT;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_poll�Ļص�����
 * @param       arg  ������Ĳ���
 * @param       tpcb ��TCP���ƿ�
 * @retval      ��
 */
err_t lwip_tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_server_struct *es;
    es = (struct tcp_server_struct *)arg;

    if (es->state == ES_TCPSERVER_CLOSING)              /* ��Ҫ�ر�����?ִ�йرղ��� */
    {
        lwip_tcp_server_connection_close(tpcb, es);     /* �ر����� */
    }

    ret_err = ERR_OK;
    return ret_err;
}

/**
 * @brief       lwIP tcp_sent�Ļص�����(����Զ���������յ�ACK�źź�������)
 * @param       arg  ������Ĳ���
 * @param       tpcb ��TCP���ƿ�
 * @param       len  �����͵ĳ���
 * @retval      ��
 */
err_t lwip_tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_server_struct *es;
    LWIP_UNUSED_ARG(len);
    es = (struct tcp_server_struct *) arg;

    if (es->p)lwip_tcp_server_senddata(tpcb, es);   /* �������� */

    return ERR_OK;
}

/**
 * @brief       �˺���������������
 * @param       tpcb ��TCP���ƿ�
 * @param       es   ��LWIP�ص�����ʹ�õĽṹ��
 * @retval      ��
 */
void lwip_tcp_server_senddata(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
    struct pbuf *ptr;
    uint16_t plen;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && es->p && (es->p->len <= tcp_sndbuf(tpcb)))
    {
        ptr = es->p;
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);    /* ��Ҫ���͵����ݼ��뷢�ͻ�������� */

        if (wr_err == ERR_OK)
        {
            plen = ptr->len;
            es->p = ptr->next;                                  /* ָ����һ��pbuf */

            if (es->p)pbuf_ref(es->p);                          /* pbuf��ref��һ */

            pbuf_free(ptr);
            tcp_recved(tpcb, plen);
        }
        else if (wr_err == ERR_MEM) es->p = ptr;
        tcp_output(tpcb);                                       /* �����ͻ�������е����ݷ��ͳ�ȥ */
    }
}

/**
 * @brief       �ر�tcp����
 * @param       tpcb ��TCP���ƿ�
 * @param       es   ��LWIP�ص�����ʹ�õĽṹ��
 * @retval      ��
 */
void lwip_tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
    tcp_close(tpcb);
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es)mem_free(es);
    g_lwip_send_flag &= ~(1 << 5);                  /* ������ӶϿ��� */
}

extern void tcp_pcb_purge(struct tcp_pcb *pcb);     /* �� tcp.c���� */
extern struct tcp_pcb *tcp_active_pcbs;             /* �� tcp.c���� */
extern struct tcp_pcb *tcp_tw_pcbs;                 /* �� tcp.c���� */

/**
 * @brief       ǿ��ɾ��TCP Server�����Ͽ�ʱ��time wait
 * @param       ��
 * @retval      ��
 */
void lwip_tcp_server_remove_timewait(void)
{
    struct tcp_pcb *pcb, *pcb2;
    uint8_t t = 0;

    while (tcp_active_pcbs != NULL && t < 200)
    {
        lwip_periodic_handle();                     /* ������ѯ */
        t++;
        delay_ms(10);                               /* �ȴ�tcp_active_pcbsΪ�� */
    }

    pcb = tcp_tw_pcbs;

    while (pcb != NULL)                             /* ����еȴ�״̬��pcbs */
    {
        tcp_pcb_purge(pcb);
        tcp_tw_pcbs = pcb->next;
        pcb2 = pcb;
        pcb = pcb->next;
        memp_free(MEMP_TCP_PCB, pcb2);
    }
}
