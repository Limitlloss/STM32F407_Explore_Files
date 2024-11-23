/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��CHECKBOX����ʹ��ʵ��
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
 * �޸�˵��
 * V1.0 20200417
 * ��һ�η���
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


/* �Ի�����Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Check box sample",   0,   10,  10, 300, 220, FRAMEWIN_CF_MOVEABLE},
    { TEXT_CreateIndirect,     "Enabled:",           0,    5,  10, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK0,    5,  30, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK1,    5,  60, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK2,    5,  90, 120,   20 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK3,    5, 125, 120,   26 },
    { TEXT_CreateIndirect,     "Disabled:",          0,  150,  10, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK4,  150,  30, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK5,  150,  60, 120,   0 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK6,  150,  90, 120,   26 },
    { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK7,  150, 125, 120,   26 },
    { BUTTON_CreateIndirect,   "OK",         GUI_ID_OK,   10, 165,  80,  30 },
    { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,  200, 165,  80,  30 },
};

/* ��ѡ���ı� */
static const char *_apLabel[] =
{
    "Default",
    "3 States",
    "Box XL",
    "Box XXL"
};

/**
 * @brief       �ص�����
 * @param       pMsg:��Ϣ
 * @return      ��
 */
static void _cbCallback(WM_MESSAGE *pMsg)
{
    int i;
    int NCode, Id;
    WM_HWIN hDlg, hItem;
    hDlg = pMsg->hWin;

    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);         /* ��ȡCHECKBOX�ľ�� */

            for (i = 0; i < 8; i++)
            {
                int Index = i % 4;
                hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0 + i); /* ��ȡCHECKBOX�ľ�� */
                CHECKBOX_SetText(hItem, _apLabel[Index]);

                switch (Index)
                {
                    case 1:
                        CHECKBOX_SetNumStates(hItem, 3);
                        CHECKBOX_SetImage(hItem, &_abmBar[0], CHECKBOX_BI_INACTIV_3STATE);
                        CHECKBOX_SetImage(hItem, &_abmBar[1], CHECKBOX_BI_ACTIV_3STATE);
                        CHECKBOX_SetState(hItem, 2);
                        break;

                    case 2:
                        CHECKBOX_SetState(hItem, 1);
                        CHECKBOX_SetImage(hItem, &_abmXL[0], CHECKBOX_BI_INACTIV_CHECKED);
                        CHECKBOX_SetImage(hItem, &_abmXL[1], CHECKBOX_BI_ACTIV_CHECKED);
                        CHECKBOX_SetFont(hItem, &GUI_FontComic18B_ASCII);
                        break;

                    case 3:
                        CHECKBOX_SetState(hItem, 1);
                        CHECKBOX_SetImage(hItem, &_abmXXL[0], CHECKBOX_BI_INACTIV_CHECKED);
                        CHECKBOX_SetImage(hItem, &_abmXXL[1], CHECKBOX_BI_ACTIV_CHECKED);
                        CHECKBOX_SetFont(hItem, &GUI_FontComic24B_ASCII);
                        break;
                }

                if (i >= 4)
                {
                    WM_DisableWindow(hItem);/* �ܶԻ����ұߵ�����CHECKС���� */
                }
            }

            break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); /* С����ID */
            NCode = pMsg->Data.v;         /* ֪ͨ���� */

            switch (NCode)
            {
                case WM_NOTIFICATION_RELEASED:
                    if (Id == GUI_ID_OK)
                    {
                        GUI_EndDialog(hDlg, 0); /* �رնԻ��� */
                    }

                    if (Id == GUI_ID_CANCEL)
                    {
                        GUI_EndDialog(hDlg, 1); /* �رնԻ��� */
                    }

                    break;
            }

            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    GUI_CURSOR_Show();
#if GUI_SUPPORT_MEMDEV
    WM_SetCreateFlags(WM_CF_MEMDEV);
#endif
    WM_SetDesktopColor(GUI_BLACK);

    while (1)
    {
        GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
        GUI_Delay(500);
    }
}
