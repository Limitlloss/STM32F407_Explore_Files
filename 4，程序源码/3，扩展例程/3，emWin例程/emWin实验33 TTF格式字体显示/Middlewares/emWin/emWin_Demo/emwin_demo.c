/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��TTF��ʽ������ʾʵ��
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

#include "./FATFS/source/ff.h"
#include "emwin_demo.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/usart/usart.h"
#include "GUI.h"
#include "os.h"
#include "cpu.h"


static GUI_TTF_CS Cs0, Cs1, Cs2, Cs3;
FIL TTFFontFile;
FSIZE_t  TTFFontFileSize;
GUI_FONT TTF12_Font;
GUI_FONT TTF18_Font;
GUI_FONT TTF24_Font;
GUI_FONT TTF36_Font;
GUI_TTF_DATA TTFData;

/**
 * @brief       ����TTF���壬��EMWINʹ��
 * @param       fxpath:���TTF�ֿ��·��
 * @retval      0,�ɹ�������ֵ��ʧ��
 */
int emwin_create_ttf_font(uint8_t *fxpath)
{
    CPU_SR_ALLOC();
    int result;
    uint16_t bread;
    char *TtfBuffer;

    result = f_open(&TTFFontFile, (const TCHAR *)fxpath, FA_READ);  /* ���ֿ��ļ� */ 
  
    if (result != FR_OK)
    {
        return 1;
    }
  

    TtfBuffer = mymalloc(SRAMEX, TTFFontFile.obj.objsize); /* �����ڴ� */
    
    if (TtfBuffer == NULL) 
    {
        printf("�ڴ�����ʧ��\r\n");
        return 3; /* �ڴ�����ʧ�� */
    }

    /* ��ȡ�������� */
#if SYS_SUPPORT_OS
    CPU_CRITICAL_ENTER();       /* �����ٽ��� */
#endif

    result = f_read(&TTFFontFile, TtfBuffer, TTFFontFile.obj.objsize, (UINT *)&bread); /* ��ȡ���� */

    if (result != FR_OK) 
    {
        return 4; /* �ļ���ʧ�ܣ����� */
    }

    f_close(&TTFFontFile); /* �ر�TTFFointFile�ļ�*/
#if SYS_SUPPORT_OS
    CPU_CRITICAL_EXIT();   /* �˳��ٽ��� */
#endif

    TTFData.pData = TtfBuffer; /* ָ���ļ���ַ */
    TTFData.NumBytes = TTFFontFile.obj.objsize; /* �ļ���С*/

    Cs0.pTTF        = &TTFData;
    Cs0.PixelHeight = 12;
    Cs0.FaceIndex   = 0;

    Cs1.pTTF        = &TTFData;
    Cs1.PixelHeight = 18;
    Cs1.FaceIndex   = 0;

    Cs2.pTTF        = &TTFData;
    Cs2.PixelHeight = 24;
    Cs2.FaceIndex   = 0;

    Cs3.pTTF        = &TTFData;
    Cs3.PixelHeight = 36;
    Cs3.FaceIndex   = 0;

    result = GUI_TTF_CreateFont(&TTF12_Font, &Cs0); /* �������� */

    if (result) return 5; /* ���崴��ʧ��*/

    result = GUI_TTF_CreateFont(&TTF18_Font, &Cs1); /* �������� */

    if (result) return 5; /* ���崴��ʧ��*/

    result = GUI_TTF_CreateFont(&TTF24_Font, &Cs2); /* ��������*/

    if (result) return 5; /* ���崴��ʧ�� */

    result = GUI_TTF_CreateFont(&TTF36_Font, &Cs3); /* ��������*/

    if (result) return 5; /* ���崴��ʧ�� */

    return 0;
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{
    int result;
    result = emwin_create_ttf_font((uint8_t *)"0:/SYSTEM/EMWINFONT/TTF/comesinhandy.ttf");

    if(result) 
    {
        printf("TTF���崴��ʧ��,%d\r\n",result);
    }
    
    GUI_SetBkColor(GUI_CYAN);
    GUI_SetColor(GUI_RED);
    GUI_UC_SetEncodeUTF8();
    GUI_Clear();
    GUI_SetFont(&TTF12_Font);
    GUI_DispStringAt("ALIENTEK STemWin TTF Font Test 0123456789!",5,10);
    GUI_SetFont(&TTF18_Font);
    GUI_DispStringAt("ALIENTEK STemWin TTF Font Test 0123456789!",5,25);
    GUI_SetFont(&TTF24_Font);
    GUI_DispStringAt("ALIENTEK STemWin TTF Font Test 0123456789!",5,45);
    GUI_SetFont(&TTF36_Font);
    GUI_DispStringAt("ALIENTEK STemWin TTF Font Test 0123456789!",5,70);
}
