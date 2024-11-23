/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��HEADER����ʹ��ʵ��
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
#include "./MALLOC/malloc.h"
#include "./BSP/TIMER/btim.h"
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


#define MSG_CHANGE_MAIN_TEXT (WM_USER+0)
#define MSG_CHANGE_INFO_TEXT (WM_USER+1)

static HEADER_Handle _hHeader;
static char _acMainText[100];
static char _acInfoText[100];


/**
 * @brief       �򱳾����ڷ���һ����Ϣ����ʹ������Ч�������������ڵĻص�����
                �ͻ���ʾ�µ��ı���Ϣ
 * @param       pStr : �����ַ���
 * @retval      ��
 */
static void emwin_change_main_text(char *pStr)
{
    WM_MESSAGE Message;
    Message.MsgId = MSG_CHANGE_MAIN_TEXT;   /* �齨��Ϣ */
    Message.Data.p = pStr;
    WM_SendMessage(WM_HBKWIN, &Message);    /* ������Ϣ */
}

/**
 * @brief       �򱳾����ڷ���һ����Ϣ����ʹ������Ч�������������ڵĻص�����
                �ͻ���ʾ�µ��ı���Ϣ
 * @param       pStr : �����ַ���
 * @retval      ��
 */
static void emwin_changeinfo_text(char *pStr)
{
    WM_MESSAGE Message;
    Message.MsgId = MSG_CHANGE_INFO_TEXT;   /* �齨��Ϣ */
    Message.Data.p = pStr;
    WM_SendMessage(WM_HBKWIN, &Message);    /* ������Ϣ */
}

/**
 * @brief       ���ڻص�����
 * @param       pMsg : ��Ϣ
 * @retval      ��
 */
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case MSG_CHANGE_MAIN_TEXT:
            strcpy(_acMainText, pMsg->Data.p);
            WM_InvalidateWindow(pMsg->hWin);    /* ����������Ч�� */
            break;

        case MSG_CHANGE_INFO_TEXT:
            strcpy(_acInfoText, pMsg->Data.p);
            WM_InvalidateWindow(pMsg->hWin);    /* ����������Ч�� */
            break;

        case WM_PAINT:
            GUI_SetBkColor(GUI_BLACK);          /* ���ú�ɫ���� */
            GUI_Clear();                        /* ˢ�±��� */
            GUI_SetColor(GUI_WHITE);            /* ����������ɫ */
            GUI_SetFont(&GUI_Font24_ASCII);     /* ���������С */
            GUI_DispStringHCenterAt("HEADER Widget - Sample", 160, 5);
            GUI_SetFont(&GUI_Font8x16);
            GUI_DispStringAt(_acMainText, 5, 40); /* ��ʾ_acMainText���� */
            GUI_SetFont(&GUI_Font8x8);
            GUI_DispStringAt(_acInfoText, 5, 60); /* ��ʾ_acInfoText���� */
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ����ɫ��
 * @param       ��
 * @retval      ��
 */
static void emwin_onpoint(void)
{
    GUI_RECT Rect;
    GUI_SetBkColor(GUI_GRAY);                         /* ���ñ�����ɫ */
    GUI_Clear();                                      /* ���� */
    WM_GetClientRect(&Rect);                          /* ��ȡ�ͻ�����С */
    Rect.x1 = HEADER_GetItemWidth(_hHeader, 0);       /* ��ȡheaderС������Ŀ0�Ŀ�� */
    GUI_SetColor(GUI_RED);
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1); /* ���ƺ�ɫ���� */
    Rect.x0 = Rect.x1;
    Rect.x1 += HEADER_GetItemWidth(_hHeader, 1);      /* ��ȡheaderС������Ŀ1�Ŀ�� */
    GUI_SetColor(GUI_GREEN);
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1); /* ������ɫ���� */
    Rect.x0 = Rect.x1;
    Rect.x1 += HEADER_GetItemWidth(_hHeader, 2);      /* ��ȡheaderС������Ŀ2�Ŀ�� */
    GUI_SetColor(GUI_BLUE);
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1); /* ������ɫ���� */
}

/**
 * @brief       ���ڻص�����
 * @param       pMsg ����Ϣ
 * @retval      ��
 */
static void _cbWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            emwin_onpoint();
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ��ʾ
 * @param       ��
 * @retval      ��
 */
