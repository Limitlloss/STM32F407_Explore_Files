本实验将实现如下功能：开机后，根据迭代次数生成颜色表（RGB565），然后计算Julia分形，并显示到LCD上面。同时，程序开启了定时器3，用于统计一帧所要的时间（ms），在一帧Julia分形图片显示完成后，程序会显示运行时间、当前是否使用FPU和缩放因子（zoom）等信息，方便观察对比。KEY0/KEY2用于调节缩放因子，KEY_UP用于设置自动缩放，还是手动缩放。DS0用于提示程序运行状况。

注意：
1，《实验46_1 FPU测试(Julia分形)实验_开启硬件FPU》和《实验46_2 FPU测试(Julia分形)实验_关闭硬件FPU》 代码完全一模一样，只是一个开启了硬件FPU，一个没开启。
2，测试的时候，先下载一个，记录下时间和对应参数。再下载另外一个，看同样的参数时，时间是多少，从而得出结论。


 



 ***********************************************************************************************************
 * 公司名称：广州市星翼电子科技有限公司（正点原子）
 * 电话号码：020-38271790
 * 传真号码：020-36773971
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 最新资料：www.openedv.com/docs/index.html
 *
 * 在线视频：www.yuanzige.com
 * B 站视频：space.bilibili.com/394620890
 * 公 众 号 ：mp.weixin.qq.com/s/y--mG3qQT8gop0VRuER9bw
 * 抖     音 ：douyin.com/user/MS4wLjABAAAAi5E95JUBpqsW5kgMEaagtIITIl15hAJvMO8vQMV1tT6PEsw-V5HbkNLlLMkFf1Bd
 ***********************************************************************************************************