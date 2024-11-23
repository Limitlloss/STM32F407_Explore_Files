/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-8-01
 * @brief       lwIP RAW TCPClient ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
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


#define LWIP_DEMO_RX_BUFSIZE         2000  /* ����udp���������ݳ��� */
#define LWIP_DEMO_PORT               8080  /* ����udp���ӵı��ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE];  
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA";

/* TCP Client ����ȫ��״̬��Ǳ���
 * bit7:0,û������Ҫ����;1,������Ҫ����
 * bit6:0,û���յ�����;1,�յ�������.
 * bit5:0,û�������Ϸ�����;1,�����Ϸ�������.
 * bit4~0:���� */
uint8_t g_lwip_send_flag;

/* tcp����������״̬ */
enum tcp_client_states
{
    ES_TCPCLIENT_NONE = 0,          /* û������ */
    ES_TCPCLIENT_CONNECTED,         /* ���ӵ��������� */
    ES_TCPCLIENT_CLOSING,           /* �ر����� */
};

/* LWIP�ص�����ʹ�õĽṹ�� */
struct tcp_client_struct
{
    uint8_t state;                  /* ��ǰ����״ */
    struct tcp_pcb *pcb;            /* ָ��ǰ��pcb */
    struct pbuf *p;                 /* ָ�����/�����pbuf */
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
 * @brief       ����Զ��IP��ַ
 * @param       ��
 * @retval      ��
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
    tbuf = mymalloc(SRAMIN, 100); /* �����ڴ� */

    if (tbuf == NULL)return;

    /* ǰ����IP���ֺ�DHCP�õ���IPһ�� */
    g_lwipdev.remoteip[0] = g_lwipdev.ip[0];
    g_lwipdev.remoteip[1] = g_lwipdev.ip[1];
//    g_lwipdev.remoteip[2] = g_lwipdev.ip[2];
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2]); /* Զ��IP */
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
            if (key == KEY0_PRES)g_lwipdev.remoteip[3]++; /* IP���� */

            if (key == KEY2_PRES)g_lwipdev.remoteip[3]--; /* IP���� */

            lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0X80, g_point_color); /* ��ʾ��IP */
        }
    }

    myfree(SRAMIN, tbuf);
}

/**
 * @brief       lwip_demo�������
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    struct tcp_pcb *tcppcb;   /* ����һ��TCP�ͻ��˿��ƿ� */
    ip_addr_t rmtipaddr;      /* Զ��ip��ַ */

    char *tbuf;
    uint8_t key;
    uint8_t res = 0;
    uint8_t t = 0;

    lwip_tcp_client_set_remoteip();  /* ��ѡ��IP */
    lcd_clear(WHITE);                /* ���� */
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCP Client Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:Send data", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:Quit", g_point_color);
    tbuf = mymalloc(SRAMIN, 200); /* �����ڴ� */

    if (tbuf == NULL)return ;     /* �ڴ�����ʧ����,ֱ���˳� */

    sprintf((char *)tbuf, "Local IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* ������IP */
    lcd_show_string(5, 130, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.%d", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]); /* Զ��IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote Port:%d", LWIP_DEMO_PORT); /* �ͻ��˶˿ں� */
    lcd_show_string(5, 170, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
    tcppcb = tcp_new();  /* ����һ���µ�pcb */

    if (tcppcb)           /* �����ɹ� */
    {
        IP4_ADDR(&rmtipaddr, g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]);
        tcp_connect(tcppcb, &rmtipaddr, LWIP_DEMO_PORT, lwip_tcp_client_connected); /* ���ӵ�Ŀ�ĵ�ַ��ָ���˿���,�����ӳɹ���ص�lwip_tcp_client_connected()���� */
    }
    else res = 1;

    while (res == 0)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)break;

        if (key == KEY0_PRES)                       /* KEY0������,�������� */
        {
            lwip_tcp_client_usersent(tcppcb);       /* �������� */
        }

        if (g_lwip_send_flag & 1 << 6)              /* �Ƿ��յ����� */
        {
            lcd_fill(5, 230, lcddev.width - 1, lcddev.height - 1, WHITE);                                                       /* ����һ������ */
            lcd_show_string(5, 230, lcddev.width - 30, lcddev.height - 230, 16, (char *)g_lwip_demo_recvbuf, g_point_color);    /* ��ʾ���յ������� */
            g_lwip_send_flag &= ~(1 << 6);          /* ��������Ѿ��������� */
        }

        if (g_lwip_send_flag & 1 << 5)              /* �Ƿ������� */
        {
            lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "STATUS:Connected   ", g_point_color);          /* ��ʾ��Ϣ */
            g_point_color = RED;
            lcd_show_string(5, 210, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", g_point_color);                /* ��ʾ��Ϣ */
            g_point_color = BLUE;
        }
        else if ((g_lwip_send_flag & 1 << 5) == 0)
        {
            lcd_show_string(5, 190, 190, 16, 16, "STATUS:Disconnected", g_point_color);
            lcd_fill(5, 210, lcddev.width - 1, lcddev.height - 1, WHITE);       /* ���� */
        }

        lwip_periodic_handle();                                                 /* LWIP��ѯ���� */
        
        delay_ms(2);
        t++;

        if (t == 200)
        {
            if ((g_lwip_send_flag & 1 << 5) == 0)                               /* δ������,�������� */
            {
                lwip_tcp_client_connection_close(tcppcb, 0);                    /* �ر����� */
                tcppcb = tcp_new();                                             /* ����һ���µ�pcb */

                if (tcppcb)                                                     /* �����ɹ� */
                {
                    tcp_connect(tcppcb, &rmtipaddr, LWIP_DEMO_PORT, lwip_tcp_client_connected); /* ���ӵ�Ŀ�ĵ�ַ��ָ���˿���,�����ӳɹ���ص�lwip_tcp_client_connected()���� */
                }
            }

            t = 0;
            LED0_TOGGLE();
        }
    }
    
    lwip_tcp_client_connection_close(tcppcb, 0);    /* �ر�TCP Client���� */
    lcd_clear(WHITE);                               /* ���� */
    g_point_color = DARKBLUE;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "TCPclient Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
    myfree(SRAMIN, tbuf);
}

