#ifndef _GBTDB_H_
#define _GBTDB_H_

#pragma once

#include <ace/Thread_Mutex.h>
#include <ace/OS.h>
#include "sqlite3.h"
#include "ComDef.h"

namespace GBT_DB
{
#define CONST_MODULE          "ucMaxModule"
#define CONST_PLAN            "ucMaxPlan" 
#define CONST_SCHEDULE        "ucMaxSchedule" 
#define CONST_SCHEDULEEVT     "ucMaxScheduleEvt" 
#define CONST_EVENTTYPE       "ucMaxEventType"
#define CONST_EVENTLOG        "ucMaxEventLog"
#define CONST_DETECTOR        "ucMaxDetector"
#define CONST_PHASE           "ucMaxPhase"
#define CONST_CHANNEL         "ucMaxChannel" 
#define CONST_PATTERN         "ucMaxPattern"
#define CONST_STAGEPATTERN    "ucMaxStagePattern"
#define CONST_STAGE           "ucMaxStage"
#define CONST_OVAERLAPPHASE   "ucMaxOverlapPhase"

#define SYS_DEVICEID        "usDeviceId"
#define SYS_SYNCHTIME       "ucSynchTime"
#define SYS_SYNCHFLAG       "usSynchFlag"
#define SYS_ZONE            "lZone"
#define SYS_DETDATA_SEQNO   "ucDetDataSeqNo"
#define SYS_DETDATA_CYCLE   "ucDetDataCycle"
#define SYS_DETPULSE_SEQNO  "ucDetPulseSeqNo"
#define SYS_DETPULSE_CYCLE  "ucDetPulseCycle"
#define SYS_FLSTIME_START   "ucFlsTimeWhenStart"
#define SYS_REDTIME_START   "ucAllRedTimeWhenStart"
#define SYS_REMOTECTRL_FLAG "ucRemoteCtrlFlag"
#define SYS_FLASH_FREQ      "ucFlashFreq"
#define SYS_BRTTIME_BEGIN   "ulBrtCtrlBgnTime"
#define SYS_BRTTIME_END     "ulBrtCtrlEndTime"
#define SYS_GLOBAL_CYCLE    "ucGlobalCycle"
#define SYS_PHASE_OFFSET    "ucCoorPhaseOffset"
#define SYS_DEGRADE_MODE    "ucDegradeMode"
#define SYS_CTRL_OPTFLAG    "ucCtrlMasterOptFlag"
#define SYS_SIGDEV_COUNT    "ucSigDevCount"
#define SYS_BASE_ADDR       "usBaseAddr"
#define SYS_DOWNLOAD_FLAG   "ucDownloadFlag"

#if defined(__GNUC__) 
#define PACKED  __attribute__((packed)) 
#else 
#pragma pack(1) 
#define PACKED 
#endif   

const Ushort SMALL_BUF_SIZE     = 256;
const Ushort LARGE_BUF_SIZE     = 1024;

const Ushort MAX_STRING_LEN     = 255;
const Ushort MAX_MODULE_COUNT   = 16;
const Ushort MAX_PLAN_COUNT     = 40;
const Ushort MAX_STAGE_COUNT    = 16;   /*方案的最大阶段数*/
const Ushort MAX_PHASE_COUNT    = 32;   /*最大相位数*/
const Byte  MAX_STAGEFACT_CNT  = 30;   //统计信息 30天



/*
	模块类型枚举
*/
enum
{
    MODULE_OTHER        = 1,        //其他类型
    MODULE_HARDWARE     = 2,        //硬件模块
    MODULE_SOFTWARE     = 3        //软件模块
};




 
enum
{
    TBL_SYSTEM,			 //系统表
    TBL_CONSTANT,        //常量表
    TBL_MODULE,			 //模块表
    TBL_PLAN,			 //时基调度表
    TBL_SCHEDULE,  		 //时段表
    TBL_PHASE,			 //相位表
    TBL_COLLISION,       //相位冲突表
    TBL_DETECTOR,        //检测器表
    TBL_CHANNEL,         //通道表
    TBL_PATTERN,         //方案表
    TBL_STAGEPATTERN,    //阶段配时表
    TBL_OVERLAPPHASE,    //跟随相位表
    TBL_EVENTTYPE,       //事件类型表
    TBL_EVENTLOG,        //事件日志表
	TBL_VEHICLESTAT,     //车辆统计 从此以下均为扩展表
	TBL_CNTDOWNDEV,      //倒计时配置
	TBL_SPECFUN,         //特殊功能配置表
	TBL_DET_EXTEND,      //检测器扩展表
	TBL_STAGE_FACTTIME,  //记录各个阶段的时间
	TBL_ADAPT_STAGETIME, //自适应控制生成的各个阶段时间
	TBL_PHASE2DIREC,     //相位与方向对应表
	TBL_ADAPT_PARA,      //自适应控制参数
   	TBL_LAMP_CHECK,      //灯泡通道检测表
    TABLE_COUNT          //信号及系统表数量
};

/*
	系统表信息结构体
*/
typedef struct
{
    int     iTblId;       //序号
    int     iFieldCount;  //表字段数
    char*   sTblName;     //表名
    char**  sFieldName;   //表字段名
}PACKED TableDesc;

/*
	数据库表处理模板类
*/
template <class TD, class TC>
class DbArray
{
public:
    DbArray(void)      				//默认构造函数
    {
        m_uCount = 0; 
        m_pData = NULL;
    };
    ~DbArray(void)                  //析构函数
    {
        if (NULL != m_pData)
        {
            ACE_OS::free(m_pData);
            
        }
        m_pData     = NULL;
        m_uCount    = 0;
    };
    DbArray(const TD* pData, TC uCount)   //带参构造函数
    {
        m_uCount   = 0;
        m_pData    = NULL;
        
        if (0 == uCount || NULL == pData)
        {
            return;
        }
        
        m_pData = (TD*)ACE_OS::malloc(uCount*sizeof(TD));

        if (NULL != pData)
        {
            m_uCount = uCount;
            ACE_OS::memcpy(m_pData, pData, uCount*sizeof(TD));
        }
    };
    
