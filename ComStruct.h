#ifndef _COMSTRUCT_H_
#define _COMSTRUCT_H_

#include "ComDef.h"
#include "ace/INET_Addr.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"
#include "ace/Synch.h"
#include "GbtDb.h"

#ifndef  ActiveTimer
typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;  //主动定时器类
#endif

#ifndef TscAceDebug
#define TscAceDebug(X) \
/*	do { \
int __ace_error = ACE_Log_Msg::last_error_adapter (); \
ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
ace___->log X; \
} while (0)
*/
#endif
#if defined(__GNUC__) 
#define PACKED  __attribute__((packed)) 
#else 
#pragma pack(1) 
#define PACKED 
#endif


/*
	GBT消息结构体
*/
struct SGbtFrame
{
	int iIndex;              //消息字节索引
	int iBufLen;             //消息长度
	Byte ucBuf[MAX_BUF_LEN]; //消息内容数组
}
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
;

/*
	GBT消息处理结构体
*/
struct SGbtDealData
{	
	int            iObjNum;        //对象数
	bool           bIsDeal;        //消息是否已经处理
	bool           bReportSelf;    //是否自动上报
#ifdef GBT_TCP
	ACE_SOCK_Stream SockStreamClient;  //TCP客户端地址
#else
	ACE_INET_Addr  AddrClient;     //UDP客户端地址
#endif
	SGbtFrame      sRecvFrame;     //接收到GBT消息结构体
	SGbtFrame      sSendFrame;     //待发送到GBT消息结构体
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
;

/*
*	各个线程间消息队列交互的消息数据结构
*/
struct SThreadMsg
{
	Ulong ulType;        //消息类型
	Ulong ulThreadPid;   //发送该消息的进程
	Byte  ucMsgOpt;      //消息的附加参数
	Uint  uiMsgDataLen;  //消息数据块长度
	void* pDataBuf;      //数据块存储指针
}PACKED;

/*
	时基调度参数表
*/
struct STimeGroup
{
	Byte   ucId;                            //调度计划号
	Ushort usMonth;                 //调度月        b0-b11   每位1月     1允许执行
	Byte   ucDayWithWeek;     //调度日(按周)  b0-b6    每位每周1天  b0周天 b1：周1
	Ulong  uiDayWithMonth;  //调度日(按月)  b0-b30   每位每月1天
	Byte   ucScheduleId;          //时段表号      0表示无效
}
PACKED;

/*
	时段表参数表
*/
struct SSchedule 
{
	Byte ucId;              //时段表号
	Byte ucEventId;         //时段事件号
	Byte ucHour;            //整点
	Byte ucMin;             //分钟
	Byte ucCtrl;            //控制方式
	Byte ucTimePatternId;   //配时方案号
	Byte ucAuxOut;          //辅助功能输出
	Byte ucSpecialOut;      //特殊功能输出
}
PACKED
;

/*
	配时方案参数
*/
struct STimePattern
{
	Byte ucId;
	Byte ucCycleLen;         //周期时长
	Byte ucPhaseOffset;      //相位差
	Byte ucAdjustPhaseGap;   //协调相位差
	Byte ucScheduleTimeId;   //对应阶段配时表号
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	阶段配时参数
*/
struct SScheduleTime
{
	Byte   ucId;
	Byte   ucScheduleId;  //阶段号
	Uint32 usAllowPhase;  //放行相位
	Byte   ucGreenTime;   //阶段绿灯时间
	Byte   ucYellowTime;  //阶段黄灯时间
	Byte   ucRedTime;
	Byte   ucOption;      //阶段选项
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	相位参数
*/
struct SPhase
{
	Byte ucId;               //相位号(索引)
	Byte ucPedestrianGreen;  //行人绿灯秒数
	Byte ucPedestrianClear;  //行人清空
	Byte ucMinGreen;         //最小绿
	Byte ucGreenDelayUnit;   //单位绿灯延长时间
	Byte ucMaxGreen1;        //最大绿灯时间1
	Byte ucMaxGreen2;        //最大绿灯时间2
	Byte ucFixedGreen;       //弹性相位固定绿灯时间
	Byte ucGreenFlash;       //绿闪时间
	Byte ucType;             //相位类型
	Byte ucOption;           //相位选项
	Byte ucExtend;           //扩展字段
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	跟随相位表参数
*/
struct SOverlapPhase
{
	Byte ucId;
	Byte ucOperateType;             //操作类型
	Byte ucIncludePhaseLen;         //包含相位长度
	Byte ucIncludePhase[MAX_PHASE]; //包含相位 每个字节1个相位号
	Byte ucCorrectPhaseLen;         //修正相位长度
	Byte ucCorrectPhase[MAX_PHASE]; //修正相位
	Byte ucTailGreen;               //尾部绿灯
	Byte ucTailYellow;   
	Byte ucTailRed;  
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	通道参数
*/
struct SChannel
{
	Byte ucId;
	Byte ucSourcePhase; //通道控制信号源，相位号
	Byte ucFlashAuto;   //自动黄闪控制
	Byte ucType;        //类型
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
*	单元参数
*/
struct SUnit
{
	Byte ucStartFlashTime;     //启动时的闪光控制时间
	Byte ucStartAllRedTime;    //启动时的全红控制时间
	Byte ucCtrl;               //当前的信号机控制状态
	Byte ucFlashCtrl;          //当前的闪光控制模式
	Byte ucWarn2;              //信号机报警
	Byte ucWarn1;              //信号机报警
	Byte ucWarnSmary;          //信号机报警摘要
	Byte ucAllowFun;           //允许远程控制实体激活信号机的某些功能
	Byte ucFlashFre;           //闪光频率
	Uint uiShineStartTime;     //辉度控制开启时间
	Uint uiShineDownTime;      //辉度控制关闭时间
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	每路灯泡检测配置情况  ADD:20130731 
*/
struct SChannelCheck
{
	Byte ucSubChannelId;		//通道的子路号   目前4板配置共48路  1---48
	Byte ucIsCheck; 			//是否需要进行灯泡检测 0-不需要 1-需要
	
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
*方向与相位的对应关系
*/

struct SPhaseToDirec
{
	Byte ucId;     //方向号
	Byte ucPhase;  //相位号
	Byte ucOverlapPhase; //跟随相位号
	Byte ucRoadCnt;   //车道数
}
PACKED
;

/*
	信号机参数配置信息
*/
struct STscConfig 
{
	int                   iDetCfg[MAX_DET_BOARD];           //检测器板的起始配置 0 1 17
	STimeGroup            sTimeGroup[MAX_TIMEGROUP];        //时基调度表
	SSchedule             sSchedule[MAX_SCHEDULE_PER_DAY];  //一天的时段表
	STimePattern          sTimePattern[MAX_TIMEPATTERN];    //配时方案表
	SScheduleTime         sScheduleTime[MAX_SCHEDULETIME_TYPE][MAX_SON_SCHEDULE];  //阶段配时表
	SPhase                sPhase[MAX_PHASE];                   //相位表
	GBT_DB::Collision sPhaseConflict[MAX_CONFLICT_PHASE];  //冲突相位表
	SOverlapPhase         sOverlapPhase[MAX_OVERLAP_PHASE];    //跟随相位表
	SChannel              sChannel[MAX_CHANNEL];               //通道表
	SUnit                 sUnit;                               //单元参数
	GBT_DB::Detector  sDetector[MAX_DETECTOR];             //检测器参数
	GBT_DB::SpecFun   sSpecFun[FUN_COUNT];                 //功能定义表
	GBT_DB::DetExtend sDetExtend[MAX_DETECTOR];            //检测器扩展参数
	//ACE_Thread_Mutex  mMutex;  
	SChannelCheck         sChannelChk[MAX_LAMP];         //通道灯泡检测配置   
	SPhaseToDirec         sPhaseToDirec[MAX_DREC] ;     //方向与相位参数   
	Byte DegradeMode ; 									//降级模式 201310191100
	Byte DegradePattern ; 						  	    //降级基准方案 201310191400
	GBT_DB::CntDownDev sCntDownDev[MAX_CNTDOWNDEV];     //倒计时表配置
	Byte cGbType ;             // 国标协议类型    20170501 1120
	//Uint DirecButtonPhase[4] ;    //方向按键相位配置北东南西
 }
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
*每个步伐的相关灯具输出信息
*/
struct SStepInfo
{
	Byte ucLampOn[MAX_LAMP];      //各个灯具亮的情况0：灭1：亮 直接与物理设备ryg对应
	Byte ucLampFlash[MAX_LAMP];   //各个灯具闪的情况
	Byte ucStepLen;               //每个步伐的总运行时间
	Byte ucPhaseColor[MAX_PHASE]; //选中相位在此步伐显示的颜色
	Byte ucOverlapPhaseColor[MAX_OVERLAP_PHASE];  //选中跟随相位相位在此步伐显示的颜色
	Uint uiAllowPhase;    //放行相位 1位1个相位
	Uint uiOverlapPhase;  //跟随相位 1位1个相位
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
*每个相位的步伐信息
*/
struct SPhaseStep
{
	bool bIsAllowPhase;   //1：放行相位 跟随相位
	Byte ucPhaseId;       //相位号，即在相位表的id
	Byte ucStepTime[4];   //绿 绿闪 黄 红 的持续时间  0:为没有该步
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
	信号机动态数据定义
*/
struct STscRunData 
{
	bool              bStartFlash;      //启动时的黄闪  true:闪  false:非闪
	Byte              ucWorkMode;       //信号机运行模式
	Uint			  uiWorkStatus;     //工作状态
	Uint              uiOldWorkStatus;  //上次的工作状态
	Uint              uiCtrl;           //控制方式
	Uint              uiOldCtrl;        //上次的控制方式
	Uint              uiUtcsHeartBeat;  //utcs联网 心跳
	Byte              ucScheduleId;     //时段表号
	Uint              uiScheduleCtrl;   //时段表里的控制方式
	Byte              ucTimePatternId;  //配时方案号
	Byte              ucScheduleTimeId; //阶段配时表号
	Byte			  ucStepNo;         //当前步伐号
	Byte              ucLockPhase;      //锁定特殊相位号
	bool              bOldLock;         //记录上次是否为锁定相位
	Byte              ucStepNum;        //当前周期的步伐总数
	Byte              ucStepTime;       //当前步伐长度
	Byte              ucElapseTime;     //当前步已运行的时间
	//Byte              ucStageElapseTime;//当前阶段已运行的时间
	Byte              ucRunTime;        //当前步还需运行的时间
	Byte              ucCycle;          //周期时长
	SScheduleTime     sScheduleTime[MAX_SON_SCHEDULE];        //当前所有子阶段的集合
	Byte              ucStageIncludeSteps[MAX_SON_SCHEDULE];  //当前各个阶段的各自包含的步伐数
	Byte              ucStageCount;     //当前阶段总数
	Byte              ucStageNo;        //当前阶段号
	bool              bNeedUpdate;      //工作区静态动态参数是否需要更新
	SStepInfo         sStageStepInfo[MAX_STEP];   //各个步伐信息
	//ACE_Thread_Mutex  mMutex;                     //互斥体
	bool              bIsChkLght ;
	bool              b8cndtown ;   //ADD:20131107
	Byte            ucManualType ;//ADD:20141021 当前手控类型	
	Byte			  flashType ;   //ADD:20141106 当系统处于黄闪状态时候黄闪类型
	Byte              ucPlanId ;    //ADD:20150310 时基号
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;



/*
	相位状态输出参数
*/
struct SPhaseSts 
{
	Byte ucId;     //编号
	Byte ucRed;    //红灯输出状态  b0-b7 1 有信号 0 无信号
	Byte ucYellow; //黄灯
	Byte ucGreen;  //绿灯
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
	通道状态输出参数
*/
struct SChannelSts 
{
	Byte ucId;     
	Byte ucRed;     //红灯输出状态 1 活动 0不活动
	Byte ucYellow;
	Byte ucGreen;
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
	跟随相位状态参数
*/
struct SOverlapPhaseSts 
{
	Byte ucId;
	Byte ucRed;      //红灯输出状态标志 每一位1个相位
	Byte ucYellow;
	Byte ucGreen;
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
	检测器状态参数
*/
struct SDetectorSts 
{
	Byte ucId;
	Byte ucStatus;  //状态
	Byte ucAlarm;   //报警
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
	交通检测数据参数
*/
struct SDetectorData 
{
	Byte ucId;
	Byte ucVolume;       //总流量
	Byte ucLongVolume;   //大型车流量
	Byte ucSmallVolume;  //小型车流量
	Byte ucOccupancy;    //占有率
	Byte ucVelocity;     //速度
	Byte ucVehLen;       //车身长度
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
	车辆检测器告警
*/
struct SDetectorAlarm
{
	Byte ucId;
	Byte ucDetAlarm;   //检测器报警状态
	Byte ucCoilAlarm;  //感应线圈报警状态
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

//信号机状态
struct STscStatus
{
	bool bStartFlash;       //启动时的黄闪
	Uint uiWorkStatus;      //工作状态
	Uint uiCtrl;            //控制方式
	Byte ucStepNo;          //当前步伐
	Byte ucStageNo;         //当前阶段
	Byte ucActiveSchNo;     //当前活动的时段表号
	Byte ucTscAlarm2;       //信号机报警2
	Byte ucTscAlarm1;       //信号机报警1
	Byte ucTscAlarmSummary; //信号机报警摘要

