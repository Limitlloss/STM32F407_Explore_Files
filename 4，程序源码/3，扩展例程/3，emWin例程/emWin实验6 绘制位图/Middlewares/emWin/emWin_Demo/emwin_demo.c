/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin�滭λͼʵ��
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


extern GUI_BITMAP bmALIENTEK;  /* ALIENTEKͼ�� */

/**
 * @brief       ��ʾC�ļ���ʽ��λͼ
 * @param       ��
 * @return      ��
 */
void emwin_draw_bitmap(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_SetColor(GUI_YELLOW);
    GUI_Clear();
    
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_SetTextMode(GUI_TM_TRANS);        /* ͸����ʾ */
    GUI_DispStringHCenterAt("ALIENTEK BITMAP DISPLAY",400,0 );
    
    GUI_DrawBitmap(&bmALIENTEK,295,194);  /* ����ALIENTEKͼ�� */
}

/**
 * @brief       ����ʾ��������λͼ
 * @param       Xmag:X����ı������ӣ���λΪǧ��֮
 * @param       Ymag:Y����ı������ӣ���λΪǧ��֮
 * @return      ��
 */
void emwin_zoom_bitmap(int Xmag,int Ymag)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_DrawBitmapEx(&bmALIENTEK,400,240,105,46,Xmag,Ymag); /* ���ձ�������λͼ */
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @retval      ��
 */
void emwin_main(void)
{
    int Xmag = 0 ,Ymag = 0;     /* ��������,��λ ǧ��֮ */
    emwin_draw_bitmap();
    GUI_Delay(2000);

    while(1)
    {
        Xmag += 100;
        Ymag += 100;

        if(Xmag >= 2000)
        {
            Xmag = 100;
        }

        if(Ymag >= 2000)
        {
            Ymag = 100;
        }

        emwin_zoom_bitmap(Xmag,Ymag); /* �������ŵ�λͼ */
        GUI_Delay(500);
    }
}
