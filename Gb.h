#ifndef _GB_H_
#define _GB_H_

static void* SignalMsgQueue(void* arg);  //启动信号机控制消息队列
static void* GbtMsgQueue(void* arg);     //启动gbt消息处理队列
static void* BroadCast(void* arg);       //开启广播线程 
void RunGb();
void StartBeep();


#endif   //_GB_H_ 
