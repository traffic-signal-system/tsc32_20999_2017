
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   GbtTimer.cpp
Date:       2013-1-1
Description:信号机gbt消息处理定时器，用于处理定时上传对象操作。
Version:    V1.0
History:    
***************************************************************/
#include "GbtTimer.h"
#include "GbtMsgQueue.h"

/**************************************************************
Function:       CGbtTimer::CGbtTimer
Description:    CGbtTimer类构造函数，用于类初始化处理				
Input:          无              
Output:         无
Return:         无
***************************************************************/
CGbtTimer::CGbtTimer()
{
	m_ucTick    = 0;
	m_ucMaxTick = 10;

	ACE_OS::memset(m_sReportInfo , 0 , MAX_REPORT_NUM*sizeof(SReportSelf) );
	ACE_DEBUG((LM_DEBUG,"%s:%d Init GbtTimer object ok !\n\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CGbtTimer::~CGbtTimer
Description:    CGbtTimer类	析构函数	
Input:          无              
Output:         无
Return:         无
***************************************************************/
CGbtTimer::~CGbtTimer()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct GbtTimer object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CGbtTimer::CreateInstance
Description:    创建	CGbtTimer静态对象
Input:          无              
Output:         无
Return:         静态对象指针
***************************************************************/
CGbtTimer* CGbtTimer::CreateInstance()
{
	static CGbtTimer cGbtTimer;
	return &cGbtTimer;
}


#ifdef GBT_TCP
/**************************************************************
Function:       CGbtTimer::TailorReport
Description:    定制主动上报信息，包含取消，以tcp方式传输
Input:          ucGbtArrIndex 待处理gbt消息下标
				uBufCnt   接收到到消息长度
				pBuf      接收到的消息缓存指针              
Output:         无
Return:         无
***************************************************************/
void CGbtTimer::TailorReport(Byte ucGbtArrIndex , Uint uBufCnt , Byte* pBuf)
{
	Byte ucObjectNum   = pBuf[3];        //对象数
	Byte ucRecvOptType = pBuf[0] & 0xf; //收到帧的操作类型
	Byte ucBufIndex    = 1;
	Byte ucObjectCmd   = 0;
	Ushort usCycle     = 0;

	ucBufIndex = 4;
	if ( GBT_SET_REQ == ucRecvOptType || GBT_SET_REQ_NOACK == ucRecvOptType ) 
	{
		for ( Byte ucIndex=0; ucIndex<ucObjectNum; ucIndex++ )
		{
			ucObjectCmd = pBuf[ucBufIndex++];  //cmdObj
			if ( ucBufIndex > uBufCnt )
			{
				SendErrMsg(ucGbtArrIndex);
				return;
			}

			usCycle  = 0;
			usCycle |= (pBuf[ucBufIndex++]<<8);
			usCycle |= pBuf[ucBufIndex++];

			if ( ucBufIndex > uBufCnt )
			{
				SendErrMsg(ucGbtArrIndex);
				return;
			}

			if ( false == CreateReport(ucGbtArrIndex,ucObjectCmd,usCycle) )  //上报信息定制与取消
			{
				SendErrMsg(ucGbtArrIndex);
				return;
			}
		}

		//正确的应答
		if ( GBT_SET_REQ == ucRecvOptType )
		{
			Byte sSendMsg[4] = {0};
			SGbtDealData* pGbtDealData = CGbtMsgQueue::CreateInstance()->GetGbtDealDataPoint();
			
			sSendMsg[0] = ( pBuf[0] & 0xf0 ) | GBT_SET_ACK;
			sSendMsg[1] = pBuf[1];
			sSendMsg[2] = pBuf[2];
			sSendMsg[3] = pBuf[3];

			pGbtDealData[ucGbtArrIndex].SockStreamClient.send(sSendMsg , 4);
		}
	}
	else 
	{
		SendErrMsg(ucGbtArrIndex);
	}
}

#else

/**************************************************************
Function:       CGbtTimer::TailorReport
Description:    定制主动上报信息，包含取消，以udp方式传输
				例如：0x81 0xf7 0x0 0x2 0xf8 0x0 0xa 
*                     0xa0 0x0 0x2 ，制定0xf8和0xa0两个对象自动上传
				前者上传频率1s一次，后者200ms一次。
Input:          addrClient 客户端地址
				uBufCnt   接收到到消息长度
				pBuf      接收到的消息缓存指针              
Output:         无
Return:         无
***************************************************************/
void CGbtTimer::TailorReport(ACE_INET_Addr& addrClient, Uint uBufCnt , Byte* pBuf)
{
	Byte ucObjectNum   = pBuf[3];        //对象数
	Byte ucRecvOptType = pBuf[0] & 0xf; //收到帧的操作类型
	Byte ucBufIndex    = 1;
	Byte ucObjectCmd   = 0;
	Ushort usCycle     = 0;

	ucBufIndex = 4;
	if ( GBT_SET_REQ == ucRecvOptType || GBT_SET_REQ_NOACK == ucRecvOptType )  //设置
	{
		for ( Byte ucIndex=0; ucIndex<ucObjectNum; ucIndex++ )
		{
			ucObjectCmd = pBuf[ucBufIndex++];  //cmdObj
			if ( ucBufIndex > uBufCnt )
			{
				SendErrMsg(addrClient);
				return;
			}

			usCycle  = 0;
			usCycle |= (pBuf[ucBufIndex++]<<8);
			usCycle |= pBuf[ucBufIndex++];

			if ( ucBufIndex > uBufCnt )
			{
				SendErrMsg(addrClient);
				return;
			}

			if ( false == CreateReport(addrClient,ucObjectCmd,usCycle) )  //上报信息定制与取消
			{
				SendErrMsg(addrClient);
				return;
			}
		}

		//正确的应答
		if ( GBT_SET_REQ == ucRecvOptType )
		{
			CGbtMsgQueue* pGbtMsgQueue = CGbtMsgQueue::CreateInstance();
			Byte sSendMsg[4] = {0};

			sSendMsg[0] = ( pBuf[0] & 0xf0 ) | GBT_SET_ACK;
			sSendMsg[1] = pBuf[1];
			sSendMsg[2] = pBuf[2];
			sSendMsg[3] = pBuf[3];

			pGbtMsgQueue->m_sockLocal.send(sSendMsg , 4 , addrClient);
		}
	}
	else 
	{
		SendErrMsg(addrClient);
	}
}
#endif

#ifdef GBT_TCP
/**************************************************************
Function:       CGbtTimer::CreateReport
Description:    创建主动上报信息，包含取消，以tcp方式传输				
Input:          ucGbtArrIndex 待处理消息下标
				ucObjectCmd   上传对象
				usCycle       上传周期              
Output:         无
Return:         false-失败 true-成功
***************************************************************/
bool CGbtTimer::CreateReport(Byte ucGbtArrIndex,Byte ucObjectCmd , Ushort usCycle)
{
	int iNewIndex = -1;

	ACE_Guard<ACE_Thread_Mutex> guard(m_sMutex);

	for ( Byte ucIndex=0; ucIndex<MAX_REPORT_NUM; ucIndex++ )
	{
		if ( m_sReportInfo[ucIndex].usCycle > 0  )
		{
			if ( m_sReportInfo[ucIndex].ucCmd ==  ucObjectCmd  //周期修改  包含取消
				&& m_sReportInfo[ucIndex].ucGbtDealDataIndex== ucGbtArrIndex )
			{
				m_sReportInfo[ucIndex].usCycle   = usCycle;
				return true;
			}
		}
		else if ( (0 == m_sReportInfo[ucIndex].usCycle) && (-1 == iNewIndex) )  
		{
			iNewIndex = ucIndex;
		}
	}

	if ( iNewIndex >= 0 && iNewIndex < MAX_REPORT_NUM ) //新的上报信息定制
	{
		m_sReportInfo[iNewIndex].ucTick             = 0;
		m_sReportInfo[iNewIndex].ucGbtDealDataIndex = ucGbtArrIndex;
		m_sReportInfo[iNewIndex].ucCmd              = ucObjectCmd;
		m_sReportInfo[iNewIndex].usCycle            = usCycle;
		return true;
	}

	return false;
}

#else

/**************************************************************
Function:       CGbtTimer::CreateReport
Description:    创建主动上报信息，包含取消，以UDP方式传输				
Input:          addrClient 客户端地址
				ucObjectCmd   上传对象
				usCycle       上传周期              
Output:         无
Return:         false-失败 true-成功
***************************************************************/
bool CGbtTimer::CreateReport(ACE_INET_Addr& addrClient,Byte ucObjectCmd , Ushort usCycle)
{
	int iNewIndex = -1;

	ACE_Guard<ACE_Thread_Mutex> guard(m_sMutex);

	for ( Byte ucIndex=0; ucIndex<MAX_REPORT_NUM; ucIndex++ )
	{
		if ( m_sReportInfo[ucIndex].usCycle > 0  )
		{
			if ( m_sReportInfo[ucIndex].ucCmd ==  ucObjectCmd  //周期修改  包含取消
				&& m_sReportInfo[ucIndex].addClient.get_ip_address() == addrClient.get_ip_address() )
			{
				m_sReportInfo[ucIndex].usCycle   = usCycle;
				m_sReportInfo[ucIndex].addClient = addrClient;
				return true;
			}
		}
		else if ( (0 == m_sReportInfo[ucIndex].usCycle) && (-1 == iNewIndex) )  
		{
			iNewIndex = ucIndex;
		}
	}

	if ( iNewIndex >= 0 && iNewIndex < MAX_REPORT_NUM ) //新的上报信息定制
	{
		m_sReportInfo[iNewIndex].ucTick    = 0;
		m_sReportInfo[iNewIndex].addClient = addrClient;
		m_sReportInfo[iNewIndex].ucCmd     = ucObjectCmd;
		m_sReportInfo[iNewIndex].usCycle   = usCycle;
		return true;
	}

	return false;
}
#endif

#ifdef GBT_TCP
/**************************************************************
Function:       CGbtTimer::SendErrMsg
Description:    发送错误信息，以tcp方式传输				
Input:          ucGbtArrIndex 待处理消息下标    
Output:         无
Return:         无
***************************************************************/
void CGbtTimer::SendErrMsg(Byte ucGbtArrIndex)
{
	SGbtDealData* pGbtDealData = CGbtMsgQueue::CreateInstance()->GetGbtDealDataPoint();
	Byte sErrMsg[3] = {0x86 , GBT_ERROR_OTHER , 0};

	pGbtDealData[ucGbtArrIndex].SockStreamClient.send(sErrMsg , 3);
}
#else
/**************************************************************
Function:       CGbtTimer::SendErrMsg
Description:    发送错误信息，以udp方式传输				
Input:          addrClient 客户端地址    
Output:         无
Return:         无
***************************************************************/
void CGbtTimer::SendErrMsg(ACE_INET_Addr& addrClient)
{
	CGbtMsgQueue* pGbtMsgQueue = CGbtMsgQueue::CreateInstance();
	Byte sErrMsg[3] = {0x86 , GBT_ERROR_OTHER , 0};

	pGbtMsgQueue->m_sockLocal.send(sErrMsg , 3 , addrClient);

}
#endif


/**************************************************************
Function:       CGbtTimer::SendErrMsg
Description:    定时器回调函数，100ms调用一次				
Input:          tCurrentTime  当前时间   
Output:         无
Return:         0
***************************************************************/
int CGbtTimer::handle_timeout(const ACE_Time_Value &tCurrentTime, const void * /* = 0 */)
{
	static Byte ucDealDataIndex = 0;
    static SThreadMsg sMsg;
	static CGbtMsgQueue* pGbtMsgQue = CGbtMsgQueue::CreateInstance();
#ifdef GBT_TCP
	static SGbtDealData* pGbtDealData = pGbtMsgQue->GetGbtDealDataPoint();
#endif

	for ( Byte ucIndex=0; ucIndex<MAX_REPORT_NUM; ucIndex++ )
	{
		if ( m_sReportInfo[ucIndex].usCycle != 0 )
		{
			if ( m_sReportInfo[ucIndex].ucTick == m_sReportInfo[ucIndex].usCycle-1 )
			{
#ifdef GBT_TCP
				ucDealDataIndex = m_sReportInfo[ucIndex].ucGbtDealDataIndex;
				pGbtDealData[ucDealDataIndex].bReportSelf = true;
#else
				ucDealDataIndex = pGbtMsgQue->GetDealDataIndex(true , m_sReportInfo[ucIndex].addClient);
#endif

				if ( ucDealDataIndex < MAX_CLIENT_NUM )
				{
					sMsg.ulType               = GBT_MSG_FIRST_RECV;
					sMsg.ucMsgOpt             = ucDealDataIndex;
					sMsg.uiMsgDataLen 		  = 3;
					sMsg.pDataBuf             = ACE_OS::malloc(3);
					((Byte*)sMsg.pDataBuf)[0] = 0x80;
					((Byte*)sMsg.pDataBuf)[1] = m_sReportInfo[ucIndex].ucCmd;
					((Byte*)sMsg.pDataBuf)[2] = 0;
					pGbtMsgQue->SendGbtMsg(&sMsg,sizeof(sMsg));
					//MOD:0515 17:30
					//ACE_DEBUG((LM_DEBUG,"report self %x %x %x\n",((Byte*)sMsg.pDataBuf)[0],((Byte*)sMsg.pDataBuf)[1],((Byte*)sMsg.pDataBuf)[2] ));
				}
				m_sReportInfo[ucIndex].ucTick = 0;
			}
			else
			{
				m_sReportInfo[ucIndex].ucTick++;
			}
		}
	}

	return 0;
}

