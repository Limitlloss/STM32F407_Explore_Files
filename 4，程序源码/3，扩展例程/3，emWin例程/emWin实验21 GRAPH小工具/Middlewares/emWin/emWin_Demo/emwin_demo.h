/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��HEADER����ʹ��ʵ��
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
#include "GRAPH.h"


typedef struct
{
    int *buffer;                     /* AD���ݴ����*/
    uint8_t        adflag;           /* AD�ɼ��Ƿ���ɱ�־*/
    int dataxsize, dataysize;        /* ʾ����������X,Y��С*/
    uint8_t        linecolorindex, backcolorindex; /* ������ɫ����ֵ�ͱ�����ɫ����ֵ*/
    GUI_COLOR linecolor, backcolor;  /* ������ɫ�ͱ�����ɫ*/
    GRAPH_DATA_Handle    graphdata;  /* GRAHP_DATA�����ݾ�� */
    int div;                         /* һ������� */
    float resolut;                   /* ÿ��ADԭʼֵ������ĵ�ѹ(��λΪmv)*/
} _oscill_dev;

extern _oscill_dev oscilldev;
void emwin_graph_init(void);
void emwin_creat_sin_buf(uint16_t maxval, uint16_t samples);
void emwin_main(void); /* emwin������� */
#endif
