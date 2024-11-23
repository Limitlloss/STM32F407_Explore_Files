/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��SLIDER����ʹ��ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407��������� 
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


/* �ؼ�ID */
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_SLIDER_0     (GUI_ID_USER + 0x01)
#define ID_SLIDER_1     (GUI_ID_USER + 0x02)
#define ID_SLIDER_2     (GUI_ID_USER + 0x03)
#define ID_SLIDER_3     (GUI_ID_USER + 0x04)
#define ID_EDIT_0       (GUI_ID_USER + 0x05)
#define ID_EDIT_1       (GUI_ID_USER + 0x06)
#define ID_EDIT_2       (GUI_ID_USER + 0x07)
#define ID_TEXT_0       (GUI_ID_USER + 0x08)
#define ID_TEXT_1       (GUI_ID_USER + 0x09)
#define ID_TEXT_2       (GUI_ID_USER + 0x0A)
#define ID_TEXT_3       (GUI_ID_USER + 0x0B)
#define ID_TEXT_4       (GUI_ID_USER + 0x0C)
#define ID_TEXT_5       (GUI_ID_USER + 0x0D)

slider_structer slider_dev;

/* �Ի�����Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0,   0,   800, 480, 0, 0x64, 0 },
    { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_0,   90,  100, 400, 35,  0, 0x0,  0 },
    { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_1,   90,  165, 400, 35,  0, 0x0,  0 },
    { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_2,   90,  230, 400, 35,  0, 0x0,  0 },
    { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_3,   90,  360, 595, 32,  0, 0x0,  0 },
    { EDIT_CreateIndirect,     "Edit",     ID_EDIT_0,     500, 100, 55,  35,  0, 0x64, 0 },
    { EDIT_CreateIndirect,     "Edit",     ID_EDIT_1,     500, 165, 55,  35,  0, 0x64, 0 },
    { EDIT_CreateIndirect,     "Edit",     ID_EDIT_2,     500, 230, 55,  35,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_0,     311, 47,  202, 29,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_1,     268, 314, 279, 31,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_2,     31,  109, 51,  20,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,     -2,  175, 92,  20,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_4,     12,  239, 80,  20,  0, 0x64, 0 },
    { TEXT_CreateIndirect,     "Text",     ID_TEXT_5,     23,  366, 66,  20,  0, 0x64, 0 },
};

/**
 * @brief       ���ڻص�����
 * @param       pMsg:��Ϣ
 * @retval      ��
 */
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ������ɫ��
 * @param       pMsg:��Ϣ
 * @retval      ��
 */
void emwin_draw_color_block(void)
{
    GUI_SetColor(slider_dev.slider_value[0]);
    GUI_FillRect(560, 100, 595, 135);
    GUI_SetColor(slider_dev.slider_value[1] << 8);
    GUI_FillRect(560, 165, 595, 200);
    GUI_SetColor((uint32_t)slider_dev.slider_value[2] << 16);
    GUI_FillRect(560, 230, 595, 265);

    GUI_SetColor(slider_dev.slider_value[0] | (((uint32_t)slider_dev.slider_value[1]) << 8) | (((uint32_t)slider_dev.slider_value[2]) << 16));
    GUI_FillRect(600, 100, 765, 265);

}

/**
 * @brief       ����SLIDER��ֵ����EDIT��ֵ
 * @param       hDlg�����ھ��
 * @param       Id:С����ID
 * @retval      ��
 */
void emwin_set_edit_value(WM_HWIN hDlg, int Id)
{
    WM_HWIN hSlider;
    WM_HWIN hEdit;
    uint8_t index;

    index = Id - ID_SLIDER_0;
    hSlider = WM_GetDialogItem(hDlg, ID_SLIDER_0 + index);     /* ��ȡ�Ǹ��������ƶ��ľ�� */
    hEdit = WM_GetDialogItem(hDlg, ID_EDIT_0 + index);         /* ��ȡ�����Ӧ��EDIT���� */
    slider_dev.slider_value[index] = SLIDER_GetValue(hSlider); /* ��ȡ�����ֵ������ṹ��slider_value */
    EDIT_SetValue(hEdit, slider_dev.slider_value[index]);      /* ����EDITֵ */
    /* ���ʹ�ͻ��˴�����Ч */
    WM_InvalidateWindow(WM_GetClientWindow(hDlg));
}

/**
 * @brief       �Ի���ص�����
 * @param       pMsg:��Ϣ
 * @retval      ��
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    uint8_t      i;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            emwin_draw_color_block();
            return;

        case WM_INIT_DIALOG:
            /* ��ʼ��FRAMEWIN */
            hItem = pMsg->hWin;
            FRAMEWIN_SetText(hItem, "ALIENTEK SLIDER");
            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
            FRAMEWIN_SetTextColor(hItem, 0x0080FFFF);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

            /* ��ʼ��TEXT0 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetText(hItem, "RGB BLOCK CONTROL");
            TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            /* ��ʼ��TEXT1 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetText(hItem, "LED1  BRIGHTNESS CONTROL");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            /* ��ʼ��TEXT2 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetText(hItem, "RED:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            /* ��ʼ��TEXT3 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
            TEXT_SetText(hItem, "GREEN:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            /* ��ʼ��TEXT4 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
            TEXT_SetText(hItem, "BLUE:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            /* ��ʼ��TEXT5 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
            TEXT_SetText(hItem, "LED:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);

            /* ��ʼ��EDIT0 */
            for (i = 0; i < 3; i++)   /* ����3��EDIT�ؼ� */
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0 + i); 
                EDIT_SetDecMode(hItem, slider_dev.slider_value[i], 0, 255, 0, 0); /* ����ʮ����ģʽ���ֵΪ255 */
                EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);/* EDIT�Ķ���ģʽ */
                EDIT_SetFont(hItem, GUI_FONT_24_ASCII); /* �������� */
            }

            /* ��ʼ��SLIDER */
            for (i = 0; i < 3; i++) /* ����3��SLIDER�ؼ� */
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0 + i);/* ��ȡ�ؼ��ľ�� */
                SLIDER_SetRange(hItem, 0, 255);  /* ���ÿؼ������ֵΪ255 */
                SLIDER_SetValue(hItem, slider_dev.slider_value[i]); /* ���û����λ�� */
            }

            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3); /* ��ȡID_SLIDER_3�ؼ���� */
            SLIDER_SetRange(hItem, 0, 5000); /* �������ֵΪ300 */
            SLIDER_SetValue(hItem, 5000);    /* ���û��鵱ǰֵΪ300����ʱLED0���� */
            break;

        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (Id)
            {
                case ID_SLIDER_0:
                case ID_SLIDER_1:
                case ID_SLIDER_2:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;

                        case WM_NOTIFICATION_VALUE_CHANGED:
                            emwin_set_edit_value(pMsg->hWin, Id);
                            break;
                    }

                case ID_SLIDER_3:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            break;

                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3);
                            slider_dev.slider_value[3] = SLIDER_GetValue(hItem);
                            
                            break;
                    }
            }

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
    slider_dev.slider_value[0] = 0;
    slider_dev.slider_value[1] = 127;
    slider_dev.slider_value[2] = 255;
    slider_dev.slider_value[3] = 5000;
    WM_SetCallback(WM_HBKWIN, _cbBkWindow);
    slider_dev.slider = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    
    while(1)
    {
        GUI_Delay(100);
    }
}
