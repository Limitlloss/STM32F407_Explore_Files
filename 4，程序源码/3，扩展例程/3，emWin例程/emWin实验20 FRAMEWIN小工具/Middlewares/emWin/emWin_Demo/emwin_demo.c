/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��FrameWin����ʹ��ʵ��
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


#define SPEED 1200
#define MSG_CHANGE_MAIN_TEXT (WM_USER+0)

static FRAMEWIN_Handle _hFrame;
static WM_CALLBACK    *_pcbOldFrame;
static char            _acMainText[100];
static int             _LockClose = 1;

/**
 * @brief       �ı䱳����������ʾ���ַ�
 * @param       pStr  : �ַ���
 * @param       Delay : ��ʱ
 * @return      ��
 */
static void emwin_change_main_text(char *pStr, int Delay)
{
    WM_MESSAGE Message;
    Message.MsgId = MSG_CHANGE_MAIN_TEXT;
    Message.Data.p = pStr;
    GUI_Delay(Delay);
    WM_SendMessage(WM_HBKWIN, &Message);
    WM_InvalidateWindow(WM_HBKWIN);
    GUI_Delay(Delay / 3);
}

/**
 * @brief       �Ӵ��ڻص�����
 * @param       pMsg  : ��Ϣ
 * @return      ��
 */
static void _cbChild(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);
            GUI_SetFont(&GUI_FontComic24B_ASCII);
            GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
            GUI_Clear();
            GUI_DispStringHCenterAt("Client window", WM_GetWindowSizeX(hWin) / 2, WM_GetWindowSizeY(hWin) / 2);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       FrameWin�ؼ��ص�����
 * @param       pMsg  : ��Ϣ
 * @return      ��
 */
static void _cbFrame(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_NOTIFY_PARENT:
            if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
            {
                int Id = WM_GetId(pMsg->hWinSrc);

                if (Id == GUI_ID_CLOSE)
                {
                    if (_LockClose) return;

                    _hFrame = 0;
                }
            }

            break;
    }

    if (_pcbOldFrame) (*_pcbOldFrame)(pMsg);
}

/**
 * @brief       ��������WM_HBKWIN�ص�����
 * @param       pMsg  : ��Ϣ
 * @return      ��
 */
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case MSG_CHANGE_MAIN_TEXT:
            strcpy(_acMainText, pMsg->Data.p);
            WM_InvalidateWindow(pMsg->hWin);
            break;

        case WM_PAINT:
            GUI_SetBkColor(GUI_BLACK);
            GUI_Clear();
            GUI_SetColor(GUI_WHITE);
            GUI_SetFont(&GUI_Font24_ASCII);
            GUI_DispStringHCenterAt("WIDGET_FrameWin - Sample", 160, 5);
            GUI_SetFont(&GUI_Font8x16);
            GUI_DispStringHCenterAt(_acMainText, 160, 40);
            GUI_SetFont(&GUI_Font6x8);
            GUI_DispStringHCenterAt("The function FRAMEWIN_Create creates both the\n"
                                    "frame window and the client window.", 160, 190);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       Framewin��ʾ
 * @param       ��
 * @return      ��
 */
static void emwin_framewin_demo(void)
{
    int i;
    char acInfoText[] = "-- sec to play with window";
    WM_HWIN hChild;
   
    WM_SetCallback(WM_HBKWIN, _cbBkWindow); /* Ϊ�������ûص����� */
    
    emwin_change_main_text("FRAMEWIN_Create", SPEED);
    _hFrame = FRAMEWIN_Create("Frame window", 0, WM_CF_SHOW, 30, 60, 260, 125);/* ����һ����ܴ��� */
    FRAMEWIN_SetTitleHeight(_hFrame, 20);
    FRAMEWIN_SetFont(_hFrame, &GUI_Font16B_ASCII);
    
    _pcbOldFrame = WM_SetCallback(_hFrame, _cbFrame);  /* Ϊ��ܴ������ûص����� */
    
    hChild = WM_GetClientWindow(_hFrame); /* ��ȡ�Ӵ��ڵľ�� */
    
    WM_SetCallback(hChild, _cbChild);/* �����Ӵ��ڵĻص����� */
   
    FRAMEWIN_SetMoveable(_hFrame, 1);  /* ���ÿ�ܴ���Ϊ���ƶ��� */
    
    FRAMEWIN_AddCloseButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0); /* ����ܴ��ڴ���������ť */
    FRAMEWIN_AddMaxButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_AddMinButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);

    /*�޸Ŀ�ܴ��ڵ����� */
    emwin_change_main_text("FRAMEWIN_SetActive", SPEED);
    FRAMEWIN_SetActive(_hFrame, 1);
    emwin_change_main_text("FRAMEWIN_SetFont", SPEED);
    FRAMEWIN_SetFont(_hFrame, &GUI_Font24_ASCII);
    FRAMEWIN_SetTitleHeight(_hFrame, 25);
    emwin_change_main_text("FRAMEWIN_SetTextColor", SPEED);
    FRAMEWIN_SetTextColor(_hFrame, GUI_YELLOW);
    emwin_change_main_text("FRAMEWIN_SetTextAlign", SPEED);
    FRAMEWIN_SetTextAlign(_hFrame, GUI_TA_HCENTER);
    emwin_change_main_text("FRAMEWIN_Minimize", SPEED);
    FRAMEWIN_Minimize(_hFrame);
    emwin_change_main_text("FRAMEWIN_Maxmize", SPEED);
    FRAMEWIN_Maximize(_hFrame);
    emwin_change_main_text("FRAMEWIN_Restore", SPEED);
    FRAMEWIN_Restore(_hFrame);
    emwin_change_main_text("FRAMEWIN_SetTitleVis", SPEED);

    for (i = 0; i < 5; i++)
    {
        FRAMEWIN_SetTitleVis(_hFrame, 0);
        GUI_Delay(200);
        FRAMEWIN_SetTitleVis(_hFrame, 1);
        GUI_Delay(200);
    }

    _LockClose = 0;

    for (i = 250; (i > 0) && _hFrame; i--)
    {
        acInfoText[0] = '0' + ((i + 9) / 100);
        acInfoText[1] = '0' + (((i + 9) / 10) % 10);
        emwin_change_main_text(acInfoText, 0);
        GUI_Delay(100);
    }

    if (_hFrame)
    {
        emwin_change_main_text("FRAMEWIN_Delete", SPEED);
    }
    else
    {
        emwin_change_main_text("", 50);
    }
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    WM_EnableMemdev(WM_HBKWIN);

    while (1)
    {
        emwin_framewin_demo();
    }
}
