#ifndef _MAINBOARDLED_H_
#define _MAINBOARDLED_H_

#include "ComStruct.h"
#define MAXLED 10   //最大LED灯显示数目 ADD:20141106


/*********LED灯条显示状态枚举********20141106*/
enum{

		LED_STATUS_MAINTAIN = 0x0 ,  // 保持原先显示不变
		LED_STATUS_OFF      = 0x1 ,  //LED灯熄灭
		LED_STATUS_ON       = 0x2 ,  //LED灯亮
		LED_STATUS_FLASH    = 0x3    //LED灯闪烁

};



/*********模块LED灯枚举*******************20141106*/

enum{

		LED_RADIATOR    = 0x0 ,  // 散热器LED灯
		LED_HEATER      = 0x1 ,  //加热器LED灯
		LED_ILLUMINATOR = 0x2 ,  //照明器LED灯
		LED_NETWORK     = 0x3 ,  //网络LED灯
		LED_3G    		= 0x4 ,  //3G  LED灯
		LED_MSG         = 0x5 ,  //短信MSG LED灯
		LED_CAM         = 0x6 ,  //CAM LED灯		
		LED_WIRELESSBUTTON       = 0x7 ,  //无线手控LED
		LED_GPS         = 0x8 ,   //GPSLED灯
		LED_YWFLASH     = 0x9    //黄闪器LED灯		

};

class CMainBoardLed
{
public:
	static CMainBoardLed* CreateInstance();

	void DoModeLed(bool bLed3Value,bool bLed4Value);
	void DoTscPscLed(bool bValue);
	void DoAutoLed(bool bValue);
	void DoRunLed();
	void DoCan0Led();
	void DoCan1Led();
	bool IsEthLinkUp() ;
	void SetLedBoardShow();//ADD 2013 0809 15 40
	void DoLedBoardShow(); //ADD: 2013 0809 1700
	void SetSignalLed(Byte LedIndex ,Byte LedStatus); //ADD:201411060926
	
	void RecvMainBdLedCan(SCanFrame sRecvCanTmp);
	void GetMBLedVer();
	Byte m_ucMBLedVer[5];
private:
	CMainBoardLed();
	~CMainBoardLed();
	void OpenDev();
	void CloseDev();
	
private:
	bool can0Bool;
	bool can1Bool;
	Byte LedBoardStaus[MAXLED]; //ADD 2013 0809 15 40
	
};

#endif /*_MAINBOARDLED_H_*/
