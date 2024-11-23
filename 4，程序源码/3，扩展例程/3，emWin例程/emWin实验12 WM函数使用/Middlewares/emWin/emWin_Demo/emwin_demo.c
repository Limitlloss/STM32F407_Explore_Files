/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��WM����ʹ��ʵ��
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


#define MSG_CHANGE_TEXT (WM_USER + 0)  /* �û��Զ������Ϣ���� */

#define SPEED           1250

static char _acInfoText[40];

static GUI_COLOR _WindowColor1 = GUI_GREEN;   /* ����1����ɫΪ��ɫ */
static GUI_COLOR _FrameColor1  = GUI_BLUE;    /* ����1�ı߽�Ϊ��ɫ */
static GUI_COLOR _WindowColor2 = GUI_RED;     /* ����2����ɫΪ��ɫ */
static GUI_COLOR _FrameColor2  = GUI_YELLOW;  /* ����2�ı߽�Ϊ��ɫ */
static GUI_COLOR _ChildColor   = GUI_YELLOW;  /* �Ӵ��ڵ���ɫΪ��ɫ */
static GUI_COLOR _ChildFrame   = GUI_BLACK;   /* �Ӵ��ڵı߽�Ϊ��ɫ */

/* �ص� */
static WM_CALLBACK *_cbBkWindowOld;


/* ���ھ�� */
static WM_HWIN _hWindow1;     /* ����1*/
static WM_HWIN _hWindow2;     /* ����2*/
static WM_HWIN _hChild;       /* �Ӵ���*/


/**
 * @brief       �򱳾����ڷ���һ����Ϣ��ʹ����������Ч
                �������ڵĻص������ͻ���ʾ���͵���Ϣ
 * @param       pStr : Ҫ���͵��ַ���
 * @return      ��
 */
static void emwin_change_info_text(char *pStr)
{
    WM_MESSAGE Message;
    Message.MsgId = MSG_CHANGE_TEXT;
    Message.Data.p = pStr;
    WM_SendMessage(WM_HBKWIN, &Message);  /* �򱳾����ڷ�����Ϣ */
    WM_InvalidateWindow(WM_HBKWIN);       /* ʹ��������ʧЧ */
}

/**
 * @brief       ��ʾһЩ��Ϣ
 * @param       pStr : �ַ���
 * @return      ��
 */
static void emwin_draw_info_text(char *pStr)
{
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("WindowManager - Sample", 160, 5);
    GUI_SetFont(&GUI_Font8x16);
    GUI_DispStringAtCEOL(pStr, 5, 40); /* ��ָ��λ����ʾ�ַ������������ĩ */
}

/**
 * @brief       ���������ƶ�
 * @param       dy:��Ҫ�ƶ��������(������)
 * @return      ��
 */
static void emwin_liftup(int dy)
{
    int i;
    int tm;

    for (i = 0; i < (dy / 4); i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, 0, -4); /* �ƶ�����1 */
        WM_MoveWindow(_hWindow2, 0, -4); /* �ƶ�����2 */

        while ((GUI_GetTime() - tm) < 20)
        {
            WM_Exec(); /* ͨ��ִ�лص������ػ���Ч���� */
        }
    }

}

/**
 * @brief       ���������ƶ�
 * @param       dy:��Ҫ�ƶ��������(������)
 * @return      ��
 */
static void emwin_liftdown(int dy)
{
    int i;
    int tm;

    for (i = 0; i < (dy / 4); i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, 0, 4);
        WM_MoveWindow(_hWindow2, 0, 4);

        while ((GUI_GetTime() - tm) < 20)
        {
            WM_Exec();
        }
    }
}

