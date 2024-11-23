/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��Buttonλͼ��ʾʵ��
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
#include "./BSP/BEEP/beep.h"
#include "buttonbmp.h"
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


#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_1 (GUI_ID_USER + 0x04)

GUI_BITMAP buttonbmp_tab[2];

/* �Ի�����Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 350, 122, 110, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 350, 251, 110, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 266, 132, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 266, 261, 80, 20, 0, 0x64, 0 },
};

/**
 * @brief       �Ի���ص�����
 * @param       pMsg
 * @return      ��
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    static uint8_t ledflag = 0;
    static uint8_t beepflag = 0;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_WHITE);
            GUI_Clear();
            break;

        case WM_INIT_DIALOG:
            /* ��ʼ���Ի��� */
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleHeight(hItem, 30);
            FRAMEWIN_SetText(hItem, "ALIENTEK BUTTONBMP DISP");
            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

            /* ��ʼ��BUTTON0 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetBitmapEx(hItem, 0, &buttonbmp_tab[0], 0, 0);
            BUTTON_SetText(hItem, "");

            /* ��ʼ��BUTTON1 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetBitmapEx(hItem, 0, &buttonbmp_tab[0], 0, 0);
            BUTTON_SetText(hItem, "");

            /* ��ʼ��TEXT0 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
            TEXT_SetText(hItem, "LED1:");

            /* ��ʼ��TEXT1 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
            TEXT_SetText(hItem, "BEEP:");
            break;

        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (Id)
            {
                case ID_BUTTON_0: /* BUTTON_0��֪ͨ���룬����LED1 */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED: /* ��ť�����²��ͷ� */
                            LED1_TOGGLE();  /* LED1��ת */
                            ledflag = ~ledflag;
                            BUTTON_SetBitmapEx(hItem, 0, ledflag ? &buttonbmp_tab[1] : &buttonbmp_tab[0], 0, 0);
                            break;
                    }

                    break;

                case ID_BUTTON_1: /* BUTTON_1��֪ͨ���룬����BEEP */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            BEEP_TOGGLE();  /* ��������ת */
                            beepflag = ~beepflag;
                            BUTTON_SetBitmapEx(hItem, 0, beepflag ? &buttonbmp_tab[1] : &buttonbmp_tab[0], 0, 0);
                            break;
                    }

                    break;
            }

            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
 * @brief       ����һ���Ի���
 * @param       ��
 * @return      ��
 */
WM_HWIN emwin_create_framewin(void)
{
    WM_HWIN hWin;
    buttonbmp_tab[0] = bmBUTTONOFF;
    buttonbmp_tab[1] = bmBUTTONON;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    emwin_create_framewin();

    while (1)
    {
        GUI_Delay(100);
    }
}
