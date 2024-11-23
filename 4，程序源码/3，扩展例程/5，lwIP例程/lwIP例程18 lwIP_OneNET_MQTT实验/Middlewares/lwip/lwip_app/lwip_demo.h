/**
 ****************************************************************************************************
 * @file        lwip_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP+OneNET+MQTTʵ��
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


/* �û���Ҫ�����豸��Ϣ�������º궨���е���Ԫ������ */
#define USER_DEVICE_NAME "MY_MQTT"                                          /* �豸�� */
#define USER_PRODUCT_ID  "wKcHrEVL0n"                                       /* ��ƷID */
#define USER_KEY         "OVhEYzZBb0tpQUVoZWI0ZUYzTWRKeTJYbjBxNldpd0I="     /* �豸��Կ */
/* ��������Ҫonenet�ṩ��token�������ó� */
#define PASSWORD "version=2018-10-31&res=products%2FwKcHrEVL0n%2Fdevices%2FMY_MQTT&et=1956499200&method=md5&sign=Xy4AtolrnCFHDrm6nXWDBQ%3D%3D"

/* ���²����ĺ궨��̶�������Ҫ�޸ģ�ֻ�޸��Ϸ��Ĳ������� */
#define HOST_NAME           "mqtts.heclouds.com"  /*onenet���� */
#define DEVICE_SUBSCRIBE    "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json/accepted"   /* ���� */
#define DEVICE_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json"            /* ���� */
#define SERVER_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/cmd/request/+"           /* �������·����� */


void lwip_demo(void);

#endif
