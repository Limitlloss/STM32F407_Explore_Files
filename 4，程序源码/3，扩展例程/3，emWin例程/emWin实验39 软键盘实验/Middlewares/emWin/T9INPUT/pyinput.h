#ifndef __PYINPUT_H
#define __PYINPUT_H
#include "./SYSTEM/sys/sys.h"


/* ƴ�������ƴ���Ķ�Ӧ�� */
typedef struct
{
    uint8_t *py_input; /* ������ַ��� */
    uint8_t *py;       /* ��Ӧ��ƴ�� */
    uint8_t *pymb;     /* ��� */
} py_index;

#define MAX_MATCH_PYMB 10  /*���ƥ���� */

/* ƴ�����뷨 */
typedef struct
{
    uint8_t(*getpymb)(uint8_t *instr);  /* �ַ���������ȡ���� */
    py_index *pymb[MAX_MATCH_PYMB];      /* �����λ�� */
} pyinput;

extern pyinput t9;
uint8_t str_match(uint8_t *str1, uint8_t *str2);  /* �Ƚ������ַ�����ƥ����� */
uint8_t get_matched_pymb(uint8_t *strin, py_index **matchlist); /* ��ȡƥ���ƴ����� */
uint8_t get_pymb(uint8_t *str);   /* �õ�ƴ����� */
#endif
