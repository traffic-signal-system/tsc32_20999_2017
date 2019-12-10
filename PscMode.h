#ifndef _PSCMODE_H_
#define _PSCMODE_H_

#include "ComStruct.h"
#include "ManaKernel.h"
#include "ComDef.h"
#include "time.h"

#define CPSCMODE_FILE "CPscMode.cpp"

const int MAX_PUSHS = 8;   //最大的行人按钮数

/***********************************************************************
CROSS1 running:
Push Button               Condiction match
STANDYSTATUS------------->WAITPGSTATUS----------------->PGENABLESTATUS
Time is over
---------->STANDYSTATUS

CROSS2 running:
Push Button              Condiction Match
STANDYSTATUS------------>WAITPGSTATUS----------------->PGENABLESTATUS
Time is over                Condiction Match
----------->NXTWAITPGSTATUS----------------->NXTPGENABLESTATUS

*************************************************************************/	

/*
*信号机psc模式
*/
class CPscMode
{
public:
	static CPscMode* CreateInstance();
	void DealButton();
	void DecTime();
	void GoNextStep();
	void PscSwitchStatus();
	void InitPara();
	void DealPSC();
	Byte m_ucBtnNum  ;
private:
	CPscMode();
	~CPscMode();

	bool m_bStartCntCown;         //是否开始倒计时
	bool m_bStep1ToStep2;         //步伐1到步伐2 true:步伐++ false：步伐--
	bool m_bCrossStage1First;     //行人阶段在前
	bool m_bBoxPush[MAX_PUSHS];  //各个按钮状态
	Byte m_ucPscStatus;    //PSC 运行状态
	Byte m_ucCurStep;      //信号机当前步伐号
	Byte m_ucStandStep;    //标准步伐号
	Byte m_ucCrossStep1;   //行人通过步伐号1
	Byte m_ucCrossStep2;   //行人通过步伐号2
	time_t m_ucNextTime ;   //用于记录行人通行结束时间，用于计算两次行人按钮生效间隔时间。
	Byte m_psc_intervaltime ;
	CManaKernel* m_pWorkParaManager;
};

#endif //_PSCMODE_H_
