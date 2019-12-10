#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include "ComStruct.h"
#include "ManaKernel.h"
#include "ComDef.h"

/*
	检测器交互控制状态
*/
enum
{
	DET_HEAD_VEHSTS     = 0x02 , //主控板请求检测器发送16个通道的车辆检测状态
	DET_HEAD_SPEED0104  = 0x03 , //主控板请求检测器发送1-4组测速线圈的平均速度
	DET_HEAD_SPEED0508  = 0x04 , //主控板请求检测器发送5-8组测速线圈的平均速度
	
	DET_HEAD_SEN0108_GET = 0x05 , //主控板请求检测器发送1-8通道检测器的检测灵敏度
	DET_HEAD_SEN0916_GET = 0x06 , //主控板请求检测器发送9-16通道检测器的检测灵敏度
	
	DET_HEAD_COIL0104_GET = 0x07 , //主控板请求检测器发送1-4组测速线圈的绑定情况
	DET_HEAD_COIL0508_GET = 0x08 , //主控板请求检测器发送5-8组测速线圈的绑定情况
	
	DET_HEAD_DISTAN0104_GET = 0x09 , //主控板请求检测器发送1-4组测速线圈的线圈距离
	DET_HEAD_DISTAN0508_GET = 0x0a , //主控板请求检测器发送5-8组测速线圈的线圈距离

	DET_HEAD_SEN0108_SET = 0x0b ,  //主控板发送1 - 8通道灵敏度设置给检测器
	DET_HEAD_SEN0916_SET = 0x0c ,  //主控板发送9 - 16通道灵敏度设置给检测器

	DET_HEAD_COIL0104_SET = 0x0d , //主控板发送1 – 4组测速线圈的绑定数据给检测器板
	DET_HEAD_COIL0508_SET = 0x0e , //主控板发送5 – 8组测速线圈的绑定数据给检测器板
	
	DET_HEAD_COILALLOW_SET= 0x1b,  //主控板设置是否允许线圈绑定。  ADD 2013 0816 1420

	DET_HEAD_DISTAN0104_SET = 0x0f , //主控板发送1 – 4组测速线圈的线圈距离给检测器板
	DET_HEAD_DISTAN0508_SET = 0x10 , //主控板发送5 – 8组测速线圈的线圈距离给检测器板

	DET_HEAD_STS = 0x11 , //主控板请求检测器发送16个通道的检测器工作状态   //包括正常，短路，开路等等信息	
	
	DET_HEAD_SENDATA0107_GET = 0x12 ,         //表示主控板请求检测器发送1-7级的灵敏度数值   //ADD: 2013 08 05 1600 
	DET_HEAD_SENDATA0814_GET = 0x13 ,         //表示主控板请求检测器发送8-14级的灵敏度数值
	DET_HEAD_SENDATA1516_GET = 0x14 ,         //表示主控板请求检测器发送15-16级的灵敏度数值

	DET_HEAD_SENDATA0107_SET = 0x15 ,         //表示主控板设置检测器发送1-7级的灵敏度数值
	DET_HEAD_SENDATA0814_SET = 0x16 ,         //表示主控板设置检测器发送8-14级的灵敏度数值
	DET_HEAD_SENDATA1516_SET = 0x17 ,         //表示主控板设置检测器发送15-16级的灵敏度数值

	DET_HEAD_FRENCY_GET = 0x18 , 		    //表示主控板请求检测器发送16个通道的震荡频率设置
	DET_HEAD_FRENCY_SET = 0x19 , 		      //表示主控板发送给检测器16个通道的震荡频率设置。

	DET_HEAD_WORK_SET = 0x1a , 		    //表示主控板设置检测器工作方式。
	DET_HEAD_VER      =0xff  ,             		   //表示检测器版本 ADD:20141201
	DET_HEAD_ID       =0x0                  		  //表示检测器的ID号
	  
};

class CDetector
{
public:
	static CDetector* CreateInstance();
	
