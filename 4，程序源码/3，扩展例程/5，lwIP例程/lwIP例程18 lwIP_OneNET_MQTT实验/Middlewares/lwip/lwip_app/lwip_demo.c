/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP+OneNET+MQTTʵ��
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
 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <netdb.h>
#include "lwip/apps/mqtt.h"
#include "./BSP/LCD/lcd.h"
#include "lwip_demo.h"
#include "token.h"
#include "string.h"


/* oneNET�ο����£�https://open.iot.10086.cn/doc/v5/develop/detail/251 */

//static const struct mqtt_connect_client_info_t mqtt_client_info =
//{
//    "MQTT",     /* �豸�� */
//    "366007",   /* ��ƷID */
//    "version=2018-10-31&res=products%2F366007%2Fdevices%2FMQTT&et=1672735919&method=md5&sign=qI0pgDJnICGoPdhNi%2BHtfg%3D%3D", /* pass */
//    100,  /* keep alive */
//    NULL, /* will_topic */
//    NULL, /* will_msg */
//    0,    /* will_qos */
//    0     /* will_retain */
//#if LWIP_ALTCP && LWIP_ALTCP_TLS  /* ���ܲ���������һ�㲻ʹ�ü��ܲ��� */
//  , NULL
//#endif
//};

static ip_addr_t g_mqtt_ip;
static mqtt_client_t* g_mqtt_client;
float g_temp = 0;    /* �¶�ֵ */
float g_humid = 0;   /* ʪ��ֵ */
unsigned char g_payload_out[200];
int g_payload_out_len = 0;
int g_publish_flag = 0;/* �����ɹ���־λ */


/**
 * @brief       mqtt�������ݻص�����
 * @param       arg������Ĳ���
 * @param       data������
 * @param       len�����ݴ�С
 * @param       flags����־
 * @retval      ��
 */
static void 
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    printf("\r\ndata cb: len %d, flags %d\n",(int)len, (int)flags);
}

/**
 * @brief       mqtt���뷢���ص�����
 * @param       arg������Ĳ���
 * @param       topic������
 * @param       tot_len�������С
 * @retval      ��
 */
static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    printf("\r\npublish cb: topic %s\r\n",topic);
    printf("\r\nlen %d\r\n",(int)tot_len);
}

/**
 * @brief       mqtt�����ص�����
 * @param       arg������Ĳ���
 * @param       err������ֵ
 * @retval      ��
 */
static void
mqtt_publish_request_cb(void *arg, err_t err)
{
    printf("publish success\r\n");
}

/**
 * @brief       mqtt������Ӧ�ص�����
 * @param       arg������Ĳ���
 * @param       err������ֵ
 * @retval      ��
 */
static void
mqtt_request_cb(void *arg, err_t err)
{
    g_publish_flag = 1;
    printf("\r\nrequest cb: err %d\r\n",(int)err);
}

/**
 * @brief       mqtt���ӻص�����
 * @param       client���ͻ��˿��ƿ�
 * @param       arg������Ĳ���
 * @param       status������״̬
 * @retval      ��
 */
static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
    
    LWIP_UNUSED_ARG(client);
    
    taskENTER_CRITICAL();           /* �����ٽ��� */
    printf("\r\nMQTT client \"%s\" connection cb: status %d\r\n", client_info->client_id, (int)status);
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    
    /* �ж��Ƿ����� */ 
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        /* �ж��Ƿ����� */ 
        if (mqtt_client_is_connected(client))
        {
            /* ���ô��뷢������Ļص� */
            mqtt_set_inpub_callback(g_mqtt_client,
                                    mqtt_incoming_publish_cb,
                                    mqtt_incoming_data_cb,
                                    NULL);
            
            /* ���Ĳ����������ö�����Ӧ��ص�����mqtt_sub_request_cb */ 
            err = mqtt_subscribe(client, DEVICE_SUBSCRIBE, 1, mqtt_request_cb, arg);
            
            if(err == ERR_OK)
            {
                printf("mqtt_subscribe return: %d\n", err);
                lcd_show_string(5, 170, 210, 16, 16, "mqtt_subscribe succeed", BLUE);
            }
            
            /* ���ķ������·������� */ 
            err = mqtt_subscribe(client, SERVER_PUBLISH, 1, mqtt_request_cb, arg);
            
            /* �ж��Ƿ��ĳɹ� */ 
            if(err == ERR_OK)
            {
                lcd_show_string(5, 190, 210, 16, 16, "mqtt_subscribe cmd succeed", BLUE);
            }
        }
    }
    else/* ����ʧ�� */
    {
        printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
    } 
}

/**
 * @brief       lwip_demo����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    static struct mqtt_connect_client_info_t mqtt_client_info;
    struct hostent *server;
    
    char version[]  = "2018-10-31";
    unsigned int expiration_time    = 1956499200;
    char authorization_buf[160] = {0};
    
    server = gethostbyname((char *)HOST_NAME);          /* ��oneNET��������ַ���� */
    memcpy(&g_mqtt_ip,server->h_addr,server->h_length); /* �ѽ����õĵ�ַ�����mqtt_ip�������� */
    
    /* ����һ���յĿͻ�����Ϣ�ṹ */
    memset(&mqtt_client_info, 0, sizeof(mqtt_client_info));
    
    /* ������Щ�������н��룬��Ȼ������������token����½��� */
    onenet_authorization(version,
                         (char *)USER_PRODUCT_ID,
                         expiration_time,
                         (char *)USER_KEY,
                         (char *)USER_DEVICE_NAME,
                         authorization_buf,
                         sizeof(authorization_buf),
                         0);
    
    /* ���ÿͻ��˵���Ϣ�� */ 
    mqtt_client_info.client_id = (char *)USER_DEVICE_NAME;      /* �豸���� */
    mqtt_client_info.client_user = (char *)USER_PRODUCT_ID;     /* ��ƷID */
    mqtt_client_info.client_pass = (char *)authorization_buf;   /* ������������� */
    mqtt_client_info.keep_alive = 100;                          /* ����ʱ�� */
    mqtt_client_info.will_msg = NULL;
    mqtt_client_info.will_qos = NULL;
    mqtt_client_info.will_retain = 0;
    mqtt_client_info.will_topic = 0;

    /* ����MQTT�ͻ��˿��ƿ� */
    g_mqtt_client = mqtt_client_new();
    
    /* ���ӷ����� */
    mqtt_client_connect(g_mqtt_client,        /* ���������ƿ� */
                        &g_mqtt_ip, MQTT_PORT,/* ������IP��˿ں� */
                        mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),/* ���÷��������ӻص����� */
                        &mqtt_client_info);   /* MQTT������Ϣ */
    while(1)
    {
        if (g_publish_flag == 1)
        {
            g_temp = 30 + rand() % 10 + 1;   /* �¶ȵ����� */
            g_humid = 54.8 + rand() % 10 + 1;/* ʪ�ȵ����� */
            sprintf((char *)g_payload_out, "{\"id\": 123,\"dp\": { \"temperatrue\": [{\"v\": %0.1f,}],\"power\": [{\"v\": %0.1f,}]}}", g_temp, g_humid);
            g_payload_out_len = strlen((char *)g_payload_out);
            mqtt_publish(g_mqtt_client,DEVICE_PUBLISH,g_payload_out,g_payload_out_len,1,0,mqtt_publish_request_cb,NULL);
        }
        
        vTaskDelay(1000);
    }
}