    DbArray(DbArray& dbArray)               //复制构造函数
    {
        m_uCount   = 0;
        m_pData    = NULL;
        
        TC  uCount;
        TD* pData;

        pData = GetData(uCount);
        if (0 == uCount || NULL == pData)
        {
            return;
        }
        
        m_pData = (TD*)ACE_OS::malloc(uCount*sizeof(TD));

        if (NULL != pData)
        {
            m_uCount = uCount;
            ACE_OS::memcpy(m_pData, pData, uCount*sizeof(TD));
        }
    };
    
    
    TD* GetData(TC& uCount)    //带参数据获取函数
    {
        uCount = m_uCount;
        return m_pData;
    };
    
    
    TD* GetData()              //无参数据获取函数
    {
        return m_pData;
    }
    TC GetCount()
    {
        return m_uCount;
    };
    
    
    bool SetData(const TD* pData, TC uCount)   //数据设置函数
    {
        if (NULL != m_pData)
        {
            ACE_OS::free(m_pData);
        }
        
        m_pData = NULL;
        m_uCount = 0;

        if (0 == uCount || NULL == pData)
        {
            return true;
        }
        
        m_pData = (TD*)ACE_OS::malloc(uCount*sizeof(TD));

        if (NULL != m_pData)
        {
            m_uCount = uCount;
            ACE_OS::memcpy(m_pData, pData, uCount*sizeof(TD));
        }
        else
        {
            return false;
        }

        return true;
    };
    
    
    bool SetString(const char* sString)     //字符串设置函数
    {
        if (1 != sizeof(TD))
        {
            return false;
        }

        TC   uCount;
        uCount = (TC)(ACE_OS::strlen(sString) + 1);
        if (!AllocData(uCount))
        {
            return false;
        }

        ACE_OS::strcpy(m_pData, sString);
        return true;
    };
    
    
    bool AllocData(TC uCount)                   //空间分配函数
    {
        if (NULL != m_pData)
        {
            ACE_OS::free(m_pData);
        }
        
        m_pData = NULL;
        m_uCount = 0;

        if (0 == uCount)
        {
            return true;
        }
        
        m_pData = (TD*)ACE_OS::malloc(uCount*sizeof(TD));

        if (NULL != m_pData)
        {
            m_uCount = uCount;
            ACE_OS::memset(m_pData, 0, sizeof(TD)*uCount);
        }
        else
        {
            return false;
        }

        return true;
    };


