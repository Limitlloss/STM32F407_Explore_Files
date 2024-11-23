/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��GIFͼƬ��ʾʵ��
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
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "EmWinHZFont.h"
#include "GUI.h"
#include "./MALLOC/malloc.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./BSP/LCD/lcd.h"
/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "EmWinHZFont.h"
#include "WM.h"
/*****************************************************************************************************/
/*uC-OS3*/
#include "os.h"
#include "cpu.h"


static FIL GIFFile;
static FIL ScrSortFile; /* ��Ļ��ͼ�ļ� */
static char gifBuffer[GIFPERLINESIZE];


/**
 * @brief       ���ڽ�����ָ������Ϊ���پ�����Ч����λ��
 * @param       p:             ָ��Ӧ�ó����������
 * @param       NumBytesReq:   �����ֽ���
 * @param       ppData:        ����ָ���ָ�롣���ָ��Ӧ������Ϊ��Чλ��.
 * @param       StartOfFile:   ����ñ�־Ϊ1��������ָ��Ӧ����Ϊ�������Ŀ�ʼ
 * @retval      ���������ֽ���.
 */
static int emwin_gif_get_data(void *p, const U8 **ppData, unsigned NumBytesReq, U32 Off)
{
    static int readaddress = 0;
    FIL *phFile;
    UINT NumBytesRead;

    phFile = (FIL *)p;

    if (NumBytesReq > sizeof(gifBuffer))
    {
        NumBytesReq = sizeof(gifBuffer);
    }

    /* �ƶ�ָ�뵽Ӧ�ö�ȡ��λ�� */
    if (Off == 1) readaddress = 0;
    else readaddress = Off;

    f_lseek(phFile, readaddress);

    /* ��ȡ���ݵ��������� */
    f_read(phFile, gifBuffer, NumBytesReq, &NumBytesRead);


    *ppData = (U8 *)gifBuffer;
    return NumBytesRead;/* ���ض�ȡ�����ֽ��� */
}

/**
 * @brief       �õ�һ���ļ���
 * @param       pname:�õ����ļ���
 * @retval      ��
 */
void emwin_emwinjpg_new_pathname(uint8_t *pname)
{
    uint8_t res;
    uint16_t index = 0;

    while (index < 0XFFFF)
    {
        sprintf((char *)pname, "0:/SCREENSHORT/��Ļ��ͼ%05d.bmp", index);
        res = f_open(&ScrSortFile, (const TCHAR *)pname, FA_READ); /* ���Դ�����ļ� */

        if (res == FR_NO_FILE)
        {
            break;                            /* ���ļ���������=����������Ҫ�� */
        }
        else if (res == FR_OK)
        {
            f_close(&ScrSortFile);           /* ����򿪳ɹ����͹رյ� */
        }

        index++;
    }
}

/**
 * @brief       �˺�����GUI_BMP_Serialize()���ã��������ļ�д���ֽ�
 * @param       ��
 * @retval      ��
 */
static void emwin_write_byte_1_2file(U8 Data, void *p)
{
    UINT nWritten;
    f_write((FIL *)p, &Data, 1, &nWritten);
}

/**
 * @brief       ����һ��BMPͼƬ�����浽SD����,ʵ����Ļ��ͼ���ܣ�
 * @param       x0:����BMP�ļ���X��ʼλ��
 * @param       y0:����BMP�ļ���Y��ʼλ��
 * @param       Xsize:X�Ĵ�С
 * @param       Ysize:Y�Ĵ�С
 * @retval      ��
 */
void emwin_create_gifpicture(uint8_t *filename, int x0, int y0, int Xsize, int Ysize)
{
    static FIL hFile;
    
    GUI_COLOR forecolor, bkcolor;
    forecolor = GUI_GetColor();   /* ��ȡ��ǰǰ��ɫ */
    bkcolor = GUI_GetBkColor();   /* ��ȡ��ǰ����ɫ */
    /* ����һ���ļ���·��Ϊfilename,����ļ�ԭ���ʹ��ڵĻ��ᱻ�½����ļ����ǵ���*/
    f_open(&hFile,(const TCHAR*)filename,FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    GUI_BMP_SerializeEx(emwin_write_byte_1_2file,x0,y0,Xsize,Ysize,&hFile);
    f_close(&hFile);  /* �ر��ļ� */
    GUI_SetColor(forecolor);   /* �ָ�ǰ��ɫ */
    GUI_SetBkColor(bkcolor);   /* �ָ�����ɫ */
}

/**
 * @brief       ��ָ��λ����ʾ���ص�RAM�е�GIFͼƬ
                GIFFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч
 * @param       x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displaygif(char *GIFFileName, uint8_t mode, uint32_t x, uint32_t y, int member, int denom)
{
    int i;
    uint16_t bread;
    char *gifbuffer;
    char result;
    int XSize, YSize;
    GUI_GIF_INFO GifInfo;
    GUI_GIF_IMAGE_INFO ImageInfo;
    float Xflag, Yflag;


    result = f_open(&GIFFile, (const TCHAR *)GIFFileName, FA_READ); /* ���ļ� */

    /* �ļ��򿪴�������ļ�����JPEGMEMORYSIZE */
    if ((result != FR_OK) || (GIFFile.obj.objsize > GIFMEMORYSIZE)) return 1;

    gifbuffer = mymalloc(SRAMIN, GIFFile.obj.objsize);

    if (gifbuffer == NULL) return 2;


    result = f_read(&GIFFile, gifbuffer, GIFFile.obj.objsize, (UINT *)&bread); /* ��ȡ���� */

    if (result != FR_OK) return 3;


    GUI_GIF_GetInfo(gifbuffer, GIFFile.obj.objsize, &GifInfo); /* ��ȡGIFͼƬ��Ϣ */

    XSize = GifInfo.xSize; /* ��ȡGIFͼƬ��X���С */
    YSize = GifInfo.ySize; /* ��ȡGIFͼƬ��Y���С */

    switch (mode)
    {
        case 0: /* ��ָ��λ����ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��ָ��λ����ʾJPEGͼƬ */
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSub(gifbuffer, GIFFile.obj.objsize, x, y, i);
                    GUI_GIF_GetImageInfo(gifbuffer, GIFFile.obj.objsize, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /*��ʱ*/
                }
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubScaled(gifbuffer, GIFFile.obj.objsize, x, y, i, member, denom);
                    GUI_GIF_GetImageInfo(gifbuffer, GIFFile.obj.objsize, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /* ��ʱ */
                }
            }

            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��LCD�м���ʾͼƬ */
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSub(gifbuffer, GIFFile.obj.objsize, (lcddev.width - XSize) / 2 - 1, (lcddev.height - YSize) / 2 - 1, i);
                    GUI_GIF_GetImageInfo(gifbuffer, GIFFile.obj.objsize, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /* ��ʱ */
                }
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                Xflag = (float)XSize * ((float)member / (float)denom);
                Yflag = (float)YSize * ((float)member / (float)denom);
                XSize = (lcddev.width - (int)Xflag) / 2 - 1;
                YSize = (lcddev.height - (int)Yflag) / 2 - 1;

                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubScaled(gifbuffer, GIFFile.obj.objsize, XSize, YSize, i, member, denom);
                    GUI_GIF_GetImageInfo(gifbuffer, GIFFile.obj.objsize, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /* ��ʱ */
                }
            }

            break;
    }

    f_close(&GIFFile);         /* �ر�JPEGFile�ļ� */
    myfree(SRAMIN, gifbuffer); /* �ͷ��ڴ� */
    return 0;
}

