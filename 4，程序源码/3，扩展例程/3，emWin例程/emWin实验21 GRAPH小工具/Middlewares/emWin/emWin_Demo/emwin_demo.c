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
#include "./BSP/TIMER/btim.h"
#include "./MALLOC/malloc.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/PWMDAC/pwmdac.h"
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


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0        (GUI_ID_USER + 0x00)
#define ID_GRAPH_0           (GUI_ID_USER + 0x01)
#define ID_TEXT_0            (GUI_ID_USER + 0x02)
#define ID_TEXT_1            (GUI_ID_USER + 0x03)
#define ID_SLIDER_0          (GUI_ID_USER + 0x04)
#define ID_SLIDER_1          (GUI_ID_USER + 0x05)
#define ID_CHECKBOX_0        (GUI_ID_USER + 0x06)
#define ID_CHECKBOX_1        (GUI_ID_USER + 0x07)
#define ID_CHECKBOX_2        (GUI_ID_USER + 0x08)
#define ID_CHECKBOX_3        (GUI_ID_USER + 0x09)
#define ID_TEXT_2            (GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_4        (GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_5        (GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_6        (GUI_ID_USER + 0x0D)
#define ID_CHECKBOX_7        (GUI_ID_USER + 0x0E)
#define ID_TEXT_3            (GUI_ID_USER + 0x0F)
#define ID_RADIO_0           (GUI_ID_USER + 0x10)
#define ID_BUTTON_1          (GUI_ID_USER + 0x12)
#define ID_BUTTON_2          (GUI_ID_USER + 0x13)
#define ID_BUTTON_3          (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_8        (GUI_ID_USER + 0x15)

#define BORDER_TOP      10
#define BORDER_BOTTOM   15
#define BORDER_LEFT     40
#define BORDER_RIGHT    10
_oscill_dev oscilldev;
static int color = 0;
static int ColorIndex = 0;   /* Ƥ����ɫ���� */

/* �Զ���Ƥ��ר�ýṹ�� */
BUTTON_SKINFLEX_PROPS    ButtonSkin;      /* ����Ƥ�� */
RADIO_SKINFLEX_PROPS     RadioSkin;       /* ��ѡ�ؼ� */
SPINBOX_SKINFLEX_PROPS   SpinBoxSkin;     /* ΢����Ƥ�� */
SLIDER_SKINFLEX_PROPS    SliderSkin;      /* ����Ƥ�� */
MULTIPAGE_SKINFLEX_PROPS MultipageSkin;   /* ��ҳƤ�� */
FRAMEWIN_SKINFLEX_PROPS  FramewinSkin;    /* ��ܴ���Ƥ�� */

static GRAPH_SCALE_Handle hScaleV;   /* ���崹ֱ�̶Ⱦ�� */
static GRAPH_SCALE_Handle hScaleH;   /* ����ˮƽ�̶Ⱦ�� */

static int Craph_Stop = 0;

GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_LIGHTBLUE, GUI_BLACK};
GUI_COLOR LineColor[] = {GUI_RED, GUI_BLACK, GUI_WHITE, GUI_YELLOW, GUI_BLUE, GUI_DARKMAGENTA};

/* Ƥ����ɫ�ṹ������Ĵ������ṹ������Ķ���͸�ֵ */
typedef struct
{
    int Color1;   /* �ؼ�δѡ�е���ɫ */
    int Color2;   /* ǰ����ɫ */
    int Color3;   /* �ؼ�������ɫ */
    int Color4;   /* ������ɫ1(��ɫ) */
    int Color5;   /* ������ɫ2(��ɫ)/������ɫ */
} SkinColorIndex; /* Ƥ����ɫ�ṹ�� */

