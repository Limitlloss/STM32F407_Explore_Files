/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��TEXT����ʹ��ʵ��
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
#include "./BSP/RTC/rtc.h"
#include "./BSP/ADC/adc.h"
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


/*�ؼ�ID */
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)

WM_HWIN TextDialoghWin;     /*�ı��Ի��򴰿� */

/*�Ի������Դ��*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { TEXT_CreateIndirect, "AD VALUE:", ID_TEXT_0, 20, 50, 120, 25, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "DATE:",     ID_TEXT_1, 68, 80, 69, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "TIME:",     ID_TEXT_2, 75, 110, 65, 20, 0, 0x64, 0 },
};

/**
 * @brief       �Ի���ص�����
 * @param       pMsg : ��Ϣ
 * @retval      ��
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    static uint16_t adcvalue = 0;
    emwin_calendar calendar;
    uint8_t tbuf[40];

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetColor(GUI_RED);
            GUI_SetFont(GUI_FONT_24B_ASCII);

            /*��ʾADֵ*/
            adcvalue = adc_get_result_average(ADC_CHANNEL_5, 10); /*��ȡADֵ*/
            GUI_DispDecAt(adcvalue, 140, 50, 4);
            /*��ʾ����*/
            rtc_get_date(&calendar.your ,&calendar.month,&calendar.date,&calendar.week); /*��ȡ����*/
            sprintf((char *)tbuf, "%04d-%02d-%02d", calendar.your, calendar.month, calendar.date);
            GUI_DispStringAt((const char *)tbuf, 140, 80); /*��ʾ����*/
            rtc_get_time(&calendar.hour, &calendar.min, &calendar.sec,&calendar.ampm);
            sprintf((char *)tbuf, "%02d:%02d:%02d", calendar.hour, calendar.min, calendar.sec);
            GUI_DispStringAt((const char *)tbuf, 140, 110); /*��ʾ����*/
        
            sprintf((char *)tbuf, "Week:%d", calendar.week);
            GUI_DispStringAt((const char *)tbuf,140,140);
            break;

        case WM_INIT_DIALOG:
            /*FRAMWIN��ʼ��*/
            hItem = pMsg->hWin;
            FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetText(hItem, "ALIENTEK TEXT");
            FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

            /*TEXT��ʼ��*/
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "AD VALUE:");

            /*���ڳ�ʼ��*/
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "DATE:");

            /*ʱ���ʼ��*/
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetTextColor(hItem, 0x00000000);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "TIME:");
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
 * @brief       ��������WM_HBKWIN�ص�����
 * @param       pMsg : ��Ϣ
 * @retval      ��
 */
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
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
    WM_SetCallback(WM_HBKWIN, _cbBkWindow); /*���ñ������ڻص�����*/
    TextDialoghWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

    while (1)
    {
        GUI_Delay(1000);
        WM_InvalidateWindow(TextDialoghWin); /*�Ի��򴰿���Ч*/
    }
}
