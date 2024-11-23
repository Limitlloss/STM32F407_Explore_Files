/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP RAW�ӿ�UDPʵ��
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


#define LWIP_DEMO_RX_BUFSIZE         2000  /* ����udp���������ݳ��� */
#define LWIP_DEMO_PORT               8080  /* ����udp���ӵı��ض˿ں� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* ������������ */
char *g_lwip_demo_sendbuf = "ALIENTEK DATA\r\n";
/* ���ݷ��ͱ�־λ */
uint8_t g_lwip_send_flag;

static void lwip_udp_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void lwip_udp_senddata(struct udp_pcb *upcb);
void lwip_udp_connection_close(struct udp_pcb *upcb);

/**
 * @brief       ����Զ��IP��ַ
 * @param       ��
 * @retval      ��
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
    tbuf = mymalloc(SRAMIN, 100); /* �����ڴ� */

    if (tbuf == NULL)return;

    /* ǰ����IP���ֺ�DHCP�õ���IPһ�� */
    g_lwipdev.remoteip[0] = g_lwipdev.ip[0];
    g_lwipdev.remoteip[1] = g_lwipdev.ip[1];
    g_lwipdev.remoteip[2] = g_lwipdev.ip[2];
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2]); /* Զ��IP */
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
            if (key == KEY0_PRES)g_lwipdev.remoteip[3] ++; /* IP���� */

            if (key == KEY2_PRES)g_lwipdev.remoteip[3] --; /* IP���� */

            lcd_show_xnum(xoff, 150, g_lwipdev.remoteip[3], 3, 16, 0X80, g_point_color); /* ��ʾ��IP */
        }
    }

    myfree(SRAMIN, tbuf);
}

/**
 * @brief       UDP�������ص�����
 * @param       arg ���������
 * @param       upcb��UDP���ƿ�
 * @param       p   : �������ݰ�
 * @param       addr��IP��ַ
 * @param       port���˿ں�
 * @retval      ��
 */
