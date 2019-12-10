
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   WirelessCoord.cpp
Date:       2013-1-1
Description:信号机无线协调模式处理类文件，用于处理信号机无线协调控制
Version:    V1.0
History:
***************************************************************/
#include "Coordinate.h"
#include "ManaKernel.h"

#ifndef WINDOWS
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#endif


/**************************************************************
Function:        CWirelessCoord::IsMasterMachine
Description:     判断是否是无线协调主机		
Input:          无           
Output:         无
Return:         0
***************************************************************/
Uint CWirelessCoord::IsMasterMachine()
{
	return 0;
}

/**************************************************************
Function:        CWirelessCoord::SyncSubMachine
Description:     判断是否是无线协调子机		
Input:          无           
Output:         无
Return:         0
***************************************************************/
void CWirelessCoord::SyncSubMachine()
{
	return ;
}


/**************************************************************
Function:        CWirelessCoord::CWirelessCoord
Description:     无线协调类构造函数，初始化类变量			
Input:          无           
Output:         无
Return:         无
***************************************************************/
CWirelessCoord::CWirelessCoord()
{
	//m_bForceAssort = false;
	m_bPlus       = true;            
	m_bMaster = false;
	m_iUtsCycle   = 0;      
	m_iUtscOffset = 0;    
	m_tLastTi     = 0;
	m_iCycle      = 0;                              
	m_iOffset     = 0;         
	m_iAdjustCnt  = 0;     

	ACE_DEBUG((LM_DEBUG,"%s:%d Init CWirelessCoord object!\n",__FILE__,__LINE__));

}

/**************************************************************
Function:        CWirelessCoord::~CWirelessCoord
Description:     无线协调类析构函数		
Input:          无           
Output:         无
Return:         无
***************************************************************/
CWirelessCoord::~CWirelessCoord()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct  CWirelessCoord object!\n",__FILE__,__LINE__));
}


/**************************************************************
Function:        CWirelessCoord::CreateInstance
Description:     创建CWirelessCoord检测器静态对象		
Input:          无           
Output:         无
Return:         静态对象指针
***************************************************************/
CWirelessCoord* CWirelessCoord::CreateInstance()
{
	static CWirelessCoord cCableless;

	return &cCableless;
}


/**************************************************************
Function:        CWirelessCoord::SetStepInfo
Description:     设置无线协调控制模式信息		
Input:          bCenter - 是否为中心协调
*        		iStepNum  - 步伐总数
*        		iCycle    - 周期时长
*       		iOffset   - 协调相位差
*        		iStepLen - 各个步伐的长度 
				iMaster   主节点编号         
Output:         无
Return:         无
***************************************************************/
void CWirelessCoord::SetStepInfo(bool bCenter
							 , int iStepNum 
							 , int iCycle 
							 , int iOffset 
							 , int* iStepLen )//,int iMaster
{
	if ( ( !bCenter && ( (iCycle != m_iCycle) || ( iOffset != m_iOffset) ) ) 
		|| ( bCenter && ( (iCycle != m_iUtsCycle) || ( iOffset != m_iUtscOffset) ) ) )  
		                                                       //周期时长 或者相位差 改变需要重新协调
	{
		m_tLastTi = 0;
	}

	m_iStepNum = iStepNum;
	ACE_OS::memset(m_iStepLen , 0 , MAX_STEP);
	for (int i=0; i<iStepNum; i++ )
	{
		m_iStepLen[i] = iStepLen[i];
	}

	if ( bCenter )
	{
		m_bUtcs       = true;
		m_iUtsCycle   = iCycle;
		m_iUtscOffset = iOffset;
	}
	else
	{
		m_bUtcs    = false;
		m_iCycle   = iCycle;
		m_iOffset  = iOffset;
	}

	//ACE_DEBUG((LM_DEBUG,"%s,%d m_iUtsCycle:%d m_iUtscOffset:%d\n",__FILE__,__LINE__,m_iUtsCycle,m_iUtscOffset));

}


