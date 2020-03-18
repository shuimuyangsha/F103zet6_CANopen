/****************************************************
*
*作者    ：周平
*邮箱    : zhpg_0803@163.com
* QQ     : 306050016 
*创建日期: 2016年12月2日
*编译环境: MDK5.14 
* 
*****************************************************/

    本程序硬件基于战舰STM32F103开发板V2版本

	源程序基于原子哥的ucosii（版本为V2.91）跑马灯实验。
    移植了CANopen协议源码，测试了读取CANopen网络数据字典OK！
	
    
    移植时，重点关注工程的以下3个文件
        mycan.c
        mytimer.c
        stm32f1_canfestival.c
    
    主要是实现以下几个接口函数，再添加Canfestival源码，基本上协议就能够跑起来了    
        UNS8 canSend(CAN_PORT notused, Message *m);
        UNS8 canChangeBaudRate(CAN_PORT port, char* baud);
        void setTimer(TIMEVAL value);
        TIMEVAL getElapsedTime(void);
        
    其实都是为了调用协议的两个重要的函数
        TimeDispatch            ---CANopen 时间管理
        canDispatch             ---CANopen 消息管理
        
        
    本工程仅供学习交流，谢谢！
 

			
			
			
			
			
			
			
			
			
			
			
			
			
			
			