SkinColorIndex SkinColor[10] =    /* Ƥ����ɫ�� */
{
    {0x00e2e2e2, 0x00e08d59, 0x00be581d, 0x00e2d6c4, GUI_WHITE},
    {0x00e2e2e2, 0x00a99a5c, 0x007e7143, 0x00e5e3d8, GUI_WHITE},
    {0x00e2e2e2, 0x00604ade, 0x00332790, 0x00dcdae5, GUI_WHITE},
    {0x00e2e2e2, 0x003876f2, 0x001e3fb7, 0x00dadee5, GUI_WHITE},
    {0x00e2e2e2, 0x0002a2ba, 0x0001586a, 0x00dae4e5, GUI_WHITE},
    {0x00e2e2e2, 0x00f87ec4, 0x00cc4373, 0x00e5dae1, GUI_WHITE},
    {0x00e2e2e2, 0x003fb88c, 0x002e8c66, 0x00dae5e1, GUI_WHITE},
    {0x00e2e2e2, 0x00034497, 0x003488e7, 0x00dadfe5, GUI_WHITE},
    {0x00e2e2e2, GUI_BLACK,  0x00646464, GUI_WHITE,  GUI_WHITE},
    {0x00b3b3b3, 0x00a5856e, 0x00646464, 0x0042230e, 0x0042230e},
};


/* ��Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 1,   0,   800, 480, 0, 0x64,   0 },
  { GRAPH_CreateIndirect,    "Graph",    ID_GRAPH_0,    20,  12,  452, 331, 0, 0x0,    0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_0,     22,  366, 80,  20,  0, 0x64,   0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_1,     22,  412, 80,  20,  0, 0x64,   0 },
  { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_0,   79,  408, 152, 32,  0, 0x0,    0 },
  { SLIDER_CreateIndirect,   "Slider",   ID_SLIDER_1,   79,  361, 152, 32,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 601, 312, 82,  43,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 288, 368, 82,  43,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_2, 688, 312, 82,  43,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_3, 390, 368, 82,  43,  0, 0x0,    0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_2,     488, 17,  97,  30,  0, 0x64,   0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_4, 491, 51,  100, 39,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_5, 491, 120, 100, 39,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_6, 491, 191, 100, 39,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_7, 491, 259, 100, 39,  0, 0x0,    0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,     488, 308, 97,  30,  0, 0x64,   0 },
  { RADIO_CreateIndirect,    "Radio",    ID_RADIO_0,    487, 334, 73,  69,  0, 0x1403, 0 },
  { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_1,   600, 120, 101, 41,  0, 0x0,    0 },
  { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_2,   600, 191, 101, 41,  0, 0x0,    0 },
  { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_3,   600, 259, 101, 41,  0, 0x0,    0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_8, 601, 370, 128, 48,  0, 0x0,    0 },
};

/**
 * @brief       ������̸�����ǰ�����ֵ�����µ�ֵ
                ����������ӵ�GRAPH_DATA������
 * @param       ��
 * @retval      ��
 */
static void emwin_add_values(void)
{
    int i;
    
        if (oscilldev.adflag == 1)
        {
            for (i = 0; i < oscilldev.dataxsize; i++)
            {
                /* ��GRAPHͼ��С�����������,ע��Ҫ��20������Ϊ��ֱ��������(����)20����
                   ���һ�Ҫ��200,��Ϊ��ֱ����������200�����ص� */
                GRAPH_DATA_YT_AddValue(oscilldev.graphdata, oscilldev.buffer[i] / 50 + 200);
            }

            memset(oscilldev.buffer, 0, oscilldev.dataxsize); /* ���buffer */
            oscilldev.adflag = 0;
        }
}

/**
 * @brief       �ص�����
 * @param       pMsg  ����Ϣ
 * @retval      ��
 */
