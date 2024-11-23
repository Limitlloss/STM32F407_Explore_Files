/**
 ****************************************************************************************************
 * @file        test.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-17
 * @brief       ̽��������Ļ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200417
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./TEST/test.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/IIC/myiic.h"
#include "./BSP/LSENS/lsens.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/REMOTE/remote.h"
#include "./MALLOC/malloc.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./BSP/ST480MC/st480mc.h"
#include "./SYSTEM/delay/delay.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"
#include "usbd_conf.h"
#include "./FATFS/source/diskio.h"
#include "./FATFS/source/ff.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/fonts.h"
#include "./BSP/ES8388/es8388.h"
#include "./TEXT/fonts.h"
#include "./TEXT/text.h"
#include "./APP/audioplay.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "./AUDIOCODEC/wav/wavplay.h"
#include <math.h>

/*************************************************************************************************/
USBD_HandleTypeDef USBD_Device;             /* USB Device����ṹ�� */
extern volatile uint8_t g_usb_state_reg;    /* USB״̬ */
extern volatile uint8_t g_device_state;     /* USB���� ��� */

/**
 * @brief       USB���Ӳ���
 * @param       ��
 * @retval      ��
 */
void usb_test(void)
{
    printf("/******************************* 1��USB���� *******************************/\r\n");
    printf("/* ע�⣺�뽫USB�����ӵ�USB_SLAVE�ӿ��ϣ�Ȼ��۲���Դ����Ƿ��г����µĴ��̣� */\r\n");
    printf("/******************************* NEXT: KEY1 *******************************/\r\n\r\n");
    
    uint8_t offline_cnt = 0;
    uint8_t tct = 0;
    uint8_t usb_sta;
    uint8_t device_sta;
    uint16_t id;

    if (sd_init())  /* ��ʼ��SD�� */
    {
        printf("SD����ʼ��ʧ�ܻ���δ�忨������\r\n");
    }
    else            /* SD ������ */
    {
        printf("SD��������������СΪ��%lldM\r\n",SD_TOTAL_SIZE_MB(&g_sdcard_handler));
    }
    
    id = norflash_read_id();
    
    if ((id == 0) || (id == 0XFFFF))
    {
        printf("SPI_FLASH ERROR\r\n");
    }
    else   /* SPI FLASH ���� */
    {
        printf("SPI_FLASH ����\r\n");
    }
        
    USBD_Init(&USBD_Device, &FS_Desc, DEVICE_FS);   /* ��ʼ��USB */
    USBD_RegisterClass(&USBD_Device, &USBD_MSC);    /* ����� */
    USBD_MSC_RegisterStorage(&USBD_Device, &USBD_Storage_Interface_fops_FS);    /* ΪMSC����ӻص����� */
    USBD_Start(&USBD_Device);                       /* ����USB */
    delay_ms(500);
    
    while(1)
    {
        delay_ms(1);

        if (usb_sta != g_usb_state_reg)             /* ״̬�ı��� */
        {
            if (g_usb_state_reg & 0x01)             /* ����д */
            {
                LED1(0);
                printf("USB Writing...\r\n");
            }

            if (g_usb_state_reg & 0x02)             /* ���ڶ� */
            {
                LED1(0);
                printf("USB Reading...\r\n");
            }

            if (g_usb_state_reg & 0x04)
            {
                printf("USB Write Err\r\n");
            }
            
            if (g_usb_state_reg & 0x08)
            {
                printf("USB Read  Err \r\n");
            }

            
            usb_sta = g_usb_state_reg;              /* ��¼����״̬ */
        }

        if (device_sta != g_device_state)
        {
            if (g_device_state == 1)
            {
                printf("USB���ӳɹ�����\r\n");
            }
            else
            {
                printf("USB����ʧ�ܣ���\r\n");
            }
            
            device_sta = g_device_state;
        }

        tct++;
        if (key_scan(0) == KEY1_PRES) break;        /* �����°���KEY1���˳� */
        if (tct == 200)
        {
            tct = 0;
            LED1(1);        /* �ر� LED1 */
            LED0_TOGGLE();  /* LED0 ��˸ */

            if (g_usb_state_reg & 0x10)
            {
                offline_cnt = 0;                    /* USB������,�����offline������ */
                g_device_state = 1;
            }
            else    /* û�еõ���ѯ */
            {
                offline_cnt++;

                if (offline_cnt > 10)
                {
                    g_device_state = 0;             /* 2s��û�յ����߱��,����USB���γ��� */
                }
            }

            g_usb_state_reg = 0;
        }
    }
}

