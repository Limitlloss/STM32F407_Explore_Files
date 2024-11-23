/**
 ****************************************************************************************************
 * @file        lwip_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP NTP ʵ��
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
 
#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


#define NTP_DEMO_RX_BUFSIZE   2000  /* ����udp���������ݳ��� */
#define NTP_DEMO_PORT         123   /* ����udp���ӵı��ض˿ں� */

typedef struct _NPTformat
{
    char    version;            /* �汾�� */
    char    leap;               /* ʱ��ͬ�� */
    char    mode;               /* ģʽ */
    char    stratum;            /* ϵͳʱ�ӵĲ��� */
    char    poll;               /* ���¼�� */
    signed char  precision;     /* ���ܶ� */
    unsigned int   rootdelay;   /* ���ص����ο�ʱ��Դ������ʱ�� */
    unsigned int   rootdisp;    /* ͳʱ����������ο�ʱ�ӵ������� */
    char    refid;              /* �ο�ʶ���� */
    unsigned long long  reftime;/* �ο�ʱ�� */
    unsigned long long  org;    /* ��ʼ��ʱ��� */
    unsigned long long  rec;    /* �յ���ʱ��� */
    unsigned long long  xmt;    /* ����ʱ��� */
} NPTformat;

typedef struct _DateTime  /*�˽ṹ�嶨����NTPʱ��ͬ������ر���*/
{
    int  year;        /* �� */
    int  month;       /* �� */
    int  day;         /* �� */
    int  hour;        /* ʱ */
    int  minute;      /* �� */
    int  second;      /* �� */
} DateTime;

#define SECS_PERDAY     86400UL         /* һ���еļ����� = 60*60*24 */
#define UTC_ADJ_HRS     8               /* SEOUL : GMT+8��������������  */
#define EPOCH           1900            /* NTP ��ʼ��  */
#define HOST_NAME  "ntp1.aliyun.com"    /*������NTP���������� */

void lwip_demo(void);


#endif /* _CLIENT_H */
