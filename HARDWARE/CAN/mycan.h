/****************************************************
*
*作者    ：周平
*邮箱    : zhpg_0803@163.com
*创建日期: 2016年12月1日
*编译环境: MDK5.14
*文件名  ：mycan.h
*功能    ：提供can的相关驱动函数
* 
*****************************************************/

#ifndef __MYCAN_H
#define __MYCAN_H	 
#include "sys.h"	   
#include "ucos_ii.h"
             
#define  CAN1_NODE_ID   0x08       //CAN 节点ID

typedef struct {
  u16 cob_id;	/**< message's ID */
  u8 rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  u8 len;		/**< message's length (0 to 8) */
  u8 data[8];   /**< message's datas */
} can_message_t;   //CAN消息基本结构                         


#define CAN1_MSG_GRP_NUM    30  //can接收消息队列大小

typedef struct{
    OS_EVENT *q_Can_Msg;                //定义CAN 消息队列指针
    void  *MsgGrp[CAN1_MSG_GRP_NUM];    //定义CAN 指针数组，用来存放各条消息的数据首地址，创建消息队列的时候使用
    can_message_t   storage[CAN1_MSG_GRP_NUM];  //消息队列存放消息的内存区
    
    u8  mbox_windex;         //正在写入的邮箱索引编号
    can_message_t *prmsg;    //接收的消息指针
}can1_recv_unit_t;

extern can1_recv_unit_t can1_recv_unit;     //CAN1接收消息队列管理单元 

extern u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化

extern u8 Can_Send_Msg(can_message_t *msg);         //CAN 发送消息,发送成功返回0，发送失败返回1

extern void Can_Receive_Msg(can_message_t *msg);    //CAN 接收消息，以阻塞方式等待接收数据

#endif








