/****************************************************
*
*作者    ：周平
*邮箱    : zhpg_0803@163.com
*创建日期: 2016年7月19日10:11:32
*编译环境: MDK5.14
*文件名  ：timer.c
*功能    ：提供定时相关的功能函数
* 
*****************************************************/

#include "mytimer.h" 	 
                                    
/******************************定时器3起始*********************************/

//最大可以计时： 2^32 *最小单位
//最小单位为1us时：最大计时 = 4294967296us=  4294967 ms = 4294 s = 71 min        //单位太小，计时不准
//最小单位为10us时：最大计时 = 42949672960us=  42949670 ms = 42940 s = 710 min =11.8h       //单位太小，计时不准
//最小单位为100us=0.1ms时：最大计时 = 429496729600us=  429496700 ms = 429400 s = 7100 min  =118h   
 
static unsigned long cg_timer3_cnt = 0;    

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  		
    OSIntEnter(); 
    
	if(TIM3->SR&0X0001)//溢出中断
	{
 		cg_timer3_cnt = cg_timer3_cnt + 1;    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	   

    OSIntExit();     
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM3->DIER|=1<<0;   //允许更新中断	  
	TIM3->CR1&=~0x01;    //失能定时器3 
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //定时器3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;     //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		    //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                            //根据指定的参数初始化VIC寄存器
    
    cg_timer3_cnt = 0;								 
}

//启动定时器3
void TIM3_Start(void)
{
    cg_timer3_cnt = 0;
    TIM3->CR1|=0x01;        //使能定时器3
}

//关闭定时器3
void TIM3_Stop(void)
{
    TIM3->CR1 &=~0x01;      //失能定时器3
}

//获取定时器3运行时间，单位0.01ms(得根据定时器3的配置)
unsigned long GetRunTime(void)
{   
    unsigned long temp = cg_timer3_cnt;

    cg_timer3_cnt = 0;
    return temp;
}

/******************************定时器3结束*********************************/

///******************************定时器4起始*********************************/
//
////最大可以计时： 2^32 *最小单位
////最小单位为1us时：最大计时 = 4294967296us=  4294967 ms = 4294 s = 71 min        //单位太小，计时不准
////最小单位为10us时：最大计时 = 42949672960us=  42949670 ms = 42940 s = 710 min =11.8h       //单位太小，计时不准
////最小单位为100us=0.1ms时：最大计时 = 429496729600us=  429496700 ms = 429400 s = 7100 min  =118h   
// 
//static unsigned long cg_timer4_cnt = 0;    
//
////定时器4中断服务程序	 
//void TIM4_IRQHandler(void)
//{ 		    		  		
//    OSIntEnter(); 
//    
//	if(TIM4->SR&0X0001)//溢出中断
//	{
// 		cg_timer4_cnt = cg_timer4_cnt + 1;    				   				     	    	
//	}				   
//	TIM4->SR&=~(1<<0);//清除中断标志位 	   
//
//    OSIntExit();     
//}
////通用定时器3中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器4!
//void TIM4_Int_Init(u16 arr,u16 psc)
//{
//    NVIC_InitTypeDef  NVIC_InitStructure;
//    
//	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
// 	TIM4->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
//	TIM4->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
//	TIM4->DIER|=1<<0;   //允许更新中断	  
//	TIM4->CR1&=~0x01;    //失能定时器4
//    
//    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //定时器4中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;     //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		    //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	                            //根据指定的参数初始化VIC寄存器
//    
//    cg_timer4_cnt = 0;								 
//}
//
////启动定时器4
//void TIM4_Start(void)
//{
//    cg_timer4_cnt = 0;
//    TIM4->CR1|=0x01;        //使能定时器4
//}
//
////关闭定时器4
//void TIM4_Stop(void)
//{
//    TIM4->CR1 &=~0x01;      //失能定时器4
//}
//
////获取定时器4运行时间，单位1ms(得根据定时器4的配置)
//unsigned long GetRunMsCnt(void)
//{   
//    unsigned long temp = cg_timer4_cnt;
// 
//    return temp;
//}
//
///******************************定时器4结束*********************************/



