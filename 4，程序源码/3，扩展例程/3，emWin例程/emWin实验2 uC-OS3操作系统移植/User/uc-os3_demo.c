/**
 ****************************************************************************************************
 * @file        uc-os3_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-03-12
 * @brief       uC-OS3-emWin ��ֲʵ��
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

#include "uc-os3_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "os.h"
#include "cpu.h"
#include "GUI.h"
#include "GUIDemo.h"
#include "WM.h"


/******************************************************************************************/
/* UCOSIII�������� */

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     3           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
OS_TCB StartTaskTCB;                    /* ������ƿ� */
CPU_STK START_TASK_STK[START_STK_SIZE]; /* �����ջ */
void start_task(void *p_arg);           /* ������ */

/* USER_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define USER_TASK_PRIO     4            /* �������ȼ� */
#define USER_STK_SIZE      1024         /* �����ջ��С */
OS_TCB User_TaskTCB;                    /* ������ƿ� */
CPU_STK USER_TASK_STK[USER_STK_SIZE];   /* �����ջ */
void user_task(void *p_arg);

/* TOUCH_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TOUCH_TASK_PRIO     5           /* �������ȼ� */
#define TOUCH_STK_SIZE      512         /* �����ջ��С */
OS_TCB Touch_TaskTCB;                   /* ������ƿ� */
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; /* �����ջ */
void touch_task(void *p_arg);

/******************************************************************************************/


/**
 * @brief       ����emWin����
 * @param       ��
 * @retval      ��
 */
void uc_os3_demo(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    OSInit(&err);          /* ��ʼ��UCOSIII */
    CPU_CRITICAL_ENTER();  /* �����ٽ��� */
    /* ������ʼ���� */
    OSTaskCreate((OS_TCB *     )&StartTaskTCB,       /* ������ƿ� */
                 (CPU_CHAR *   )"start task",        /* �������� */
                 (OS_TASK_PTR  )start_task,          /* ������ */
                 (void *       )0,                   /* ���ݸ��������Ĳ��� */
                 (OS_PRIO      )START_TASK_PRIO,     /* �������ȼ� */
                 (CPU_STK *    )&START_TASK_STK[0],  /* �����ջ����ַ */
                 (CPU_STK_SIZE )START_STK_SIZE / 10, /* �����ջ�����λ */
                 (CPU_STK_SIZE )START_STK_SIZE,      /* �����ջ��С */
                 (OS_MSG_QTY   )0,                   /* �����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ */
                 (OS_TICK      )0,                   /* ��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��� */
                 (void *       )0,                   /* �û�����Ĵ洢�� */
                 (OS_OPT       )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, /* ����ѡ�� */
                 (OS_ERR *     )&err);  /* ��Ÿú�������ʱ�ķ���ֵ */

    CPU_CRITICAL_EXIT();  /* �˳��ٽ��� */
    OSStart(&err);       /* ����UCOSIII */
}

/**
 * @brief       start_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;
    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);     /* ͳ������ */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN        /* ���ʹ���˲����жϹر�ʱ�� */
    CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN       /* ��ʹ��ʱ��Ƭ��ת��ʱ�� */
    /* ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms */
    OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif
    
    __HAL_RCC_CRC_CLK_ENABLE();       /* ʹ��CRCʱ�� */
    GUI_Init();                       /* STemWin ��ʼ�� */

    CPU_CRITICAL_ENTER();             /* �����ٽ��� */
    /* ����User���� */
    OSTaskCreate((OS_TCB *    )&User_TaskTCB,
                 (CPU_CHAR *  )"User task",
                 (OS_TASK_PTR )user_task,
                 (void *      )0,
                 (OS_PRIO     )USER_TASK_PRIO,
                 (CPU_STK *   )&USER_TASK_STK[0],
                 (CPU_STK_SIZE)USER_STK_SIZE / 10,
                 (CPU_STK_SIZE)USER_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ����Touch���� */
    OSTaskCreate((OS_TCB *    )&Touch_TaskTCB,
                 (CPU_CHAR *  )"Touch task",
                 (OS_TASK_PTR )touch_task,
                 (void *      )0,
                 (OS_PRIO     )TOUCH_TASK_PRIO,
                 (CPU_STK *   )&TOUCH_TASK_STK[0],
                 (CPU_STK_SIZE)TOUCH_STK_SIZE / 10,
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    CPU_CRITICAL_EXIT();           /* �˳��ٽ��� */
    OSTaskDel((OS_TCB *)0, &err);  /* ɾ��start_task�������� */
}

/**
 * @brief       ִ��emwin����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void user_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    GUIDEMO_Main();
    
    while (1)
    {
        OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);  /* ��ʱ5ms */
    }
}

/**
 * @brief       emwin����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void touch_task(void *p_arg)
{
    p_arg = p_arg;
    GUI_CURSOR_Show();

    while (1)
    {
        GUI_TOUCH_Exec();
        GUI_Delay(5);
    }
}