    DbArray &operator = (DbArray& dbArray)    //运算符重载
    {
        if (NULL != m_pData)
        {
            ACE_OS::free(m_pData);
        }
        
        m_pData = NULL;
        m_uCount = 0;

        
        TC  uCount;
        TD* pData = dbArray.GetData(uCount);

        if (0 == uCount || NULL == pData)
        {
            return *this;
        }

        m_pData = (TD*)ACE_OS::malloc(uCount*sizeof(TD));

        if (NULL != m_pData)
        {
            m_uCount = uCount;
            ACE_OS::memcpy(m_pData, pData, uCount*sizeof(TD));
        }

        return *this;
    };
 private:
    TC      m_uCount;
    TD*     m_pData;
};

typedef DbArray<Char, Ushort>       DbString;      //构造对象

typedef DbArray<Byte, Byte>         DbByteArray;   //构造对象


/*
描述:公共配置相关对象表 0x81-0x84
说明:C2.2    包含了实现大多数设备类型的配置功能的公用对象
*/
typedef struct
{
    Ushort usDevicId;       /*0x81 公共设备识别参数*/
    Byte  ucMaxModule;     /*0x82 公共模块表的最大行数*/
    Ushort usSynchTime;     /*0x83 公共同步时间*/
    Ushort usSynchFlag;     /*0x84 公共同步标志*/
}PACKED TblCommonPara;

/*
描述:模块参数表 0x86
说明:C2.3
    含有软件合硬件厂商的信息，每个关联模块的型号、版本，
    以及模块使硬件产品还是软件产品的定义信息
*/
typedef struct
{
    Byte       ucModuleId;                 /*模块表索引,1-16*/
    DbString    strDevNode;                 /*模块设备节点*/
    DbString    strCompany;                 /*模块制造商*/
    DbString    strModel;                   /*模块型号*/
    DbString    strVersion;                 /*模块版本*/
    Byte       ucType;                     /*模块类型,见枚举enModuleType*/
}Module;
typedef DbArray<Module, Byte>     TblModule;  //定义模块数据表处理类

/*
描述:公共时间参数 0x86-0x8C
说明:C3.1
    包含了实现大多数设备类型的时间相关功能的共用对象
*/
typedef struct
{
    Ulong       ulUtc;                      /*0x86 UTC时间,从1970-01-01 00:00:00至今的秒数*/
    Long        lZone;                      /*0x87 时区,于UTC时间的相差的秒数*/
    Ulong       ulLocal;                    /*0x88 本地时间*/
    Byte       ucMaxPlanCnt;               /*0x89 调度计划表的最大行数*/
    Byte       ucMaxScheduleCnt;           /*0x8A 时段表的最大数*/
    Byte       ucMaxEvtCount;              /*0x8B 时段表最大时段数(事件)*/
    Byte       ucActiveSheduleNo;          /*0x8C 活动时段表的编号*/
}PACKED TblTimePara;

/*
描述:时基调度表 0x8D
说明:C3.2
*/
typedef struct
{
    Byte       ucId;                       /*调度计划号,1-40*/
    Ushort      usMonthFlag;                /*调度月，bit1至bit12分别对应1到12个月*/
    Byte       ucWeekFlag;                 /*调度天，bit1至bit7对应周一至周天*/
    Ulong       ulDayFlag;                  /*调度天，bit1至bit7对应1号至31号*/
    Byte       ucScheduleId;               /*时段表号*/
}PACKED Plan;
typedef DbArray<Plan, Byte>       TblPlan; //定义时基调度表处理类

/*
描述:时段表   0x8E
说明:C3.3
*/
typedef struct
{
    Byte       ucScheduleId;               /*时段表号*/
    Byte       ucEvtId;                    /*时段事件号*/
    Byte       ucBgnHour;                  /*开始执行的整点数*/
    Byte       ucBgnMinute;                /*开始执行的整分数*/
    Byte       ucCtrlMode;                 /*控制方式，见枚举enControlMode*/
    Byte       ucPatternNo;                /*配时方案号*/
    Byte       ucAuxOut;                   /*辅助功能输出*/
    Byte       ucSpecialOut;               /*特殊功能输出*/
}PACKED Schedule;
typedef DbArray<Schedule, Ushort>       TblSchedule;  //定义时段表处理类


/*
描述:事件参数表
说明:C4.1
*/
typedef struct
{
    Byte   ucMaxEvtType;                   /*0x8F 事件类型表的最大行数*/
    Byte   ucMaxLogLine;                   /*0x90 事件日志表的最大行数*/
}PACKED TblEventPara;


/*
描述:事件类型表 0x91
说明:C4.2

*/
typedef struct
{
    Byte       ucEvtTypeId;                /*事件类型编号*/
    Ulong       ulClearTime;                /*事件类型的清除时间*/
    DbString    strEvtDesc;                 /*事件类型描述*/
    Byte       ucLogCount;                 /*该类事件在事件日志表中的行数*/
}EventType;
typedef DbArray<EventType, Byte>   TblEventType;  //定义日志类型处理类



/*
描述:事件日志表 0x92
说明:C4.3
*/
typedef struct
{
    Byte       ucEventId;                  /*事件编号*/
    Byte       ucEvtType;                  /*事件类型*/
    Ulong       ulHappenTime;               /*事件发生的时间*/
    Ulong       ulEvtValue;                 /*事件值*/
}PACKED EventLog;
typedef DbArray<EventLog, unsigned int>     TblEventLog; //定义日志处理类



typedef struct
{
	Ulong       ulId;                       /*编号*/
	Byte       ucDetId;                    /*检测器id*/
	Ulong       ulCarTotal;                 /*车辆总流量排队长度*/
	Byte       ucOccupancy;                /*占有率*/
	Ulong       ulAddtime;                 /*记录添加的时间*/
}PACKED VehicleStat;
typedef DbArray<VehicleStat, Ulong>    TblVehicleStat;  //定义车辆统计类

/*
描述:相位相关参数表
说明:C5.1
*/
typedef struct
{
    Byte       ucMaxPhaseCount;            /*0x93 相位表的最大行数*/
    Byte       ucMaxPhaseGrp;              /*0x94 相位状态组的最大数量*/
}PACKED PhasePara;


/*
描述:相位相关参数表  0x95
说明:C5.2
*/
typedef struct
{
    Byte       ucPhaseId;                  /*相位号*/
    Byte       ucPedestrianGreen;          /*行人绿灯时间*/
    Byte       ucPedestrianClear;          /*行人清空时间*/
    Byte       ucMinGreen;                 /*最小绿灯时间*/
    Byte       ucGreenDelayUnit;           /*单位绿灯延长时间*/
    Byte       ucMaxGreen1;                /*最大绿灯时间1*/
    Byte       ucMaxGreen2;                /*最大绿灯时间2*/
    Byte       ucFixGreen;                 /*弹性相位固定绿灯时间*/
    Byte       ucGreenFlash;               /*绿闪时间*/
    Byte       ucPhaseTypeFlag;            /*相位类型*/
    Byte       ucPhaseOption;              /*相位选项功能*/
    Byte       ucExtend;                   /*扩展字段*/
}PACKED Phase;
typedef DbArray<Phase, Byte>   TblPhase; //定义相位参数处理类


/*
描述:相位输出状态表 0x96
说明:C5.3
*/
typedef struct
{
    Byte       ucId;                       /*索引*/
    Byte       ucRedOut;                   /*相位的红灯输出状态*/
    Byte       ucYellowOut;                /*相位的黄灯输出状态*/
    Byte       ucGreenOut;                 /*相位的绿灯输出状态*/
}PACKED PhaseSignal;

typedef DbArray<PhaseSignal, Byte>   TblPhaseSignal; //定义相位输出状态处理类


/*
描述:相位冲突表 0x97
说明:C5.4

*/
typedef struct
{
    Byte    ucPhaseId;         /*相位号*/
    Uint32  usCollisionFlag ;    /*冲突相位*/
}PACKED Collision;
typedef DbArray<Collision, Byte>   TblCollision;



/*
描述:检测器参数
说明:C6.1
*/
typedef struct
{
    Byte       ucMaxDetCount;          /*0x98 检测器的最大数量*/
    Byte       uMaxDetGrpCount;        /*0x99 检测器状态组的最大数*/
    Byte       ucDataSeqNo;            /*0x9A 检测器数据流水号*/
    Byte       ucDataCycle;            /*0x9B 数据采集周期*/
    Byte       uDetCount;              /*0x9C 活动检测器的个数*/
    Byte       ucPulseSeqNo;           /*0x9D 脉冲数据流水号*/
    Byte       ucPulseCycle;           /*0x9E 脉冲数据采集周期*/
}PACKED DetectorPara;


/*
描述:车辆检测器表 0x9F
说明:C6.2
*/
typedef struct
{
    Byte       ucDetectorId;       /*检测器ID*/
    Byte       ucPhaseId;          /*监测器对应的相位*/
    Byte       ucDetFlag;          /*检测器类型标识*/
    Byte       ucDirect;           /*检测器方向*/
    Byte       ucValidTime;        /*检测器有效时间*/
    Byte       ucOptionFlag;       /*检测器选项参数*/
    Ushort      usSaturationFlow;   /*饱和流量*/
    Byte       ucSaturationOccupy; /*饱和占有率*/
}PACKED Detector;
typedef DbArray<Detector, Byte>   TblDetector;  //定义车辆检测器表处理类


/*
*新增表 倒计时配置表
*/
typedef struct
{
	Byte       ucDevId;     
	Ushort      usPhase;  
	Byte       ucOverlapPhase;       
	Byte       ucMode;       
}PACKED CntDownDev;
typedef DbArray<CntDownDev, Byte>   TblCntDownDev; //定义倒计时设备类

/*
描述:新增表 检测器扩展表
*/
typedef struct
{
	Byte       ucId;          //检测器号
	Byte       ucSensi;       //灵敏度
	Byte       ucGrpNo;       //检测器组号
	Byte       ucPro;         //权值
	Byte       ucOcuDefault;  //占有率默认值
	Ushort      usCarFlow;     //车流量默认值
	Byte       ucFrency ;      //线圈振荡频率  ADD:20120807 10 12
	Byte       ucGrpData ;     //线圈绑定值
	Ushort     ucGrpDistns ;   //线圈绑定距离
}PACKED DetExtend;

typedef DbArray<DetExtend, Byte>  TblDetExtend;  //定义检测器扩展表类

/*
描述:新增表 自适应参数值表
*/
typedef struct
{
	Byte       ucType;         //自适应类型
	Byte       ucFirstPro;     //第一个周期的权值
	Byte       ucSecPro;       //第二个周期的权值
	Byte       ucThirdPro;     //第三个周期的权值
	Byte       ucOcuPro;       //占有率权值
	Byte       ucCarFlowPro;   //车流量权值
	Byte       ucSmoothPara;   //平滑平均法参数
}PACKED AdaptPara;

typedef DbArray<AdaptPara, Byte>  TblAdaptPara;  //定义自适应类

/*
描述:检测器状态表 0xA0
说明:C6.3
*/
typedef struct
{
    Byte       ucId;               /*检测器状态表行号*/
    Byte       ucStatus;           /*检测器状态*/
    Byte       ucAlarm;            /*检测器故障*/
}PACKED DetectorStatus;
typedef DbArray<DetectorStatus, Byte>  TblDetectorSts; //定义检测器状态处理类

/*
描述:检测器数据采集表 0xA1
说明:C6.4
*/
typedef struct
{
    Byte       ucDectorId;         /*检测器编号*/
    Byte       ucVehCount;         /*总流量或中型车的总数*/
    Byte       ucLargeCount;       /*大型车的数量*/
    Byte       ucSmallCount;       /*小型车的数量*/
    Byte       ucOccupy;           /*占有率*/
    Byte       ucVelocity;         /*速度*/
    Byte       ucVehLeng;          /*车身长度*/
}PACKED DetectorData;
typedef DbArray<DetectorData, Byte>  TblDetectorData;

/*
描述:检测器故障报警表 0xA2
说明:C6.5
*/
typedef struct
{

    Byte       ucDetectorId;           /*检测器编号*/
    Byte       ucDetAlmFlag;           /*检测器故障状态*/
    Byte       ucLoopAlmFlag;          /*线圈报警状态*/
}PACKED DetAlmSts;
typedef DbArray<DetAlmSts, Byte>  TblDetAlmSts;

/*
描述:单元相关参数表
说明:C7.1
*/
typedef struct
{
    Byte       ucFlsTimeWhenStart;     /*0xA3 启动时的闪光时间*/
    Byte       ucAllRedTimeWhenStart;  /*0xA4 启动时的全红时间*/
    Byte       ucCtrlSts;              /*0xA5 当前信号机的控制状态*/
    Byte       ucFlashCtrlMode;        /*0xA6 当前的闪光控制模式*/
    Byte       ucAlarm2;               /*0xA7 信号机报警2*/
    Byte       ucAlarm1;               /*0xA8 信号机报警1*/
    Byte       ucAlmSts;               /*0xA9 信号机报警摘要*/
    Byte       ucRemoteCtrlFlag;       /*0xAA 远程控制标识*/
    Byte       ucFlashFreq;            /*0xAB 闪光频率*/
    Ulong       ulBrtCtrlStrTime;       /*0xAC 辉度控制开始时间*/
    Ulong       ulBrtCtrlEndTime;       /*0xAD 灰度控制结束时间*/
}PACKED TblCtrlUnitPara;

/*
描述:灯控端口参数
说明:C8.1
*/
typedef struct
{
    Byte       ucMaxChannel;           /*0xAE信号机支持的最大通道数*/
    Byte       ucChnGrpCount;          /*0xAF通道组数*/
}PACKED TblChannelPara;

/*
描述:灯控端口参数 0xB0
说明:C8.2
*/
typedef struct
{
    Byte       ucChannelId;            /*通道号*/
    Byte       ucCtrlSrc;              /*通道控制的信号源，即相位号*/
    Byte       ucAutoFlsCtrlFlag;      /*自动闪光的控制模式时的通道状态*/
    Byte       ucCtrlType;             /*通道控制类型*/
}PACKED Channel;
typedef DbArray<Channel, Byte>   TblChannel;

/*
描述:通道输出状态表 0xB1
说明:C8.3
*/
typedef struct
{
    Byte   ucId;                       /*通道状态表行号，1，2*/
    Byte   ucRedSts;                   /*红灯状态*/
    Byte   ucYellowSts;                /*黄灯状态*/
    Byte   ucGreenSts;                 /*绿灯状态*/
}PACKED ChannelStatus;
typedef DbArray<ChannelStatus, Byte>   TblChannelSts;

/*
描述:控制参数表
说明:C9.1
*/
typedef struct
{
    Byte   ucPatternCount;             /*0xB2 配时方案数*/
    Byte   ucMaxStgPatternCount;       /*0xB3 最大阶段配时表大小*/
    Byte   ucMaxStageCount;            /*0xB4 最大阶段数*/
    Byte   ucManuCtrlFlag;             /*0xB5 手动控制方案标志*/
    Byte   ucSysCtrlFlag;              /*0xB6 系统控制方案标识*/
    Byte   ucCtrlMode;                 /*0xB7 控制方式*/
    Byte   ucGlobalCycle;              /*0xB8 公共周期时长*/
    Byte   ucPhaseOffset;              /*0xB9 协调相位差*/
    Byte   ucStageSts;                 /*0xBA 阶段状态*/
    Byte   ucStepCmd;                  /*0xBB 步进指令*/
    Byte   ucDegradeMode;              /*0xBC 降级模式*/
    Byte   ucDegradePattern[14];       /*0xBD 降级基准方案表*/
    Byte   ucStageTime[MAX_STAGE_COUNT];/*0xBE 当前方案各阶段的时长*/
    Byte   ucGreenTime[MAX_STAGE_COUNT];/*0xBF 当前方案各个关键相位的的绿灯时长*/
}PACKED CtrlPara;


/*
描述:配时方案表
说明:C9.2
*/
typedef struct
{
    Byte   ucPatternId;                 /*方案号*/
    Byte   ucCycleTime;                 /*周期时长*/
    Byte   ucOffset;                    /*相位差*/
    Byte   ucCoorPhase;                /*协调相位*/
    Byte   ucStagePatternId;           /*阶段配时表号*/
}PACKED Pattern;
typedef DbArray<Pattern, Byte>  TblPattern;


/*
描述:阶段配时表
说明:C9.3
*/
typedef struct
{
    Byte   ucStagePatternId;       /*阶段配时方案号*/
    Byte   ucStageNo;              /*阶段号*/
    Uint32  usAllowPhase;           /*放行相位*/
    Byte   ucGreenTime;            /*阶段绿灯时间*/
    Byte   ucYellowTime;           /*阶段绿黄灯时间*/
    Byte   ucRedTime;              /*阶段红灯时间*/
    Byte   ucOption;               /*阶段选项参数*/
}PACKED StagePattern;
typedef DbArray<StagePattern, Ushort>  TblStagePattern;


/*
跟随相位相关参数
描述:跟随相位表
说明:C13.1
*/
typedef struct
{
    Byte ucOverlapPhaseId;          /*跟随相位表号*/
    Byte ucOperateType;            /*操作类型*/
    Byte ucIncldPhaseCnt;          /*包含相位的个数*/
    Byte ucIncldPhase[MAX_PHASE_COUNT];/*包含相位*/
    Byte ucAdjustPhaseCnt;             /*修正相位的个数*/
    Byte ucAdjustPhase[MAX_PHASE_COUNT];/*修正相位*/
    Byte ucTailGreen;                  /*尾部绿灯*/
    Byte ucTailYellow;                  /*尾部黄灯*/
    Byte ucTailRed;                    /*尾部红灯*/
}PACKED OverlapPhase;
typedef DbArray<OverlapPhase, Byte>  TblOverlapPhase;

/*
*信号机特殊功能配置表
*/
typedef struct
{
	Byte ucFunType ;  //功能类型
	Byte ucValue   ;  //值 不同的功能类型对应不同的值
}PACKED SpecFun;
typedef DbArray<SpecFun, Byte>  TblSpecFun;

/*
*信号机的阶段绿灯时间 自适应控制
*/
typedef struct
{
	Ulong      uiAddTime;        //记录的时间
	Byte      ucStageCnt;       //阶段个数
	DbString   sStageGreenTime;  //各个阶段的绿灯时间
}StageFactTime;
typedef DbArray<StageFactTime, Ulong> TblStageFactTime;

/*
*自适应控制生成的阶段绿灯时间
*/
typedef struct
{
	Byte      ucId;             //表id
	Byte      ucWeekType;       //星期
	Byte      ucHour;           //时
	Byte      ucMin;            //分
	Byte      ucStageCnt;       //阶段个数
	DbString   sStageGreenTime;  //各个阶段的绿灯时间
}AdaptStageTime;
typedef DbArray<AdaptStageTime, Byte> TblAdaptStageTime;

/*
*方向与相位对应表
*/
typedef struct 
{
	Byte ucId;
	Byte ucPhase;
	Byte ucOverlapPhase;
	Byte ucRoadCnt;
}PhaseToDirec;
typedef DbArray<PhaseToDirec, Byte> TblPhaseToDirec;

/*
通道灯泡检测配置表  ADD:2013 0801 0901

*/
typedef struct
{	
	Byte ucSubChannelId;		//通道的子路号	 目前4板配置共48路	1---48
	Byte ucIsCheck; 			//是否需要进行灯泡检测 0-不需要 1-需要
}PACKED ChannelChk;
typedef DbArray<ChannelChk,Byte>TblChannelChk;



#undef PACKED 
#if !defined(__GNUC__) 
#pragma pack() 
#endif

/*
	数据库表集合处理类
*/
class CSqliteRst
{
public:
    CSqliteRst();
    ~CSqliteRst();
    void Clear();
public:
    char**  m_ppResult;  //数据库表查询结果集
    int     m_iRow;      //表行数
    int     m_iColum;    //表列数
};

class CGbtTscDb
{
public:
    CGbtTscDb();
    ~CGbtTscDb();