static void emwin_function_demo(void)
{
    int Cnt = 10;
    char acInfoText[] = "-- sec to play with header control";
    emwin_changeinfo_text("HEADER_ADDItem");
    HEADER_AddItem(_hHeader, 100, "Red", GUI_TA_VCENTER | GUI_TA_HCENTER); /* �����ĿRed */
    HEADER_AddItem(_hHeader, 0, "Green", GUI_TA_VCENTER | GUI_TA_HCENTER); /* �����ĿGreen */
    HEADER_AddItem(_hHeader, 0, ":-)", GUI_TA_VCENTER | GUI_TA_HCENTER);   /* �����Ŀ:- */
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetItemWidth");                             /* ������Ŀ1��� */
    HEADER_SetItemWidth(_hHeader, 1, 60);
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetItemText");                              /* ������Ŀ2��� */
    HEADER_SetItemWidth(_hHeader, 2, 100);
    HEADER_SetItemText(_hHeader, 2, "Blue");
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetFont");
    HEADER_SetFont(_hHeader, &GUI_Font8x8);                             /* �������� */
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetHeight");                                /* ���ø߶� */
    HEADER_SetHeight(_hHeader, 50);
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetTextColor");
    HEADER_SetTextColor(_hHeader, GUI_YELLOW);                          /* ����������ɫ */
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetBkColor");
    HEADER_SetBkColor(_hHeader, GUI_DARKGRAY);                          /* ���ñ�����ɫ */
    GUI_Delay(750);

    emwin_changeinfo_text("HEADER_SetTextAlign");
    HEADER_SetTextAlign(_hHeader, 0, GUI_TA_HCENTER);                   /* ���ö��뷽ʽ */

    while (Cnt > 0)
    {
        acInfoText[0] = '0' + (Cnt / 10);
        acInfoText[1] = '0' + (Cnt-- % 10);
        emwin_changeinfo_text(acInfoText);
        GUI_Delay(1000);
    }
}

/**
 * @brief       ��FRAMEWIN�ؼ���ʹ��header
 * @param       ��
 * @retval      ��
 */
static void emwin_header_framwin(void)
{
    FRAMEWIN_Handle hFrameWin;
    emwin_change_main_text("HEADER control inside a FRAMEWIN");
    hFrameWin = FRAMEWIN_Create("Title", _cbWindow, WM_CF_SHOW, 10, 80, 300, 140); /* ����FRAMEWIN  */
    FRAMEWIN_SetActive(hFrameWin, 1);                                              /* ����FRAMEWIN  */
    _hHeader = HEADER_CreateAttached(WM_GetClientWindow(hFrameWin), 1234, 0);      /* ����������FRAMEWIN��HEADERС���� */
    emwin_function_demo();
    WM_DeleteWindow(hFrameWin);
}

/**
 * @brief       �ڴ�����ʹ��header
 * @param       ��
 * @retval      ��
 */
static void emwin_headerwin(void)
{
    WM_HWIN hWin;
    emwin_change_main_text("HEADER control inside a window");
    hWin = WM_CreateWindow(10, 80, 300, 140, WM_CF_SHOW, _cbWindow, 0);     /* �������� */
    _hHeader = HEADER_CreateAttached(hWin, 1234, 0);                        /* ���������ڴ��ڵ�HEADERС���� */
    emwin_function_demo();
    WM_DeleteWindow(hWin);                                                  /* ɾ������ */
}

/**
 * @brief       ֱ��ʹ��HEADER
 * @param       ��
 * @retval      ��
 */
static void emwin_header(void)
{
    emwin_change_main_text("HEADER control without parent");
    _hHeader = HEADER_Create(10, 80, 300, 0, 0, 1234, WM_CF_SHOW, 0);       /* ����HEADER */
    emwin_function_demo();
    WM_DeleteWindow(_hHeader);
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void)
{
    WM_EnableMemdev(WM_HBKWIN);             /* ʹ�ܴ洢�豸 */
    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_SetCallback(WM_HBKWIN, _cbBkWindow); /* ���ñ������ڻص����� */
    GUI_CURSOR_Show();

    while (1)                               /* Demo��ʾ*/
    {
        emwin_header_framwin();
        emwin_headerwin();
        emwin_header();
    }
}

