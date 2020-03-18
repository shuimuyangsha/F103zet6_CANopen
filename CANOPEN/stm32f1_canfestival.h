

#ifndef __STM32F1_CANFESTIVAL_H__
#define __STM32F1_CANFESTIVAL_H__
 
#include "canfestival.h" 
 
extern void initTimer(void);
extern UNS8 canSend(CAN_PORT notused, Message *m);
extern UNS8 canChangeBaudRate(CAN_PORT port, char* baud);
extern void setTimer(TIMEVAL value);
extern TIMEVAL getElapsedTime(void);

#endif