    /*初始化数据库*/
    bool InitDb(const char* pDbPath);
    /*关闭数据库*/
    void CloseDb();
    /*插入默认数据*/
	void InitDefaultData(); //ADD:20141209
/***********************模块表处理函数***************************/
    /*查询全部*/
    bool QueryModule(TblModule& tblModule);

   /*根据模块表ID查询*/
    bool QueryModule(Byte uModuleId, Module& sModule);

    /*根据生产商查询*/
    bool QueryModule(const char* sCompany, TblModule& tblModule);

    /*添加模块表*/
    bool AddModule(Byte uModuleId, Module& sModule);

    /*修改模块表*/
    bool ModModule(Byte uModuleId, Module& sModule);

    /*删除模块表*/
    bool DelModule(Byte uModuleId);

    /*检查模块表*/
    bool IsModuleValid(Byte ucModuleId, Module& sModule);

/***********************自适应控制的阶段绿灯数据处理函数***************************/
	/*查询全部阶段绿灯表*/
	bool QueryStageFactTime(TblStageFactTime& tblStageFactTime);

	/*查询特定时间段里的阶段绿灯表*/
	bool QueryStageFactTime(TblStageFactTime& tblStageFactTime,Ulong ulStartTime,Ulong ulEndTime);

	/*添加阶段绿灯表*/
	bool AddStageFactTime(StageFactTime& sStageFactTime);

