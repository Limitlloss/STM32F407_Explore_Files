/**
 ****************************************************************************************************
 * @file        lwip_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP+OneNET+MQTT实验
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


/* 用户需要根据设备信息完善以下宏定义中的三元组内容 */
#define USER_DEVICE_NAME "MY_MQTT"                                          /* 设备名 */
#define USER_PRODUCT_ID  "wKcHrEVL0n"                                       /* 产品ID */
#define USER_KEY         "OVhEYzZBb0tpQUVoZWI0ZUYzTWRKeTJYbjBxNldpd0I="     /* 设备密钥 */
/* 该密码需要onenet提供的token软件计算得出 */
#define PASSWORD "version=2018-10-31&res=products%2FwKcHrEVL0n%2Fdevices%2FMY_MQTT&et=1956499200&method=md5&sign=Xy4AtolrnCFHDrm6nXWDBQ%3D%3D"

/* 以下参数的宏定义固定，不需要修改，只修改上方的参数即可 */
#define HOST_NAME           "mqtts.heclouds.com"  /*onenet域名 */
#define DEVICE_SUBSCRIBE    "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json/accepted"   /* 订阅 */
#define DEVICE_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json"            /* 发布 */
#define SERVER_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/cmd/request/+"           /* 服务器下发命令 */


void lwip_demo(void);

#endif
