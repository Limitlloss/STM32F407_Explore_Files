/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2021-12-31
 * @brief       通用定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211231
 * 第一次发布
 * V1.1 20211231
 * 新增gtim_timx_pwm_chy_init函数
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 通用定时器 定义 */

/* TIMX 中断定义 
 * 默认是针对TIM2~TIM5, TIM9~TIM14.
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM14任意一个定时器.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ RCC->APB1ENR |= 1 << 1; }while(0)  /* TIM3 时钟使能 */


/* TIMX PWM输出定义 
 * 这里输出的PWM控制LED0(RED)的亮度
 * 默认是针对TIM2~TIM5, TIM9~TIM14.
 * 注意: 通过修改这8个宏定义,可以支持TIM1~TIM14任意一个定时器,任意一个IO口输出PWM
 */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOF
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          SYS_GPIO_PIN9
#define GTIM_TIMX_PWM_CHY_GPIO_AF           9                           /* AF功能选择 */
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ RCC->AHB1ENR |= 1 << 5; }while(0)   /* PF口时钟使能 */

#define GTIM_TIMX_PWM                       TIM14 
#define GTIM_TIMX_PWM_CHY                   1                           /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX              TIM14->CCR1                 /* 通道Y的输出比较寄存器 */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ RCC->APB1ENR |= 1 << 8; }while(0)   /* TIM14 时钟使能 */

/******************************************************************************************/


void gtim_timx_int_init(uint16_t arr, uint16_t psc);        /* 通用定时器 定时中断初始化函数 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);    /* 通用定时器 PWM初始化函数 */


#endif

















