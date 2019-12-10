#ifndef _CAN_H_
#define _CAN_H_

#include "ComDef.h"
#include "ComStruct.h"
#include "MainBoardLed.h"

#ifdef LINUX
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#endif

#include <ace/Thread_Mutex.h>
#include <ace/OS.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

class Can
{
public:
	static Can* CreateInstance();
	
	static void* RunCanRecv(void* arg); // ADD: CAN数据接收线程函数 20130701 1101 
	static void* DealCanData(void* arg) ; //ADD:CAN数据接收线程函数 20130701 1121

	void InitCan();
	int GetHandle();
	bool Send(SCanFrame& sendFrame);
	bool Recv(SCanFrame& recvFrame);
	static void BuildCanId(Ulong u1CanMsgType
		          , Ulong u1ModuleAddr
				  , Ulong u1FrameMode
				  , Ulong u1RemodeAddr
				  , Ulong* ulCanId);
	static void ExtractCanId(Ulong& u1CanMsgType
		            , Ulong& u1ModuleAddr
				    , Ulong& u1FrameMode
				    , Ulong& u1RemodeAddr
					, Ulong& ulProtocolVersion
				    , Ulong ulCanId);
	static void PrintInfo(char* file,int line,int iSub,SCanFrame canFrame);
	static void PrintCurTime(char* file,int line,char* sTmp);

	ACE_Message_Queue<ACE_MT_SYNCH>* m_CanMsgQue;

private:
	Can();
	~Can();
	int m_socketHandle;
	CMainBoardLed* pMainBoardLed;
	#ifdef LINUX
	struct sockaddr_can m_addrCan;
	struct ifreq m_ifrCan;
	struct can_frame m_frameCan;
	#endif
	
	ACE_Thread_Mutex    m_mutexCan;
};

#endif  //_CAN_H_
