/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin的PNG图片显示实验
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



/* 使用GUI_PNG_Draw()函数绘制BMP图片的话
   图片是加载到RAM中的，因此不能大于PNGMEMORYSIZE
   注意：显示PNG图片时内存申请使用的EMWIN的内存申请函数，因此
   PNGMEMORYSIZE不能大于我们给EMWIN分配的内存池大小 */
#define PNGMEMORYSIZE 500*1024 /*图片大小不大于500kb  */

/* 绘制无需加载到RAM中的PNG图片时，图片每行的字节数 */
#define PNGPERLINESIZE 5*1024 

void emwin_create_pngpicture(uint8_t *filename, int x0, int y0, int Xsize, int Ysize);
void emwin_main(void); /* emwin例程入口 */

#endif