	/*删除阶段绿灯表*/
	bool DeleteStageFactTime(Ulong ulAddTime);

/*********************自适应自学习生成的阶段时间表处理函数***********************/
	/*查询全部自适应阶段绿灯表*/
	bool QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime);

	/*根据星期查询全部阶段绿灯表*/
	bool QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime,Byte ucWeekType);

	/*根据ucid查询记录*/
	bool QueryAdaptStageTime(Byte ucId, AdaptStageTime& sAdaptStageTime);

	/*根据ucid修改记录*/
	bool ModAdaptStageTime(AdaptStageTime& sAdaptStageTime);

	/*添加自适应阶段绿灯表*/
	bool AddAdaptStageTime(AdaptStageTime& sAdaptStageTime);

	/*删除全部自适应阶段绿灯表*/
	bool DeleteAdaptStageTime();

/*****************调度计划表处理函数*****************************/
    /*查询时基调度表调度计划表*/
    bool QueryPlan(TblPlan& tblPlan);
    
    /*根据计划号查询调度计划*/
    bool QueryPlan(Byte uPlanId, Plan& sPlan);
    
    /*添加时基调度表,调度计划表*/
    bool AddPlan(Byte ucPlanId, /*调度计划号*/
                 Plan& sPlan);    /*调度计划表*/

    /*删除时基调度表*/
    bool DelPlan(Byte ucPlanId);

	bool DelPlan();

    /*修改时基调度表*/
    bool ModPlan(Byte ucPlanId, Plan& sPlan);

    /*检查时基调度表*/
    bool IsPlanValid(Byte ucPlanId,Plan& sPlan);

