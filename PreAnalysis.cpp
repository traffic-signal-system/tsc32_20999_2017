
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   PreAnalysis.cpp
Date:       2015-5-18
Description:信号机处理公交优先策略
Version:    V1.0
History:
***************************************************************/
#include "SerialCtrl.h"
#include "GbtMsgQueue.h"
#include "ManaKernel.h"
#include <termios.h>
#include <stdlib.h>
#include "PreAnalysis.h"
#include "GaCountDown.h"
#include "TscMsgQueue.h"
#include "Rs485.h"

typedef struct
{
	Byte   stx;	  // 0xAA 
	Uint   ECRID;  //机号
	Ushort cmd;	  // 命令
	Ushort addr;  // 地址
	Byte   PgNo;   // 包号
	Ushort len;  //  数据长度
	Byte   sum;	  // 校验码
}
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
HECRHEADST;


/**************************************************************
Function:        CPreAnalysis::CPreAnalysis
Description:     CPreAnalysis类构造函数，初始化类			
Input:          无           
Output:         无
Return:         无
***************************************************************/
CPreAnalysis::CPreAnalysis()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Init CBusPriority object !\r\n",__FILE__,__LINE__));
	
	 Byte m_iGpsFd  = CSerialCtrl::CreateInstance()->GetSerialFd2();
	 p_BusDelayTime    = 5 ; //默认车辆启动时间
	 p_PerBusCrossTime = 2 ;//默认单车通过路口时间
	 ACE_OS::memset(&sPreAnalysisParaData,0x0 ,sizeof(sPreAnalysisParaData));
	 for(Byte index = 0x0 ;index <MAX_PHASE ;index++)
	 {
		 sPreAnalysisParaData[index].DevId = index+1 ;		 
		 sPreAnalysisParaData[index].IsUsed= false ;
	 }
}

/**************************************************************
Function:       CBusPriority::~CBusPriority
Description:    CBusPriority类析构函数		
Input:          无              
Output:         无
Return:         无
***************************************************************/
CPreAnalysis::~CPreAnalysis()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct CBusPriority object!\r\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CBusPriority::CBusPriority
Description:    创建CGps类静态对象		
Input:          无              
Output:         无
Return:         CBusPriority静态对象指针
***************************************************************/
CPreAnalysis* CPreAnalysis::CreateInstance()
{
	static CPreAnalysis CPreAnalysis;
	return &CPreAnalysis;
}



/**************************************************************
Function:        QueryAccessDev
Description:    向接入设备发出查询请求
Input:           arg - 线程函数参数        
Output:         无
Return:         无
Date:           20150518
***************************************************************/
void CPreAnalysis::QueryAccessDev()
{
	for(Byte index = 0 ;index <MAX_PHASE ;index++)
	{
		if(sPreAnalysisParaData[index].IsUsed == true)
		{
			//向每个设备发送查询请求 1秒直行一次
		}
	}
}

/**************************************************************
Function:        PreAnalysisExceptionHandle
Description:    接入设备异常处理
Input:           arg - 线程函数参数        
Output:         无
Return:         无
Date:           20150518
***************************************************************/
void CPreAnalysis::PreAnalysisExceptionHandle()
{

 	//异常处理接口函数
 	//异常情况包括某些设备通讯中断、数据包校验错误
}


/**************************************************************
Function:        AcceptDevCarsInfo
Description:    接收接入设备传递过来的车辆信息
Input:           arg - 线程函数参数        
Output:         无
Return:         无
Date:           20150527
***************************************************************/
void CPreAnalysis::AcceptDevCarsInfo()
{
	//接入设备数据接收函数接口
	//主要接收从路口设备发送过来的车辆信息。
}


/**************************************************************
Function:        HandPreAnalysis
Description:    处理
Input:           arg - 线程函数参数        
Output:         无
Return:         无
Date:           20150518
***************************************************************/
void CPreAnalysis::HandPreAnalysis()
{
	CManaKernel * pManaKernel = CManaKernel::CreateInstance();
	Byte CurStageTotalTime = 0x0 ;
	Byte CurStageElapseTime = 0x0 ;	
	Byte NextStageAllowPhases = 0x0 ;
	Byte NextStageGreenStepNum = 0x0 ;
	int NextStageGreenStepLength = 0x0 ;  

	int NextStageAllowPhasesMinTime = 0x0 ;
	int NextStageAllowPhasesMaxTime = 0x0 ;
	
	pManaKernel->GetStageTime(&CurStageTotalTime,&CurStageElapseTime);
	if(CurStageTotalTime-CurStageElapseTime <=0x2) //阶段剩余2秒时间内，下个相位车流预计时间
	{
		for(Byte index = 0x0 ;index<MAX_SON_SCHEDULE ;index++)
		{
			if(pManaKernel->m_pRunData->ucStageNo+1 >= pManaKernel->m_pRunData->ucStageCount)
				pManaKernel->m_pRunData->ucStageNo = 0x0 ;
			if(pManaKernel->m_pRunData->sScheduleTime[index].ucScheduleId= pManaKernel->m_pRunData->ucStageNo+1)
			{
				NextStageAllowPhases = pManaKernel->m_pRunData->sScheduleTime[index].usAllowPhase ;
			}
			ACE_OS::printf("%s:%d Next Stage Phase :%d \r\n",__FILE__,__LINE__,NextStageAllowPhases);
			
			NextStageGreenStepNum = pManaKernel->StageToStep(pManaKernel->m_pRunData->ucStageNo);

			
			NextStageAllowPhasesMinTime = pManaKernel->GetCurStepMinGreen(NextStageGreenStepNum, &NextStageAllowPhasesMaxTime,NULL);
			if(pManaKernel->IsLongStep(NextStageGreenStepNum))
			{
				pManaKernel->m_pRunData->sStageStepInfo[NextStageGreenStepNum].ucStepLen = 0x0 ; //下一阶段绿灯时长值
				for(Byte index = 0 ;index <MAX_PHASE ;index++)
				{
					if(sPreAnalysisParaData[index].IsUsed == true && sPreAnalysisParaData[index].CarNumber >0)
						if(NextStageAllowPhases>>(sPreAnalysisParaData[index].PhaseId-1 ==0x1))
						{
						   NextStageGreenStepLength = p_BusDelayTime + sPreAnalysisParaData[index].CarNumber*p_PerBusCrossTime ;
						   if(NextStageGreenStepLength <NextStageAllowPhasesMinTime )
						   	NextStageGreenStepLength = NextStageAllowPhasesMinTime ;
						   else if(NextStageGreenStepLength >NextStageAllowPhasesMaxTime )
						   	NextStageGreenStepLength = NextStageAllowPhasesMaxTime ;

							pManaKernel->m_pRunData->sStageStepInfo[NextStageGreenStepNum].ucStepLen = NextStageGreenStepLength ;
							break ;		//获取到放行相位的第一个有效接入设备
							
						}
				}
				
			}
		}

	}
	
		
}