static void lwip_udp_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t data_len = 0;
    struct pbuf *q;

    if (p != NULL) /* ���յ���Ϊ�յ�����ʱ */
    {
        memset(g_lwip_demo_recvbuf, 0, LWIP_DEMO_RX_BUFSIZE); /* ���ݽ��ջ��������� */

        for (q = p; q != NULL; q = q->next) /* ����������pbuf���� */
        {
            /* �ж�Ҫ������LWIP_DEMO_RX_BUFSIZE�е������Ƿ����LWIP_DEMO_RX_BUFSIZE��ʣ��ռ䣬������� */
            /* �Ļ���ֻ����LWIP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е����� */
            if (q->len > (LWIP_DEMO_RX_BUFSIZE - data_len)) memcpy(g_lwip_demo_recvbuf + data_len, q->payload, (LWIP_DEMO_RX_BUFSIZE - data_len)); /* �������� */
            else memcpy(g_lwip_demo_recvbuf + data_len, q->payload, q->len);

            data_len += q->len;

            if (data_len > LWIP_DEMO_RX_BUFSIZE) break; /* ����UDP�ͻ��˽�������,���� */
        }

        upcb->remote_ip = *addr;                                        /* ��¼Զ��������IP��ַ */
        upcb->remote_port = port;                                       /* ��¼Զ�������Ķ˿ں� */
        g_lwipdev.remoteip[0] = upcb->remote_ip.addr & 0xff;              /* IADDR4 */
        g_lwipdev.remoteip[1] = (upcb->remote_ip.addr >> 8) & 0xff;       /* IADDR3 */
        g_lwipdev.remoteip[2] = (upcb->remote_ip.addr >> 16) & 0xff;      /* IADDR2 */
        g_lwipdev.remoteip[3] = (upcb->remote_ip.addr >> 24) & 0xff;      /* IADDR1 */
        g_lwip_send_flag |= 1 << 6;                                       /* ��ǽ��յ������� */
        pbuf_free(p);                                                   /* �ͷ��ڴ� */
    }
    else
    {
        udp_disconnect(upcb);
        lcd_clear(WHITE);           /* ���� */
        lcd_show_string(30, 30, 200, 16, 16, "STM32", g_point_color);
        lcd_show_string(30, 50, 200, 16, 16, "UDP Test", g_point_color);
        lcd_show_string(30, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
        lcd_show_string(30, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
        lcd_show_string(30, 190, 210, 16, 16, "Connect break!", g_point_color);
    }
}

/**
 * @brief       lwip_democ�������
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    err_t err;
    struct udp_pcb *udppcb;      /* ����һ��UDP���������ƿ� */
    ip_addr_t rmtipaddr;         /* Զ��ip��ַ */

    char *tbuf;
    uint8_t key;
    uint8_t res = 0;
    uint8_t t = 0;

    lwip_udp_set_remoteip();/* ��ѡ��IP */
    lcd_clear(WHITE);    /* ���� */
    g_point_color = RED;
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "UDP Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY0:Send data", g_point_color);
    lcd_show_string(5, 110, 200, 16, 16, "KEY1:Exit", g_point_color);
    tbuf = mymalloc(SRAMIN, 200); /* �����ڴ� */

    if (tbuf == NULL)return ;     /* �ڴ�����ʧ����,ֱ���˳� */

    sprintf((char *)tbuf, "Local IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* ������IP */
    lcd_show_string(5, 130, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote IP:%d.%d.%d.%d", g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]); /* Զ��IP */
    lcd_show_string(5, 150, 210, 16, 16, tbuf, g_point_color);
    sprintf((char *)tbuf, "Remote Port:%d", LWIP_DEMO_PORT); /* �ͻ��˶˿ں� */
    lcd_show_string(5, 170, 210, 16, 16, tbuf, g_point_color);
    g_point_color = BLUE;
    udppcb = udp_new();

    if (udppcb) /* �����ɹ� */
    {
        IP4_ADDR(&rmtipaddr, g_lwipdev.remoteip[0], g_lwipdev.remoteip[1], g_lwipdev.remoteip[2], g_lwipdev.remoteip[3]);
        err = udp_connect(udppcb, &rmtipaddr, LWIP_DEMO_PORT);      /* UDP�ͻ������ӵ�ָ��IP��ַ�Ͷ˿ںŵķ����� */

        if (err == ERR_OK)
        {
            err = udp_bind(udppcb, IP_ADDR_ANY, LWIP_DEMO_PORT);    /* �󶨱���IP��ַ��˿ں� */

            if (err == ERR_OK)  /* ����� */
            {
                udp_recv(udppcb,lwip_udp_callback, NULL);           /* ע����ջص����� */
                g_point_color = BLUE;
                lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", g_point_color); /* ��ʾ��Ϣ */
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

        if (key == KEY0_PRES) /* KEY0������,�������� */
        {
            lwip_udp_senddata(udppcb);
        }

        if (g_lwip_send_flag & 1 << 6) /* �Ƿ��յ����� */
        {
            lcd_fill(5, 230, lcddev.width - 1, lcddev.height - 1, WHITE); /* ����һ������ */
            /* ��ʾ���յ������� */
            lcd_show_string(6, 230, lcddev.width - 2, lcddev.height - 230, 16, (char *)g_lwip_demo_recvbuf, g_point_color); 
            g_lwip_send_flag &= ~(1 << 6); /* ��������Ѿ��������� */
            
        }

        lwip_periodic_handle(); /* LWIP��ѯ���� */
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
 * @brief       UDP��������������
 * @param       upcb: UDP���ƿ�
 * @retval      ��
 */
void lwip_udp_senddata(struct udp_pcb *upcb)
{
    struct pbuf *ptr;
    ptr = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)g_lwip_demo_sendbuf), PBUF_POOL); /* �����ڴ� */

    if (ptr)
    {
        pbuf_take(ptr, (char *)g_lwip_demo_sendbuf, strlen((char *)g_lwip_demo_sendbuf)); /* ��g_lwip_demo_sendbuf�е����ݴ����pbuf�ṹ�� */
        udp_send(upcb, ptr);    /* udp�������� */
        pbuf_free(ptr);         /* �ͷ��ڴ� */
    }
}

/**
 * @brief       �ر�tcp����
 * @param       upcb: UDP���ƿ�
 * @retval      ��
 */
void lwip_udp_connection_close(struct udp_pcb *upcb)
{
    udp_disconnect(upcb);
    udp_remove(upcb);               /* �Ͽ�UDP���� */
    g_lwip_send_flag &= ~(1 << 5);    /* ������ӶϿ� */
    lcd_clear(WHITE);               /* ���� */
    lcd_show_string(5, 30, 200, 16, 16, "STM32", g_point_color);
    lcd_show_string(5, 50, 200, 16, 16, "UDP Test", g_point_color);
    lcd_show_string(5, 70, 200, 16, 16, "ATOM@ALIENTEK", g_point_color);
    lcd_show_string(5, 90, 200, 16, 16, "KEY1:Connect", g_point_color);
    lcd_show_string(5, 190, 210, 16, 16, "STATUS:Disconnected", g_point_color);
}