/**
 * @brief       lwIP TCP���ӽ�������ûص�����
 * @param       arg   : �ص���������Ĳ���
 * @param       tpcb  : TCP���ƿ�
 * @param       err   : ������
 * @retval      ���ش�����
 */
err_t lwip_tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    struct tcp_client_struct *es = NULL;

    if (err == ERR_OK)
    {
        es = (struct tcp_client_struct *)mem_malloc(sizeof(struct tcp_client_struct)); /* �����ڴ� */

        if (es) /* �ڴ�����ɹ� */
        {
            es->state = ES_TCPCLIENT_CONNECTED;         /* ״̬Ϊ���ӳɹ� */
            es->pcb = tpcb;
            es->p = NULL;
            tcp_arg(tpcb, es);                          /* ʹ��es����tpcb��callback_arg */
            tcp_recv(tpcb, lwip_tcp_client_recv);       /* ��ʼ��LwIP��tcp_recv�ص����� */
            tcp_err(tpcb, lwip_tcp_client_error);       /* ��ʼ��tcp_err()�ص����� */
            tcp_sent(tpcb, lwip_tcp_client_sent);       /* ��ʼ��LwIP��tcp_sent�ص����� */
            tcp_poll(tpcb, lwip_tcp_client_poll, 1);    /* ��ʼ��LwIP��tcp_poll�ص����� */
            g_lwip_send_flag |= 1 << 5;                 /* ������ӵ��������� */
            err = ERR_OK;
        }
        else
        {
            lwip_tcp_client_connection_close(tpcb, es); /* �ر����� */
            err = ERR_MEM;                              /* �����ڴ������� */
        }
    }
    else
    {
        lwip_tcp_client_connection_close(tpcb, 0);      /* �ر����� */
    }

    return err;
}

/**
 * @brief       lwIP tcp_recv()�����Ļص�����
 * @param       arg   : �ص���������Ĳ���
 * @param       tpcb  : TCP���ƿ�
 * @param       p     : �������ݰ�
 * @param       err   : ������
 * @retval      ���ش�����
 */
err_t lwip_tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    uint32_t data_len = 0;
    struct pbuf *q;
    struct tcp_client_struct *es;
    err_t ret_err;
    LWIP_ASSERT("arg != NULL", arg != NULL);
    es = (struct tcp_client_struct *)arg;

    if (p == NULL)                                                  /* ����ӷ��������յ��յ�����֡�͹ر����� */
    {
        es->state = ES_TCPCLIENT_CLOSING;                           /* ��Ҫ�ر�TCP ������ */
        es->p = p;
        ret_err = ERR_OK;
    }
    else if (err != ERR_OK)                                         /* �����յ�һ���ǿյ�����֡,����err!=ERR_OK */
    {
        if (p)pbuf_free(p);                                         /* �ͷŽ���pbuf */

        ret_err = err;
    }
    else if (es->state == ES_TCPCLIENT_CONNECTED)                   /* ����������״̬ʱ */
    {
        if (p != NULL)                                              /* ����������״̬���ҽ��յ������ݲ�Ϊ��ʱ */
        {
            memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE);   /* ���ݽ��ջ��������� */

            for (q = p; q != NULL; q = q->next)                     /* ����������pbuf���� */
            {
                /* �ж�Ҫ������LWIP_DEMO_RX_BUFSIZE�е������Ƿ����LWIP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
                /* �Ļ���ֻ����LWIP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
                if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* �������� */
                else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

                data_len += q->len;

                if (data_len > LWIP_DEMO_RX_BUFSIZE) break; /* ����TCP�ͻ��˽�������,���� */
            }

            g_lwip_send_flag |= 1 << 6;                     /* ��ǽ��յ������� */
            tcp_recved(tpcb, p->tot_len);                   /* ���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ�������� */
            pbuf_free(p);                                   /* �ͷ��ڴ� */
            ret_err = ERR_OK;
        }
    }
    else                                                    /* ���յ����ݵ��������Ѿ��ر� */
    {
        tcp_recved(tpcb, p->tot_len);                       /* ���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ�������� */
        es->p = NULL;
        pbuf_free(p);                                       /* �ͷ��ڴ� */
        ret_err = ERR_OK;
    }

    return ret_err;
}