	void SelectDetectorBoardCfg(int *pDetCfg);
	bool CheckDetector();	
	bool SelectBrekonCardStatus(Byte ucBoardIndex, Byte ucAddress);
	void SearchAllStatus(bool chkcar,bool chkdecstatus);
	void SearchSpeed(Byte ucBoardIndex, Byte ucAddress, Byte ucRecAddress);
	void GetOccupy();
	int GetActiveDetSum();
	void GetDetStatus(SDetectorSts* pDetStatus);
	void GetDetData(SDetectorData* pDetData);
	void GetDetAlarm(SDetectorAlarm* pDetAlarm);
	bool IsDetError();
	int GetDetBoardType();
	bool HaveDetBoard();
	void IsVehileHaveCar();
	bool IsHaveCarPhaseGrp(Uint uiPhase,Byte& ucPhaseIndex , SPhaseDetector* pPhaseDet);

	//主控板请求全部检测器发送16个通道的车辆检测状态
	void GetAllVehSts(Byte QueryType,Byte ucBdindex);  //ADD: 2013 0710 10 54

	//主控板请求单个检测器发送16个通道的车辆检测状态
	void GetVehSts(Byte ucBoardAddr,Byte QueryType);   //ADD: 2013 0710 10 54

	//主控板请求发送所有检测器的工作状态 //ADD: 2013 1114 0930
	void GetAllWorkSts(); 
	void GetAllDecVer(); //查询所有车检板接口板存在与否ADD 20150202 1427

	void RecvDetCan(Byte ucBoardAddr,SCanFrame sRecvCanTmp);// 处理从CAN总线接收回来的数据   //ADD: 2013 0710 10 54
	Byte GetDecBoardIndex(Byte DecBoardHexAddr) ;  //返回检测器板接口板地址索引ADD: 20150202
/*
#ifndef WINDOWS
	void GetHaveCarTime(time_t* pTime);
#endif
*/
	//void SendDetLink(Byte ucBoardIndex);
	Byte GetDecAddr(Byte ucBoardIndex);
	void SendDecIsLink(Byte ucBoardIndex,Byte IsAllowLink);
	void GetDecVars(Byte ucBoardIndex,Byte GetType);
	void SendDecWorkType(Byte ucBoardIndex);
	void SendDecFrency(Byte ucBoardIndex);
	void SendDecSenData(Byte ucBoardIndex,Byte ucSetType) ; //ADD  20130816 1600
	void SendDetLink(Byte ucBoardIndex,Byte SetType);
	void SearchDetLink(Byte ucBoardIndex);

	//void SendRoadDistance(Byte ucBoardIndex);
	void SendRoadDistance(Byte ucBoardIndex,Byte SetType);
	void SearchRoadDistance(Byte ucBoardIndex);

	void SearchRoadSpeed(Byte ucBoardIndex);

	//void SendDelicacy(Byte ucBoardIndex);
	void SendDelicacy(Byte ucBoardIndex,Byte SetType);
	void SearchDelicacy(Byte ucBoardIndex);

	void GetAdaptInfo(int* pDetTimeLen , int* pTotalStat);
	void SetStatCycle(Byte ucCycle);

	void SendRecordBoardMsg(Byte ucDetIndex,Byte ucType);
	void PrintDetInfo(char* pFileName,int iFileLine,Byte ucBoardIndex,char* sErrSrc,int iPrintCnt,Byte* ucRecvBuf);
public:
	
	Byte m_ucActiveBoard1; 	/* 1  - 16 活动的检测器板*/
	Byte m_ucActiveBoard2;   //17 - 32 活动的检测器板
	Byte m_ucActiveBoard3;	//33 - 48 活动的检测器板
	Byte m_ucActiveBoard4 ; //65-96	活动的检测器板
	bool m_bErrFlag[MAX_DETECTOR];       //错误标志

	bool m_bRecordSts[MAX_DET_BOARD];    //上一次记录检测板的通信状态

