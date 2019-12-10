#ifndef _LAMP_H_
#define _LAMP_H_

#include "ComDef.h"
#include "ComStruct.h"

#define LAMP_BOARD_START_ADDR 5  /*起始灯控板的位置*/
#define LAMP_CPP "Lamp.cpp"

/*
*控制灯具信息输入输出
*整个项目单个实例
*/
class CLamp 
{
public:
	static CLamp* CreateInstance();
	void SetFlashFre(Byte ucFlashFre);
	void SetLamp(Byte* pLampOn,Byte* pLampFlash);
	void SendLamp();
	void SendToCom1();
	void RecvFromCom1(int iBoardIndex);
	//void SendLampStsToCom2();
	void GetLampBoardExit(bool* bLampBoardExit);
	void IsGreenConfict();
	void SetOverCycle();
	void SetSeriousFlash();
	void SendRecordBoardMsg(Byte ucBoardIndex,Byte ucType);
	void PrintLampInfo(char* pFileName,int iFileLine,Byte ucBoardIndex,char* sErrSrc,int iPrintCnt,Byte* ucRecvBuf);
	bool CheckSendFrame(Byte* ucSendFrame);

	bool m_bRecordSts[MAX_LAMP_BOARD];      //上一次记录灯控板的通信状态

private:
	CLamp();
	~CLamp();

#ifdef WINDOWS
	void SimulateSend();
#endif
	
	bool LampGreenAlwaysOn();
	bool LampAllRedOff();
	//bool InConflictPhase();
	void ReviseLampInfo(Byte ucType,Byte* pLampInfo);
	
	bool m_bSeriousFlash;
	bool m_firstSend;   //第一次发送
	bool m_bLampBoardExit[MAX_LAMP_BOARD];  //配置里的灯控板是否存在

	Byte m_ucNoCnt[MAX_LAMP_BOARD];         //连续没有接受数据的次数
	Byte m_ucErrAddrCnt[MAX_LAMP_BOARD];    //连续接受到错误地址的次数
	Byte m_ucErrCheckCnt[MAX_LAMP_BOARD];   //连续接受到校验错误的次数
	Byte m_ucRightCnt[MAX_LAMP_BOARD];      //连续接受到正确数据的次数

	Byte m_ucOverCnt;     //程序重启一个周期后才开始检测灯泡故障状态
	Byte m_tick;
	Byte m_ucFlashFre;  //闪光频率 1:0.5HZ 2:1HZ 4:2HZ
	Byte m_ucLampOn[MAX_LAMP];         //与物理端子对应 亮：1 灭：0
	Byte m_ucLampFlash[MAX_LAMP];
	Byte m_ucLampError[MAX_LAMP];            //灯具检测状态
	Byte m_ucLastLampError[MAX_LAMP];        //上次灯具检测状态
	Byte m_ucLampErrTime[MAX_LAMP];          //错误次数
	Byte m_ucLampBoardError[MAX_LAMP_BOARD]; //灯控板检测状态
	bool m_bLampErrFlag[MAX_LAMP];           //灯泡错误标志
	bool m_bLampGreenFlag[MAX_LAMP];         //绿冲突记录

	Byte m_ucLampStatus[3*MAX_LAMP_BOARD];  //3位表示一块板，具体参考通信协议

	Byte m_ucSendFrame[3+MAX_LAMP/8];  //发送数据 addr+len+data+sum 

	int  m_iSerial1Fd;

	int m_iGreenConflict;

//#ifdef CALLNOTEFAULT
	int  m_iSerial2Fd;
//#endif

#ifdef WINDOWS
	unsigned short m_usLampSendData[MAX_LAMP_BOARD];
#endif
	ACE_Thread_Mutex  m_mMutex; 
	ACE_Thread_Mutex  m_mutexLamp;

	STscConfig* m_pTscCfg;

};

#endif //_LAMP_H_

