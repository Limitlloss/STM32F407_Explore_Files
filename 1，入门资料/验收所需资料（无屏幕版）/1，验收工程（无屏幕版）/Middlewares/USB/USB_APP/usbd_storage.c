/**
 ****************************************************************************************************
 * @file        usbd_storage.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-24
 * @brief       usbd_storage 驱动代码
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
 * 修改说明
 * V1.0 20211124
 * 第一次发布
 *
 ****************************************************************************************************
 */
#include "usbd_storage.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./BSP/SDIO/sdio_sdcard.h"


/* 文件系统在外部FLASH的起始地址
 * 我们定义SPI FLASH前12M给文件系统用, 所以地址从0开始
 */
#define USB_STORAGE_FLASH_BASE  0


/* 自己定义的一个标记USB状态的寄存器 */
/* bit0:表示电脑正在向SD卡写入数据 */
/* bit1:表示电脑正从SD卡读出数据 */
/* bit2:SD卡写数据错误标志位 */
/* bit3:SD卡读数据错误标志位 */
/* bit4:1,表示电脑有轮询操作(表明连接还保持着) */
volatile uint8_t g_usb_state_reg = 0;

/* USB Mass storage 标准查询数据(每个lun占36字节) */
const int8_t STORAGE_Inquirydata_FS[] = {

   /* LUN 0 */
    0x00,
    0x80,
    0x02,
    0x02,
    (STANDARD_INQUIRY_DATA_LEN - 4),
    0x00,
    0x00,
    0x00,
    /* Vendor Identification */
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',/* 9字节 */
    /* Product Identification */
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',/* 15字节 */
    ' ', 'D', 'i', 's', 'k', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                         /* 4字节 */

    /* LUN 1 */
    0x00,
    0x80,
    0x02,
    0x02,
    (STANDARD_INQUIRY_DATA_LEN - 4),
    0x00,
    0x00,
    0x00,
    /* Vendor Identification */
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',    /* 9字节 */
    /* Product Identification */
    'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',    /* 15字节 */
    'D', 'i', 's', 'k', ' ', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             /* 4字节 */
};

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/**
 * @brief       初始化存储设备
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SPI FLASH
 *   @arg                  1, SD卡
 * @retval      操作结果
 *   @arg       0    , 成功
 *   @arg       其他 , 错误代码
 */
int8_t STORAGE_Init_FS(uint8_t lun)
{
    uint8_t res = 0;

    switch (lun)
    {
        case 0: 
            norflash_init();
            break;

        case 1: /* SD卡 */
            res = sd_init();
            break;
    }
    return (res);
}

/**
 * @brief       获取存储设备的容量和块大小
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SD卡
 *   @arg                  1, SPI FLASH
 * @param       block_num  : 块数量(扇区数)
 * @param       block_size : 块大小(扇区大小)
 * @retval      操作结果
 *   @arg       0    , 成功
 *   @arg       其他 , 错误代码
 */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    switch (lun)
    {
        case 0: /* SPI FLASH */
            *block_size = 512;
            *block_num = 1024*1024*12/512;    /* SPI FLASH的前面12M字节,文件系统用 */
            break;

        case 1: /* SD卡 */
            HAL_SD_GetCardInfo(&g_sdcard_handler, &g_sd_card_info_handle);
            *block_num = g_sd_card_info_handle.LogBlockNbr - 1;
            *block_size = g_sd_card_info_handle.LogBlockSize;
            break;
    }

    return 0;
}

/**
 * @brief       查看存储设备是否就绪
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SD卡
 *   @arg                  1, SPI FLASH
 * @retval      就绪状态
 *   @arg       0    , 就绪
 *   @arg       其他 , 未就绪
 */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
    g_usb_state_reg |= 0X10;    /* 标记轮询 */
    return (USBD_OK);
}

/**
 * @brief       查看存储设备是否写保护
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SD卡
 *   @arg                  1, SPI FLASH
 * @retval      读保护状态
 *   @arg       0    , 没有读保护
 *   @arg       其他 , 有读保护
 */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
    return (USBD_OK);
}

/**
 * @brief       从存储设备读取数据
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SD卡
 *   @arg                  1, SPI FLASH
 * @param       buf        : 数据存储区首地址指针
 * @param       blk_addr   : 要读取的地址(扇区地址)
 * @param       blk_len    : 要读取的块数(扇区数)
 * @retval      操作结果
 *   @arg       0    , 成功
 *   @arg       其他 , 错误代码
 */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = 0;
    g_usb_state_reg |= 0X02;    /* 标记正在读数据 */
    switch (lun)
    {
        case 0: /* SPI FLASH */
            norflash_read(buf, USB_STORAGE_FLASH_BASE + blk_addr*512, blk_len*512);
            break;

        case 1: /* SD卡 */
            res = sd_read_disk(buf, blk_addr, blk_len);
            break;
    }

    if (res)
    {
        printf("rerr:%d,%d", lun, res);
        g_usb_state_reg |= 0X08;    /* 读错误! */
    }
    return (USBD_OK);
}

/**
 * @brief       向存储设备写数据
 * @param       lun        : 逻辑单元编号
 *   @arg                  0, SD卡
 *   @arg                  1, SPI FLASH
 * @param       buf        : 数据存储区首地址指针
 * @param       blk_addr   : 要写入的地址(扇区地址)
 * @param       blk_len    : 要写入的块数(扇区数)
 * @retval      操作结果
 *   @arg       0    , 成功
 *   @arg       其他 , 错误代码
 */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = 0;
    g_usb_state_reg |= 0X01;    /* 标记正在写数据 */

    switch (lun)
    {
        case 0: /* SPI FLASH */
            norflash_write(buf, USB_STORAGE_FLASH_BASE + blk_addr * 512, blk_len * 512);
            break;

        case 1: /* SD卡 */
            res = sd_write_disk(buf, blk_addr, blk_len);
            break;
    }

    if (res)
    {
        g_usb_state_reg |= 0X04;    /* 写错误! */
        printf("werr:%d,%d", lun, res);
    }

    return res;
}

/**
 * @brief       获取支持的最大逻辑单元个数
 *   @note      注意, 这里返回的逻辑单元个数是减去了1的.
 *              0, 就表示1个; 1, 表示2个; 以此类推
 * @param       无
 * @retval      支持的逻辑单元个数 - 1
 */
int8_t STORAGE_GetMaxLun_FS(void)
{
    HAL_SD_GetCardInfo(&g_sdcard_handler,&g_sd_card_info_handle);

    /* STORAGE_LUN_NBR 在usbd_conf.h里面定义, 默认是2 */
    if (g_sd_card_info_handle.LogBlockNbr)    /* 如果SD卡正常, 则支持2个磁盘 */
    {
        return STORAGE_LUN_NBR - 1;
    }
    else    /* SD卡不正常, 则只支持1个磁盘 */
    {
        return STORAGE_LUN_NBR - 2;
    }
}