static void _cbCallback(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    int     Value;
    WM_HWIN  hDlg;

    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
            hDlg = WM_GetDialogItem(hItem, ID_GRAPH_0); /* ��ȡID_GRAPH_0�ؼ��ľ�� */

            /* Framewin��ʼ�� */
            FRAMEWIN_SetTitleHeight(hItem, 20);
            FRAMEWIN_SetText(hItem, "ALIENTEK");
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
            FRAMEWIN_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));

            /* ����GRAPH */
            oscilldev.linecolor = LineColor[oscilldev.linecolorindex];
            oscilldev.graphdata = GRAPH_DATA_YT_Create(oscilldev.linecolor, 600, 0, 0); /* ����YT���ݶ��� */
            GRAPH_AttachData(hDlg, oscilldev.graphdata);

            GRAPH_SetGridDistX(hDlg,50);  /* ����ˮƽ������ */
            GRAPH_SetGridDistY(hDlg,20);  /* ���ô�ֱ������ */
            GRAPH_SetGridVis(hDlg,1);     /* ��������ɼ� */

            /* ������ֱ�̶� */
            hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
            GRAPH_SCALE_SetTextColor(hScaleV, GUI_YELLOW);
            GRAPH_AttachScale(hDlg, hScaleV);
            GRAPH_SCALE_SetFactor(hScaleV, 20); /* ���ÿ̶�ϵ�� */
            GRAPH_SCALE_SetOff(hScaleV,200);    /* ��ֱ�̶������ƶ�200�����ص㣬������ֱ�̶Ȼ��и�ֵ */

            /* ����ˮƽ�̶� */
            hScaleH = GRAPH_SCALE_Create(420, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
            GRAPH_SCALE_SetTextColor(hScaleH, GUI_DARKGREEN);
            GRAPH_AttachScale(hDlg, hScaleH);

            /* Text��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetText(hItem, " Axis X:");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetText(hItem, " Axis Y:");
            TEXT_SetFont(hItem, GUI_FONT_13_ASCII);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
            CHECKBOX_SetText(hItem, "Stop");
            CHECKBOX_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
            CHECKBOX_SetText(hItem, "HScrol");
            CHECKBOX_SetState(hItem, 1);
            CHECKBOX_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF8000));

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
            CHECKBOX_SetText(hItem, "Grid");
            CHECKBOX_SetState(hItem, 1);
            CHECKBOX_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FF00));

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
            CHECKBOX_SetText(hItem, "VScrol");
            CHECKBOX_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF8000));

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "Setting");
            TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00000000));
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
            TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00000000));
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "Selecting");

            /* Checkbox��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
            CHECKBOX_SetText(hItem, "Left");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
            CHECKBOX_SetText(hItem, "Top");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
            CHECKBOX_SetText(hItem, "Right");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
            CHECKBOX_SetText(hItem, "Bown");
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
            CHECKBOX_SetText(hItem, "Reverse");
            CHECKBOX_SetTextColor(hItem, GUI_MAKE_COLOR(0x00804000));
            CHECKBOX_SetFont(hItem, GUI_FONT_24_ASCII);

            /* RADIO��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
            RADIO_SetText(hItem, "3D", 0);
            RADIO_SetText(hItem, "Flat", 1);
            RADIO_SetText(hItem, "-", 2);
            RADIO_SetFont(hItem, GUI_FONT_16_ASCII);

            
            /* Button��ʼ�� */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            BUTTON_SetText(hItem, "bk_Color");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
            BUTTON_SetText(hItem, "Amplified_waveform");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
            BUTTON_SetText(hItem, "small_waveform");

            oscilldev.resolut = (float)3300 / 4096;      /* STM32��ADΪ12λ����ôÿ��ADֵΪ3300/4096=0.806mv*/
            oscilldev.div = 100;
            btim_timx_int_init(oscilldev.div - 1, 72 -1);  /* AD����Ƶ�ʣ�10KHz*/
            break;

        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (Id)
            {
                case ID_SLIDER_0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
                            Value = SLIDER_GetValue(pMsg->hWinSrc) * 10;
                            GRAPH_SetGridDistX(hItem, Value);
                            GRAPH_SCALE_SetTickDist(hScaleH, Value);
                            break;
                    }

                    break;

                case ID_SLIDER_1:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
                            Value = SLIDER_GetValue(pMsg->hWinSrc) * 5;
                            GRAPH_SetGridDistY(hItem, Value);
                            GRAPH_SCALE_SetTickDist(hScaleV, Value);
                            break;
                    }

                    break;

                case ID_CHECKBOX_0: 
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            Craph_Stop ^= 1;
                            break;
                    }

                    break;

                case ID_CHECKBOX_1:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

                            if (CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
                            {
                                GRAPH_SetVSizeX(hItem, 500);
                            }
                            else
                            {
                                GRAPH_SetVSizeX(hItem, 0);
                            }

                            break;
                    }

                    break;

                case ID_CHECKBOX_2:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
                            GRAPH_SetGridVis(hItem, CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2)));
                            break;
                    }

                    break;

                case ID_CHECKBOX_3:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

                            if (CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3)))
                            {
                                GRAPH_SetVSizeY(hItem, 400);
                            }
                            else
                            {
                                GRAPH_SetVSizeY(hItem, 0);
                            }
                            break;
                    }

                    break;

                case ID_CHECKBOX_4:
                case ID_CHECKBOX_5:
                case ID_CHECKBOX_7:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
                            GRAPH_SetBorder(hItem,
                                            CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4)) * 40,
                                            CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5)) * 5,
                                            CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6)) * 5,
                                            CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7)) * 5);
                            break;
                    }

                    break;

                case ID_RADIO_0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

                            switch (RADIO_GetValue(pMsg->hWinSrc))
                            {
                                case 0:
                                    WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
                                    break;

                                case 1:
                                    WIDGET_SetEffect(hItem, &WIDGET_Effect_Simple);
                                    break;

                                case 2:
                                    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
                                    break;
                            }

                            break;
                    }

                    break;

                case ID_BUTTON_1:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);  /* ���ضԻ��Ķ����� */

                            if (color > 3)
                            {
                                color = 0;
                            }

                            GRAPH_SetColor(hItem, _aColor[color], GRAPH_CI_BK); /*����GRAPH������ɫ */
                            color ++;
                            break;
                    }

                    break;

                case ID_BUTTON_2:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            oscilldev.div -= 30;

                            if (oscilldev.div < 10) oscilldev.div = 10;

                            __HAL_TIM_DISABLE(&g_timx_handler);/* TIM6ʧ�� */
                            btim_timx_int_init(oscilldev.div, 84 - 1);
                            break;
                    }

                    break;

                case ID_BUTTON_3:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            oscilldev.div += 30;

                            if (oscilldev.div > 1000)oscilldev.div = 100;

                            __HAL_TIM_DISABLE(&g_timx_handler);/* TIM6ʧ�� */
                            btim_timx_int_init(oscilldev.div, 84 - 1);
                            break;
                    }

                    break;

                case ID_CHECKBOX_8:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
                                if (CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8)))
                                {
                                    GRAPH_DATA_YT_SetAlign(oscilldev.graphdata, GRAPH_ALIGN_LEFT);
                                    GRAPH_DATA_YT_MirrorX (oscilldev.graphdata, 1);
                                }
                                else
                                {
                                    GRAPH_DATA_YT_SetAlign(oscilldev.graphdata, GRAPH_ALIGN_RIGHT);
                                    GRAPH_DATA_YT_MirrorX (oscilldev.graphdata, 0);
                                }

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
 * @brief       Ϊ�ؼ�����Ƥ��(���رհ�ť��Knob�ؼ���)
 * @param       ��
 * @retval      ��
 */