/**************************************************************
Function:        CWirelessCoord::GetDeflection
Description:     获取总调整时间		
Input:          无  
Output:         无
Return:         无
***************************************************************/
void CWirelessCoord::GetDeflection()
{
	int iDeflection = 0;
	int iCycle      = 0;
	int iOffset     = 0;
	if ( m_bUtcs )
	{
		iCycle  = m_iUtsCycle;
		iOffset = m_iUtscOffset;
	}
	else
	{
		iCycle  = m_iCycle;
		iOffset = m_iOffset;
	}

#ifndef WINDOWS

	time_t tTi, tTi0;
	struct tm *tNow;
	
	tTi  = time(NULL);	
	tNow = localtime(&tTi);

	tNow->tm_hour = 0;
	tNow->tm_min  = 0;
	tNow->tm_sec  = 0;
	tTi0 = mktime(tNow);

	if ( 0 == iCycle )
	{
		m_iAdjustCnt = 0;
		return;
	}

	iDeflection = (tTi - iOffset - tTi0) % iCycle;
	
	//ACE_DEBUG((LM_DEBUG, "%s:%d iDeflection:%d iCycle:%d iOffset:%d tTi:%d  tTi0:%d \n"
		//	, __FILE__ , __LINE__  , iDeflection , iCycle , iOffset , (long)tTi , (long)tTi0 ));

	if ( 0 == iDeflection )
	{
		m_iAdjustCnt = 0;
		return;
	}
	
#else

	iDeflection = 10;

#endif

	m_iAdjustCnt = (iCycle - iDeflection) % iCycle;

	if ( ( m_iAdjustCnt * 100 / iCycle ) > PLUS_LINE )
	{
		m_bPlus      = false;
		m_iAdjustCnt = iCycle - m_iAdjustCnt;
	}
	else
	{
		m_bPlus      = true;
	}
	
	//ACE_DEBUG((LM_DEBUG,"%s:%d This cycle m_iAdjustCnt:%d m_bPlus:%d\n" , __FILE__ , __LINE__ , m_iAdjustCnt , m_bPlus));
}


/**************************************************************
Function:        CWirelessCoord::GetAdjust
Description:     获取所有步伐的调整时间		
Input:          无  
Output:         无
Return:         无
***************************************************************/
void CWirelessCoord::GetAdjust()
{
	int i              = 0;
	int iCycle         = 0;
	int iAdjustPerStep = 0;
	int iMaxAdjustCnt  = 0; 
	int iMinGreen      = 0;
	int iMaxGreen      = 0;
	int iAdjustCnt     = m_iAdjustCnt;
	CManaKernel* pWorkParaManager = CManaKernel::CreateInstance();
	Byte ucStageCnt    = pWorkParaManager->m_pRunData->ucStageCount;

	if ( m_bUtcs )
	{
		iCycle  = m_iUtsCycle;
	}
	else
	{
		iCycle = m_iCycle;
	}

	for ( i = 0; i < MAX_STEP; i++ )
	{
		m_iAdjustSecond[i] = 0;
	}

	if ( m_bPlus )
	{
		iMaxAdjustCnt = iCycle * MAX_PLUS_SCALE / 100;   //每个周期最大的增加的调整数
	}
	else
	{
		iMaxAdjustCnt = iCycle * MAX_MINU_SCALE / 100;   //每个周期最大的减少的调整数
	}

	if ( iMaxAdjustCnt > iAdjustCnt )  //该周期所需调整的总数
	{
		iMaxAdjustCnt = iAdjustCnt;
	}

	for ( i = 0; i < MAX_STEP && i < m_iStepNum; i++ ) //平分
	{
		if ( !pWorkParaManager->IsLongStep(i) )
		{
			continue;
		}
		m_iAdjustSecond[i] = iMaxAdjustCnt / ucStageCnt;  //初步确定的调整8
	}
	iMaxAdjustCnt = iMaxAdjustCnt - ( iMaxAdjustCnt / ucStageCnt ) * ucStageCnt;

	//调整范围核实
	for ( i = 0; i < MAX_STEP && i < m_iStepNum; i++ ) 
	{
		if ( !pWorkParaManager->IsLongStep(i) )
		{
			continue;
		}

		iAdjustPerStep = 0;
		iMinGreen      = pWorkParaManager->GetCurStepMinGreen(i,&iMaxGreen,NULL);

		if ( m_bPlus )
		{
			iAdjustPerStep = iMaxGreen - m_iStepLen[i];	//32
		}
		else
		{
			iAdjustPerStep = m_iStepLen[i] - iMinGreen; //28
		}

		if ( iAdjustPerStep < 0 )  //预防最小绿或最大绿的设置错误
		{
			iAdjustPerStep = m_iStepLen[i] / MAX_ADJUST_CYCLE;
		}

		if ( m_iAdjustSecond[i] > iAdjustPerStep )
		{
			iMaxAdjustCnt      = iMaxAdjustCnt + ( m_iAdjustSecond[i] - iAdjustPerStep );
			m_iAdjustSecond[i] = iAdjustPerStep;
		}
		else
		{
			iAdjustPerStep = iAdjustPerStep - m_iAdjustSecond[i];//20
		}

		if ( iAdjustPerStep > iMaxAdjustCnt )  //调整完毕
		{
			iAdjustPerStep = iMaxAdjustCnt;
		}

		m_iAdjustSecond[i]  = m_iAdjustSecond[i] + iAdjustPerStep;
		iMaxAdjustCnt       = iMaxAdjustCnt - iAdjustPerStep;

		//ACE_DEBUG((LM_DEBUG,"%s:%d m_iAdjustSecond[%d]:%d \n",__FILE__,__LINE__,i,m_iAdjustSecond[i]));
	}
}


