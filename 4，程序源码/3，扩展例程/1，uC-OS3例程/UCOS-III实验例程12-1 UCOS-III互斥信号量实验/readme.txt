/**
 ***************************************************************************************************
 * 实验简介
 * 实验名称：uC/OS-III 互斥信号量实验
 * 实验平台：正点原子 探索者F407开发板
 * 实验目的：学习 uC/OS-III 的互斥信号量

 ***************************************************************************************************
 * 硬件资源及引脚分配
 * 1 串口1 （PA9/PA10连接在板载USB转串口芯片CH340上面）
 * 2 正点原子 2.8/3.5/4.3/7/10寸TFTLCD模块(仅限MCU屏，16位8080并口驱动)

 
 ***************************************************************************************************
 * 实验现象
 * 1 TFTLCD模块显示本实验的相关信息。
 * 2 任务1和任务2不断通过串口打印信息。

 ***************************************************************************************************
 * 注意事项
 * 1 电脑端串口调试助手波特率必须是115200
 * 2 请使用XCOM/SSCOM串口调试助手,其他串口助手可能控制DTR/RTS导致MCU复位/程序不运行
 * 3 串口输入字符串以回车换行结束
 * 4 请用USB线连接在USB_UART,找到USB转串口后测试本例程
 * 5 P4的PA9/PA10必须通过跳线帽连接在RXD/TXD上
 * 6 本例程仅支持MCU屏，不支持RGB屏
 * 7 4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
 * 8 切记写入地址不能是用户代码区，否则可能死机

 ***************************************************************************************************
 * 公司名称 : 广州市星翼电子科技有限公司
 * 电话     : 020-38271790
 * 传真     : 020-36773971
 * 在线视频 : www.yuanzige.com
 * 技术论坛 : www.openedv.com
 * 公司网址 : www.alientek.com
 * 购买地址 : openedv.taobao.com
 ***************************************************************************************************
 */