/**
 * @brief       �������ڵĻص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case MSG_CHANGE_TEXT:
            strcpy(_acInfoText, (char const *)pMsg->Data.p);

        case WM_PAINT:
            GUI_SetBkColor(GUI_BLACK);
            GUI_Clear();
            GUI_SetColor(GUI_WHITE);
            GUI_SetFont(&GUI_Font24_ASCII);
            GUI_DispStringHCenterAt("WindowManager - Sample", 160, 5);
            GUI_SetFont(&GUI_Font8x16);
            GUI_DispStringAt(_acInfoText, 5, 40);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ����1�ص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbWindow1(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int x;
    int y;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            WM_GetInsideRect(&Rect);    /*���ؿͻ��������� */
            GUI_SetBkColor(_WindowColor1);
            GUI_SetColor(_FrameColor1);
            GUI_ClearRectEx(&Rect);     /*���һ���������� */
            GUI_DrawRectEx(&Rect);      /*����һ������ */
            GUI_SetColor(GUI_WHITE);
            GUI_SetFont(&GUI_Font24_ASCII);
            x = WM_GetWindowSizeX(pMsg->hWin);  /*��ȡ���ڵ�X���С */
            y = WM_GetWindowSizeY(pMsg->hWin);  /*��ȡ���ڵ�Y���С */
            GUI_DispStringHCenterAt("Window 1", x / 2, (y / 2) - 12);
            break;

        default:
            WM_DefaultProc(pMsg);

    }
}