void emwin_widget_set_skin()
{
    /* ��ť�ؼ�(���û��ͨ��BUTTON_DrawSkin()��������Ƥ���İ�ť) */
    BUTTON_GetSkinFlexProps(&ButtonSkin, BUTTON_SKINFLEX_PI_ENABLED);
    ButtonSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorFrame[2] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorUpper[0] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorUpper[1] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorLower[0] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorLower[1] = SkinColor[ColorIndex].Color2;
    BUTTON_SetSkinFlexProps(&ButtonSkin, BUTTON_SKINFLEX_PI_ENABLED);

    BUTTON_GetSkinFlexProps(&ButtonSkin, BUTTON_SKINFLEX_PI_PRESSED);
    ButtonSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    ButtonSkin.aColorFrame[2] = SkinColor[ColorIndex].Color3;
    ButtonSkin.aColorUpper[0] = SkinColor[ColorIndex].Color3;
    ButtonSkin.aColorUpper[1] = SkinColor[ColorIndex].Color3;
    ButtonSkin.aColorLower[0] = SkinColor[ColorIndex].Color3;
    ButtonSkin.aColorLower[1] = SkinColor[ColorIndex].Color3;
    BUTTON_SetSkinFlexProps(&ButtonSkin, BUTTON_SKINFLEX_PI_PRESSED);

    /* ��ѡ�ؼ� */
    RADIO_GetSkinFlexProps(&RadioSkin, RADIO_SKINFLEX_PI_UNPRESSED);
    RadioSkin.aColorButton[0] = SkinColor[ColorIndex].Color1;
    RadioSkin.aColorButton[1] = SkinColor[ColorIndex].Color1;
    RadioSkin.aColorButton[2] = SkinColor[ColorIndex].Color1;
    RadioSkin.aColorButton[3] = SkinColor[ColorIndex].Color1;
    RadioSkin.ButtonSize = 10;
    RADIO_SetSkinFlexProps(&RadioSkin, RADIO_SKINFLEX_PI_UNPRESSED);

    RADIO_GetSkinFlexProps(&RadioSkin, RADIO_SKINFLEX_PI_PRESSED);
    RadioSkin.aColorButton[0] = SkinColor[ColorIndex].Color2;
    RadioSkin.aColorButton[1] = SkinColor[ColorIndex].Color2;
    RadioSkin.aColorButton[2] = SkinColor[ColorIndex].Color2;
    RadioSkin.ButtonSize = 10;
    RADIO_SetSkinFlexProps(&RadioSkin, RADIO_SKINFLEX_PI_PRESSED);

    /* ������ť�ؼ�(emWin����:Disabledʵ����������Pressed��Ƥ��) */
    SPINBOX_GetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_DISABLED);
    SpinBoxSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorUpper[0] = SkinColor[ColorIndex].Color3;
    SpinBoxSkin.aColorUpper[1] = SkinColor[ColorIndex].Color3;
    SpinBoxSkin.aColorLower[0] = SkinColor[ColorIndex].Color3;
    SpinBoxSkin.aColorLower[1] = SkinColor[ColorIndex].Color3;
    SpinBoxSkin.ColorArrow = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorBk = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorText = SkinColor[ColorIndex].Color2;
    SPINBOX_SetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_DISABLED);

    SPINBOX_GetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_ENABLED);
    SpinBoxSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorUpper[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorUpper[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorLower[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorLower[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.ColorArrow = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorBk = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorText = SkinColor[ColorIndex].Color2;
    SPINBOX_SetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_ENABLED);

    SPINBOX_GetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_PRESSED);
    SpinBoxSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorUpper[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorUpper[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorLower[0] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.aColorLower[1] = SkinColor[ColorIndex].Color2;
    SpinBoxSkin.ColorArrow = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorBk = SkinColor[ColorIndex].Color5;
    SpinBoxSkin.ColorText = SkinColor[ColorIndex].Color2;
    SPINBOX_SetSkinFlexProps(&SpinBoxSkin, SPINBOX_CI_PRESSED);

    /* ����ؼ� */
    SLIDER_GetSkinFlexProps(&SliderSkin, SLIDER_SKINFLEX_PI_UNPRESSED);
    SliderSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    SliderSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    SliderSkin.aColorInner[0] = SkinColor[ColorIndex].Color2;
    SliderSkin.aColorInner[1] = SkinColor[ColorIndex].Color2;
    SliderSkin.aColorShaft[0] = SkinColor[ColorIndex].Color1;
    SliderSkin.aColorShaft[1] = SkinColor[ColorIndex].Color1;
    SliderSkin.aColorShaft[2] = SkinColor[ColorIndex].Color2;
    SLIDER_SetSkinFlexProps(&SliderSkin, SLIDER_SKINFLEX_PI_UNPRESSED);

    SLIDER_GetSkinFlexProps(&SliderSkin, SLIDER_SKINFLEX_PI_PRESSED);
    SliderSkin.aColorFrame[0] = SkinColor[ColorIndex].Color3;
    SliderSkin.aColorFrame[1] = SkinColor[ColorIndex].Color3;
    SliderSkin.aColorInner[0] = SkinColor[ColorIndex].Color3;
    SliderSkin.aColorInner[1] = SkinColor[ColorIndex].Color3;
    SliderSkin.aColorShaft[0] = SkinColor[ColorIndex].Color1;
    SliderSkin.aColorShaft[1] = SkinColor[ColorIndex].Color1;
    SliderSkin.aColorShaft[2] = SkinColor[ColorIndex].Color2;
    SLIDER_SetSkinFlexProps(&SliderSkin, SLIDER_SKINFLEX_PI_PRESSED);

    /* ��ܴ��� */
    FRAMEWIN_GetSkinFlexProps(&FramewinSkin, FRAMEWIN_CI_ACTIVE);
    FramewinSkin.aColorTitle[0] = SkinColor[ColorIndex].Color3;
    FramewinSkin.aColorTitle[1] = SkinColor[ColorIndex].Color3;
    FramewinSkin.aColorFrame[0] = SkinColor[ColorIndex].Color3;
    FramewinSkin.aColorFrame[1] = SkinColor[ColorIndex].Color3;
    FramewinSkin.aColorFrame[2] = SkinColor[ColorIndex].Color3;
    FRAMEWIN_SetSkinFlexProps(&FramewinSkin, FRAMEWIN_CI_ACTIVE);

    FRAMEWIN_GetSkinFlexProps(&FramewinSkin, FRAMEWIN_CI_INACTIVE);
    FramewinSkin.aColorTitle[0] = SkinColor[ColorIndex].Color2;
    FramewinSkin.aColorTitle[1] = SkinColor[ColorIndex].Color2;
    FramewinSkin.aColorFrame[0] = SkinColor[ColorIndex].Color2;
    FramewinSkin.aColorFrame[1] = SkinColor[ColorIndex].Color2;
    FramewinSkin.aColorFrame[2] = SkinColor[ColorIndex].Color2;
    FRAMEWIN_SetSkinFlexProps(&FramewinSkin, FRAMEWIN_CI_INACTIVE);
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @retval      ��
 */
void emwin_main(void)
{
    WM_HWIN hDlg;
    WM_HWIN _hGraph;
    WM_HWIN ClientWin;
    int Graph_xSize, Graph_ySize;

    _hGraph = 0;
    
    
    emwin_widget_set_skin();

    WM_SetDesktopColor(GUI_BLACK);
#if GUI_SUPPORT_MEMDEV
    WM_SetCreateFlags(WM_CF_MEMDEV);
#endif
    hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);
    ClientWin = WM_GetDialogItem(hDlg, ID_GRAPH_0);
    Graph_xSize = WM_GetWindowSizeX(ClientWin);
    Graph_ySize = WM_GetWindowSizeY(ClientWin);

    oscilldev.dataxsize = Graph_xSize - BORDER_LEFT - BORDER_RIGHT - 3;
    oscilldev.dataysize = Graph_ySize - BORDER_TOP - BORDER_BOTTOM - 3;
    oscilldev.buffer = mymalloc(SRAMIN, 4096); /* �����ڴ� */
    if (oscilldev.buffer == NULL)
    {
        printf("�����ڴ�ʧ��\r\n");
    }

    while (1)
    {
        if (!Craph_Stop)
        {
            if (!_hGraph)
            {
                _hGraph = WM_GetDialogItem(hDlg, ID_GRAPH_0);
            }

            emwin_add_values();
        }
        GUI_Delay(100);
        GUI_Exec();
    }
}

uint16_t g_dac_sin_buf[4096];           /* �������ݻ����� */

/**
 * @brief       �������Ҳ�������
 *   @note      �豣֤: maxval > samples/2
 *
 * @param       maxval : ���ֵ(0 < maxval < 2048)
 * @param       samples: ������ĸ���
 *
 * @retval      ��
 */
void emwin_creat_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint8_t i;
    float inc = (2 * 3.1415962) / samples; /* ����������һ������DAC_SIN_BUF���㣩*/
    float outdata = 0;

    for (i = 0; i < samples; i++)
    {
        outdata = maxval * (1 + sin(inc * i)); /* ������dots����Ϊ���ڵ�ÿ�����ֵ���Ŵ�maxval������ƫ�Ƶ��������� */
        if (outdata > 4095)
            outdata = 4095; /* �����޶� */
        g_dac_sin_buf[i] = outdata;
    }
}

void emwin_graph_init(void)
{
    pwmdac_init(256 - 1, 0);            /* PWM DAC��ʼ��, Fpwm = 72M / 256 =281.25Khz */
    
    /* ��ʼ��DACͨ��1 DMA������� */
    dac_dma_wave_init(1);
    emwin_creat_sin_buf(2048, 100);
    dac_dma_wave_enable(1, 100, 10 - 1, 28 - 1);    /* 300Khz����Ƶ��, 100����, ���Եõ����3KHz�����Ҳ�. */
}
