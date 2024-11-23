/**
 ****************************************************************************************************
 * @file        i2s.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-14
 * @brief       I2S ��������
 *              ����ʹ�õ���I2S2���I2S
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
 * V1.0 20220114
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __I2S_H
#define __I2S_H
#include "./SYSTEM/SYS/sys.h"


/******************************************************************************************/
/* I2S ���� ���� */

#define I2S_LRCK_GPIO_PORT              GPIOB
#define I2S_LRCK_GPIO_PIN               SYS_GPIO_PIN12
#define I2S_LRCK_GPIO_AF                5                                               /* AF����ѡ�� */
#define I2S_LRCK_GPIO_CLK_ENABLE()      do{ RCC->AHB1ENR |= 1 << 1; }while(0)           /* PB��ʱ��ʹ�� */

#define I2S_SCLK_GPIO_PORT              GPIOB
#define I2S_SCLK_GPIO_PIN               SYS_GPIO_PIN13
#define I2S_SCLK_GPIO_AF                5                                               /* AF����ѡ�� */
#define I2S_SCLK_GPIO_CLK_ENABLE()      do{ RCC->AHB1ENR |= 1 << 1; }while(0)           /* PB��ʱ��ʹ�� */

#define I2S_SDOUT_GPIO_PORT             GPIOC
#define I2S_SDOUT_GPIO_PIN              SYS_GPIO_PIN2
#define I2S_SDOUT_GPIO_AF               6                                               /* AF����ѡ��,����I2S2ext_SD����ѡ6! */
#define I2S_SDOUT_GPIO_CLK_ENABLE()     do{ RCC->AHB1ENR |= 1 << 2; }while(0)           /* PC��ʱ��ʹ�� */

#define I2S_SDIN_GPIO_PORT              GPIOC
#define I2S_SDIN_GPIO_PIN               SYS_GPIO_PIN3
#define I2S_SDIN_GPIO_AF                5                                               /* AF����ѡ�� */
#define I2S_SDIN_GPIO_CLK_ENABLE()      do{ RCC->AHB1ENR |= 1 << 2; }while(0)           /* PC��ʱ��ʹ�� */

#define I2S_MCLK_GPIO_PORT              GPIOC
#define I2S_MCLK_GPIO_PIN               SYS_GPIO_PIN6
#define I2S_MCLK_GPIO_AF                5                                               /* AF����ѡ�� */
#define I2S_MCLK_GPIO_CLK_ENABLE()      do{ RCC->AHB1ENR |= 1 << 2; }while(0)           /* PC��ʱ��ʹ�� */


/* I2S��ض��� */
#define I2S_SPI                         SPI2
#define I2S_SPI_CLK_ENABLE()            do{ RCC->APB1ENR |= 1 << 14; }while(0)          /* I2S ʱ��ʹ�� */

/* I2S DMA��ض��� */
#define I2S_TX_DMASx                    DMA1_Stream4
#define I2S_TX_DMASx_Channel            0
#define I2S_TX_DMASx_IRQHandler         DMA1_Stream4_IRQHandler
#define I2S_TX_DMASx_IRQn               DMA1_Stream4_IRQn
#define I2S_TX_DMA_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 21; }while(0)          /* I2S TX DMAʱ��ʹ�� */

#define I2S_TX_DMASx_IS_TC()            ( DMA1->HISR & (1 << 5) )                       /* �ж� DMA1_Stream4 ������ɱ�־, ����һ���ٺ�����ʽ,
                                                                                         * ���ܵ�����ʹ��, ֻ������if��������� 
                                                                                         */
#define I2S_TX_DMASx_CLR_TC()           do{ DMA1->HIFCR |= 1 << 5; }while(0)            /* ��� DMA1_Stream4 ������ɱ�־ */


/*****************************************************************************************************/

extern void (*i2s_tx_callback)(void);   /* I2S TX�ص�����ָ��  */

void i2s_init(uint32_t std, uint32_t mode, uint32_t cpol, uint32_t datalen);    /* I2S��ʼ�� */
uint8_t i2s_samplerate_set(uint32_t samplerate);                    /* ����I2S�Ĳ����� */
void i2s_tx_dma_init(uint8_t* buf0, uint8_t *buf1, uint16_t num);   /* I2S TX DMA���� */
void i2s_play_start(void);              /* I2S��ʼ���� */
void i2s_play_stop(void);               /* I2Sֹͣ���� */

#endif





















