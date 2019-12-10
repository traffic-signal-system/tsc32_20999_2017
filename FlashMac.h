#ifndef _FLASHMAC_H_

const int MAX_FRAME_LEN = 4;     			 //每次与spi交互的数据长度
const int MAX_DEFAULT_TEMPERATURE    = 50;   //最高温度
const int MIN_DEFAULT_TEMPERATURE    = -10;  //最低温度
const int DROP_NORMAL_TEMPERATURE    = 30;   //降温到预定的温度
const int UP_NORMAL_TEMPERATURE      = 0;    //加热到预定的温度


//风扇操作类型
enum
{
	FAN_HOT  = 0 , //加热
	FAN_COLD = 1 , //散热
	FAN_NULL = 2 , //无操作
	FAN_OTHER    
};

class CFlashMac
{
public:
	static CFlashMac* CreateInstance();	
	//void SetHardwareFlash();
	void SetHardwareFlash(bool isflash);//MOD 2013 0808 1640
	bool GetHardwareFlash();
	void WriteAndRead();
	void SetWriteData(Byte* pTx);
	void ResolveReadData(Byte* pTx);
	void SetMaxMinTemperature(int iMaxTpt,int iMinTpt);
	void SendRecordBoardMsg(Byte ucType);

	
	void FlashHeartBeat();					   //黄闪器心跳检测  ADD: 0604 1646
	void RecvFlashCan(SCanFrame sRecvCanTmp) ; //黄闪器CAN数据接收处理函数 ADD: 2013 0712 1530

	void FlashCfgSet(); 				       //黄闪器配置设置函数 ADD: 0604 17 58
	void FlashCfgGet(); 					   //黄闪器配置获取函数 ADD:0605 08 47

	void FlashForceStart(Byte ucType);  	   // 黄闪器强制黄闪   ADD:0605 08 49
	void FlashForceEnd();   				   //黄山器退出强制黄闪  ADD:0605 08 50
	void FlashGetVer();

	bool m_bGetHardwareFlash;  //获取 硬件黄闪
	bool m_bGetColdFan;        //获取 散热风扇
	bool m_bGetHotFan;         //获取 加热风扇

	bool m_bGetForDoor;        //前门打开
	bool m_bGetAfterDoor;      //后门打开

	bool m_bRecordSts;      //上一次记录黄闪的通信状态

	bool m_bPowerType;         //供电类型 true:交流电 false:太阳能
	int  m_iVoltage;           //电压
	int  m_iTemperature;       //温度

	/****	设置黄闪器配置数据项    ********/
	Byte m_ucSetDutyCycle;  //占空比
	Byte m_ucSetFlashRate;  //黄闪频率
	Byte m_ucSetSyType;   //黄闪同步方式
	Byte m_ucSetFlashStatus ; //黄闪类型
		
	
	/****	获取黄闪器配置数据项	********/
	Byte m_ucGetDutyCycle;	//占空比
	Byte m_ucGetFlashRate;	//黄闪频率
	Byte m_ucGetSyType;   //黄闪同步方式
	Byte m_ucFlashStatus ; //黄闪类型
	Byte m_ucFlashVer[5] ; //黄闪器版本

private:
	CFlashMac();
	~CFlashMac();

	Byte m_ucErrCheckCnt;   	//连续接受到校验错误的次数
	Byte m_ucRightCnt;      	//连续接受到正确数据的次数

	bool m_bSetHardwareFlash;  //设置 硬件黄闪
	Byte m_ucSetFanSts;        //风扇状态
	Byte m_ucLastSetFanSts;    //风扇上次的状态

	Byte  m_ucSetColdCnt;        //设置 散热风扇的次数 连续
	Byte  m_ucSetHotCnt;         //设置 加热风扇的次数 连续
	Byte  m_ucSetNullFanCnt;     //风扇不起作用

	int  m_iDevFd;             //设备fd
	int  m_iMaxTemperature;    //最高温度
	int  m_iMinTemperature;    //最低温度

	STscConfig* m_pTscCfg;
};

#endif  //FlashMac
