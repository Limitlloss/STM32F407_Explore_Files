/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin的HEADER部件使用实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
    int *buffer;                     /* AD数据存放区*/
    uint8_t        adflag;           /* AD采集是否完成标志*/
    int dataxsize, dataysize;        /* 示波器数据区X,Y大小*/
    uint8_t        linecolorindex, backcolorindex; /* 线条颜色索引值和背景颜色索引值*/
    GUI_COLOR linecolor, backcolor;  /* 线条颜色和背景颜色*/
    GRAPH_DATA_Handle    graphdata;  /* GRAHP_DATA的数据句柄 */
    int div;                         /* 一格代表几秒 */
    float resolut;                   /* 每个AD原始值代表多大的电压(单位为mv)*/
} _oscill_dev;

extern _oscill_dev oscilldev;
void emwin_graph_init(void);
void emwin_creat_sin_buf(uint16_t maxval, uint16_t samples);
void emwin_main(void); /* emwin例程入口 */
#endif
