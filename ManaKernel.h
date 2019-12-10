#ifndef _MANAKERNEL_H_
#define _MANAKERNEL_H_

#include "ComStruct.h"
#include "Coordinate.h"


class CManaKernel
{
public:
	static CManaKernel* CreateInstance();
	
	void InitWorkPara();
	void DecTime();
	void GoNextStep();
	void OverCycle();
	void SwitchStatus(unsigned int uiWorkStatus);
	void SwitchCtrl(unsigned int uiCtrl);
	void SetUpdateBit();
	void LockStage(Byte ucStage);
	void LockStep(Byte ucStep);
	void LockPhase(Byte PhaseId);
	void ReadTscEvent();
	void CorrectTime(Byte ucType,Byte* pValue);
	void GetTscStatus(Byte ucDealDataIndex);
	void GetTscExStatus(Byte ucDealDataIndex);

	void SetRestart();
	bool GetRestart();
	void RestartTsc();
	bool IsLongStep(int iStepNo);
	int GetCurStepMinGreen(int iStepNo , int* iMaxGreenTime, int* iAddGreenTime);
	void GetVehilePara(bool* pbVehile , bool* bDefStep , int* piAdjustTime , Uint* piCurPhase 
		                              , Uint* piNextPhase, SPhaseDetector* psPhaseDet);
	void AddRunTime(int iAddTime,Byte ucPhaseIndex);
	int GetMaxStageMinGreen(int iStepNo);
	void SpecTimePattern(int iTimePatternId );
	bool IsMainPhaseGrp(Uint uiPhase);
	void LockNextStage();
	void GetOverlapPhaseIndex(Byte iPhaseId , Byte* ucCnt , Byte* pOverlapPhase);
	bool IsInChannel(int iChannelId);
	void DealGreenConflict(Byte ucdata);
	void GetSignalGroupId(bool bIsAllowPhase,Byte ucPhaseId,Byte* pNum,Byte* pSignalGroup);
	void SetCycleStepInfo(Byte ucLockPhase = 0);
	Byte StageToStep(int iStageNo);
	Byte StepToStage(int iStepNo,bool* bStageFirstStep);
	void GetStageTime(Byte* pTotalTime,Byte* pElapseTime);
	
	bool InPhaseDetector(Byte ucDetId);
	void UpdateConfig();
	void GetRunDataStandard();
	void SetCycleBit(bool bSetCycle);

	bool IsVehile(); 		 //ADD: 201307241020
	void ChangePatter(Byte iParama);	 //ADD: 201309181530
	void SndMsgLog(Byte ucLogType,Byte ucLogVau1,Byte ucLogVau2,Byte ucLogVau3,Byte ucLogVau4); //ADD:201309251100
	void  SetWirelessBtnDirecCfg(Uint RecvBtnDirecData , Byte Lampcolor); //ADD:201410211700
	void CwpmGetCntDownSecStep();	
	Byte GetStageMaxDetectorCars(Uint32 StageAllowPhases); //获取阶段相位检测器的排排队长度ADD 201508171514
	
	Byte GetMaxGreen2(int iStepNo); //获取步伐最大绿2



	bool m_bFinishBoot;         //psc模式使用变量 开机完成过度步后进入可以进入psc模式
	STscConfig* m_pTscConfig;   //信号机配置信息A
	STscRunData* m_pRunData;    //信号机动态参数信息
	/*************按照特定配时方案号执行*************/
	int m_iTimePatternId;    //指定的配时方案号  合法值1-32 0没有指定
	
	bool bNextDirec ;      //下一方向
	bool bTmpPattern ;     //临时方案
	bool bUTS ;            //中心平台联网控制
	bool bDegrade ;        //是否降级
	//bool bChkManul ;       //用于判断系统启动初始手控状态NEW:单片机代替
	bool bSecondPriority ; //用于次线半感应控制逻辑判断  ADD：20140618
	private:
	CManaKernel();
	~CManaKernel();
	void SetPhaseColor(bool bOverPhase,Byte iPhaseId);
	void SetLampColor(Byte ColorType);
	void SetDirecChannelColor(Byte iDirecType);
	bool GaGetDirLane(Byte ucTableId , Byte  ucDir );
	void SelectDataFromDb();
	void ResetRunData(Byte ucTime);
	Byte GetScheduleId(Byte ucMonth,Byte ucDay,Byte ucWeek);
	Byte GetTimePatternId(Byte ucScheduleId,Byte* ucCtrl,Byte* ucStatus);
	Byte GetScheduleTimeId(Byte ucTimePatternId,Byte& ucCycle,Byte& ucOffSet);
	bool GetSonScheduleTime(Byte ucScheduleTimeId);
	void SetStepInfoWithStage(Byte ucCurStageIndex,Byte* ucCurStepIndex
		                                        ,SScheduleTime* pScheduleTime);
	void GetPhaseStepTime(Byte ucPhaseId,SScheduleTime* pScheduleTime,Byte* pTime,Byte ucCurStageIndex);
	Byte GetPhaseStepLen( SPhaseStep* pPhaseStep );
	Byte GetPhaseStepIndex( SPhaseStep* pPhaseStep );

