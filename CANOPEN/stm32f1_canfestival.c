/****************************************************
*
*作者    ：周平
*邮箱    : zhpg_0803@163.com
*创建日期: 2016年12月2日
*编译环境: MDK5.14
*文件名  ：stm32_canfestival.c
*功能    ：CANopen 协议 （CANfestival移植需要实现的函数
* 
*****************************************************/

#include "stm32f1_canfestival.h"
#include "sys.h"
#include "mycan.h"

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

unsigned int TimeCNT=0;             //时间计数
unsigned int NextTime=0;            //下一次触发时间计数
unsigned int TIMER_MAX_COUNT=70000; //最大的时间计数

//通用定时器5中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器5!
static void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM5初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
 
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					 
}

void initTimer(void)
{
    TIM5_Int_Init(1000-1, 72-1);     //1Mhz的计数频率，计数到1000为1ms
}

//另外还要开一个1毫秒的定时器，每隔1ms调用一次这个函数
void TimerForCan(void)
{
    TimeCNT++;
    
    if (TimeCNT >= TIMER_MAX_COUNT)
    {
        TimeCNT=0;
    }
    if (TimeCNT == NextTime)
    {
        TimeDispatch();     //定时时间到，执行时间相关的分发任务
    }
}

//定时器5每隔1ms中断1次
void TIM5_IRQHandler(void)
{	
    OSIntEnter(); 
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
        TimerForCan();                                 //检查时间相关任务
	}
    OSIntExit();  		    
}
 
UNS8 canSend(CAN_PORT notused, Message *m)
{ 
    return Can_Send_Msg((can_message_t *) m);   //发送CAN消息
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
    return 0;
}

void setTimer(TIMEVAL value)
{ 
    NextTime = (TimeCNT+value)%TIMER_MAX_COUNT;
}

TIMEVAL getElapsedTime(void)
{
    int ret=0;
    
    ret = TimeCNT> last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
    last_time_set = TimeCNT;
    return ret;
}


 