/**
 * @brief       lwIP tcp_err�����Ļص�����
 * @param       arg   : �ص���������Ĳ���
 * @retval      ��
 */
void lwip_tcp_client_error(void *arg, err_t err)
{
    /* �������ǲ����κδ��� */
}

/**
 * @brief       LWIP���ݷ��ͣ��û�Ӧ�ó�����ô˺�������������
 * @param       tpcb: TCP���ƿ�
 * @retval      ����ֵ:0���ɹ���������ʧ��
 */
err_t lwip_tcp_client_usersent(struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_client_struct *es;
    es = tpcb->callback_arg;

    if (es != NULL) /* ���Ӵ��ڿ��п��Է������� */
    {
        es->p = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)g_lwip_demo_sendbuf), PBUF_POOL); /* �����ڴ� */
        pbuf_take(es->p, (char *)g_lwip_demo_sendbuf, strlen((char *)g_lwip_demo_sendbuf)); /* ��tcp_client_sentbuf[]�е����ݿ�����es->p_tx�� */
        lwip_tcp_client_senddata(tpcb, es);                                                 /* ��tcp_client_sentbuf[]���渴�Ƹ�pbuf�����ݷ��ͳ�ȥ */
        g_lwip_send_flag &= ~(1 << 7);                                                      /* ������ݷ��ͱ�־ */

        if (es->p)pbuf_free(es->p);                                                         /* �ͷ��ڴ� */

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
 * @param       arg : �ص���������Ĳ���
 * @param       tpcb: TCP���ƿ�
 * @retval      ERR_OK
 */
err_t lwip_tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_client_struct *es;
    es = (struct tcp_client_struct *)arg;

    if (es->state == ES_TCPCLIENT_CLOSING)              /* ���ӶϿ� */
    {
        lwip_tcp_client_connection_close(tpcb, es);     /* �ر�TCP���� */
    }

    ret_err = ERR_OK;
    return ret_err;
}

/**
 * @brief       lwIP tcp_sent�Ļص�����(����Զ���������յ�ACK�źź�������)
 * @param       arg : �ص���������Ĳ���
 * @param       tpcb: TCP���ƿ�
 * @param       len : ����
 * @retval      ERR_OK
 */
err_t lwip_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_client_struct *es;
    LWIP_UNUSED_ARG(len);
    es = (struct tcp_client_struct *)arg;

    if (es->p)lwip_tcp_client_senddata(tpcb, es); /* �������� */

    return ERR_OK;
}

/**
 * @brief       ������������
 * @param       tpcb: TCP���ƿ�
 * @param       es  : LWIP�ص�����ʹ�õĽṹ��
 * @retval      ��
 */
void lwip_tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && es->p && (es->p->len <= tcp_sndbuf(tpcb))) /* ��Ҫ���͵����ݼ��뵽���ͻ�������� */
    {
        ptr = es->p;
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK)
        {
            es->p = ptr->next;             /* ָ����һ��pbuf */

            if (es->p)pbuf_ref(es->p);     /* pbuf��ref��һ */

            pbuf_free(ptr);                /* �ͷ�ptr */
        }
        else if (wr_err == ERR_MEM)es->p = ptr;

        tcp_output(tpcb);                  /* �����ͻ�������е������������ͳ�ȥ */
    }
}

/**
 * @brief       �ر��������������
 * @param       tpcb: TCP���ƿ�
 * @param       es  : LWIP�ص�����ʹ�õĽṹ��
 * @retval      ��
 */
void lwip_tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{
    /* �Ƴ��ص� */
    tcp_abort(tpcb);                        /* ��ֹ����,ɾ��pcb���ƿ� */
    tcp_arg(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es)mem_free(es);

    g_lwip_send_flag &= ~(1 << 5);          /* ������ӶϿ��� */
}
