/**
 ****************************************************************************************************
 * @file        lwip_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP NTP 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


#define NTP_DEMO_RX_BUFSIZE   2000  /* 定义udp最大接收数据长度 */
#define NTP_DEMO_PORT         123   /* 定义udp连接的本地端口号 */

typedef struct _NPTformat
{
    char    version;            /* 版本号 */
    char    leap;               /* 时钟同步 */
    char    mode;               /* 模式 */
    char    stratum;            /* 系统时钟的层数 */
    char    poll;               /* 更新间隔 */
    signed char  precision;     /* 精密度 */
    unsigned int   rootdelay;   /* 本地到主参考时钟源的往返时间 */
    unsigned int   rootdisp;    /* 统时钟相对于主参考时钟的最大误差 */
    char    refid;              /* 参考识别码 */
    unsigned long long  reftime;/* 参考时间 */
    unsigned long long  org;    /* 开始的时间戳 */
    unsigned long long  rec;    /* 收到的时间戳 */
    unsigned long long  xmt;    /* 传输时间戳 */
} NPTformat;

typedef struct _DateTime  /*此结构体定义了NTP时间同步的相关变量*/
{
    int  year;        /* 年 */
    int  month;       /* 月 */
    int  day;         /* 天 */
    int  hour;        /* 时 */
    int  minute;      /* 分 */
    int  second;      /* 秒 */
} DateTime;

#define SECS_PERDAY     86400UL         /* 一天中的几秒钟 = 60*60*24 */
#define UTC_ADJ_HRS     8               /* SEOUL : GMT+8（东八区北京）  */
#define EPOCH           1900            /* NTP 起始年  */
#define HOST_NAME  "ntp1.aliyun.com"    /*阿里云NTP服务器域名 */

void lwip_demo(void);


#endif /* _CLIENT_H */
