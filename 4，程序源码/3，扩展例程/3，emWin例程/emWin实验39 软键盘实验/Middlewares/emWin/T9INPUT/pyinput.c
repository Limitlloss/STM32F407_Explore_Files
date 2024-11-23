#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "pymb.h"
#include "pyinput.h"
#include "string.h"

/* ƴ�����뷨  */
pyinput t9 =
{
    get_pymb,
    0,
};

/**
 * @brief       �Ƚ������ַ�����ƥ�����
 * @param       ����ֵ:0xff,��ʾ��ȫƥ��
 * @retval      ����,ƥ����ַ���
 */
uint8_t str_match(uint8_t *str1, uint8_t *str2)
{
    uint8_t i = 0;

    while (1)
    {
        if (*str1 != *str2)break; /* ����ƥ�� */

        if (*str1 == '\0')
        {
            i = 0XFF;            /* ��ȫƥ�� */
            break;
        }

        i++;
        str1++;
        str2++;
    }

    return i; /* �����ַ������  */
}

/**
 * @brief       ��ȡƥ���ƴ�����
 * @param       strin:������ַ���,����:"726"
 * @param       matchlist:�����ƥ���
 * @retval      [7],0,��ʾ��ȫƥ�䣻1����ʾ����ƥ�䣨����û����ȫƥ���ʱ��Ż���֣�
                [6:0],��ȫƥ���ʱ�򣬱�ʾ��ȫƥ���ƴ������
                ����ƥ���ʱ�򣬱�ʾ��Чƥ���λ��
 */
uint8_t get_matched_pymb(uint8_t *strin, py_index **matchlist)
{
    py_index *bestmatch = 0; /* ���ƥ�� */
    uint16_t pyindex_len = 0;
    uint16_t i = 0;
    uint8_t temp, mcnt = 0, bmcnt = 0;
    bestmatch = (py_index *)&py_index3[0];                  /* Ĭ��Ϊa��ƥ��  */
    pyindex_len = sizeof(py_index3) / sizeof(py_index3[0]); /* �õ�py������Ĵ�С. */

    for (i = 0; i < pyindex_len; i++)
    {
        temp = str_match(strin, (uint8_t *)py_index3[i].py_input);

        if (temp)
        {
            if (temp == 0XFF)matchlist[mcnt++] = (py_index *)&py_index3[i];
            else if (temp > bmcnt)                     /* �����ƥ�� */
            {
                bmcnt = temp;
                bestmatch = (py_index *)&py_index3[i]; /* ��õ�ƥ��. */
            }
        }
    }

    if (mcnt == 0 && bmcnt)  /* û����ȫƥ��Ľ��,�����в���ƥ��Ľ�� */
    {
        matchlist[0] = bestmatch;
        mcnt = bmcnt | 0X80; /* ���ز���ƥ�����Чλ�� */
    }

    return mcnt;/* ����ƥ��ĸ��� */
}

/**
 * @brief       �õ�ƴ�����
 * @param       str:�����ַ���
 * @retval      ƥ�����
 */
uint8_t get_pymb(uint8_t *str)
{
    return get_matched_pymb(str, t9.pymb);
}
