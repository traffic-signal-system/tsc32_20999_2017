#ifndef _GB25280TIMER_H_
#define _GB25280TIMER_H_
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"
#include "ComDefGb25280.h"
#include "ace/OS.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "GbtMsgQueue.h"
#include "TscKernelGb25280.h"


class CGB25280Timer : public ACE_Event_Handler
{
public: 		
	static CGB25280Timer* CreateInstance();
	CTscKernelGb25280 *pCTscKernelGb25280;
	CGbtMsgQueue* pGbtMsgQueue;
	Byte *  pBytesArrayRequest ;	
	Byte *  pBytesArrayQuery ;
	int BytesRequestLen ,BytesQueryLen ;
	Byte ucNoQueryResponseTime ;
	
	virtual int handle_timeout(const ACE_Time_Value &tCurrentTime, const void * = 0); //定时器回调函数
private:
	Byte m_ucTick;
	Byte m_ucMaxTick;
	
	ACE_INET_Addr *pAddrClient;
	CGB25280Timer();
	~CGB25280Timer();
};

#endif //_CTSCTIMER_H_


