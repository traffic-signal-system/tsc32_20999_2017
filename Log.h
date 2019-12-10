#ifndef _LOG_H_
#define _LOG_H_

#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include "ace/Auto_Ptr.h"

#include "ComStruct.h"

/*
*日志记录 采取命令设计模式 
*防止记录日志消耗时间过多影响到工作线程
*/
class CLogger:public ACE_Task<ACE_MT_SYNCH>
{
public:
	CLogger();
	int svc();
	void WriteEventLog(Byte ucEvtType,Uint uiEvtValue);
	void WriteEventLogActive(Byte ucEvtType,Uint uiEvtValue);
	void SetMaxMinId(Uint ucMaxId,Uint ucMinId);

public:
	Uint m_uiLogMaxId;   //当前日志最大行数id
	Uint m_uiLogMinId;   //当前日志最小行数id
private:
	ACE_Activation_Queue m_queCmd;    //命令队列
};

class CLogCmd:public ACE_Method_Request
{
public:
	CLogCmd(CLogger *pLog,Byte ucEvtType,Uint uiEvtValue);
	int call();

private:
	Byte m_ucEvtType;   //事件类型
	Uint m_uiEvtValue;  //事件值
	CLogger* m_pLog;   
};

#endif  //_LOG_H_