/**************************************************************
Function:        CWirelessCoord::OverCycle
Description:     一次周期开始后的操作		
Input:          无  
Output:         无
Return:         无
***************************************************************/
void CWirelessCoord::OverCycle()
{
	GetDeflection();
	if ( 0 == m_iAdjustCnt ) 
	{		
		ACE_OS::memset(m_iAdjustSecond,0x0,MAX_STEP);
		return;
	}
	else
	{
		GetAdjust();
	}
	
}


/**************************************************************
Function:        CWirelessCoord::GetStepLength
Description:     每次步伐开始调用一次		
Input:          iCurStepNo:当前的步伐号  
Output:         无
Return:         返回当前步伐的长度 -1:错误
***************************************************************/
int CWirelessCoord::GetStepLength(int iCurStepNo)
{
	int iCurSetpTime = 0;
	if ( iCurStepNo > m_iStepNum )  /*异常情况*/
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d,iCurStepNo:%d m_iStepNum:%d \n",__FILE__,__LINE__,iCurStepNo,m_iStepNum));
		return m_iStepLen[0];
	}
	m_iAdjustCnt -= m_iAdjustSecond[iCurStepNo];

	if ( m_bPlus )
	{
		iCurSetpTime = m_iStepLen[iCurStepNo] + m_iAdjustSecond[iCurStepNo];
	}
	else
	{
		iCurSetpTime = m_iStepLen[iCurStepNo] - m_iAdjustSecond[iCurStepNo];
	}
	
	//ACE_DEBUG((LM_DEBUG,"%s:%d  StepNo =%d StepTime =%d !\n",__FILE__,__LINE__,iCurStepNo,iCurSetpTime ));
	return iCurSetpTime;
}


/**************************************************************
Function:        CWirelessCoord::ForceAssort
Description:     设置强制协调属性	
Input:          iCurStepNo:当前的步伐号  
Output:         无
Return:         无
***************************************************************/
void CWirelessCoord::ForceAssort()
{
	m_tLastTi = 0;
}

/**************************************************************
Function:        CWirelessCoord::ForceAssort
Description:     设置降级时步伐数和步伐长度清零	
Input:          无 
Output:         无
Return:         无
***************************************************************/
 void CWirelessCoord::SetDegrade()
{
	 m_iStepNum = 0 ;                //步伐总数
	 memset(m_iStepLen,0,MAX_STEP);      //各步长

}
