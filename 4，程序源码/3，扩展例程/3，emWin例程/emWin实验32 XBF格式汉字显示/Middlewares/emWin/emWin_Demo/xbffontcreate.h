/**
 ****************************************************************************************************
 * @file        xbffontcreate.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��XBF��ʽ������ʾʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef _XBFFONTCREATE_H
#define _XBFFONTCREATE_H
#include "./SYSTEM/sys/sys.h"
#include "GUI.h"

extern GUI_FONT XBF12_Font;
extern GUI_FONT XBF16_Font;
extern GUI_FONT XBF24_Font;
extern GUI_FONT XBF36_Font;

uint8_t Create_XBF12(uint8_t *fxpath); 
uint8_t Create_XBF16(uint8_t *fxpath); 
uint8_t Create_XBF24(uint8_t *fxpath); 
uint8_t Create_XBF36(uint8_t *fxpath);

#endif
