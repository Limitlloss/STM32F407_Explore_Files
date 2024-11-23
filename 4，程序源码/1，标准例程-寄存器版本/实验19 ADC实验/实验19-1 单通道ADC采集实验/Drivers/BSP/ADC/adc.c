/**
 ****************************************************************************************************
 * @file        adc.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-09
 * @brief       ADC ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220109
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ADC��ʼ������
 *   @note      ������֧��ADC1/ADC2����ͨ��, ���ǲ�֧��ADC3
 *              ����ʹ��12λ����, ADC����ʱ��=21M, ת��ʱ��Ϊ: �������� + 12��ADC����
 *              ��������������: 480, ��ת��ʱ�� = 492 ��ADC���� = 21.87us
 * @param       ��
 * @retval      ��
 */
void adc_init(void)
{
    ADC_ADCX_CHY_GPIO_CLK_ENABLE(); /* IO��ʱ��ʹ�� */
    ADC_ADCX_CHY_CLK_ENABLE();      /* ADCʱ��ʹ�� */
    
    sys_gpio_set(ADC_ADCX_CHY_GPIO_PORT, ADC_ADCX_CHY_GPIO_PIN,
                 SYS_GPIO_MODE_AIN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* AD�ɼ�����ģʽ����,ģ������ */

    RCC->APB2RSTR |= 1 << 8;        /* ADC1 & ADC2 & ADC3 ��λ, ע��, ���︴λ������ADC!!! */
    RCC->APB2RSTR &= ~(1 << 8);     /* ��λ���� */

    /* ADCʱ������ APB2, ��PCLK2, Ƶ��Ϊ84Mhz, ADC���ʱ��һ�㲻Ҫ����36M
     * ��84M PCLK2������, ����ʹ��4��Ƶ, �õ�PCLK2 / 4 = 21Mhz ��ADCʱ��
     */
    ADC->CCR &= ~(3 << 16);         /* ADCPRE[1:0] ADCʱ��Ԥ��Ƶ���� */
    ADC->CCR |= 1 << 16;            /* ����ADCʱ��Ԥ��Ƶϵ��Ϊ 4, �� PCLK2 / 4 = 21Mhz */

    ADC_ADCX->CR1 = 0;              /* CR1���� */
    ADC_ADCX->CR2 = 0;              /* CR2���� */

    ADC_ADCX->CR1 |= 0 << 8;        /* ��ɨ��ģʽ */
    ADC_ADCX->CR1 |= 0 << 24;       /* 12λģʽ */

    ADC_ADCX->CR2 |= 0 << 1;        /* ����ת��ģʽ */
    ADC_ADCX->CR2 |= 0 << 11;       /* �Ҷ��� */
    ADC_ADCX->CR2 |= 0 << 28;       /* ������� */
    
    ADC_ADCX->SQR1 &= ~(0XF << 20); /* L[3:0]���� */
    ADC_ADCX->SQR1 |= 0 << 20;      /* 1��ת���ڹ��������� Ҳ����ֻת����������1 */

    ADC_ADCX->CR2 |= 1 << 0;        /* ����ADת���� */
}

/**
 * @brief       ����ADCͨ������ʱ��
 * @param       adcx : adc�ṹ��ָ��, ADC1 / ADC2
 * @param       ch   : ͨ����, 0~18
 * @param       stime: ����ʱ��  0~7, ��Ӧ��ϵΪ:
 *   @arg       000, 3��ADCʱ������          001, 15��ADCʱ������
 *   @arg       010, 28��ADCʱ������         011, 56��ADCʱ������
 *   @arg       100, 84��ADCʱ������         101, 112��ADCʱ������
 *   @arg       110, 144��ADCʱ������        111, 480��ADCʱ������ 
 * @retval      ��
 */
void adc_channel_set(ADC_TypeDef *adcx, uint8_t ch, uint8_t stime)
{
    if (ch < 10)    /* ͨ��0~9,ʹ��SMPR2���� */
    { 
        adcx->SMPR2 &= ~(7 << (3 * ch));        /* ͨ��ch ����ʱ����� */
        adcx->SMPR2 |= 7 << (3 * ch);           /* ͨ��ch ������������,����Խ�߾���Խ�� */
    }
    else            /* ͨ��10~19,ʹ��SMPR2���� */
    { 
        adcx->SMPR1 &= ~(7 << (3 * (ch - 10))); /* ͨ��ch ����ʱ����� */
        adcx->SMPR1 |= 7 << (3 * (ch - 10));    /* ͨ��ch ������������,����Խ�߾���Խ�� */
    } 
}

/**
 * @brief       ���ADCת����Ľ�� 
 * @param       ch: ͨ����, 0~18
 * @retval      ��
 */
uint32_t adc_get_result(uint8_t ch)
{
    adc_channel_set(ADC_ADCX, ch, 7);   /* ����ADCX��Ӧͨ������ʱ��Ϊ480��ʱ������ */

    ADC_ADCX->SQR3 &= ~(0X1F << 5 * 0); /* ��������1ͨ������ */
    ADC_ADCX->SQR3 |= ch << (5 * 0);    /* ��������1 ͨ�� = ch */
    ADC_ADCX->CR2 |= 1 << 30;           /* ��������ת��ͨ�� */

    while (!(ADC_ADCX->SR & 1 << 1));   /* �ȴ�ת������ */

    return ADC_ADCX->DR;                /* ����adcֵ */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~19
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc_get_result_average(uint8_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++) /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;    /* ����ƽ��ֵ */
}









