#ifndef __MACCONTROL__H__
#define __MACCONTROL__H__

#include "ComDef.h"

#include "ComStruct.h"

class CMacControl
{
public:
	static CMacControl* CreateInstance();

	//主控板请求控制模块发送环境数据
	void GetEnvSts();

	//主控板发送给控制模块配置数据
	void SetCfg1();

	//主控板发送给控制模块配置数据
	void SetCfg2();

	//主控板直接控制模块中设备
	void MainBoardCtrl();
	void RecvMacCan(SCanFrame sRecvCanTmp); //ADD 20130815 0850
	void SndLcdShow() ; //ADD 201309101706
	Byte GetCtrlStaus() ; //ADD 201309101400
	void GetMacControlVer();

private:
	CMacControl();
	~CMacControl();
		
	Byte LcdCtrlMod ;          //当前控制模式状态
	
	int m_iSetAddHotTemp;  //加热器开启的温度值
	int m_iSetReduHotTemp; //散热器开启的温度值
	Byte m_ucSetHumWarn;     //湿度预警数值 实际的百分比数值
	Byte m_ucSetAddHotPlan;  //加热器预案 0-保持 1-自动控制 2-受主板控制 
	Byte m_ucSetReduHotPlan; //散热器预案 0-保持 1-自动控制 2-受主板控制
	Byte m_ucSetCabinetWarn; //机器震动   0-保持 1-震动报警打开 2-震动报警关闭
	Byte m_ucSetHumPlan;     //湿度预案   0-保持 1-自动打开散热 2-自动打开加热 3-保留

	int m_iGetAddHotTemp; 
	int m_iGetReduHotTemp;  
	Byte m_ucGetHumWarn;      
	Byte m_ucGetAddHotPlan;   
	Byte m_ucGetReduHotPlan;  
	Byte m_ucGetCabinetWarn;  
	Byte m_ucGetHumPlan; 

	//in
	Byte m_ucSetLightPlan;  //照明灯开关预案
	Byte m_ucSetDoorPlan;   //门开关
	Byte m_ucSetLedLight;   //LCD背光
	Byte m_ucSetLedDisplay; //LCD显示

	Byte m_ucGetLightPlan;  
	Byte m_ucGetDoorPlan;    
	Byte m_ucGettLedLight;   
	Byte m_ucGettLedDisplay; 

	//in
	Byte m_ucSetMainAddHot;  //加热器
	Byte m_ucSetMainReduHot; //散热器
	Byte m_ucSetMainWarn;    //本地报警器
	Byte m_ucSetMainLight;   //照明设备
	Byte m_ucSetFarIo1;      //远程1
	Byte m_ucSetFarIo2;      //远程2

	Byte m_ucGetMainAddHot;   
	Byte m_ucGetMainReduHot; 
	Byte m_ucGetMainWarn;     
	Byte m_ucGetMainLight;    
	Byte m_ucGetFarIo1;       
	Byte m_ucGetFarIo2;      
	bool bSendCtrlOk ; //判断是否控制发送LCD显示成功

	Byte uiOldLcdCtrl ;
	Byte uiOldPatternNo  ;
	Byte uiOldFlashType ;
	bool uiOldbDegrade ; 
		
public:
	//out
	//Byte m_ucManualSts;  //手控状态
	//Byte m_ucManual;     //0自动运行 1手动控制
	Byte m_ucDoorFront;  //前门 0关闭 1打开
	Byte m_ucDoorBack;   //后门
	Byte m_ucLightDev;   //照明设备
	Byte m_ucWarnDev;    //本地报警器

	Byte m_ucTemp;   //当前温度
	Byte m_ucHum;    //当前湿度

	Byte m_ucFarOut1;  //远程输出口1
	Byte m_ucFarOut2;  //远程输出口2
	Byte m_ucFarIn1;   //远程输入口1
	Byte m_ucFarIn2;   //远程输入口2

	Byte m_ucAddHot;   //加热器
	Byte m_ucReduHot;  //散热器
	Byte m_ucCabinet;  //机柜  0静止 1震动
	Byte m_ucMacContolVer[5]; // 控制器程序版本
	Byte m_ucPsc ;  //行人按钮值
	
	
};


#endif //__MACCONTROL__H__
