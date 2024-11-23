/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��JPEGͼƬ��ʾʵ��
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
#include "./FATFS/source/ff.h"
#include "./SYSTEM/usart/usart.h"
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


static FIL JPEGFile;
static FIL ScrSortFile; /* ��Ļ��ͼ�ļ� */
static char jpegBuffer[JPEGPERLINESIZE];
extern GUI_FONT TTF12_Font;
extern GUI_FONT TTF18_Font;
extern GUI_FONT TTF24_Font;
extern GUI_FONT TTF36_Font;

/*******************************************************************
*
*       Static functions
*
********************************************************************
*/
/*********************************************************************
*
*       _GetData
*
* Function description
*   This routine is called by GUI_JPEG_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int emwin_jpeg_get_data(void *p, const uint8_t **ppData, unsigned NumBytesReq, uint32_t Off)
{
    static int readaddress = 0;
    FIL *phFile;
    UINT NumBytesRead;

    phFile = (FIL *)p;

    if (NumBytesReq > sizeof(jpegBuffer))
    {
        NumBytesReq = sizeof(jpegBuffer);
    }

    /*�ƶ�ָ�뵽Ӧ�ö�ȡ��λ�� */
    if (Off == 1) 
    {
        readaddress = 0;
    }
    else 
    {
        readaddress = Off;
    }

    f_lseek(phFile, readaddress);

    /* ��ȡ���ݵ��������� */
    f_read(phFile, jpegBuffer, NumBytesReq, &NumBytesRead);

    *ppData = (uint8_t *)jpegBuffer;
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
    uint16_t index=0;

    while(index<0XFFFF)
    {
        sprintf((char*)pname,"0:/SCREENSHORT/��Ļ��ͼ%05d.bmp",index);
        res=f_open(&ScrSortFile,(const TCHAR*)pname,FA_READ);/* ���Դ�����ļ� */

        if (res==FR_NO_FILE)
        {
            break;                            /* ���ļ���������=����������Ҫ�� */
        }
        else if (res==FR_OK)
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
static void emwin_write_byte_1_2file(U8 Data, void * p) 
{
    UINT nWritten;
    f_write((FIL*)p,&Data,1,&nWritten);
}

/**
 * @brief       ����һ��BMPͼƬ�����浽SD����,ʵ����Ļ��ͼ���ܣ�
 * @param       x0:����BMP�ļ���X��ʼλ��
 * @param       y0:����BMP�ļ���Y��ʼλ��
 * @param       Xsize:X�Ĵ�С
 * @param       Ysize:Y�Ĵ�С
 * @retval      ��
 */
void emwin_create_jpgpicture(uint8_t *filename,int x0,int y0,int Xsize,int Ysize)
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
 * @brief       ��ָ��λ����ʾ���ص�RAM�е�JPEGͼƬ
                JPEGFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
 * @param       X:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displyjpeg(uint8_t *JPEGFileName, uint8_t mode, uint32_t x, uint32_t y, int member, int denom)
{
    uint16_t bread;
    int flag;
    char *jpegbuffer;
    char result;
    int XSize, YSize;
    GUI_JPEG_INFO JpegInfo;
    float Xflag, Yflag;


    result = f_open(&JPEGFile, (const TCHAR *)JPEGFileName, FA_READ); /* ���ļ� */

    /*�ļ��򿪴�������ļ�����JPEGMEMORYSIZE*/
    if ((result != FR_OK) || (JPEGFile.obj.objsize > JPEGMEMORYSIZE))
    {
        printf("�ļ��򿪴���\r\n");
        return 1;
    }

    jpegbuffer = mymalloc(SRAMIN, JPEGFile.obj.objsize); /* �����ڴ� */

    if (jpegbuffer == NULL) 
    {
        printf("�����ڴ�ʧ��\r\n");
        return 2;
    }


    result = f_read(&JPEGFile, jpegbuffer, JPEGFile.obj.objsize, (UINT *)&bread); /* ��ȡ���� */

    if (result != FR_OK)
    {
        printf("��ȡʧ��\r\n");
        return 3;
    }

    GUI_JPEG_GetInfo(jpegbuffer, JPEGFile.obj.objsize, &JpegInfo); /* ��ȡJEGPͼƬ��Ϣ */
    XSize = JpegInfo.XSize;   /* ��ȡJPEGͼƬ��X���С */
    YSize = JpegInfo.YSize;   /* ��ȡJPEGͼƬ��Y���С */

    switch (mode)
    {
        case 0: /*��ָ��λ����ʾͼƬ*/
            if ((member == 1) && (denom == 1)) /*�������ţ�ֱ�ӻ���*/
            {
                flag = GUI_JPEG_Draw(jpegbuffer, JPEGFile.obj.objsize, x, y); /*��ָ��λ����ʾJPEGͼƬ*/
                if (flag != 0)
                {
                    printf("����0\r\n");
                }
            }
            else  /*����ͼƬ��Ҫ����*/
            {
                GUI_JPEG_DrawScaled(jpegbuffer, JPEGFile.obj.objsize, x, y, member, denom);
            }

            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��LCD�м���ʾͼƬ */
                GUI_JPEG_Draw(jpegbuffer, JPEGFile.obj.objsize, (lcddev.width - XSize) / 2 - 1, (lcddev.height - YSize) / 2 - 1);
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                Xflag = (float)XSize * ((float)member / (float)denom);
                Yflag = (float)YSize * ((float)member / (float)denom);
                XSize = (lcddev.width - (int)Xflag) / 2 - 1;
                YSize = (lcddev.height - (int)Yflag) / 2 - 1;
                GUI_JPEG_DrawScaled(jpegbuffer, JPEGFile.obj.objsize, XSize, YSize, member, denom);
            }

            break;
    }

    f_close(&JPEGFile);         /* �ر�JPEGFile�ļ� */
    myfree(SRAMIN, jpegbuffer); /* �ͷ��ڴ� */
    return 0;
}

