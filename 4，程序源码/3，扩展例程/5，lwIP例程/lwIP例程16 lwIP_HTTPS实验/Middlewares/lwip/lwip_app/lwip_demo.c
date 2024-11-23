/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP HTTPS ʵ��
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
#include <stdint.h>
#include "lwip_demo.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "lwip/tcpip.h"
#include <stdio.h>
#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/dns.h"
#include "MALLOC/malloc.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"
#include <netdb.h>


static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static const char http_index_html[] = 
                                    "<!DOCTYPE html>"\
                                    "<html>"\
                                    "<head>"\
                                    "<title>����ԭ�� Webserverʵ��</title>"\
                                    "<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
                                    "<style type='text/css'>"\
                                    "* {"\
                                    "margin: 0;"\
                                    "padding: 0;"\
                                    "text-decoration: none;"\
                                    "box-sizing: border-box;"\
                                    "}"\
                                    "body {"\
                                    "min-height: 100vh;"\
                                    "background-image: linear-gradient(120deg, #3498db, #8e44ad);"\
                                    "}"\
                                    ".box-form {"\
                                    "width: 360px;"\
                                    "background: #f1f1f1;"\
                                    "height: 500px;"\
                                    "padding: 40px 40px;"\
                                    "border-radius: 10px;"\
                                    "position: absolute;"\
                                    "left: 50%;"\
                                    "top: 50%;"\
                                    "transform: translate(-50%, -50%);"\
                                    "}"\
                                    ".box-form h1 {"\
                                    "text-align: center;"\
                                    "margin-bottom: 20px;"\
                                    "}"\
                                    ".sendbtn {"\
                                    "display: block;"\
                                    "width: 100%;"\
                                    "height: 50px;"\
                                    "border: none;"\
                                    "background: linear-gradient(120deg, #3498db, #8e44ad);"\
                                    "background-size: 200%;"\
                                    "color: #fff;"\
                                    "cursor: pointer;"\
                                    "outline: none;"\
                                    "transition: .5s;"\
                                    "font-size: 20px;"\
                                    "letter-spacing: 10px;"\
                                    "}"\
                                    ".sendbtn:hover {"\
                                    "background-position: right;"\
                                    "}"\
                                    ".bottom-text {"\
                                    "margin-top: 40px;"\
                                    "text-align: center;"\
                                    "font-size: 13px;"\
                                    "}"\
                                    ".checkboxes"\
                                    "{"\
                                    "border-bottom: 1px solid #0064cd;"\
                                    "}"\
                                    ".label"\
                                    "{"\
                                    "background: linear-gradient(120deg, #3498db, #8e44ad);"\
                                    "background-size: 250%;"\
                                    "color:white;"\
                                    "display:inline-block;"\
                                    "font-size:18px;"\
                                    "font-weight:bold;"\
                                    "height:30px;"\
                                    "line-height:30px;"\
                                    "text-align:center;"\
                                    "width:150px;"\
                                    "}"\
                                    "</style>"\
                                    "</head>"\
                                    "<body>"\
                                    "<form id='LEDSetting' method='POST' action='beep_leds.cgi' class='box-form' >"\
                                    "<div style='text-align:center;width:280px;border:greensolid1px;'>"\
                                    "<img src='https://image.uc.cn/o/wemedia/s/upload/2019/PeVKk81djjeoqio/883a8ce8d02275310a88e7578b82a64d.png;,3,jpegx;3,310x' 'style='margin:0 auto;' width='100' />"\
                                    "</div>"\
                                    "<h1>����ԭ��</h1>"\
                                    "<div class='label' >"\
                                    "<label>LED State:</label>"\
                                    "</div>"\
                                    "<div class='checkboxes'>"\
                                    "<input type='checkbox' name='led1' value='1' />��&nbsp;&nbsp;&nbsp;<input type='checkbox' name='led1' value='2' />�ر�"\
                                    "</div>"\
                                    "<br>"\
                                    "<br>"\
                                    "<div class='label'>"\
                                    "<label>BEEP State:</label>"\
                                    "</div>"\
                                    "<div class='checkboxes'>"\
                                    "<input type='checkbox' name='led1' value='1' />��&nbsp;&nbsp;&nbsp;<input type='checkbox' name='led1' value='2' />�ر�"\
                                    "</div>"\
                                    "<br>"\
                                    "<br>"\
                                    "<input type='submit' class='sendbtn' value='����'>"\
                                    "<br>"\
                                    "<div style='margin:5px 5px;'>"\
                                    "&copy;Copyright 2023 by ����ԭ��"\
                                    "</div>"\
                                    "</form>"\
                                    "</body>"\
                                    "</html>";
