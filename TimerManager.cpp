#include "TimerManager.h"
#include "ComFunc.h"
#include "ManaKernel.h"

//定时器下标
enum 
{
	TIMER_TSC_INDEX    = 0,  //信号机控制模块的定时器
	TIMER_GBT_INDEX    = 1,  //通信协议模块的定时器
	TIMER_GB25280_INDEX =2
};


CTimerManager::CTimerManager()
{
	for (int iIndex=0; iIndex<MAX_TIMER; iIndex++)
	{
		m_lTimerId[iIndex] = 0;
	}

	m_pTscTimer = new CTscTimer(10);
	m_pGbtTimer = CGbtTimer::CreateInstance();
	m_pGb25280Timer = CGB25280Timer::CreateInstance();
	
	m_tActiveTimer.timer_queue()->gettimeofday(getCurrTime);
	m_tActiveTimer.activate();
	ACE_DEBUG((LM_DEBUG,"%s:%d Init TimerManager object ok !\n",__FILE__,__LINE__));
}

CTimerManager::~CTimerManager()
{
	CloseAllTimer();

	if ( m_pTscTimer != NULL )
	{
		ACE_OS::free(m_pTscTimer);
	}

	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct TimerManager object ok !\n",__FILE__,__LINE__));
}

CTimerManager* CTimerManager::CreateInstance()
{
	static CTimerManager tTimerManager;

	return &tTimerManager;   //自动析构
}

/*
*启动所有的定时器
*/
void CTimerManager::CreateAllTimer()
{

	ACE_DEBUG((LM_DEBUG,"%s:%d***CreateAllTimer*** Delay 3 seconds to boot all timers!\n",__FILE__,__LINE__));
	
       const ACE_Time_Value curr_tv = getCurrTime();
	//100毫秒定时器,延迟3秒启动
	m_lTimerId[TIMER_TSC_INDEX] = m_tActiveTimer.schedule(m_pTscTimer,NULL,curr_tv+ACE_Time_Value(1),ACE_Time_Value(0,100*1000)); 

	//100毫秒精确定时器，采用os相对时间
	//m_lTimerId[TIMER_TSC_INDEX ] =  m_tActiveTimer.schedule(m_pTscTimer,  NULL,GetCurTime(),ACE_Time_Value(0,100*1000)); 

	m_lTimerId[TIMER_GBT_INDEX] = m_tActiveTimer.schedule(m_pGbtTimer,NULL,curr_tv+ACE_Time_Value(1),ACE_Time_Value(0,100*1000)); 
        
	if(CManaKernel::CreateInstance()->m_pTscConfig->cGbType ==GB25280)
		m_lTimerId[TIMER_GB25280_INDEX] = m_tActiveTimer.schedule(m_pGb25280Timer,NULL,curr_tv+ACE_Time_Value(1),ACE_Time_Value(1)); 
}

/*
*关闭所有的定时器
*/
void CTimerManager::CloseAllTimer()
{
	for (int iIndex=0; iIndex<MAX_TIMER; iIndex++)
	{
		if ( m_lTimerId[iIndex] > 0 )
		{
			m_tActiveTimer.cancel(m_lTimerId[iIndex]);
		}
	}
}

/*
*开启所有的定时器
*/
void CTimerManager::StartAllTimer()
{
	//100毫秒定时器,,立马启动
	m_lTimerId[TIMER_TSC_INDEX] = m_tActiveTimer.schedule(m_pTscTimer,NULL,getCurrTime(),ACE_Time_Value(0,100*1000)); 

	m_lTimerId[TIMER_GBT_INDEX] = m_tActiveTimer.schedule(m_pGbtTimer,NULL,getCurrTime(),ACE_Time_Value(0,100*1000)); 
	
	m_lTimerId[TIMER_GB25280_INDEX] = m_tActiveTimer.schedule(m_pGb25280Timer,NULL,getCurrTime(),ACE_Time_Value(1)); 
}

/*
*开启Tsc模块的定时器
*/
void CTimerManager::StartTscTimer()
{
	m_lTimerId[TIMER_TSC_INDEX] =   //100毫秒定时器,立马启动
		m_tActiveTimer.schedule(m_pTscTimer,NULL,getCurrTime(),ACE_Time_Value(0,100*1000)); 
}

/*
*关闭Tsc模块的定时器
*/
void CTimerManager::CloseTscTimer()
{
	if ( m_lTimerId[TIMER_TSC_INDEX] > 0 )
	{
		m_tActiveTimer.cancel(m_lTimerId[TIMER_TSC_INDEX]);
	}
}



