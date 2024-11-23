/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��GUIbuliderʹ��ʵ��
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
#include "DIALOG.h"
/*****************************************************************************************************/
/*uC-OS3*/
#include "os.h"
#include "cpu.h"


/* �Զ����ǩ */
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x01)
#define ID_EDIT_0 (GUI_ID_USER + 0x02)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 (GUI_ID_USER + 0x04)
#define ID_BUTTON_1 (GUI_ID_USER + 0x05)
#define ID_SCROLLBAR_0 (GUI_ID_USER + 0x06)


/* GUI��Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { EDIT_CreateIndirect,     "Edit",     ID_EDIT_0, 410, 20, 350, 250, 0, 0x64, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 20, 20, 350, 250, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_0, 93, 290, 150, 45, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_1, 506, 290, 150, 45, 0, 0x0, 0 },
    { SCROLLBAR_CreateIndirect,"Scrollbar",ID_SCROLLBAR_0, 9, 373, 772, 49, 0, 0x0, 0 },
};

/**
 * @brief       �ص�����
 * @param       ��
 * @return      ��
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG: /* Framewin��ʼ�� */
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleHeight(hItem, 40);
            FRAMEWIN_SetText(hItem, "ALIENTEK BUIBulider");
            FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
            FRAMEWIN_SetTextColor(hItem, 0x000000FF);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            
            /* Edit�ؼ���ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
            EDIT_SetText(hItem, "123");

            /* Listview�ؼ���ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
            LISTVIEW_AddColumn(hItem, 30, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 30, "Year", GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 30, "Num",  GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddRow(hItem, NULL);
            LISTVIEW_SetGridVis(hItem, 1);
            LISTVIEW_SetFont(hItem, GUI_FONT_20_ASCII);
            LISTVIEW_SetItemText(hItem, 0, 0, "ZhangSan");
            LISTVIEW_SetAutoScrollH(hItem, 1);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_SetHeaderHeight(hItem, 30);
            LISTVIEW_SetItemText(hItem, 1, 0, "26");
            LISTVIEW_SetItemText(hItem, 2, 0, "20150328001");
            LISTVIEW_SetRowHeight(hItem, 25);

            /* Button��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetText(hItem, "OK");
            BUTTON_SetFont(hItem, GUI_FONT_COMIC24B_1);
            
            /* Button��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetText(hItem, "CANCEL");
            BUTTON_SetFont(hItem, GUI_FONT_COMIC24B_1);

            break;

        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (Id)
            {
              case ID_EDIT_0: /* Edit����֪ͨ */
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED: /* ֪ͨ����� */
                            /* ִ�еĹ���............... */
                            break;

                        case WM_NOTIFICATION_RELEASED: /* ֪ͨ���ͷ� */
                            /* ִ�еĹ���............... */
                            break;

                        case WM_NOTIFICATION_VALUE_CHANGED: /* ֵ���ı� */
                            /* ִ�еĹ���............... */
                            break;
                    }

                    break;

                case ID_LISTVIEW_0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;

                        case WM_NOTIFICATION_SEL_CHANGED:
                            break;
                    }

                    break;

                case ID_BUTTON_0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;
                    }

                    break;

                case ID_BUTTON_1: 
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;
                    }

                    break;

                case ID_SCROLLBAR_0: 
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;

                        case WM_NOTIFICATION_VALUE_CHANGED:
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
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{

    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    
    while(1)
    {
        GUI_Delay(200);
    }
}