/**
 * @brief       Ѱ��ָ���ַ�λ��
 * @param       buf   ������ָ��
 * @param       name  Ѱ���ַ�
 * @retval      �����ַ��ĵ�ַ
 */
char *lwip_data_locate(char *buf, char *name)
{
    char *p;
    p = strstr((char *)buf, name);

    if (p == NULL)
    {
        return NULL;
    }

    p += strlen(name);
    return p;
}

/**
 * @brief       ����HTTP�߳��н��ܵ�һ��HTTP����
 * @param       conn   netconn���ƿ�
 * @retval      ��
 */
static void lwip_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;
    char *ptemp;
    /* �Ӷ˿ڶ�ȡ���ݣ�������ﻹû�����ݣ���������
      ���Ǽ�������(���ǹ��ĵĲ���)��һ��netbuf�� */
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void **)&buf, &buflen);

        /* ����һ��HTTP GET������?ֻ���ǰ5���ַ�����Ϊ
           GET����������ʽ�����Ǳ��ּ�)*/
        if (buflen >= 5 &&
                buf[0] == 'G' &&
                buf[1] == 'E' &&
                buf[2] == 'T' &&
                buf[3] == ' ' &&
                buf[4] == '/' )
        {

start_html:
            /* ����HTML����
            �Ӵ�С�м�ȥ1����Ϊ����û�����ַ����з���\0
            NETCONN_NOCOPY:���ǵ������ǳ�����̬�ģ����Բ���Ҫ������ */
            netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);

            /* �������ǵ�HTMLҳ�� */
            netconn_write(conn, http_index_html, sizeof(http_index_html) - 1, NETCONN_NOCOPY);
        }
        else if(buflen>=8&&buf[0]=='P'&&buf[1]=='O'&&buf[2]=='S'&&buf[3]=='T')
        {
            ptemp = lwip_data_locate((char *)buf, "led1=");

            if (ptemp != NULL)
            {
                if (*ptemp == '1')    /* �鿴led1��ֵ��Ϊ1�������Ϊ2���𣬴�ֵ��HTML��ҳ�������й� */
                {
                    LED0(0);  /* ����LED1 */
                }
                else
                {
                    LED0(1);    /* Ϩ��LED1 */
                }

            }

            ptemp = lwip_data_locate((char *)buf, "beep=");    /* �鿴beep��ֵ��Ϊ3�������Ϊ4���𣬴�ֵ��HTML��ҳ�������й� */

            if (ptemp != NULL )
            {
                if (*ptemp == '3')
                {
                    /* �򿪷����� */
                }
                else
                {
                    /* �رշ����� */
                }
            }
            goto start_html;
        }
    }

    /* �ر�����(��������HTTP�йر�) */
    netconn_close(conn);

    /* ɾ��������(netconn_recv����������Ȩ��
       �������Ǳ���ȷ���ͷŻ�����) */
    netbuf_delete(inbuf);
}

/**
* @brief  lwip_demo�������
* @param  ��
* @retval ��
*/
void lwip_demo(void)
{
    struct netconn *conn, *newconn;
    err_t err;

    /* ����һ���µ�TCP���Ӿ�� */
    /* ʹ��Ĭ��IP��ַ�󶨵��˿�80 (HTTP) */

    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, IP_ADDR_ANY, 80);

    /* ��������������״̬ */
    netconn_listen(conn);

    do
    {
        err = netconn_accept(conn, &newconn);

        if (err == ERR_OK)
        {
            lwip_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    }
    while (err == ERR_OK);

    netconn_close(conn);
    netconn_delete(conn);
}
