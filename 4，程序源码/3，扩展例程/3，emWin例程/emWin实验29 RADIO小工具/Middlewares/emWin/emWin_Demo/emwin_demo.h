/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��RADIO����ʹ��ʵ��
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

#ifndef _EMWIN_DEMO_H
#define _EMWIN_DEMO_H
#include "./SYSTEM/sys/sys.h"
#include "GUI.h"
#include "WM.h"


typedef struct
{
    WM_HWIN rdadio;       /* RADIO���ھ�� */
    uint8_t      value;   /* RADIO��ǰ��ѡ�е�ITEM */
}radio_struct;

void emwin_main(void); /* emwin������� */

#endif
