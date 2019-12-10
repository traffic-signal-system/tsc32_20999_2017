#ifndef _POWERBOARD_H_
#define _POWERBOARD_H_

#include "ComDef.h"
#include <ace/Thread_Mutex.h>
#include <ace/OS.h>

#define POWERBOARD_DEBUG

class CPowerBoard
{
public:
	static CPowerBoard* CreateInstance();

public:
	//电压数据，电压状态
	void CheckVoltage();

	//主控板请求电源模块发送配置数据
	void GetPowerBoardCfg();

	//主控板发送给电源模块配置数据
	void SetPowerBoardCfg();
	void SetPowerCfgData(Byte m_ucWarnHighVol,Byte m_ucWarnLowVol,Byte m_ucVolPlan,Byte m_ucDogCfg);
	//广播心跳 
	void HeartBeat();
	//接收电源板的Can数据并处理
	void RecvPowerCan(Byte ucBoardAddr,SCanFrame sRecvCanTmp);
	
	int GetStongVoltage() ;//获取高电压ADD：201309281540
	void GetPowerVer(Byte PowerBdindex); //获取电源板版本
	static Byte iHeartBeat ;
private:
	CPowerBoard();
	~CPowerBoard();

public:
	//input
	Byte m_iSetWarnHighVol;  //高压预警电压(设置值)
	Byte m_iSetWarnLowVol;   //低压预警电压(设置值)
	Byte m_ucSetStongHighVolPlan;  //强电高压预案
	Byte m_ucSetStongLowVolPlan;   //强电低压预案
	Byte m_ucSetWeakHighVolPlan;   //弱电高压预案
	Byte m_ucSetWeakLowVolPlan;    //弱电低压预案

	//output
	int m_iStongVoltage;  //强电电压
	int m_iWeakVoltage;   //弱电电压
	int m_iBusVoltage;    //总线电压

	int m_iGetWarnHighVol;  //高压预警电压(设置值)
	int m_iGetWarnLowVol;   //低压预警电压(设置值)
	Byte m_ucGetStongHighVolPlan;  //强电高压预案
	Byte m_ucGetStongLowVolPlan;   //强电低压预案
	Byte m_ucGetWeakHighVolPlan;   //弱电高压预案
	Byte m_ucGetWeakLowVolPlan;    //弱电低压预案
	Byte m_ucSetWatchCfg ;         //电源板看门狗设置 //ADD:201404021301
	Byte m_ucPowerBoardVer[MAX_POWERBOARD][5];

	ACE_Thread_Mutex  m_mutexVoltage;

};

#endif //_POWERBOARD_H_