/**
 * @brief       ָ��λ����ʾ������ص�RAM�е�GIFͼƬ(���ļ�ϵͳ֧�֣�����СRAM���Ƽ�ʹ�ô˷�����)
                GIFFileName:ͼƬ��SD�����������洢�豸�е�·��
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч
 * @param       x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displaygifex(char *GIFFileName, uint8_t mode, uint32_t x, uint32_t y, int member, int denom)
{
    char result;
    int i;
    int XSize, YSize;
    float Xflag, Yflag;
    GUI_GIF_INFO GifInfo;
    GUI_GIF_IMAGE_INFO ImageInfo;

    result = f_open(&GIFFile, (const TCHAR *)GIFFileName, FA_READ); /* ���ļ� */

    /* �ļ��򿪴��� */
    if (result != FR_OK)    return 1;

    GUI_GIF_GetInfoEx(emwin_gif_get_data, &GIFFile, &GifInfo);
    XSize = GifInfo.xSize; /* GIFͼƬX��С */
    YSize = GifInfo.ySize; /* GIFͼƬY��С */

    switch (mode)
    {
        case 0: /* ��ָ��λ����ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��ָ��λ����ʾBMPͼƬ */
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubEx(emwin_gif_get_data, &GIFFile, x, y, i);
                    GUI_GIF_GetImageInfoEx(emwin_gif_get_data, &GIFFile, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /*��ʱ*/
                }
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubScaledEx(emwin_gif_get_data, &GIFFile, x, y, i, member, denom);
                    GUI_GIF_GetImageInfoEx(emwin_gif_get_data, &GIFFile, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /* ��ʱ */
                }
            }

            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��LCD�м���ʾͼƬ */
                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubEx(emwin_gif_get_data, &GIFFile, (lcddev.width - XSize) / 2 - 1, (lcddev.height - YSize) / 2 - 1, i);
                    GUI_GIF_GetImageInfoEx(emwin_gif_get_data, &GIFFile, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /*��ʱ*/
                }
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                Xflag = (float)XSize * ((float)member / (float)denom);
                Yflag = (float)YSize * ((float)member / (float)denom);
                XSize = (lcddev.width - (int)Xflag) / 2 - 1;
                YSize = (lcddev.height - (int)Yflag) / 2 - 1;

                for (i = 0; i < GifInfo.NumImages; i++)
                {
                    GUI_GIF_DrawSubScaledEx(emwin_gif_get_data, &GIFFile, XSize, YSize, i, member, denom);
                    GUI_GIF_GetImageInfoEx(emwin_gif_get_data, &GIFFile, &ImageInfo, i);
                    GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay * 10 : 100); /* ��ʱ */
                }
            }

            break;
    }


    f_close(&GIFFile); /* �ر�GIFFile�ļ� */
    return 0;
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void emwin_main(void) 
{

    GUI_SetBkColor(GUI_BLUE);
    GUI_SetFont(&GUI_FontHZ16);
    GUI_SetColor(GUI_RED);
    GUI_Clear();

    while (1)
    {
        GUI_DispStringHCenterAt("��ָ��λ����ʾһ��������ص�GIFͼƬ", 400, 0);
        emwin_displaygifex("0:/PICTURE/GIF/��.gif", 0, 20, 20, 1, 1);
        GUI_Delay(1000);
        GUI_Clear();

        GUI_DispStringHCenterAt("��LCD�м���ʾһ����С1/2��������ص�GIFͼƬ", 400, 0);
        emwin_displaygifex("0:/PICTURE/GIF/�����.gif", 1, 0, 0, 1, 2);
        GUI_Delay(1000);
        GUI_Clear();
    }
}