	Byte m_ucDetError[MAX_DETECTOR];     //故障状态      32 - 47:第1块检测器      48 - 63:第2块检测器
	Byte m_ucLastDetError[MAX_DETECTOR]; //上次故障状态   32 - 47:第1块检测器      48 - 63:第2块检测器
	Byte m_ucDetErrTime[MAX_DETECTOR]; //故障次数      32 - 47:第1块检测器      48 - 63:第2块检测器

	int m_iAdapDetTimeLen[MAX_DETECTOR];    //有车时间的统计  100ms/单位 用于自适应控制
	int m_iAdapTotalStat[MAX_DETECTOR];     //车辆统计   /次 用于自适应控制

	Byte m_ucRoadSpeed[MAX_DET_BOARD][8];     //车道的平均速度 

	Byte m_ucSetRoadDis[MAX_DET_BOARD][8];    //设置的车道距离
	Byte m_ucGetRoadDis[MAX_DET_BOARD][8];    //获取到的车道距离

	Byte m_ucSetDetDelicacy[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD];  //设置检测器板的灵敏度
	Byte m_ucGetDetDelicacy[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD];  //获取到的检测器板的灵敏度

	Byte m_sSetLookLink[MAX_DET_BOARD][8];  //设置的线圈对应关系
	Byte m_sGetLookLink[MAX_DET_BOARD][8];  //获取到的线圈对应关系
	
	Byte m_ucSetFrency[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD] ;
	Byte m_ucGetFrency[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD] ;
	Byte m_iChkType ;
	Byte m_ucSetSensibility[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD] ; //ADD 2013 0816 1530
	Byte m_ucGetSensibility[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD] ; //ADD 2013 0816 1530
	Byte m_ucDecBoardVer[MAX_DET_BOARD][5]; //ADD:20141201  检测器板卡 的程序版本
	Byte m_ucDecBoardId[MAX_DET_BOARD][4]; //ADD 20150112 检测器板卡ID
	VehicleStat m_ucDecCarsAnaly[MAX_DETECTOR];
	
private:
	CDetector();
	~CDetector();

	Byte m_ucNoCnt[MAX_DET_BOARD];          //连续没有接受数据的次数
	Byte m_ucErrAddrCnt[MAX_DET_BOARD];    //连续接受到错误地址的次数
	Byte m_ucErrCheckCnt[MAX_DET_BOARD];   //连续接受到校验错误的次数
	Byte m_ucRightCnt[MAX_DET_BOARD];      //连续接受到正确数据的次数

	int m_iDevFd;
	int m_iTotalDistance;             //统计间隔 	
	
	int m_iDetCfg[MAX_DET_BOARD];      // 0-不启用 1-第一个检测器 17-第17个检测器
	int m_iBoardErr[MAX_DET_BOARD];    //true:好 false:坏掉
	int m_iLastDetSts[MAX_DETECTOR];  //上次车的状态
	int m_iDetStatus[MAX_DETECTOR];/* //1:有车 0:无车  0 - 15:第1块检测器接口板 16 - 31:第2块检测器接口板*/
	int m_iDetTimeLen[MAX_DETECTOR];  //有车时间的统计  100ms/单位
	int m_iDetOccupy[MAX_DETECTOR];   //占有率
	Byte m_ucTotalStat[MAX_DETECTOR];  //车辆统计   /次
	Byte m_iDetSpeedAvg[MAX_DETECTOR]; //车辆速度
	Byte m_ucDetSts[MAX_DET_BOARD][MAX_DETECTOR_PER_BOARD]; //有车无车标志 ADD: 2013 0710 1050
	//int m_iLastDetTimeLen[MAX_DETECTOR];  //上个统计周期的有车时间的统计  100ms/单位
	
	STscConfig* m_pTscCfg;

	ACE_Thread_Mutex  m_sMutex;
};

#endif //_DETECTOR_H_
