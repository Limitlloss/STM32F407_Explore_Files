/**
 ****************************************************************************************************
 * @file        uc-os3_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-03-12
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

#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./TEXT/fonts.h"
#include "uc-os3_demo.h"
#include "os.h"
#include "cpu.h"
#include "GUI.h"
#include "EmWinHZFont.h"
#include "WM.h"
#include "emwin_demo.h"


/******************************************************************************************/
/* UCOSIII�������� */

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
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
#define TOUCH_TASK_PRIO     7           /* �������ȼ� */
#define TOUCH_STK_SIZE      512         /* �����ջ��С */
OS_TCB Touch_TaskTCB;                   /* ������ƿ� */
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; /* �����ջ */
void touch_task(void *p_arg);

/* LED0_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED0_TASK_PRIO     6           /* �������ȼ� */
#define LED0_STK_SIZE      128         /* �����ջ��С */
OS_TCB Led0_TaskTCB;                   /* ������ƿ� */
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];  /* �����ջ */
void led0_task(void *p_arg);

/* KEY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define KEY_TASK_PRIO     5            /* �������ȼ� */
#define KEY_STK_SIZE      128          /* �����ջ��С */
OS_TCB Key_TaskTCB;                    /* ������ƿ� */
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];    /* �����ջ */
void key_task(void *p_arg);

/* FONT_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define FONT_TASK_PRIO     3            /* �������ȼ� */
#define FONT_STK_SIZE      512          /* �����ջ��С */
OS_TCB Font_TaskTCB;                    /* ������ƿ� */
CPU_STK FONT_TASK_STK[FONT_STK_SIZE];   /* �����ջ */
void font_task(void *p_arg);

/* IMAGE_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define IMAGE_TASK_PRIO     4           /* �������ȼ� */
#define IMAGE_STK_SIZE      512         /* �����ջ��С */
OS_TCB Image_TaskTCB;                   /* ������ƿ� */
CPU_STK IMAGE_TASK_STK[IMAGE_STK_SIZE]; /* �����ջ */
void image_task(void *p_arg);

OS_SEM    Font_SEM;    /* ����һ���ź��������ڸ������� */
OS_SEM    Imsge_SEM;   /* ����һ���ź��������ڽ�ͼ */

/******************************************************************************************/


/**
 * @brief       ִ��emwin����
 * @param       ��
 * @retval      ��
 */
void uc_os3_demo(void)
{
    OS_ERR err;
    
    /* �ر������ж� */
    CPU_IntDis();
    
    /* ��ʼ��uC/OS-III */
    OSInit(&err);
    
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

    /* ��ʼ������� */
    OSStart(&err);
    
    for (;;)
    {
        /* ����������� */
    }
}