	Byte ucCurStageLen[16]; //当前方案各阶段时长
	Byte ucCurKeyGreen[16]; //当前方案各关键相位绿灯时长
	
	Byte ucActiveDetCnt;             //活动检测器总数
	SDetectorSts  sDetSts[8];        //检测器状态
	SDetectorData sDetData[48];      //交通检测器数据
	SDetectorAlarm    sDetAlarm[48]; //检测器报警

	SPhaseSts  sPhaseSts[MAX_PHASE/8];       //相位状态输出
	SOverlapPhaseSts  sOverlapPhaseSts;      //跟随相位状态输出
	SChannelSts  sChannelSts[MAX_CHANNEL/8]; //通道状态输出
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
*相位与检测器的对应关系
*/
struct SPhaseDetector 
{
	int iRoadwayCnt;                 //车道数 或 检测器数
	int iDetectorId[MAX_DETECTOR];   //对应的检测器id
	int iDetectorCarNumbers[MAX_DETECTOR] ; //对应检测器车数量
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;

/*
*新国标GAT508-2014 32个地址倒计时信息
*/
struct SNewGBCntDownTimeInfo
{
	Byte CntDownColor;               //倒计时设备颜色
	Byte CntDownTime;                //倒计时设备时间
}
PACKED
;




/*
*Can数据结构
*/
struct SCanFrame 
{
	Ulong ulCanId;     //canid 4字节32bit 有效位29位 11个基本ID和18个扩展ID
	Byte pCanData[8];  //can数据最大8个字节
	Byte ucCanDataLen; //can数据的有效长度
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;


/*
* Detctor Loop
*
*/
struct SLoopLink
{
	Byte ucForDetId;    //1-16
	Byte ucBackDetId;   //1-16
}
/*
#ifndef WINDOWS
__attribute__( ( packed, aligned(1) ) )
#endif
*/
PACKED
;
/*
*事前分析控制传递参数
*/
struct SPreAnalysisParaData 
{
	Byte DevId ; //设备号	
	bool    IsUsed ;
	bool    IsOffline ; //是否离线
	Ushort CarLength ; //车辆排队长度
	Ushort CarNumber ; //车辆数目
	Byte   CarDirecLamp ; //车辆车道方向
	Byte   PhaseId ;	
};

struct VehicleStat
{
	Ulong       ulId;                       /*编号*/
	Byte       ucDetId;                    /*检测器id*/
	Ulong       ulCarTotal;                 /*车辆总流量排队长度*/
	Byte       ucOccupancy;                /*占有率*/
	Ulong       ulAddtime;                 /*记录添加的时间*/
	bool        bRecordQueueLength ;       //记录排队长度
}PACKED ;

/*Send CntDownNum to client*/
struct SendCntDownNum
{
	bool bSend ;
	bool bUsed ;
	ACE_INET_Addr addClient;  //客户端地址
};

//#pragma pack(pop)
#undef PACKED 
#if !defined(__GNUC__) 
#pragma pack() 
#endif

/*
*主动上报
*/
struct SReportSelf
{
	Byte   ucTick;            //累计次数
	Ushort usCycle;           //周期 单位100ms
	Byte   ucCmd;             //命令字
#ifdef GBT_TCP
	Byte   ucGbtDealDataIndex;  //处理的下标
#else
	ACE_INET_Addr addClient;  //客户端地址
#endif
};




#endif  //_COMSTRUCT_H_
