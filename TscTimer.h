#ifndef _TSCTIMER_H_
#define _TSCTIMER_H_

#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"
#include "Define.h"
/*
*信号机控制定时器，如发送整秒事件等
*/
class CTscTimer : public ACE_Event_Handler
{
public: 
	CTscTimer(Byte ucMaxTick);
	~CTscTimer();
	virtual int handle_timeout(const ACE_Time_Value &tCurrentTime, const void * = 0); //定时器回调函数
	void ChooseDecTime();
private:
	bool m_bWatchdog;
	Byte m_ucTick;
	Byte m_ucMaxTick;
};

#endif //_CTSCTIMER_H_