	void GetPhaseStatus(SPhaseSts* pPhaseSts);
	void GetOverlapPhaseStatus(SOverlapPhaseSts* pOverlapPhaseSts);
	void GetChannelStatus(SChannelSts* pChannelSts);
	Byte GetPhaseIdWithChannel(Byte ucChannelNo);

	/*
	void GetDetectorSts(SDetectorStsPara* pDetStatus);
	void GetDetectorData(SDetectorDataPara* pDetData);
	void GetDetectorAlarm(SDetectorAlarm* pDetAlarm);
	*/

	void GetCurStageLen(Byte* pCurStageLen);
	void GetCurKeyGreenLen(Byte* pCurKeyGreen);
	void SetDetectorPhase();
	void GetStageDetector(int iStageNo);
	void GetAllStageDetector();
	
	bool InConflictPhase();
	void AllotActiveTime();
	void SetRedOtherLamp(Byte* ucLampOn,Byte* ucLampFlash);
	void UtcsAdjustCycle();

	void GetOverlapPhaseStepTime( 
								  Byte ucCurStageIndex
		                        , Byte ucOverlapPhaseId 
		                        , Byte* pIncludeTime
		                        , Byte* pOverlapTime
								, Byte ucPhaseIndex
								, Byte ucStageYellow);

	void GetUseLampBoard(bool* bUseLampBoard);
	bool GetUseLampBoard(int iLampBoard);
	Byte OverlapPhaseToPhase(Uint uiCurAllowPhase,Byte ucOverlapPhaseIndex);
	int  GetStageMinGreen(Ushort usAllowPhase);
	bool ExitOvelapPhase(Byte ucOverlapPhaseId,Byte ucPhaseCnt,SPhaseStep* pPhaseStep);
	bool ExitStageStretchPhase(SScheduleTime* pScheduleTime);
    
	

	void ValidSoftWare() ;  //ADD 201310221450
	void DealPSC();          //ADD 20160923   
	
	bool m_bRestart;        //是否重启 gbt协议测试部分
	bool m_bWaitStandard;   //等待由四面红进入标准
	bool m_bSpeStatusTblSchedule;  //时段表里定义的特殊工作状态
	bool m_bVirtualStandard;      //非真正进入标准状态
	bool m_bSpeStatus;            //标示特殊状态 用于特殊状态-->STANDARD重新刷新参数

	

		/**************特定相位控制*****************/
	bool m_bSpePhase;           //特定相位控制
	Byte m_ucLampOn[MAX_LAMP]; //与物理端子对应 亮：1 灭：0
	Byte m_ucLampFlash[MAX_LAMP]; 

	/************************感应控制参数***************************/
	//bool m_bPhaseDetCfg;  //是否有相位与检测器的对应配置
	bool m_bAddTimeCount; //感应控制增加时间是否倒计时
	bool m_bVehile;        //是否感应控制
	Byte m_ucAddTimeCnt;  //一个阶段增加的时间总和
	int  m_iAdjustTime;   //调整时间 单位绿灯延长时间
	int  m_iMinStepTime;  //步伐最小绿
	int  m_iMaxStepTime;  //步伐最大绿

	bool bValidSoftWare ;

	Byte iCntFlashTime ;

public:
	Uint m_uiStagePhase[MAX_SON_SCHEDULE];  //每个阶段对应的相位      普通相位0-15
	SPhaseDetector m_sPhaseDet[MAX_PHASE]; //相位与检测器关系 普通相位0-15

	bool m_bCycleBit;              //已走完一个周期置位  psc使用
	bool m_bNextPhase ;
	/*************群绿波控制********************/
	Byte m_ucUtcsComCycle;    //中心公共周期时长
	Byte m_ucUtscOffset;      //中心协调相位差

	/*****************************************/
	int m_ucStageDynamicMinGreen[MAX_SON_SCHEDULE] ; //动态预分析控制阶段动态最小绿
	int m_ucStageDynamicMaxGreen[MAX_SON_SCHEDULE] ; //动态预分析控制阶段动态最大绿
	int m_ucStageDynamicAddGreen[MAX_SON_SCHEDULE] ;
	/*****************************************/
	
	//ACE_Thread_Mutex m_mutexSetCycle;
	//lastUcTimePatternId 上一个周期的配置 方案与当前是否一致。
	Byte lastUcTimePatternId;
	ACE_Thread_Mutex  m_mutexRunData;

	//CWirelessCoord m_cableless;     //无电线缆控制
}; 

#endif  //_MANAKERNEL_H_

