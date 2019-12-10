#ifndef __TIMERMANAGER_H_
#define __TIMERMANAGER_H_

#include "ComStruct.h"
#include "TscTimer.h"
#include "GbtTimer.h"
#include "Gb25280timer.h"
#include "WirelessButtons.h"
//#include "ComDef.h"

const int MAX_TIMER = 3;   //最大的定时器数量 
/*
*对整个项目的定时器的管理，包含初始化建立与删除
*单个实例存在
*/
class CTimerManager
{
public:
	static CTimerManager* CreateInstance();
	void CreateAllTimer();
	void CloseAllTimer(); 
	void StartAllTimer(); 
	void CloseTscTimer();
	void StartTscTimer();
	void StartGb25280Timer();

private:
	CTimerManager();
	~CTimerManager();
	long m_lTimerId[MAX_TIMER];
	ActiveTimer m_tActiveTimer;
	CTscTimer* m_pTscTimer;
	CGbtTimer* m_pGbtTimer;
	CGB25280Timer * m_pGb25280Timer;
	
};

#endif //_TIMERMANAGER_H_