/**
 * @brief       start_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_INT32U cnts;
    p_arg = p_arg;
    
    /* ��ʼ��CPU�� */
    CPU_Init();
    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OSCfg_TickRate_Hz);
    OS_CPU_SysTickInit(cnts);
    
    /* ����ʱ��Ƭ���ȣ�ʱ��Ƭ��ΪĬ��ֵ */
    OSSchedRoundRobinCfg(OS_TRUE, 0, &err);
    
    __HAL_RCC_CRC_CLK_ENABLE();       /* ʹ��CRCʱ�� */
    GUI_Init();                       /* emWin ��ʼ�� */

    /* ����һ�����������ź��� */
    OSSemCreate ((OS_SEM*   )&Font_SEM,
                 (CPU_CHAR* )"Font_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);

    /* ����һ����ͼ�ź��� */
    OSSemCreate ((OS_SEM*   )&Imsge_SEM,
                 (CPU_CHAR* )"IMAGE_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    
    /* ����User���� */
    OSTaskCreate((OS_TCB *    )&User_TaskTCB,
                 (CPU_CHAR *  )"User task",
                 (OS_TASK_PTR )user_task,
                 (void *      )0,
                 (OS_PRIO     )USER_TASK_PRIO,
                 (CPU_STK *   )&USER_TASK_STK,
                 (CPU_STK_SIZE)USER_STK_SIZE / 10,
                 (CPU_STK_SIZE)USER_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ����Led0���� */
    OSTaskCreate((OS_TCB *    )&Led0_TaskTCB,
                 (CPU_CHAR *  )"led0_task",
                 (OS_TASK_PTR )led0_task,
                 (void *      )0,
                 (OS_PRIO     )LED0_TASK_PRIO,
                 (CPU_STK *   )&LED0_TASK_STK,
                 (CPU_STK_SIZE)LED0_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED0_STK_SIZE,
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
                 (CPU_STK *   )&TOUCH_TASK_STK,
                 (CPU_STK_SIZE)TOUCH_STK_SIZE / 10,
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ����KEY���� */
    OSTaskCreate((OS_TCB *    )&Key_TaskTCB,
                 (CPU_CHAR *  )"Keyd task",
                 (OS_TASK_PTR )key_task,
                 (void *      )0,
                 (OS_PRIO     )KEY_TASK_PRIO,
                 (CPU_STK *   )&KEY_TASK_STK,
                 (CPU_STK_SIZE)KEY_STK_SIZE / 10,
                 (CPU_STK_SIZE)KEY_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);
                 
    /* ����FONT���� */
    OSTaskCreate((OS_TCB *    )&Font_TaskTCB,
                 (CPU_CHAR *  )"Font task",
                 (OS_TASK_PTR )font_task,
                 (void *      )0,
                 (OS_PRIO     )FONT_TASK_PRIO,
                 (CPU_STK *   )&FONT_TASK_STK,
                 (CPU_STK_SIZE)FONT_STK_SIZE / 10,
                 (CPU_STK_SIZE)FONT_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);
                 
    /* ����IMAGE���� */
    OSTaskCreate((OS_TCB *    )&Image_TaskTCB,
                 (CPU_CHAR *  )"Image task",
                 (OS_TASK_PTR )image_task,
                 (void *      )0,
                 (OS_PRIO     )IMAGE_TASK_PRIO,
                 (CPU_STK *   )&IMAGE_TASK_STK,
                 (CPU_STK_SIZE)IMAGE_STK_SIZE / 10,
                 (CPU_STK_SIZE)IMAGE_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);
                 
    /* ɾ��Start Task */
    OSTaskDel((OS_TCB *)0, &err);
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
    
    emwin_main();  /* ִ��emwin���� */
    
    while(1)
    {
        OSTimeDly(5, OS_OPT_TIME_DLY, &err);    /* ��ʱ5ticks */
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

/**
 * @brief       ����ϵͳ��������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void led0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    while (1)
    {
        LED0_TOGGLE();
        OSTimeDly(500, OS_OPT_TIME_DLY, &err);     /* ��ʱ500ticks */
    }
}

/**
 * @brief       ��ͼ
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void key_task(void *p_arg)
{
    OS_ERR err;
    uint8_t key;
    p_arg = p_arg;

    while(1)
    {
        key = key_scan(0);

        switch(key)
        {
            case KEY0_PRES:
              printf("���ͽ�ͼ�ź���\r\n");
              OSSemPost (&Imsge_SEM, OS_OPT_POST_1, &err);   /* ���ͽ�ͼ�ź��� */
              break;
            
            case WKUP_PRES:
              printf("���������ź���\r\n");
              OSSemPost (&Font_SEM, OS_OPT_POST_1, &err);   /* ���͸��������ź��� */
              break;
        }

        OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);  /* ��ʱ10ms */

    }
}

/**
 * @brief       font_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void font_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    while (1)
    {
        OSSemPend(&Font_SEM, 0,OS_OPT_PEND_BLOCKING,0,&err); /* �����ͼ�ź��� */
        GUI_COLOR forecolor, bkcolor;
        forecolor = GUI_GetColor();   /* ��ȡ��ǰǰ��ɫ */
        bkcolor = GUI_GetBkColor();   /* ��ȡ��ǰ����ɫ */
        OSSchedLock(&err);
        lcd_show_string(10,50,250,30,16,"Font Updataing,Please Wait...",RED);
        fonts_update_font(30,90,16,(uint8_t *)"0:", RED);
        lcd_show_string(10,50,280,30,16,"Font Updata finshed,Please Restart!",RED);
        OSSchedUnlock(&err);
        GUI_SetColor(forecolor);   /* �ָ�ǰ��ɫ */
        GUI_SetBkColor(bkcolor);   /* �ָ�����ɫ */
        OSSched();
    }
}

/**
 * @brief       image_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void image_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    while (1)
    {
        OSSemPend(&Imsge_SEM, 0,OS_OPT_PEND_BLOCKING,0,&err); /* �����ͼ�ź��� */
        OSSchedLock(&err);
        LED1(0);
        emwin_create_jpgpicture((uint8_t *)"0:/SCREENSHORT/��Ļ��ͼ0.bmp",0,0,lcddev.width,lcddev.height); /* ����BMP��ͼ */
        LED1(1);
        OSSchedUnlock(&err);
        OSSched();
    }
}
