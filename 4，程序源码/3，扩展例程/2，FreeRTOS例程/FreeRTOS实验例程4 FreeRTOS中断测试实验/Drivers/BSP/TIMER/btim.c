/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       ������ʱ�� ��������
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
 * V1.0 20211015
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/USART/usart.h"

TIM_HandleTypeDef g_tim3_handle;        /* ��ʱ��3��� */
TIM_HandleTypeDef g_tim5_handle;        /* ��ʱ��5��� */

/**
 * @brief       ������ʱ��TIM3�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM3_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim3_handle);  /*��ʱ���ص�����*/
}

/**
 * @brief       ������ʱ��TIM5�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM5_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim5_handle);  /*��ʱ���ص�����*/
}

/**
 * @brief       �ص���������ʱ���жϷ���������
 * @param       ��
 * @retval      ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&g_tim3_handle))
    {
        printf("TIM3���\r\n");
    }
    else if (htim == (&g_tim5_handle))
    {
        printf("TIM5���\r\n");
    }
}

/**
 * @brief       ������ʱ��TIM3��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim3_int_init(uint16_t arr, uint16_t psc)
{
    g_tim3_handle.Instance = BTIM_TIM3_INT;                      /*ͨ�ö�ʱ��3*/
    g_tim3_handle.Init.Prescaler = psc;                          /* ����Ԥ��Ƶ��  */
    g_tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ���ϼ����� */
    g_tim3_handle.Init.Period = arr;                             /*�Զ�װ��ֵ*/
    g_tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   /*ʱ�ӷ�Ƶ����*/
    HAL_TIM_Base_Init(&g_tim3_handle);

    HAL_TIM_Base_Start_IT(&g_tim3_handle);                       /*ʹ��ͨ�ö�ʱ��3�ͼ�������жϣ�TIM_IT_UPDATE*/
}

/**
 * @brief       ������ʱ��TIM5��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim5_int_init(uint16_t arr, uint16_t psc)
{
    g_tim5_handle.Instance = BTIM_TIM5_INT;                      /*ͨ�ö�ʱ��5*/
    g_tim5_handle.Init.Prescaler = psc;                          /* ����Ԥ��Ƶ��  */
    g_tim5_handle.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ���ϼ����� */
    g_tim5_handle.Init.Period = arr;                             /*�Զ�װ��ֵ*/
    g_tim5_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   /*ʱ�ӷ�Ƶ����*/
    HAL_TIM_Base_Init(&g_tim5_handle);

    HAL_TIM_Base_Start_IT(&g_tim5_handle);                       /*ʹ��ͨ�ö�ʱ��5�ͼ�������жϣ�TIM_IT_UPDATE*/
}

/**
 * @brief       ��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
                �˺����ᱻHAL_TIM_Base_Init()��������
 * @param       ��
 * @retval      ��
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIM3_INT)
    {
        BTIM_TIM3_INT_CLK_ENABLE();                     /*ʹ��TIMʱ��*/
        HAL_NVIC_SetPriority(BTIM_TIM3_INT_IRQn, 4, 0); /* ��ռ4�������ȼ�0����4 */
        HAL_NVIC_EnableIRQ(BTIM_TIM3_INT_IRQn);         /*����ITM3�ж�*/
    }
    else if (htim->Instance == BTIM_TIM5_INT)
    {
        BTIM_TIM5_INT_CLK_ENABLE();                     /*ʹ��TIMʱ��*/
        HAL_NVIC_SetPriority(BTIM_TIM5_INT_IRQn, 6, 0); /* ��ռ6�������ȼ�0����4 */
        HAL_NVIC_EnableIRQ(BTIM_TIM5_INT_IRQn);         /*����ITM5�ж�*/
    }
}