/**
 * @brief       LED����
 * @param       ��
 * @retval      ��
 */
void led_tset()
{
    printf("/******************************* 2��LED���� *******************************/\r\n");
    printf("/********************* ע�⣺�۲�LED0��LED1�Ƿ���˸��*********************/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    while (1)
    {
        LED0_TOGGLE();                              /* LED0��ת */
        LED1_TOGGLE();                              /* LED1��ת */
        delay_ms(50);
        if (key_scan(0) == KEY1_PRES) break;        /* �����°���KEY1���˳� */
    }  
}

/**
 * @brief       ����������
 * @param       ��
 * @retval      ��
 */
void beep_test(void)
{
    printf("/******************************* 3������������ *******************************/\r\n");
    printf("/********************* ע�⣺���·������Ƿ����У�*********************/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    beep_init();
    while (1)
    {  
        BEEP(0);                                    /* �رշ����� */
        delay_ms(50);
        BEEP(1);                                    /* �򿪷����� */
        delay_ms(50);
        if (key_scan(0) == KEY1_PRES) break;        /* �����°���KEY1���˳� */
    }   
    BEEP(0);                                        /* �رշ����� */
}

/**
 * @brief       ��������
 * @param       ��
 * @retval      ��
 */
void key_test(void)
{
    printf("/******************************* 4���������� *******************************/\r\n");
    printf("/*********************����KEY0��KEY1��KEY2��KEY_UP�����˳���*********************/\r\n"); 
    uint8_t key = 0;
    uint8_t status = 0;
    
    while(1)
    {
        key = key_scan(0);
        if (key)
        {
            status |= (1<<(key-1));
            switch(key)
            {
                case KEY0_PRES: printf("����KEY0������\r\n");    break;
                case KEY1_PRES: printf("����KEY1������\r\n");    break;
                case KEY2_PRES: printf("����KEY2������\r\n");    break;
                case WKUP_PRES: printf("����KEY_UP������\r\n");  break;
            }
        }
        if (status == 0x0F) 
        {
            printf("����ȫ�����£������˳���\r\n");
            delay_ms(100);
            return;
        }
    }
}

/**
 * @brief       ����ң�ز���
 * @param       ��
 * @retval      ��
 */
void remote_test(void)
{
    printf("\r\n/******************************* 5������ң�������� *******************************/\r\n");
    printf("/********* ע�⣺���º���ң���������������Ӧ��ֵ��������Power���˳����� *********/\r\n");
    printf("/****************************** NEXT: POWER ******************************/\r\n\r\n");
    uint8_t key , old_key = 0;
    char *str = 0;
    remote_init();
        
    while(1)
    {   
        /* ���Ժ��� */
        key = remote_scan();
        switch (key)
        {
            case 0:
                str = "ERROR";
                break;

            case 69:
                str = "POWER";
                break;

            case 70:
                str = "UP";
                break;

            case 64:
                str = "PLAY";
                break;

            case 71:
                str = "ALIENTEK";
                break;

            case 67:
                str = "RIGHT";
                break;

            case 68:
                str = "LEFT";
                break;

            case 7:
                str = "VOL-";
                break;

            case 21:
                str = "DOWN";
                break;

            case 9:
                str = "VOL+";
                break;

            case 22:
                str = "1";
                break;

            case 25:
                str = "2";
                break;

            case 13:
                str = "3";
                break;

            case 12:
                str = "4";
                break;

            case 24:
                str = "5";
                break;

            case 94:
                str = "6";
                break;

            case 8:
                str = "7";
                break;

            case 28:
                str = "8";
                break;

            case 90:
                str = "9";
                break;

            case 66:
                str = "0";
                break;

            case 74:
                str = "DELETE";
                break;
        }
        if(old_key != key)
        {
            printf("%s\r\n",str);
            old_key = key;
        }
        if (key == 69) break;                       /* �����µ��ݴ����������˳� */
    }

}


/**
 * @brief       ��������
 * @param       ��
 * @retval      ��
 */
void lsens_test(void)
{
    uint8_t i = 0;
    printf("\r\n/******************************* 6���������� *******************************/\r\n");
    printf("/*** ע�⣺�����ڵ�LS1�����۲����ǿ��ֵ�仯��0~100��������Խ����ֵԽ�� ***/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    lsens_init();                                           /* ��ʼ������������ */

    while(1)
    {
        if(i++ > 100)
        {
            printf("����ǿ��ֵ��%d\r\n",lsens_get_val());
            i = 0;
        }
        
        LED0_TOGGLE();
        delay_ms(10);
        if (key_scan(0) == KEY1_PRES) break;                /* �����°���KEY1���˳� */
    }
}
/******************************************* ��������� ******************************************/

/* ������ƽ��У׼���� 
 * У׼ԭ����ο�(����2):http://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
 */
int16_t g_magx_offset;      /* x�Ჹ��ֵ */
int16_t g_magy_offset;      /* y�Ჹ��ֵ */

/**
 * @brief       ��ʾ������ԭʼ����
 * @param       x, y : ����
 * @param       title: ����
 * @param       val  : ֵ
 * @retval      ��
 */
void user_show_mag(uint16_t x, uint16_t y, char * title, int16_t val)
{
    char buf[20];
    sprintf(buf,"%s%d", title, val);                /* ��ʽ����� */
    printf("%s\r\n",buf);
}

/**
 * @brief       ��ʾ��λ��
 * @param       x, y : ����
 * @param       angle: �Ƕ�
 * @retval      ��
 */
void user_show_angle(uint16_t x, uint16_t y, float angle)
{
    char buf[20];
    sprintf(buf,"Angle:%3.1fC", angle);             /* ��ʽ����� */
    printf("��λ�ǣ�%s\r\n",buf);
}

/**
 * @brief       ��ʾ�¶�
 * @param       x, y : ����
 * @param       temp : �¶�
 * @retval      ��
 */
void user_show_temprate(uint16_t x, uint16_t y, float temp)
{
    char buf[20];
    sprintf(buf,"Temp:%2.1fC", temp);               /* ��ʽ����� */
    printf("�¶ȣ�%s\r\n",buf);
}

/**
 * @brief       ����(������)У׼����
 *   @note      ��������ʹ����򵥵�ƽ��У׼����.
 *              ����˺�����,��ˮƽת������������һ��(360��),ת����ɺ�, ��WKUP���˳�!
 * @param       ��
 * @retval      ��
 */
void compass_calibration(void)
{
    int16_t x_min = 0;
    int16_t x_max = 0;
    int16_t y_min = 0;
    int16_t y_max = 0;

    int16_t magx, magy, magz;
    uint8_t res;
    uint8_t key = 0;
    
    printf("����KEY_UP�˳�У׼������\r\n");

    while (1)
    {
        key = key_scan(0);
        
        if (key == WKUP_PRES)   /* ����У׼ */
        {
            break;
        }
        
        res = st480mc_read_magdata(&magx, &magy, &magz);    /* ��ȡ���� */
        
        if (res == 0)
        {
            x_max = x_max < magx ? magx : x_max;            /* ��¼x���ֵ */
            x_min = x_min > magx ? magx : x_min;            /* ��¼x��Сֵ */
            y_max = y_max < magy ? magy : y_max;            /* ��¼y���ֵ */
            y_min = y_min > magy ? magy : y_min;            /* ��¼y��Сֵ */
        }
        
        LED0_TOGGLE();                                      /* LED0��˸,��ʾ�������� */
    }
    
    g_magx_offset = (x_max + x_min) / 2;                    /* X��ƫ���� */
    g_magy_offset = (y_max + y_min) / 2;                    /* Y��ƫ���� */
   
    /* ���ڴ�ӡˮƽУ׼��ز��� */
    printf("x_min:%d\r\n", x_min);
    printf("x_max:%d\r\n", x_max);
    printf("y_min:%d\r\n", y_min);
    printf("y_max:%d\r\n", y_max);
    
    printf("g_magx_offset:%d\r\n", g_magx_offset);
    printf("g_magy_offset:%d\r\n", g_magy_offset);
}

/**
 * @brief       ���̻�ȡ�Ƕ�
 *   @note      ��ȡ��ǰ���̵ĽǶ�(�شŽǶ�)
 * @param       ��
 * @retval      �Ƕ�
 */
float compass_get_angle(void)
{
    float angle;
    int16_t magx, magy, magz;
    
    st480mc_read_magdata_average(&magx, &magy, &magz, 10);  /* ��ȡԭʼ����, 10��ȡƽ�� */
    
    magx = (magx - g_magx_offset) ;     /* ����У׼����, �����µ���� */
    magy = (magy - g_magy_offset) ;     /* ����У׼����, �����µ���� */

    /* ���ݲ�ͬ���������, ���з�λ�ǻ��� */
    if ((magx > 0) && (magy > 0))
    {
        angle = (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx > 0) && (magy < 0))
    {
        angle = 360 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx == 0) && (magy > 0))
    {
        angle = 90;
    }
    else if ((magx == 0) && (magy < 0))
    {
        angle = 270;
    }
    else if (magx < 0)
    {
        angle = 180 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    
    if (angle > 360) angle = 360; /* �޶���λ�Ƿ�Χ */
    if (angle < 0) angle = 0;     /* �޶���λ�Ƿ�Χ */

    return angle;
}


/**
 * @brief       �����Ʋ���
 * @param       ��
 * @retval      ��
 */
void st480mc_test(void)
{
    printf("\r\n/******************************* 7�������Ʋ��� *******************************/\r\n");
    printf("/*********************** ע�⣺�۲��������������Ƿ����� ***********************/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");

    uint8_t t = 0;
    uint8_t key;
    float angle;
    float temperature;
    int16_t magx, magy, magz;
    while (st480mc_init())                              /* ST480MC��ʼ�� */
    {
        printf("ST480MC Error!!\r\n");
        delay_ms(500);
    }
    RST :
    printf("ST480MC OK!!\r\n");
    while(1)
    {
        key = key_scan(0);
        delay_ms(10);
        t++;
        
        if (t == 20)                                    /* 0.2�����Ҹ���һ���¶�/������ԭʼֵ */
        {        
            angle = compass_get_angle();                /* ִ��һ��Լ150ms */
            user_show_angle(30, 130, angle);            /* ��ʾ�Ƕ� */
      
            st480mc_read_temperature(&temperature);     /* ��ȡ��ʪ��ֵ */
            user_show_temprate(30, 150, temperature);   /* ��ʾ�¶� */
            
            st480mc_read_magdata(&magx, &magy, &magz);  /* ��ȡԭʼ���� */
            user_show_mag(30, 170, "MagX:", magx);      /* ��ʾmagx */
            user_show_mag(30, 190, "MagY:", magy);      /* ��ʾmagy */
            user_show_mag(30, 210, "MagZ:", magz);      /* ��ʾmagz */
            
            t = 0;
            LED0_TOGGLE();                              /* LED0��˸ */
        }
        if (key == KEY0_PRES)                           /* KEY0 ���� ,ִ��У׼ */
        {
            compass_calibration();                      /* У׼���� */
            goto RST;                                   /* У׼���,����RST, ������ʾ��ʾ��Ϣ */
        }else if (key == KEY1_PRES) break;              /* �����°���KEY1���˳� */
    }
}

/**
 * @brief       mp3����
 * @param       ��
 * @retval      ��
 */
void wav_test(void)
{
    printf("\r\n/******************************* 8��MP3���� *******************************/\r\n");
    printf("/* ע�⣺�����SD��������SD���贴��MUSIC�ļ��У���test.wav�����ļ�������ȥ */\r\n");
    printf("/****************************** NEXT: KEY0 ******************************/\r\n\r\n");
    
    const char *mp3_path = "0:/MUSIC/TEST.wav";

    f_mount(fs[0], "0:", 1);    /* ����SD�� */
    f_mount(fs[1], "1:", 1);    /* ����SPI FLASH */
    exfuns_init();              /* FATFS �����ڴ� */
    
    while (sd_init())           /* ��ʼ��SD�� */
    {
        printf("SD�������δ����SD��\r\n");
        delay_ms(500);
    }
    es8388_init();              /* ES8388��ʼ�� */
    es8388_adda_cfg(1, 0);      /* ����DAC�ر�ADC */
    es8388_output_cfg(1, 1);    /* DACѡ��ͨ����� */
    es8388_hpvol_set(25);       /* ���ö������� */
    es8388_spkvol_set(30);      /* ������������ */
    es8388_adda_cfg(1, 0);      /* ����DAC�ر�ADC */
    es8388_output_cfg(1, 1);    /* DACѡ��ͨ��1��� */
    
    while(1)
    {
        wav_play_song((char*)mp3_path);
        break;
    }

}

/**
 * @brief       ADC����
 * @param       ��
 * @retval      ��
 */
void adc_tset()
{
    printf("\r\n/******************************* 9��ADC���� *******************************/\r\n");
    printf("/ ע�⣺ʹ�öŰ��߽�P11����ADC��RV1���ж̽ӣ�Ȼ�����RV1�ۿ�ADC�����仯���� /\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    uint16_t adcx,t = 0;
    float temp;
    adc_init();
    while (1)
    {
        if(t++ > 100)
        {
            t = 0;
            adcx = adc_get_result_average(ADC_ADCX_CHY, 10);    /* ��ȡͨ��14��ת��ֵ��10��ȡƽ�� */
            printf("ADCԭʼֵ��%d\r\n",adcx);
            temp = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
            printf("ADC��ѹֵ��%.2fV\r\n",temp);
        }
        delay_ms(10);
        if (key_scan(0) == KEY1_PRES) break;                    /* �����°���KEY1���˳� */
    }  
}
/**
 * @brief       �������
 * @param       ��
 * @retval      ��
 */
void net_test(void)
{
    printf("\r\n/******************************* 10��������� *******************************/\r\n");
    printf("/*************** ע�⣺�뽫���������������߲�����ӵ���̫���ӿ� ***************/\r\n");
    printf("/****************************** NEXT: AUTO ******************************/\r\n\r\n");
    
    while (lwip_comm_init() != 0)
    {
        printf("lwIP Init failed!!\r\n");
        printf("��������ߣ���\r\n"); 
        delay_ms(500);
        LED1_TOGGLE();
    }
    while (!ethernet_read_phy(PHY_SR))  /* ���MCU��PHYоƬ�Ƿ�ͨ�ųɹ� */
    {
        printf("MCU��PHYоƬͨ��ʧ�ܣ������·����Դ�룡������\r\n");
    }
    
    printf("����ͨ�ųɹ�����\r\n");
    ETHERNET_RST(0);                    /* Ӳ����λPHY ���븴λ������ֻҪ�������ߣ��ͻ������ݸ���  */
}

/**
 * @brief       ������������
 * @param       ��
 * @retval      ��
 */
void test_system(void)
{
    printf("\r\n/******************************* ����Ļ�������� *******************************/\r\n");
    
    at24cxx_init();
    while ( at24cxx_check() )                   /* ���24c02�Ƿ���������Ҫ��ȡ24c02��һ����־�ж��Ƿ���й��������� */
    {
        printf("24C02 ERROR!\r\n");
        delay_ms(100);
    }    
        
    /* usb���Ӳ��� */
    usb_test();
    
    /* LED���� */
    led_tset();
    
    /* ���������� */
    beep_test();
    
    /* �������� */
    key_test();
    
    /* ����ң�������� */
    remote_test();
    
    /* �������� */
    lsens_test();
    
    /* �����Ʋ��� */
    st480mc_test();
    
    /* wav���� */
    wav_test();
    
    /* ADC���� */
    adc_tset();
    
    /* ������� */
    net_test();     
    
    printf("\r\n/******************************* ������� *******************************/\r\n");
}



