/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��GIFͼƬ��ʾʵ��
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


#define GIFMEMORYSIZE 500*1024 /* ͼƬ��С������500kb */

/* ����������ص�RAM�е�GIFͼƬʱ��ͼƬÿ�е��ֽ��� */
#define GIFPERLINESIZE 2*1024 

void emwin_create_gifpicture(uint8_t *filename, int x0, int y0, int Xsize, int Ysize);
void emwin_main(void); /* emwin������� */

#endif