/**
 * @brief       ����2�ص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbWindow2(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int x;
    int y;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            WM_GetInsideRect(&Rect);    /* ���ؿͻ��������� */
            GUI_SetBkColor(_WindowColor2);
            GUI_SetColor(_FrameColor2);
            GUI_ClearRectEx(&Rect);     /* ���һ���������� */
            GUI_DrawRectEx(&Rect);      /* ����һ������ */
            GUI_SetColor(GUI_WHITE);
            GUI_SetFont(&GUI_Font24_ASCII);
            x = WM_GetWindowSizeX(pMsg->hWin);    /* ��ȡ���ڵ�X���С */
            y = WM_GetWindowSizeY(pMsg->hWin);    /* ��ȡ���ڵ�Y���С */
            GUI_DispStringHCenterAt("Window 2", x / 2, (y / 4) - 12);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       �Ӵ��ڵĻص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbChild(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int x;
    int y;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            WM_GetInsideRect(&Rect);
            GUI_SetBkColor(_ChildColor);
            GUI_SetColor(_ChildFrame);
            GUI_ClearRectEx(&Rect);
            GUI_DrawRectEx(&Rect);
            GUI_SetColor(GUI_RED);
            GUI_SetFont(&GUI_Font24_ASCII);
            x = WM_GetWindowSizeX(pMsg->hWin);
            y = WM_GetWindowSizeY(pMsg->hWin);
            GUI_DispStringHCenterAt("Child window", x / 2, (y / 2) - 12);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ����1����һ���ص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbDemoCallback1(WM_MESSAGE *pMsg)
{
    int x, y;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_GREEN);
            GUI_Clear();
            GUI_SetColor(GUI_RED);
            GUI_SetFont(&GUI_FontComic18B_1);
            x = WM_GetWindowSizeX(pMsg->hWin);
            y = WM_GetWindowSizeY(pMsg->hWin);
            GUI_DispStringHCenterAt("Window 1\nanother Callback", x / 2, (y / 2) - 18);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ����2����һ���ص�����
 * @param       pMsg:����
 * @return      ��
 */
static void _cbDemoCallback2(WM_MESSAGE *pMsg)
{
    int x, y;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_MAGENTA);
            GUI_Clear();
            GUI_SetColor(GUI_YELLOW);
            GUI_SetFont(&GUI_FontComic18B_1);
            x = WM_GetWindowSizeX(pMsg->hWin);
            y = WM_GetWindowSizeY(pMsg->hWin);
            GUI_DispStringHCenterAt("Window 2\nanother Callback", x / 2, (y / 4) - 18);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * @brief       ���ô��ڵı�����ɫ,������ʾWM_SetDesktopColor()����
 * @param       ��
 * @return      ��
 */
static void emwin_set_desktop_color(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    emwin_draw_info_text("WM_SetDesktopColor()");
    GUI_Delay(SPEED * 3 / 2);
    WM_SetDesktopColor(GUI_BLACK);
    GUI_Delay(SPEED / 2);

    GUI_SetBkColor(GUI_BLACK);
    WM_SetDesktopColor(GUI_INVALID_COLOR); /* �������洰����ɫ */
}

/**
 * @brief       �������ڵ���ʾ����,��Ҫ��ʾWM_CreateWindow
 * @param       ��
 * @return      ��
 */
static void emwin_create_window(void)
{
    _cbBkWindowOld = WM_SetCallback(WM_HBKWIN, _cbBkWindow); /* ���ñ�������Ļص�����  */
    /* �������� */
    emwin_change_info_text("WM_CreateWindow()");
    GUI_Delay(SPEED);
    _hWindow1 = WM_CreateWindow(50, 70, 165, 100,	WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow1, 0); /* ��������1��ʹ���Զ��洢�豸 */
    GUI_Delay(SPEED / 3);
    _hWindow2 = WM_CreateWindow(105, 125, 165, 100, WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow2, 0); /* ��������2��ʹ���Զ��洢�豸 */
    GUI_Delay(SPEED);
}

/**
 * @brief       �����Ӵ��ڣ���Ҫ��ʾWM_CreateWindowAsChild()����
 * @param       ��
 * @return      ��
 */
static void emwin_create_windowAsChild(void)
{
    emwin_change_info_text("WM_CreateWindowChild()");
    GUI_Delay(SPEED);
    _hChild = WM_CreateWindowAsChild(10, 50, 145, 40, _hWindow2, WM_CF_SHOW | WM_CF_MEMDEV, _cbChild, 0); /* �򴰿�2����һ���Ӵ��� */
    GUI_Delay(SPEED);
}

/**
 * @brief       ��ָ���Ĵ�����Ч����Ҫ��ʾWM_InvalidateWindow()����
 * @param       ��
 * @return      ��
 */
static void emwin_invaildate_window(void)
{
    emwin_change_info_text("WM_InvalidateWindow()");
    _WindowColor1 = GUI_BLUE;
    _FrameColor1 = GUI_GREEN;
    GUI_Delay(SPEED);
    /* ʹ����1��Ч�����ػ洰�ڣ�ע�ⴰ����ɫ�ı仯 */
    WM_InvalidateWindow(_hWindow1);
    GUI_Delay(SPEED);
}

/**
 * @brief       ��ָ�����ڷ�������ͬ�����ڵ�ǰ�棬��ҪչʾWMBringToTop()������ʹ��
 * @param       ��
 * @return      ��
 */
static void emwin_bring_to_top(void)
{
    emwin_change_info_text("WM_BringToTop()");
    GUI_Delay(SPEED);
    WM_BringToTop(_hWindow1); /* ʹ����1��������ͬ�����ڵ�ǰ�� */
    GUI_Delay(SPEED);
}

/**
 * @brief       �ƶ����ڵ�ָ���ط�����ҪչʾWM_MoveTo()����
 * @param       ��
 * @return      ��
 */
static void emwin_moveto(void)
{
    int i;
    int tm;
    int tDiff;

    emwin_change_info_text("WM_MoveTo()");
    GUI_Delay(SPEED);

    for (i = 1; i < 56; i++)
    {
        tm = GUI_GetTime();
        WM_MoveTo(_hWindow1, 50 + i, 70 + i); /* �ƶ�ָ�����ڵ�ָ��λ�� */
        WM_MoveTo(_hWindow2, 105 - i, 125 - i);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 1; i < 56; i++)
    {
        tm = GUI_GetTime();
        WM_MoveTo(_hWindow1, 105 - i, 125 - i);
        WM_MoveTo(_hWindow2, 50 + i, 70 + i);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    GUI_Delay(SPEED);
}

/**
 * @brief       ��ָ�����ڷ�������ͬ�����ڵ�ǰ�棬��ҪչʾWMBringToBottom()������ʹ��
 * @param       ��
 * @return      ��
 */
static void emwin_bring_to_bottom(void)
{
    emwin_change_info_text("WM_BringToBottom()");
    GUI_Delay(SPEED);
    WM_BringToBottom(_hWindow1); 	//ʹ����1��������ͬ�����ڵĺ���
    GUI_Delay(SPEED);
}

/**
 * @brief       �ƶ�����ָ���ľ��룬��ҪչʾWM_MoveWindow()������ʹ��
                WM_MoveWindow()�����ǰѴ����ƶ�ָ������
                ��WM_MoveTo()�ǽ������ƶ���ָ��λ��,ע����������������
 * @param       ��
 * @return      ��
 */
static void emwin_move_window(void)
{
    int i;
    int tm;
    int tDiff;

    emwin_change_info_text("WM_MoveWindow()");
    GUI_Delay(SPEED);

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, 1, 1);   /* �ƶ�����1 */
        WM_MoveWindow(_hWindow2, -1, -1); /* �ƶ�����2 */
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, -1, -1);
        WM_MoveWindow(_hWindow2, 1, 1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    GUI_Delay(SPEED);
}

/**
 * @brief       ���غ���ʾ������,��Ҫ��ʾWM_HideWindow()��WM_ShowWindow()������ʹ��
                ����WM_HideWindow()����WM_ShowWindow()�����������󲢲����������ػ���
                ��ʾ���ڣ���Ҫִ��WM_Exec()�����ػ棬���Ҫ�������ػ�����ʾ�Ļ�Ӧ����
                WM_Paint()������GUI_Delay()�������������WM_Exec()�������������ǿ���
                �ĺ������������ػ�����ʾ�˴���
 * @param       ��
 * @return      ��
 */
static void emwin_hide_show_parent(void)
{
    emwin_change_info_text("WM_HideWindow(Parent)");
    GUI_Delay(SPEED);
    WM_HideWindow(_hWindow2);   /* ���ش���2 */
    GUI_Delay(SPEED / 3);
    WM_HideWindow(_hWindow1);   /* ���ش��� */
    GUI_Delay(SPEED);

    emwin_change_info_text("WM_ShowWindow(Parent)");
    GUI_Delay(SPEED);
    WM_ShowWindow(_hWindow1);   /* ��ʾ����1 */
    WM_ShowWindow(_hWindow2);   /* ʾ����2 */
    GUI_Delay(SPEED);

}

/**
 * @brief       ���غ���ʾ�Ӵ���,��Ҫ��ʾWM_HideWindow()��WM_ShowWindow()������ʹ��
 * @param       ��
 * @return      ��
 */
static void emwin_hide_show_child(void)
{
    emwin_change_info_text("WM_HideWindow(Child)");
    GUI_Delay(SPEED);
    WM_HideWindow(_hChild); /* �����Ӵ��� */
    GUI_Delay(SPEED);

    emwin_change_info_text("WM_ShowWindow(Child)");
    GUI_Delay(SPEED);
    WM_ShowWindow(_hChild); /* ��ʾ�Ӵ��� */
    GUI_Delay(SPEED);
}

/**
 * @brief       ��ʾ���ڲü�����
 * @param       ��
 * @return      ��
 */
static void emwin_clipping(void)
{
    int i;
    int tm;
    int tDiff;

    emwin_change_info_text("Demostrating clipping of child");

    for (i = 0; i < 25; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hChild, 1, 0);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 25; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hChild, 0, 1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 50; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hChild, -1, 0);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 25; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hChild, 0, -1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 25; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hChild, 1, 0);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    GUI_Delay(SPEED);
}

