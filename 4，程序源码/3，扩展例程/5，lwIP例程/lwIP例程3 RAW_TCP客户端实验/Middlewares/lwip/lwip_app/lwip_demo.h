/**
 ****************************************************************************************************
 * @file        lwip_demo.h
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
 
#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


#define LWIP_SEND_DATA              0X80        /* ���������ݷ��� */
extern uint8_t g_lwip_send_flag;                /* UDP���ݷ��ͱ�־λ */

void lwip_demo(void);

#endif /* _CLIENT_H */
