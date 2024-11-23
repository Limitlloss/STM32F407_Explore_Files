/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin�洢�豸ʵ��
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
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
/*****************************************************************************************************/
/*uC-OS3*/
#include "os.h"
#include "cpu.h"


/* ����һ������ */
static GUI_RECT Rect = {200, 150, 300, 210}; 

/**
 * @brief       δʹ�ô洢�豸����
 * @param       Delay : ��ʱ
 * @return      ��
 */
static void emwin_draw(int Delay) 
{
    GUI_SetPenSize(5);
    GUI_SetColor(GUI_RED);
    GUI_DrawLine(Rect.x0 + 3, Rect.y0 + 3, Rect.x1 - 3, Rect.y1 - 3);
    GUI_Delay(Delay);
    GUI_SetColor(GUI_GREEN);
    GUI_DrawLine(Rect.x0 + 3, Rect.y1 - 3, Rect.x1 - 3, Rect.y0 + 3);
    GUI_Delay(Delay);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringInRect("Closed", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_Delay(Delay);
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    GUI_MEMDEV_Handle hMem;    /* �洢�豸�ľ�� */
    int i;

    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("MEMDEV_MemDev - Sample", 400, 0);
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_DispStringHCenterAt("Shows the advantage of using a\nmemorydevice", 400,50);
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispStringHCenterAt("Draws the picture\nwithout a\nmemory device", 250, 100);
    GUI_DispStringHCenterAt("Draws the picture\nusing a\nmemory device", 550, 100);
    

    
    hMem = GUI_MEMDEV_Create(Rect.x0, Rect.y0, Rect.x1 - Rect.x0, Rect.y1 - Rect.y0); /* �����洢�豸 */
    GUI_MEMDEV_Select(hMem);  /* ѡ��hMem��Ϊ���Ʋ���Ŀ��Ĵ洢�豸 */
    emwin_draw(0);            /* ִ��Ҫ��ʾ�����ݸ��Ƶ��洢�豸�� */
    GUI_MEMDEV_Select(0);     /* ѡ��LCD */

    while (1) 
    {
        for (i = 0; i < 3; i++) 
        {
            GUI_Delay(250);
            GUI_ClearRect(LCD_GetXSize() - Rect.x1, Rect.y0, LCD_GetXSize(), Rect.y1);  /* ���ѡ�е���Ļ */
            GUI_Delay(250);
            GUI_MEMDEV_CopyToLCDAt(hMem, LCD_GetXSize() - Rect.x1, Rect.y0); /* ���洢�豸�����ݴ��ڴ渴�Ƶ�LCD��ָ��λ�� */
        }

        GUI_Delay(500);  
      
        /*  δʹ�ô洢�豸 */
        emwin_draw(400);
        GUI_Delay(400);
        GUI_ClearRect(0, 150, 799, 210);
    }
}