/**
 * @brief       ��ʾʹ�ûص�������ʵ�ִ��ڵ��ػ�
 * @param       ��
 * @return      ��
 */
static void emwin_redrawing(void)
{
    int i;
    int tm;
    int tDiff;

    emwin_change_info_text("Demonstrating redrawing");
    GUI_Delay(SPEED);
    emwin_liftup(40);    /*����40������ */
    GUI_Delay(SPEED / 3);
    emwin_change_info_text("Using a callback for redrawing");
    GUI_Delay(SPEED / 3);

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1,  1,  1);
        WM_MoveWindow(_hWindow2, -1, -1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, -1, -1);
        WM_MoveWindow(_hWindow2,  1,  1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    GUI_Delay(SPEED / 4);
    emwin_liftdown(30);
    GUI_Delay(SPEED / 2);
    emwin_change_info_text("Without redrawing");
    GUI_Delay(SPEED);
    emwin_liftup(30);
    GUI_Delay(SPEED / 4);
    WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);  /*ȡ���������ڵĻص����� */

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1,  1,  1);
        WM_MoveWindow(_hWindow2, -1, -1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 55; i++)
    {
        tm = GUI_GetTime();
        WM_MoveWindow(_hWindow1, -1, -1);
        WM_MoveWindow(_hWindow2,  1,  1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    GUI_Delay(SPEED / 3);
    WM_SetCallback(WM_HBKWIN, _cbBkWindow); /* �ָ��������ڵĻص����� */
    emwin_liftdown(40);
    GUI_Delay(SPEED);
}

/**
 * @brief       ��ʾ���Ŵ��ڣ���Ҫ��WM_ResizeWindow()������ʹ��
 * @param       ��
 * @return      ��
 */
static void emwin_resize_window(void)
{
    int i;
    int tm;
    int tDiff;

    emwin_change_info_text("WM_ResizeWindow()");
    GUI_Delay(SPEED);
    emwin_liftup(30);

    for (i = 0; i < 20; i++)
    {
        tm = GUI_GetTime();
        WM_ResizeWindow(_hWindow1, 1, 1);   /* ����1��X,Y��ÿ������1�����ش�С */
        WM_ResizeWindow(_hWindow2, -1, -1); /* ����2��X,Y��ÿ�μ�Сһ�����ش�С */
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 40; i++)
    {
        tm = GUI_GetTime();
        WM_ResizeWindow(_hWindow1, -1, -1);
        WM_ResizeWindow(_hWindow2,  1,  1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    for (i = 0; i < 20; i++)
    {
        tm = GUI_GetTime();
        WM_ResizeWindow(_hWindow1,  1,  1);
        WM_ResizeWindow(_hWindow2, -1, -1);
        tDiff = 15 - (GUI_GetTime() - tm);
        GUI_Delay(tDiff);
    }

    emwin_liftdown(30);
    GUI_Delay(SPEED);
}

/**
 * @brief       ���ûص���������Ҫ��ʾWM_SetCallback()������ʹ��
 * @param       ��
 * @return      ��
 */
static void emwin_set_callback(void)
{
    emwin_change_info_text("WM_SetCallback()");
    WM_SetCallback(_hWindow1, _cbDemoCallback1);
    WM_InvalidateWindow(_hWindow1);
    GUI_Delay(SPEED / 2);
    WM_SetCallback(_hWindow2, _cbDemoCallback2);
    WM_InvalidateWindow(_hWindow2);
    GUI_Delay(SPEED * 3);
    WM_SetCallback(_hWindow1, _cbWindow1);
    WM_InvalidateWindow(_hWindow1);
    GUI_Delay(SPEED / 2);
    WM_SetCallback(_hWindow2, _cbWindow2);
    WM_InvalidateWindow(_hWindow2);
    GUI_Delay(SPEED);
}

/**
 * @brief       ɾ������
 * @param       ��
 * @return      ��
 */
static void emwin_delete_window(void)
{
    emwin_change_info_text("WM_DeleteWindow()");
    GUI_Delay(SPEED);
    WM_DeleteWindow(_hWindow2); /* ɾ������2*/
    GUI_Delay(SPEED / 3);
    WM_DeleteWindow(_hWindow1); /* ɾ������1*/
    GUI_Delay(SPEED);
    emwin_change_info_text("");
    GUI_Delay(SPEED);

    /* �ָ�������ɫ�ͻص����� */
    WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);
    _WindowColor1 = GUI_GREEN;
    _WindowColor2 = GUI_RED;
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    GUI_SetBkColor(GUI_BLACK);

    while (1)
    {
        emwin_set_desktop_color();
        emwin_create_window();
        emwin_create_windowAsChild();
        emwin_invaildate_window();
        emwin_bring_to_top();
        emwin_moveto();
        emwin_bring_to_bottom();
        emwin_move_window();
        emwin_hide_show_parent();
        emwin_hide_show_child();
        emwin_clipping();
        emwin_redrawing();
        emwin_resize_window();
        emwin_set_callback();
        emwin_delete_window();
    }
}