/**
 * @brief       ��ָ��λ����ʾ������ص�RAM�е�BMPͼƬ(���ļ�ϵͳ֧�֣�����СRAM���Ƽ�ʹ�ô˷�����)
                JPEGFileName:ͼƬ��SD�����������洢�豸�е�·��
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
 * @param       X:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displayjpegex(uint8_t *JPEGFileName, uint8_t mode, uint32_t x, uint32_t y, int member, int denom)
{
    volatile char result;
    volatile int flag;
    volatile int XSize, YSize;
    volatile float Xflag, Yflag;
    GUI_JPEG_INFO JpegInfo;

    result = f_open(&JPEGFile, (const TCHAR *)JPEGFileName, FA_READ); /* ���ļ� */

    /* �ļ��򿪴��� */
    if (result != FR_OK)
    {
        printf("�ļ��򿪴���\r\n");
        return 1;
    }

    GUI_JPEG_GetInfoEx((GUI_GET_DATA_FUNC *)emwin_jpeg_get_data, &JPEGFile, &JpegInfo);
    XSize = JpegInfo.XSize; /* JPEGͼƬX��С */
    YSize = JpegInfo.YSize; /* JPEGͼƬY��С */

    switch (mode)
    {
        case 0:   /*��ָ��λ����ʾͼƬ*/
            if ((member == 1) && (denom == 1))   /*�������ţ�ֱ�ӻ���*/
            {
                flag = GUI_JPEG_DrawEx((GUI_GET_DATA_FUNC *)emwin_jpeg_get_data, &JPEGFile, x, y); /*��ָ��λ����ʾBMPͼƬ*/
                if (flag != 0)
                {
                    printf("����1\r\n");
                }
            }
            else   /*����ͼƬ��Ҫ����*/
            {
                GUI_JPEG_DrawScaledEx((GUI_GET_DATA_FUNC *)emwin_jpeg_get_data, &JPEGFile, x, y, member, denom);
            }

            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            if ((member == 1) && (denom == 1)) /* �������ţ�ֱ�ӻ��� */
            {
                /* ��LCD�м���ʾͼƬ */
                GUI_JPEG_DrawEx((GUI_GET_DATA_FUNC *)emwin_jpeg_get_data, &JPEGFile, (lcddev.width - XSize) / 2 - 1, (lcddev.height - YSize) / 2 - 1);
            }
            else  /* ����ͼƬ��Ҫ���� */
            {
                Xflag = (float)XSize * ((float)member / (float)denom);
                Yflag = (float)YSize * ((float)member / (float)denom);
                XSize = (lcddev.width - (int)Xflag) / 2 - 1;
                YSize = (lcddev.height - (int)Yflag) / 2 - 1;
                GUI_JPEG_DrawScaledEx((GUI_GET_DATA_FUNC *)emwin_jpeg_get_data, &JPEGFile, XSize, YSize, member, denom);
            }

            break;
    }

    f_close(&JPEGFile);  /* �ر�BMPFile�ļ� */
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
    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_FontHZ16);
    GUI_Clear();

    while (1)
    {
        GUI_DispStringHCenterAt("��ָ��λ����ʾһ��������ص�JPEGͼƬ", 240, 0);
        emwin_displayjpegex((uint8_t *)"0:/PICTURE/JPEG/ʵ��ͼƬ.jpg", 0, 0, 0, 1, 1);
        GUI_Delay(1000);
        GUI_Clear();

        GUI_DispStringHCenterAt("��LCD�м���ʾһ������2����������ص�JPEGͼƬ", 240, 0);
        emwin_displayjpegex((uint8_t *)"0:/PICTURE/JPEG/jpeg0.jpg", 1, 0, 0, 2, 1);
        GUI_Delay(1000);
        GUI_Clear();

        GUI_DispStringHCenterAt("��LCD�м���ʾһ����С1/2��������ص�JPEGͼƬ", 240, 0);
        emwin_displayjpegex((uint8_t *)"0:/PICTURE/JPEG/SIM900A.jpg", 1, 0, 0, 1, 2);
        GUI_Delay(1000);
        GUI_Clear();
    }
}