/*************************时段表**********************************/
    /*查询时段表*/
    bool QuerySchedule(TblSchedule& tblSchedule);

    /*根据时段号查询时段表*/
    bool QuerySchedule(Byte uScheduleId, TblSchedule& tblSchedule);


    /*根据时段号和事件号查询时段表*/
    bool QuerySchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);
    
    /*添加时段表*/
    bool AddSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);

    /*修改时段表*/
    bool ModSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);

    /*删除时段表*/
    bool DelSchedule(Byte uScheduleId, Byte uEvtId);

    /*根据时段号删除时段表*/
    bool DelSchedule(Byte uScheduleId);

     /*删除时段表*/
    bool DelSchedule();

    bool IsScheduleValid(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);
    
/*****************相位表处理函数*****************************/
    /*查询所有的相位*/
    bool QueryPhase(TblPhase& tblPhase);

    /*根据相位号查询*/
    bool QueryPhase(Byte uPhaseId, Phase& sPhase);
    
    /*添加相位*/
    bool AddPhase(Byte uPhaseId, Phase& sPhase);  
    
    /*修改相位表*/
    bool ModPhase(Byte uPhaseId, Phase& sPhase);

    /*删除相位表*/
    bool DelPhase(Byte uPhaseId);
	bool DelPhase();
    bool IsPhaseValid(Byte uPhaseId, Phase& sPhase);
    
/*********************相位冲突表处理函数******************************************/
    /*查询所有的相位冲突*/
    bool QueryCollision(TblCollision& tblCollision);

    /*根据相位号查询*/
    bool QueryCollision(Byte uPhaseId, Collision& sCollision);
    
    /*添加相位冲突*/
    bool AddCollision(Byte uPhaseId, Collision& sCollision);  
    
    /*修改相位冲突表*/
    bool ModCollision(Byte uPhaseId, Collision& sCollision);

    /*删除相位冲突表*/
    bool DelCollision(Byte uPhaseId);

    bool IsCollisionValid(Byte uPhaseId, Collision& sCollision);
/**********************检测器参数表处理函数*************************************/
    /*查询检测器参数*/
    bool QueryDetPara(DetectorPara& detPara);

public:
/******************************检测器表处理函数*************************************/
    /*查询所有检测器*/
    bool QueryDetector(TblDetector& tblDetector);

    /*根据检测器号查询*/
    bool QueryDetector(Byte uDetectorId, Detector& sDetector);
    
    /*添加检测器*/
    bool AddDetector(Byte uDetectorId, Detector& sDetector);  
    
    /*修改检测器表*/
    bool ModDetector(Byte uDetectorId, Detector& sDetector);

    /*删除检测器表*/
    bool DelDetector(Byte uDetectorId);

	bool DelDetector();

    bool IsDetectorValid(Byte uDetectorId, Detector& sDetector);

/********************************倒计时配置表处理函数************************************/
	bool QueryCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev);

	bool QueryCntDownDev(TblCntDownDev& tblCntDownDev);

	bool AddCntDownDev(CntDownDev& sCntDownDev);

	bool DelCntDownDev(Byte ucDevId);

	bool ModCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev);

/*******************************特殊功能开启配置表处理函数******************************/
	bool QuerySpecFun(SpecFun& sSpecFun);

	bool QuerySpecFun(TblSpecFun& tblSpecFun);

	bool AddSpecFun(Byte ucFunType,Byte ucValue);

	bool ModSpecFun(Byte ucFunType,Byte ucValue);

/*******************************检测器扩展表处理函数***********************************/
	bool QueryDetExtend(DetExtend& sDetExtend);

	bool QueryDetExtend(Byte uDetId, DetExtend& sDetExtend);

	bool QueryDetExtend(TblDetExtend& tblDetExtend);

	bool AddDetExtend(DetExtend sDetExtend);

	bool ModDetExtend(Byte uDetExtendId, DetExtend& sDetExtend);

/*****************************自适应参数表处理函数**********************************/
	bool QueryAdaptPara(TblAdaptPara& tblAdaptPara);
	
	bool QueryAdaptPara(AdaptPara& sAdaptPara);

	bool AddAdaptPara(AdaptPara sAdaptPara);

	bool ModAdaptPara(AdaptPara& sAdaptPara);

/********************************通道表处理函数*****************************************/
    /*查询所有的通道*/
    bool QueryChannel(TblChannel& tblChannel);

    /*根据通道号查询*/
    bool QueryChannel(Byte uChannelId, Channel& sChannel);
    
    /*添加通道*/
    bool AddChannel(Byte uChannelId, Channel& sChannel);  

    /*修改通道表*/
    bool ModChannel(Byte uChannelId, Channel& sChannel);

    /*删除相位冲突表*/
    bool DelChannel(Byte uChannelId);

	bool DelChannel();

    bool IsChannelValid(Byte uChannelId, Channel& sChannel);
/********************************配时方案表处理函数*****************************************/
    /*查询所有的配时方案*/
    bool QueryPattern(TblPattern& tblPattern);

    /*根据方案号查询*/
    bool QueryPattern(Byte uPatternId, Pattern& sPattern);
    
    /*添加配时方案*/
    bool AddPattern(Byte uPatternId, Pattern& sPattern);  
    
    /*修改配时方案表*/
    bool ModPattern(Byte uPatternId, Pattern& sPattern);

    /*删除相位冲突表*/
    bool DelPattern(Byte uPatternId);
	bool DelPattern();
    bool IsPatternValid(Byte uPatternId, Pattern& sPattern);  

