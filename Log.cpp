
 /***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   Log.cpp
Date:       2013-1-1
Description:信号机日志处理文件
Version:    V1.0
History:    
***************************************************************/
#include "Log.h"
#include "DbInstance.h"
#include "TscKernelGb25280.h"

/**************************************************************
Function:       CLogger::CLogger
Description:    CLogger，用于类初始化处理				
Input:          无              
Output:         无
Return:         0
***************************************************************/
CLogger::CLogger()
{
	this->activate();
	ACE_DEBUG((LM_DEBUG,"%s:%d Init Log object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CLogger::WriteEventLog
Description:    调用gbt数据库处理类添加日志函数	
Input:          ucEvtType  日志类型
				uiEvtValue 日志值              
Output:         无
Return:         无
***************************************************************/
void CLogger::WriteEventLog(Byte ucEvtType,Uint uiEvtValue)
{
	(CDbInstance::m_cGbtTscDb).AddEventLog(&m_uiLogMaxId,&m_uiLogMinId,ucEvtType,uiEvtValue);
	 if(ucEvtType  ==LOG_TYPE_GREEN_CONFIG||
				ucEvtType ==LOG_TYPE_DOOR_WARN||
				ucEvtType ==LOG_TYPE_OUTLAMP_ERR||
				ucEvtType ==LOG_TYPE_LAMP||
				ucEvtType ==LOG_TYPE_CAN||
				ucEvtType ==LOG_TYPE_FLASHBOARD||
				ucEvtType ==LOG_TYPE_DETECTOR	)
	 {
		 CTscKernelGb25280::CreateInstance()->SelfReportTscError();  //发送

	 }
}

/**************************************************************
Function:       CLogger::WriteEventLogActive
Description:    生成命令对象，插入到命令队列中	
Input:          ucEvtType  日志类型
				uiEvtValue 日志值              
Output:         无
Return:         无
***************************************************************/
void CLogger::WriteEventLogActive(Byte ucEvtType,Uint uiEvtValue)
{
	//生成命令对象，插入到命令队列中
	m_queCmd.enqueue(new CLogCmd(this,ucEvtType,uiEvtValue));
}

/**************************************************************
Function:       CLogger::SetMaxMinId
Description:    设置日志最大和最小id	
Input:          uiMaxId  最大ID
				uiMinId  最小ID              
Output:         无
Return:         无
***************************************************************/
void CLogger::SetMaxMinId(Uint uiMaxId,Uint uiMinId)
{
	m_uiLogMaxId = uiMaxId;
	m_uiLogMinId = uiMinId;
	//this->Add(); //ADD: 2013 0727
}


/**************************************************************
Function:       CLogger::svc
Description:    CLogger类到srv服务函数，从基类继承过来	
Input:          无          
Output:         无
Return:         0
***************************************************************/
int CLogger::svc()
{
	while ( true )
	{
		//遍历命令队列，执行命令
		auto_ptr<ACE_Method_Request> mo(this->m_queCmd.dequeue ());

		if ( mo->call() == -1 )
		{
			break;
		}
	}
	return 0;
}



/**************************************************************
Function:       CLogCmd::CLogCmd
Description:    CLogCmd类带参构造函数
Input:          pLog    CLogger对象指针 
				ucEvtType 日志类型     
				uiEvtValue 日志值
Output:         无
Return:         无
***************************************************************/
CLogCmd::CLogCmd(CLogger *pLog,Byte ucEvtType,Uint uiEvtValue)
{
	this->m_pLog       = pLog;
	this->m_ucEvtType  = ucEvtType;
	this->m_uiEvtValue = uiEvtValue;
}

/**************************************************************
Function:       CLogCmd::call
Description:    调用CLogger对象日志添加函数
Input:          无
Output:         无
Return:         0
***************************************************************/
int CLogCmd::call()
{
	this->m_pLog->WriteEventLog(m_ucEvtType,m_uiEvtValue);
	return 0;
}


