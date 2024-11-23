/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-5-04
 * @brief       lwIP lwipperf 实验
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

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "lwip_demo.h"


/**
 * @breif       加载UI
 * @param       mode :  bit0:0,不加载;1,加载前半部分UI
 *                      bit1:0,不加载;1,加载后半部分UI
 * @retval      无
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    uint8_t buf[30];
    
    if (mode & 1<< 0)
    {
        lcd_show_string(6, 10, 200, 32, 32, "STM32", DARKBLUE);
        lcd_show_string(6, 40, lcddev.width, 24, 24, "lwIP Lwipperf Test", DARKBLUE);
        lcd_show_string(6, 70, 200, 16, 16, "ATOM@ALIENTEK", DARKBLUE);
    }
    
    if (mode & 1 << 1)
    {
        lcd_show_string(5, 110, 200, 16, 16, "lwIP Init Successed", MAGENTA);
        
        if (g_lwipdev.dhcpstatus == 2)
        {
            sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);     /* 显示动态IP地址 */
        }
        else
        {
            sprintf((char*)buf,"Static IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* 打印静态IP地址 */
        }
        
        lcd_show_string(5, 130, 200, 16, 16, (char*)buf, MAGENTA);
        
        speed = ethernet_chip_get_speed();      /* 得到网速 */
        
        if (speed)
        {
            lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:100M", MAGENTA);
        }
        else
        {
            lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:10M", MAGENTA);
        }
        
        lcd_show_string(5, 170, 200, 16, 16, "Local address  :", BROWN);
        lcd_show_string(5, 190, 200, 16, 16, "Port           :", BROWN);
        lcd_show_string(5, 210, 200, 16, 16, "Remote address :", BROWN);
        lcd_show_string(5, 230, 200, 16, 16, "Port           :", BROWN);
        lcd_show_string(5, 250, 200, 16, 16, "Transferred(KB):", BROWN);
        lcd_show_string(5, 270, 200, 16, 16, "Duration(ms)   :", BROWN);
        lcd_show_string(5, 290, 200, 16, 16, "Bandwidth(kb/s):", BROWN);
    }
}

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    usmart_dev.init(84);                /* 初始化USMART */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    sram_init();                        /* SRAM初始化 */
    
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);                /* 初始化外部SRAM内存池 */
    my_mem_init(SRAMCCM);               /* 初始化内部CCM内存池 */
    
    lwip_test_ui(1);                    /* 加载前半部分UI */

    lcd_show_string(6, 110, 200, 16, 16, "lwIP Init !!", BLUE);
    
    while (lwip_comm_init() != 0)
    {
        lcd_show_string(6, 110, 200, 16, 16, "lwIP Init failed!!", BLUE);
        delay_ms(500);
        lcd_fill(6, 50, 200 + 30, 50 + 16, WHITE);
        lcd_show_string(6, 110, 200, 16, 16, "Retrying...       ", BLUE);
        delay_ms(500);
        LED1_TOGGLE();
    }

    while (!ethernet_read_phy(PHY_SR))  /* 检查MCU与PHY芯片是否通信成功 */
    {
        printf("MCU与PHY芯片通信失败，请检查电路或者源码！！！！\r\n");
    }
    
#if LWIP_DHCP
    lcd_show_string(6, 130, 200, 16, 16, "DHCP IP configing... ", BLUE);    /* 开始DHCP */
    
    while ((g_lwipdev.dhcpstatus != 2) && (g_lwipdev.dhcpstatus != 0XFF))          /* 等待DHCP获取成功/超时溢出 */
    {
        lwip_periodic_handle();
        delay_ms(1000);
    }
#endif
    
    lwip_test_ui(2);            /* 加载后半部分UI */
    lwip_demo();                /* lwIP程序入口 */

    
    while (1);
}


