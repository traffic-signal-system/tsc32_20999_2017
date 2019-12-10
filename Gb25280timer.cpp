/***********************************************************************************************************************
Copyright(c) 2017  AITON. All rights reserved.
Author:     AITON
FileName:   Gb25280timer.cpp
Date:       2017-5-1
Description:信号机GB25280-2016协议定时器处理类。
                     用于定时发送联机请求、联机查询、交通流主动上传、信号机版本更新主动上传
                     定时器激发处理时间1秒一次.                     
Version:    V1.0
History:
************************************************************************************************************************/
#include "Gb25280timer.h"
#include "ComDefGb25280.h"
#include "ace/Date_Time.h"	
/**************************************************************
Function:       CGB25280Timer::CGB25280Timer
Description:    CGB25280Timer类构造函数
Input:            无                 
Output:         无
Return:         无
***************************************************************/
CGB25280Timer::CGB25280Timer()
{
	m_ucTick    =0x0;
	m_ucMaxTick = 0xA;
	ucNoQueryResponseTime = 0x0 ;               //联机查询无未收到应答次数
	pCTscKernelGb25280= CTscKernelGb25280::CreateInstance();	
	pGbtMsgQueue = CGbtMsgQueue::CreateInstance();
	pBytesArrayRequest = pCTscKernelGb25280->ConstruectOnlineRequest(BytesRequestLen);  // 设置联机请求发送数据
	pBytesArrayQuery     =   pCTscKernelGb25280->ConstruectOnlineQuery(BytesQueryLen);     //获取联机查询发送数据
	pAddrClient = new ACE_INET_Addr(pCTscKernelGb25280->TscGB25280TscConfig.iPort,pCTscKernelGb25280->TscGB25280TscConfig.TscClientIp.c_str());
	ACE_DEBUG((LM_DEBUG,"\n%s:%d Init Gb25280Timer object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CGB25280Timer::~CGB25280Timer
Description:    CGB25280Timer类析构函数
Input:            无               
Output:         无
Return:         无
***************************************************************/
CGB25280Timer::~CGB25280Timer()
{
       if(pBytesArrayRequest != NULL)
      		 delete []pBytesArrayRequest;
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct Gbt25280Timer object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:        CGB25280Timer::CreateInstance
Description:    创建CGB25280Timer静态单例对象
Input:             无              
Output:         无
Return:         CGB25280Timer静态对象指针
***************************************************************/
CGB25280Timer* CGB25280Timer::CreateInstance()
{
	static CGB25280Timer cGB25280Timer;
	return &cGB25280Timer;
}

/******************************************************************************************************
Function:       CGB25280Timer::handle_timeout
Description:    信号机定GB25280定时器定时回调函数，1000ms执行一次
Input:             默认处理，用户无需输入              
Output:         无
Return:         0
******************************************************************************************************/
int CGB25280Timer::handle_timeout(const ACE_Time_Value &tCurrentTime, const void * /* = 0 */)
{       
	switch(m_ucTick)
	{
        case 0:
	  {			
		if(  pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus== TSC_COMMUICATION_OFFLINE)
		{		
		      pCTscKernelGb25280->SendBytesGbDataToClient(pBytesArrayRequest,BytesRequestLen);   //向上位端发送联机请求指令		   
		}
		break;
	   }
       case 1:
	   	break;
       case 2:
	   	break;
	   case 3:
	   	{ 	
		     if(pCTscKernelGb25280->TscGB25280TscConfig.bUpTrafficFlow ==true && pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE)
		     {
				  Byte * pBytesTrafficFlowUp =pCTscKernelGb25280->ConstruectTrafficFlowSelfReport( BytesQueryLen);
				  if(pBytesTrafficFlowUp != NULL)
				  {
					  pCTscKernelGb25280->SendBytesGbDataToClient(pBytesTrafficFlowUp,BytesQueryLen);	  //向上位端发送交通流主动上传数据
					  delete []pBytesTrafficFlowUp;
				  }
				  	
		      }
	      }
	   	break ;
	   case 4:	
	    {			
	         if(pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE)
		    {	
			     pCTscKernelGb25280->SendBytesGbDataToClient(pBytesArrayQuery,BytesQueryLen);	 //向上位端发送联机查询指令	
			     pCTscKernelGb25280->TscGB25280TscConfig.bGetQueryResponse= false ;
		    }
	    }
	   break ;
	   case 5:	
		break ;	   
	   case 6:
	   	break ;
	   case 7:
	   	{
		//联机查询指令发送3秒后没收到应答则,未收到应答计数+1,当累积次数达到3次则信号机设置为离线状态
		if(  pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE&&pCTscKernelGb25280->TscGB25280TscConfig.bGetQueryResponse== false)
		{
		    ucNoQueryResponseTime++;		   
		 //  ACE_OS::printf("%s:%d TscGB25280TscConfig.TscCommunicationStatus time out time =%d!\r\n",__FILE__,__LINE__,ucNoQueryResponseTime);
		    if(ucNoQueryResponseTime ==0x3)
		     {
			     pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus= TSC_COMMUICATION_OFFLINE;			
			     ucNoQueryResponseTime = 0x0;			
		     }
		}		
	     }
	   	 break ;
	   case 8:	   	
	    {			
	         if(pCTscKernelGb25280->TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE)
		   {			
		        if(pCTscKernelGb25280->TscGB25280TscConfig.bUpTrafficFlow ==true )
		         {
				  Byte *  pBytesTrafficFlowUp =pCTscKernelGb25280->ConstruectTrafficFlowSelfReport( BytesQueryLen);  //交通流信息主动上传数据发送
				  if(pBytesTrafficFlowUp != NULL)
				  {				  
					  pCTscKernelGb25280->SendBytesGbDataToClient(pBytesTrafficFlowUp,BytesQueryLen);	
					  delete []pBytesTrafficFlowUp;
				  }		 
				  					  
		         }
			if(pCTscKernelGb25280->TscGB25280TscConfig.bUpTscVerUp ==true)
			{
				Byte *pBytesTssVerUp =pCTscKernelGb25280->ConstruectscVersionSelfReply( BytesQueryLen); //信号机版本更新主动上传数据发送
				 if(pBytesTssVerUp != NULL)
				 {
					 pCTscKernelGb25280->SendBytesGbDataToClient(pBytesTssVerUp,BytesQueryLen);    
					 delete []pBytesTssVerUp;
				 }
			}
		    }		 
		     
	       }
	   	break;
	   case 9:	   	
	   	break;
	   default:
	   	break ;
	}
	m_ucTick++;
	if(m_ucTick>=m_ucMaxTick)
		m_ucTick =0x0;
	return 0;
}



