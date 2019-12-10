#ifndef _WIRELESSCOORD_H_
#define _WIRELESSCOORD_H_

#include "ComStruct.h"
#include "ComDef.h"


/*
*无电线缆协调
*/
class CWirelessCoord
{
public:
	void SetStepInfo(bool bUtcsPara,int iStepNum , int iCycle , int iOffset , int* iStepLen );
	void GetDeflection();
	void GetAdjust();
	//判断是否是主要机器，以主要机器来管理次要机器
	Uint IsMasterMachine();
	//同步主要的配置信息
	void SyncSubMachine();
	void OverCycle();
	int GetStepLength(int iCurStepNo);

	void ForceAssort();
    void SetDegrade();       //联网降级时设置操作
	static CWirelessCoord* CreateInstance();

private:
	CWirelessCoord();
	~CWirelessCoord();

private:
	//bool m_bForceAssort;     //强制协调
	bool m_bUtcs;             //中心协调
	bool m_bPlus;             //周期增加
	bool m_bMaster;
	int m_iUtsCycle;       //中心公共周期时长
	int m_iUtscOffset;     //中心协调的相位差
	
	int m_iCycle;          //信号机自身运行的周期                          
	int m_iOffset;         //信号机自身协调的相位差
	int m_iAdjustCnt;      //总调整时间
	int m_iStepNum;                //步伐总数
	int m_iStepLen[MAX_STEP];      //各步长
	int m_iAdjustSecond[MAX_STEP]; //各个调整步伐
	time_t m_tLastTi;              //上次协调完毕的时间
};

#endif  //_WIRELESSCOORD_H_
