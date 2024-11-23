/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��SWIPELIST����ʹ��ʵ��
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

#include "emwin_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "music.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
/*****************************************************************************************************/
/*uC-OS3*/
#include "os.h"
#include "cpu.h"


GUI_BITMAP image_tab[2];
#define COLOR_GREEN     GUI_MAKE_COLOR(0x14FF39)  /* ����һ����ɫ */
#define NUM_SEPARATORS  3                         /* ���÷ָ�������Ŀ */
#define NUM_ITEMS       5                         /* ������Ŀ���� */


/* �������������ÿ�������Ϣ */
static const char *paItemText[3][5] =
{
    {
        "Brightness",
        "Mobile Data",
        "Bluetooth",
        "Hot Spot",
        "Mobil",
    },
    {
        "Wifi",
        "music",
        "Idle",
        "Night Mode",
        "Font Size",
    },
    {
        "Account",
        "Sound",
        "Memory",
        "Alert",
        "Clock",
    },
};

/* ���ڱ������Ϣ */
static const char *paSeperatorText[] =
{
    "Display",
    "Network",
    "Settings"
};

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    WM_HWIN hSwipelist;
    WM_HWIN hItem;
    int i;
    int j;
    int Xsize;
    int Ysize;
    
    Xsize = LCD_GetXSize();  /* ��ȡLCD��X��С */
    Ysize = LCD_GetYSize();  /* ��ȡLCD��Y��С */

    image_tab[0] = bmwifi;   /* ����λͼ */
    image_tab[1] = bmmusic;
  
    WM_MULTIBUF_Enable(1);  /* �Զ�ʹ�ö໺�� */

    WM_MOTION_Enable(1);              /* ʹ���ƶ� */
    WM_SetBkWindowColor(GUI_BLUE);    /* ����һ��Ĭ�ϵı�����ɫ */

    SWIPELIST_SetDefaultBkColor(SWIPELIST_CI_BK_ITEM_SEL, COLOR_GREEN);  /* ����ѡ����Ŀ����ɫ */
    SWIPELIST_SetDefaultBkColor(SWIPELIST_CI_BK_SEP_ITEM, GUI_DARKGRAY); /* ���÷ָ�������ɫ */
    

    /* ����һ��swipelist */
    hSwipelist = SWIPELIST_CreateEx(10, 10, Xsize/2, Ysize/2, WM_HBKWIN, WM_CF_SHOW, 0, 0);
    WM_MOTION_SetMoveable(hSwipelist, WM_CF_MOTION_Y, 1);
    
    for (j = 0; j < NUM_SEPARATORS; j++) /* ��ÿ���ָ��������5����Ŀ */
    {
        SWIPELIST_AddSepItem(hSwipelist, paSeperatorText[j], 35);
        
      
        for (i = 0; i < NUM_ITEMS; i++)
        {
            SWIPELIST_AddItem(hSwipelist, paItemText[j][i], 35);  /* ���һ���µ��� */
            SWIPELIST_SetSepColor(hSwipelist,i,GUI_RED);          /* ������Ŀ֮�����ɫ�� */
        } 
       
    }

    for (i = 0; i < 5; i++) /* ����ѡ�򸽼ӵ�ǰ�����Ŀ */
    {
        hItem = CHECKBOX_Create(0, 0, 15, 15, WM_HBKWIN, 0, WM_CF_SHOW);
        SWIPELIST_ItemAttachWindow(hSwipelist, 1 + i, hItem, Xsize/2-20, 10);
    }
    
    for (i = 0; i < 2; i++) /* ����λͼ */
    {
        SWIPELIST_SetBitmap(hSwipelist,i+7,SWIPELIST_BA_RIGHT,&image_tab[i]);
    }
    
    while(1)
    {
        GUI_Delay(200);
    }
}