/********************************阶段配时表处理函数*****************************************/
    /*查询所有的阶段配时*/
    bool QueryStagePattern(TblStagePattern& tblStage);

    /*根据阶段配时号查询*/
    bool QueryStagePattern(Byte uStagePatternId, TblStagePattern& tblStage);

    /*根据阶段配时号和阶段号查询*/
    bool QueryStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);
    
    /*添加阶段配时方案*/
    bool AddStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);  
    
    /*修改阶段配时表*/
    bool ModStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);

    /*删除阶段配时表*/
    bool DelStagePattern(Byte uStagePatternId);

     /*删除阶段配时表*/
    bool DelStagePattern(Byte uStagePatternId, Byte uStageNo);

	bool DelStagePattern();

    bool IsStagePatternValid(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);
    
    
/********************************跟随相位表处理函数*****************************************/
    /*查询所有的跟随相位*/
    bool QueryOverlapPhase(TblOverlapPhase& tblFollow);

    /*根据相位号查询*/
    bool QueryOverlapPhase(Byte uOverlapPhase, OverlapPhase& sOverlapPhase);
    
    /*添加跟随相位*/
    bool AddOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);  
    
    /*修改跟随相位*/
    bool ModOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);

    /*删除跟随相位表*/
    bool DelOverlapPhase(Byte uOverlapPhaseId);

	bool DelOverlapPhase();

    bool IsOverlapPhaseValid(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);
    
/********************************事件类型表处理函数*****************************************/
    /*查询事件类型表*/
    bool QueryEventType(TblEventType& tblEvtType);

    /*根据事件类型号查询*/
    bool QueryEventType(Byte uEvtTypeId, EventType& sEvtType);
    
    /*添加事件类型表*/
    bool AddEventType(Byte uEvtTypeId, EventType& sEvtType);  
    
    /*修改事件类型表*/
    bool ModEventType(Byte uEvtTypeId, EventType& sEvtType);

    /*删除事件类型表*/
    bool DelEventType(Byte uEvtTypeId);

    /*删除事件类型表*/
    bool DelEventType();
    
    
/********************************事件日志表处理函数*****************************************/
    /*查询事件日志表*/
    bool QueryEventLog(TblEventLog& tblLog);

    /*根据事件类型号查询*/
    bool QueryEventLog(Byte uEvtTypeId, TblEventLog& tblLog);

    /*根据日志ID查询*/
    bool QueryEventLog(Byte uEvtLogId, EventLog& sEvtLog);

	/*获取日志表的最大行数以及时间最小的id*/
	bool GetLogMaxMin(Uint32* pMaxId, Uint32* pMinId);
    
    /*添加事件日志*/
    bool AddEventLog(Uint32* pMaxId, Uint32* pMinId, Byte uEvtType, Ulong uEvtValue);

    /*根据事件类型删除事件日志表*/
    bool DelEventLog(Byte uEvtTypeId);

    /*删除日志*/
    bool DelEventLog();
	 /*删除日志*/
   bool DelEventLog(Byte uEvtTypeId ,Uint uStartTime ,Uint uEndTime) ;

/******************************通道灯泡检测配置表******************************/

/*查询通道灯泡检测配置表*/
bool QueryChannelChk(TblChannelChk & tblChanChk);//(TblEventType& tblEvtType)

/*删除通道灯泡检测配置表*/
bool DelChannelChk();

/*添加通道灯泡检测配置表*/
bool AddChannelChk(Byte uChanChkId, ChannelChk& sChannelChk);

/*修改通道灯泡检测配置表*/
bool ModChannelChk(Byte uChanChkId, ChannelChk& sChannelChk);

/******************************************************************************/

/******************************车辆统计表处理函数***************************************/
    bool AddVehicleStat(Byte ucDetId , Ulong ulCarTotal , Byte ucOccupancy);

	bool DelVehicleStat(Ulong ulId);

	bool QueryVehileStat(VehicleStat& sVehicleStat);

	bool QueryVehicleStat(TblVehicleStat& tblVehicleStat);

