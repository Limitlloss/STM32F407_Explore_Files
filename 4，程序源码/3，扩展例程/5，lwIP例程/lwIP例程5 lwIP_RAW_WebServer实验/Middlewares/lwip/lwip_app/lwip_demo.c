/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP WebServer ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "lwip_demo.h"
#include "httpd.h"
#include "httpd_cgi_ssi.h"

/**
 * @brief       lwip_demo ����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    /* Httpd Init */
    httpd_init();

    /* ����SSI������� */
    httpd_ssi_init();

    /* ����CGI������ */
    httpd_cgi_init();
}
