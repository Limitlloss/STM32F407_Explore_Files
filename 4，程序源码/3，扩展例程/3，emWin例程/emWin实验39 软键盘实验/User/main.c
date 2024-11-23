/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       emWin的软键盘实验
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
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/SPI/spi.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/fonts.h"
#include "uc-os3_demo.h"


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟, 168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    tp_dev.init();                      /* 触摸屏初始化 */
    sram_init();                        /* SRAM初始化 */
    
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);                /* 初始化外部SRAM内存池 */
    my_mem_init(SRAMCCM);               /* 初始化内部CCM内存池 */

    exfuns_init();                      /* 为fatfs相关变量申请内存 */
    f_mount(fs[0], "0:", 1);            /* 挂载SD卡 */
    f_mount(fs[1], "1:", 1);            /* 挂载FLASH. */

    while(fonts_init())                 /* 初始化字库 */
    {
        lcd_show_string(30,70,200,16,16,"Font Error!", RED);

        while (sd_init())               /* 检测不到SD卡 */
        {
            lcd_show_string(30,90,200,16,16,"SD Card Failed!", RED);
            delay_ms(200);
            lcd_fill(30,90,200+30,70+16,WHITE);
            delay_ms(200);
        }
        
        fonts_update_font(30,90,16,(uint8_t *)"0:", RED);
        delay_ms(2000);
        lcd_clear(WHITE);
        break;
    }
    
    uc_os3_demo();                      /* 创建任务，并执行emwin例程 */
    
    while(1);

}