/****************************相位与方向对应表处理函数*******************************/
	bool QueryPhaseToDirec(TblPhaseToDirec& tblPhaseToDirec);

	bool QueryPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool AddPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool ModPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool DelPhaseToDirec();

   /*获取公共配置相关*/
    bool GetCommonPara(Ushort& usDeviceId, Byte& ucMaxModule, Byte& usSynchTime, Ushort& usSynchFlag);
   
    /*修改公共配置参数*/
    bool SetCommonPara(Ushort* usDeviceId, Byte* ucMaxModule, Byte* usSynchTime, Ushort* usSynchFlag);

    /*获取公共时间参数*/
    bool GetCommTimePara(Long& lZone, Byte& ucMaxPlanCnt, Byte& ucMaxScheduleCnt, Byte& ucMaxEvtCount);
    
    /*修改公共时间参数*/
    bool SetCommTimePara(Long* lZone, Byte* ucMaxPlanCnt, Byte* ucMaxScheduleCnt, Byte* ucMaxEvtCount);
    
    /*获取事件参数*/
    bool GetEventPara(Byte& ucMaxEvtType, Byte& ucMaxLogCount);
    
    /*设置事件参数*/
    bool SetEventPara(Byte* ucMaxEvtType, Byte* ucMaxLogCount);

    /*获取相位参数*/
    bool GetPhasePara(Byte& ucMaxPhaseCount);

    /*设置相位参数*/
    bool SetPhasePara(Byte& ucMaxPhaseCount);

    /*获取灯控参数*/
    bool GetChannelPara(Byte& ucMaxChannel);

    /*设置灯控参数*/
    bool SetChannelPara(Byte& ucMaxChannel);   
    
   /*获取控制单元相关参数*/
    bool GetUnitCtrlPara(Byte& ucFlsTimeWhenStart, Byte& ucAllRedTimeWhenStart, Byte& ucRemoteCtrlFlag, Byte& ucFlashFreq);
    
    /*设置控制单元相关参数*/
    bool SetUnitCtrlPara(Byte* ucFlsTimeWhenStart, Byte* ucAllRedTimeWhenStart, Byte* ucRemoteCtrlFlag, Byte* ucFlashFreq);
  
    /*获取配时方案参数*/
    bool GetPatternPara(Byte& ucMaxPatternCount, Byte& ucMaxStgPatternCount, Byte& ucMaxStageCount);
    
    /*设置配时方案参数*/
    bool SetPatternPara(Byte* ucMaxPatternCount, Byte* ucMaxStgPatternCount, Byte* ucMaxStageCount);
  
    /*获取公共周期时长*/
    bool GetGlobalCycle(Byte& ucGlobalCycle);
   
    /*设置公共周期时长*/
    bool SetGlobalCycle(Byte ucGlobalCycle);

    /*获取协调相位差*/
    bool GetCoorPhaseOffset(Byte& ucPhaseOffset);

    /*设置协调相位差*/
    bool SetCoorPhaseOffset(Byte& ucPhaseOffset);
    
    /*获取降级配置*/
    bool GetDegradeCfg(Byte& ucDegradeMode, Byte& ucDegradePattern);

    /*获取降级基准方案*/
    bool GetDegradePattern(DbByteArray& tblDegrade);

    /*根据方案号获取*/
    bool GetDegradePattern(Byte ucDegradeMode, Byte& ucDegradePattern);

    /*设置降级配置*/
    bool SetDegradeMode(Byte ucDegradeMode);

    /*设置降级基准方案*/
    bool SetDegradePattern(DbByteArray& tblDegrade);

    /*设置降级基准方案*/
    bool SetDegradePattern(Byte ucDegradeMode, Byte ucDegradePattern);

    /*获取控制主机参数*/
    bool GetCtrlMasterOptFlag(Byte& ucCtrlOptFlag);
    
    /*设置控制主机参数*/
    bool SetCtrlMasterOptFlag(Byte ucCtrlOptFlag);
    
     /*获取多路口信号机参数*/
    bool GetMultiCtrlPara(Ushort& usBaseAddr, Byte& ucDevCount);
    
    /*设置多路口信号机参数*/
    bool SetMultiCtrlPara(Ushort* usBaseAddr, Byte* ucDevCount);
    
    /*获取保存加密验证字符串*/
    bool GetEypSerial(char * SysEypSerial);   //201310101344
    /*设置保存加密验证字符串*/
    bool SetEypSerial(char *passwd);  //201310101350

     /*查询系统表*/
    bool GetSystemData(const char* sField, Ulong& ulData);

    /*设置系统表*/
    bool SetSystemData(const char* sField, Ulong ulData);

    /*查询*/
    bool GetFieldData(const char* sTable, const char* sField, Ulong& ulData);

    /*设置*/
    bool SetFieldData(const char* sTable, const char* sField, Ulong ulData);

    /*查询*/
    bool GetFieldSignData(const char* sTable, const char* sField, Long& lData);

    /*设置*/
    bool SetFieldSignData(const char* sTable, const char* sField, Long lData);

private:
    /*查询*/
    bool Query(const char* sSql, CSqliteRst& sRst);
    /*删除、修改等*/
    bool ExecuteCmd(const char* sSql);
    /*表是否存在*/
    bool IsTableExist(const char* sTable);
    void AddDefault();

    /*添加检测器参数*/
    bool AddDetPara(DetectorPara& detPara);

    /*检测系统参数*/
    bool IsSysParaValid(const char* sField, Ulong& ulValue);

    bool IsSysParaValid(const char* sField, Long& lValue);
private:
    sqlite3*            m_pSqlite;
    ACE_Thread_Mutex    m_mutexDb;
};
extern TableDesc* m_gTableDesc;
extern CGbtTscDb* m_gTscDb;

int ExchangeData(int iOptType,
                Byte bObjId, 
                Byte uIdxFst, 
                Byte uIdxSnd, 
                Byte uSubId, 
                Byte* pData, 
                int uDataSize,
                Byte& uErrorSts,
                Byte& uErrorIdx);

Byte GetFieldsValue(const char* sConstant, const char uSplit, Byte* sValue, Byte uValSize);

#define UNPACKET_ULONG(sValueSeq, ulValue) \
ulValue = ((Ulong)((sValueSeq)[0])<<24)|((Ulong)((sValueSeq)[1])<<16)|((Ulong)((sValueSeq)[2])<<8)|(sValueSeq)[3]

#define UNPACKET_USHORT(sValueSeq, usValue) \
usValue = ((Ushort)((sValueSeq)[0])<<8)|(sValueSeq)[1]

#define UNPACKET_UCHAR(sValueSeq, ucValue) \
ucValue = sValueSeq[0]

#define PACKET_ULONG(sValueSeq, ulValue) \
(sValueSeq)[0] = (Byte)((ulValue>>24)&0xFF);\
(sValueSeq)[1] = (Byte)((ulValue>>16)&0xFF);\
(sValueSeq)[2] = (Byte)((ulValue>>8)&0xFF);\
(sValueSeq)[3] = (Byte)(ulValue&0xFF);

#define PACKET_USHORT(sValueSeq, usValue) \
(sValueSeq)[0] = (Byte)((usValue>>8)&0xFF);\
(sValueSeq)[1] = (Byte)(usValue&0xFF);

#define PACKET_UCHAR(sValueSeq, ucValue) \
(sValueSeq)[0] = (Byte)(ucValue&0xFF);

#define PACKET_DATA(VALSIZE, SEQ, DATA)\
    switch (VALSIZE)\
    {\
    case 4:\
        PACKET_ULONG(SEQ, DATA);\
        break;\
    case 2:\
        PACKET_USHORT(SEQ, DATA);\
        break;\
    default:\
        PACKET_UCHAR(SEQ, DATA);\
    }

#define UNPACKET_DATA(VALSIZE, SEQ, DATA)\
    switch (VALSIZE)\
    {\
    case 4:\
        UNPACKET_ULONG(SEQ, DATA);\
        break;\
    case 2:\
        UNPACKET_USHORT(SEQ, DATA);\
        break;\
    default:\
        UNPACKET_UCHAR(SEQ, DATA);\
    }
}
#endif

