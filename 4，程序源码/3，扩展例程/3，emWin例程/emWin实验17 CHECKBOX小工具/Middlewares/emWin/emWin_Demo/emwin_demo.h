/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��CHECKBOX����ʹ��ʵ��
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
 * �޸�˵��
 * V1.0 20200417
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef _EMWIN_DEMO_H
#define _EMWIN_DEMO_H
#include "./SYSTEM/sys/sys.h"
#include "GUI.h"
#include "WM.h"


extern const GUI_BITMAP _abmBar[2];
extern const GUI_BITMAP _abmXL[2];
extern const GUI_BITMAP _abmXXL[2];

void emwin_main(void); /* emwin������� */

#endif
