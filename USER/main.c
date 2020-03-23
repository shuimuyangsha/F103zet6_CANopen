#include "sys.h" 	
#include "delay.h"	
#include "usart.h"
#include "led.h"
#include "mycan.h"
#include "EwtCanAnalysis_objdict.h"
#include "stm32f1_canfestival.h"

#include "includes.h"
#include "mytimer.h"

 

/************************************************
 ALIENTEK战舰STM32开发板实验51
 UCOSII实验1-任务调度 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			6 
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

 int main(void)
 {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
    uart_init(115200);
    TIM3_Int_Init(100-1, 72-1);   //1Mhz的计数频率，计数10次为100us, 定时器3每隔100us中断一次 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	  	 
}
	  
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
    
    CAN_Mode_Init(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq,4, CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps   
        
    initTimer();
    setNodeId(&EwtCanAnalysis_Data, CAN1_NODE_ID);  //设置CAN 节点ID
    setState(&EwtCanAnalysis_Data, Initialisation);	// 初始化CAN网络状态
    setState(&EwtCanAnalysis_Data, Operational);	// debug, 正式情况需要去掉
    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

//LED0任务
void led0_task(void *pdata)
{	
//    u8 i=0;
//    can_message_t  msg;
//    msg.cob_id = 0x01;
//    msg.len = 8;
//    msg.rtr = 0;
//    
//    for(i=0; i<8; i++)
//    {
//        msg.data[i] = 0x0 + i;
//    }
//    
    Message can_msg;
	//Message can_msg = {
	//					0x181,	/**< message's ID */
	//					0,		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
	//					8,		/**< message's length (0 to 8) */
	//					{1,2,3,4,5,6,7,8}, /**< message's datas */
	//					};
     
	while(1)
	{
//        Can_Send_Msg(&msg);
//        msg.cob_id++;

        Can_Receive_Msg((can_message_t *)&can_msg);     //接收消息
        canDispatch(&EwtCanAnalysis_Data, &can_msg);    //处理消息
		//OSTimeDlyHMSM(0, 0, 0, 200);
	};
}

void CheckReadSDO(CO_Data* d, UNS8 nodeid)
{
	UNS32 abortCode;
	UNS32 data=0;
	UNS32 size=64;

	if(getReadResultNetworkDict(&EwtCanAnalysis_Data, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
		printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
	else
		printf("\nResult : %x\n", data);

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(&EwtCanAnalysis_Data, nodeid, SDO_CLIENT);
}

extern unsigned long g_time5_cnt;
//LED1任务
void led1_task(void *pdata)
{	  
    unsigned long tempcnt = 0;
    
	while(1)
	{
		LED1=0;
        
        TIM3_Start();     
        OSTimeDlyHMSM(0, 0, 0, 50);        
        TIM3_Stop(); 
        tempcnt = GetRunTime();
        DEBUG("运行时间1为:%d\r\n", (int)(tempcnt/10));        
		
        LED1=1;
        
        TIM3_Start();      
        OSTimeDlyHMSM(0, 0, 0, 950);        
        TIM3_Stop(); 
        tempcnt = GetRunTime();
        DEBUG("运行时间2为:%d\r\n", (int)(tempcnt/10));      
        
        //readNetworkDictCallback (&EwtCanAnalysis_Data, 5, 0x2001, 0x01, 0, CheckReadSDO, 0);
	};
}






