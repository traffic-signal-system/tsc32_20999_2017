
#ifndef WIRELESSBUTTONS_H_
#define WIRELESSBUTTONS_H_

#include "ComDef.h"	
#include "ManaKernel.h"

class CWirelessBtnTimer : public ACE_Event_Handler
{
public: 
	CWirelessBtnTimer();
	~CWirelessBtnTimer();
	static CWirelessBtnTimer* CreateInstance();
	virtual int handle_timeout(const ACE_Time_Value &tCurrentTime, const void * = 0); //定时器回调函数
	void Resettimeout();  //重置无线按键手控超时计数
private:
	Uint m_uctimeout ;
	Uint m_ucSystemSetTimeOut ; //数据库预定义的超时时间
};

class CWirelessBtn 
{
public:
	CWirelessBtn();
	virtual ~CWirelessBtn();
	static CWirelessBtn* CreateInstance();	
	void RecvMacCan(SCanFrame sRecvCanTmp);  //ADD:201410211450
	void BackToAuto();
	void EntryWirelessManul();
	void HandleSpecialDirec(Uint *RecvDircData); //ADD:20141105
	bool SetCurrenStepFlash(Byte LampColor); //ADD:201411071644
	void HandWirelessBtnMsg(); //ADD:201411171521
	bool GetbHandleWirelessBtnMsg(); //ADD:201411171550
	void StartWirelessBtnTimer() ; //ADD:201411180800
	void CloseWirelessBtn() ;//ADD:201411180800
private:	
	Byte m_ucLastManualSts;  //之前的手动状态	
	Byte m_ucdirectype ;     //下一方向保存变量 取值范围0-3 ，北东南西
	SCanFrame sWirelessBtnCanMsg;      //线程无线手控按键消息变量
	CManaKernel * pManaKernel ;
	bool bFirstOperate ;  //是否刚进入手控
	bool bTransitSetp ;   //是否当前处于过渡步状态,比如按键
	Uint RecvoldDirec ;  //上次特殊方向按键值(南北左 南北直右等等)
	Byte m_ucAllowLampStatus ; //当前方向相位灯色状态	
	bool bHandleWirelessBtbMsg ; //ADD:20141117  是否需要线程处理无线手控按钮信息
	ActiveTimer m_tActiveWirelessBtnTimer; //ADD:201411180800
	long m_lWirelessBtnTimerId ; //ADD:201411180800
	CWirelessBtnTimer* m_pWirelessBtnTimer; //ADD:201411180800
};



#endif /* MANUAL_H_ */
