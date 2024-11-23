/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin的FrameWin部件使用实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
 * @brief       改变背景窗口中显示的字符
 * @param       pStr  : 字符串
 * @param       Delay : 延时
 * @return      无
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
 * @brief       子窗口回调函数
 * @param       pMsg  : 消息
 * @return      无
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
 * @brief       FrameWin控件回调函数
 * @param       pMsg  : 消息
 * @return      无
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
 * @brief       背景窗口WM_HBKWIN回调函数
 * @param       pMsg  : 消息
 * @return      无
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
 * @brief       Framewin演示
 * @param       无
 * @return      无
 */
static void emwin_framewin_demo(void)
{
    int i;
    char acInfoText[] = "-- sec to play with window";
    WM_HWIN hChild;
   
    WM_SetCallback(WM_HBKWIN, _cbBkWindow); /* 为背景设置回调函数 */
    
    emwin_change_main_text("FRAMEWIN_Create", SPEED);
    _hFrame = FRAMEWIN_Create("Frame window", 0, WM_CF_SHOW, 30, 60, 260, 125);/* 创建一个框架窗口 */
    FRAMEWIN_SetTitleHeight(_hFrame, 20);
    FRAMEWIN_SetFont(_hFrame, &GUI_Font16B_ASCII);
    
    _pcbOldFrame = WM_SetCallback(_hFrame, _cbFrame);  /* 为框架窗口设置回调函数 */
    
    hChild = WM_GetClientWindow(_hFrame); /* 获取子窗口的句柄 */
    
    WM_SetCallback(hChild, _cbChild);/* 设置子窗口的回调函数 */
   
    FRAMEWIN_SetMoveable(_hFrame, 1);  /* 设置框架窗口为可移动的 */
    
    FRAMEWIN_AddCloseButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0); /* 给框架窗口创建几个按钮 */
    FRAMEWIN_AddMaxButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_AddMinButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);

    /*修改框架窗口的属性 */
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
 * @brief       emwin_main(emwin入口)
 * @param       无
 * @return      无
 */
void emwin_main(void) 
{
    WM_EnableMemdev(WM_HBKWIN);

    while (1)
    {
        emwin_framewin_demo();
    }
}
