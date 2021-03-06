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
const Ushort MAX_STAGE_COUNT    = 16;   /*????????????????*/
const Ushort MAX_PHASE_COUNT    = 32;   /*??????????*/
const Byte  MAX_STAGEFACT_CNT  = 30;   //???????? 30??



/*
	????????????
*/
enum
{
    MODULE_OTHER        = 1,        //????????
    MODULE_HARDWARE     = 2,        //????????
    MODULE_SOFTWARE     = 3        //????????
};




 
enum
{
    TBL_SYSTEM,			 //??????
    TBL_CONSTANT,        //??????
    TBL_MODULE,			 //??????
    TBL_PLAN,			 //??????????
    TBL_SCHEDULE,  		 //??????
    TBL_PHASE,			 //??????
    TBL_COLLISION,       //??????????
    TBL_DETECTOR,        //????????
    TBL_CHANNEL,         //??????
    TBL_PATTERN,         //??????
    TBL_STAGEPATTERN,    //??????????
    TBL_OVERLAPPHASE,    //??????????
    TBL_EVENTTYPE,       //??????????
    TBL_EVENTLOG,        //??????????
	TBL_VEHICLESTAT,     //???????? ??????????????????
	TBL_CNTDOWNDEV,      //??????????
	TBL_SPECFUN,         //??????????????
	TBL_DET_EXTEND,      //????????????
	TBL_STAGE_FACTTIME,  //??????????????????
	TBL_ADAPT_STAGETIME, //????????????????????????????
	TBL_PHASE2DIREC,     //????????????????
	TBL_ADAPT_PARA,      //??????????????
   	TBL_LAMP_CHECK,      //??????????????
    TABLE_COUNT          //????????????????
};

/*
	????????????????
*/
typedef struct
{
    int     iTblId;       //????
    int     iFieldCount;  //????????
    char*   sTblName;     //????
    char**  sFieldName;   //????????
}PACKED TableDesc;

/*
	??????????????????
*/
template <class TD, class TC>
class DbArray
{
public:
    DbArray(void)      				//????????????
    {
        m_uCount = 0; 
        m_pData = NULL;
    };
    ~DbArray(void)                  //????????
    {
        if (NULL != m_pData)
        {
            ACE_OS::free(m_pData);
            
        }
        m_pData     = NULL;
        m_uCount    = 0;
    };
    DbArray(const TD* pData, TC uCount)   //????????????
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
    
    DbArray(DbArray& dbArray)               //????????????
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
    
    
    TD* GetData(TC& uCount)    //????????????????
    {
        uCount = m_uCount;
        return m_pData;
    };
    
    
    TD* GetData()              //????????????????
    {
        return m_pData;
    }
    TC GetCount()
    {
        return m_uCount;
    };
    
    
    bool SetData(const TD* pData, TC uCount)   //????????????
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
    
    
    bool SetString(const char* sString)     //??????????????
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
    
    
    bool AllocData(TC uCount)                   //????????????
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


    DbArray &operator = (DbArray& dbArray)    //??????????
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

typedef DbArray<Char, Ushort>       DbString;      //????????

typedef DbArray<Byte, Byte>         DbByteArray;   //????????


/*
????:?????????????????? 0x81-0x84
????:C2.2    ????????????????????????????????????????????
*/
typedef struct
{
    Ushort usDevicId;       /*0x81 ????????????????*/
    Byte  ucMaxModule;     /*0x82 ????????????????????*/
    Ushort usSynchTime;     /*0x83 ????????????*/
    Ushort usSynchFlag;     /*0x84 ????????????*/
}PACKED TblCommonPara;

/*
????:?????????? 0x86
????:C2.3
    ????????????????????????????????????????????????????
    ????????????????????????????????????????
*/
typedef struct
{
    Byte       ucModuleId;                 /*??????????,1-16*/
    DbString    strDevNode;                 /*????????????*/
    DbString    strCompany;                 /*??????????*/
    DbString    strModel;                   /*????????*/
    DbString    strVersion;                 /*????????*/
    Byte       ucType;                     /*????????,??????enModuleType*/
}Module;
typedef DbArray<Module, Byte>     TblModule;  //????????????????????

/*
????:???????????? 0x86-0x8C
????:C3.1
    ????????????????????????????????????????????????
*/
typedef struct
{
    Ulong       ulUtc;                      /*0x86 UTC????,??1970-01-01 00:00:00??????????*/
    Long        lZone;                      /*0x87 ????,??UTC????????????????*/
    Ulong       ulLocal;                    /*0x88 ????????*/
    Byte       ucMaxPlanCnt;               /*0x89 ????????????????????*/
    Byte       ucMaxScheduleCnt;           /*0x8A ??????????????*/
    Byte       ucMaxEvtCount;              /*0x8B ????????????????(????)*/
    Byte       ucActiveSheduleNo;          /*0x8C ????????????????*/
}PACKED TblTimePara;

/*
????:?????????? 0x8D
????:C3.2
*/
typedef struct
{
    Byte       ucId;                       /*??????????,1-40*/
    Ushort      usMonthFlag;                /*????????bit1??bit12????????1??12????*/
    Byte       ucWeekFlag;                 /*????????bit1??bit7??????????????*/
    Ulong       ulDayFlag;                  /*????????bit1??bit7????1????31??*/
    Byte       ucScheduleId;               /*????????*/
}PACKED Plan;
typedef DbArray<Plan, Byte>       TblPlan; //????????????????????

/*
????:??????   0x8E
????:C3.3
*/
typedef struct
{
    Byte       ucScheduleId;               /*????????*/
    Byte       ucEvtId;                    /*??????????*/
    Byte       ucBgnHour;                  /*????????????????*/
    Byte       ucBgnMinute;                /*????????????????*/
    Byte       ucCtrlMode;                 /*????????????????enControlMode*/
    Byte       ucPatternNo;                /*??????????*/
    Byte       ucAuxOut;                   /*????????????*/
    Byte       ucSpecialOut;               /*????????????*/
}PACKED Schedule;
typedef DbArray<Schedule, Ushort>       TblSchedule;  //????????????????


/*
????:??????????
????:C4.1
*/
typedef struct
{
    Byte   ucMaxEvtType;                   /*0x8F ????????????????????*/
    Byte   ucMaxLogLine;                   /*0x90 ????????????????????*/
}PACKED TblEventPara;


/*
????:?????????? 0x91
????:C4.2

*/
typedef struct
{
    Byte       ucEvtTypeId;                /*????????????*/
    Ulong       ulClearTime;                /*??????????????????*/
    DbString    strEvtDesc;                 /*????????????*/
    Byte       ucLogCount;                 /*????????????????????????????*/
}EventType;
typedef DbArray<EventType, Byte>   TblEventType;  //??????????????????



/*
????:?????????? 0x92
????:C4.3
*/
typedef struct
{
    Byte       ucEventId;                  /*????????*/
    Byte       ucEvtType;                  /*????????*/
    Ulong       ulHappenTime;               /*??????????????*/
    Ulong       ulEvtValue;                 /*??????*/
}PACKED EventLog;
typedef DbArray<EventLog, unsigned int>     TblEventLog; //??????????????



typedef struct
{
	Ulong       ulId;                       /*????*/
	Byte       ucDetId;                    /*??????id*/
	Ulong       ulCarTotal;                 /*??????????????????*/
	Byte       ucOccupancy;                /*??????*/
	Ulong       ulAddtime;                 /*??????????????*/
}PACKED VehicleStat;
typedef DbArray<VehicleStat, Ulong>    TblVehicleStat;  //??????????????

/*
????:??????????????
????:C5.1
*/
typedef struct
{
    Byte       ucMaxPhaseCount;            /*0x93 ????????????????*/
    Byte       ucMaxPhaseGrp;              /*0x94 ????????????????????*/
}PACKED PhasePara;


/*
????:??????????????  0x95
????:C5.2
*/
typedef struct
{
    Byte       ucPhaseId;                  /*??????*/
    Byte       ucPedestrianGreen;          /*????????????*/
    Byte       ucPedestrianClear;          /*????????????*/
    Byte       ucMinGreen;                 /*????????????*/
    Byte       ucGreenDelayUnit;           /*????????????????*/
    Byte       ucMaxGreen1;                /*????????????1*/
    Byte       ucMaxGreen2;                /*????????????2*/
    Byte       ucFixGreen;                 /*????????????????????*/
    Byte       ucGreenFlash;               /*????????*/
    Byte       ucPhaseTypeFlag;            /*????????*/
    Byte       ucPhaseOption;              /*????????????*/
    Byte       ucExtend;                   /*????????*/
}PACKED Phase;
typedef DbArray<Phase, Byte>   TblPhase; //??????????????????


/*
????:?????????????? 0x96
????:C5.3
*/
typedef struct
{
    Byte       ucId;                       /*????*/
    Byte       ucRedOut;                   /*??????????????????*/
    Byte       ucYellowOut;                /*??????????????????*/
    Byte       ucGreenOut;                 /*??????????????????*/
}PACKED PhaseSignal;

typedef DbArray<PhaseSignal, Byte>   TblPhaseSignal; //??????????????????????


/*
????:?????????? 0x97
????:C5.4

*/
typedef struct
{
    Byte    ucPhaseId;         /*??????*/
    Uint32  usCollisionFlag ;    /*????????*/
}PACKED Collision;
typedef DbArray<Collision, Byte>   TblCollision;



/*
????:??????????
????:C6.1
*/
typedef struct
{
    Byte       ucMaxDetCount;          /*0x98 ????????????????*/
    Byte       uMaxDetGrpCount;        /*0x99 ????????????????????*/
    Byte       ucDataSeqNo;            /*0x9A ????????????????*/
    Byte       ucDataCycle;            /*0x9B ????????????*/
    Byte       uDetCount;              /*0x9C ????????????????*/
    Byte       ucPulseSeqNo;           /*0x9D ??????????????*/
    Byte       ucPulseCycle;           /*0x9E ????????????????*/
}PACKED DetectorPara;


/*
????:???????????? 0x9F
????:C6.2
*/
typedef struct
{
    Byte       ucDetectorId;       /*??????ID*/
    Byte       ucPhaseId;          /*????????????????*/
    Byte       ucDetFlag;          /*??????????????*/
    Byte       ucDirect;           /*??????????*/
    Byte       ucValidTime;        /*??????????????*/
    Byte       ucOptionFlag;       /*??????????????*/
    Ushort      usSaturationFlow;   /*????????*/
    Byte       ucSaturationOccupy; /*??????????*/
}PACKED Detector;
typedef DbArray<Detector, Byte>   TblDetector;  //??????????????????????


/*
*?????? ????????????
*/
typedef struct
{
	Byte       ucDevId;     
	Ushort      usPhase;  
	Byte       ucOverlapPhase;       
	Byte       ucMode;       
}PACKED CntDownDev;
typedef DbArray<CntDownDev, Byte>   TblCntDownDev; //????????????????

/*
????:?????? ????????????
*/
typedef struct
{
	Byte       ucId;          //????????
	Byte       ucSensi;       //??????
	Byte       ucGrpNo;       //??????????
	Byte       ucPro;         //????
	Byte       ucOcuDefault;  //????????????
	Ushort      usCarFlow;     //????????????
	Byte       ucFrency ;      //????????????  ADD:20120807 10 12
	Byte       ucGrpData ;     //??????????
	Ushort     ucGrpDistns ;   //????????????
}PACKED DetExtend;

typedef DbArray<DetExtend, Byte>  TblDetExtend;  //??????????????????

/*
????:?????? ??????????????
*/
typedef struct
{
	Byte       ucType;         //??????????
	Byte       ucFirstPro;     //????????????????
	Byte       ucSecPro;       //????????????????
	Byte       ucThirdPro;     //????????????????
	Byte       ucOcuPro;       //??????????
	Byte       ucCarFlowPro;   //??????????
	Byte       ucSmoothPara;   //??????????????
}PACKED AdaptPara;

typedef DbArray<AdaptPara, Byte>  TblAdaptPara;  //????????????

/*
????:???????????? 0xA0
????:C6.3
*/
typedef struct
{
    Byte       ucId;               /*????????????????*/
    Byte       ucStatus;           /*??????????*/
    Byte       ucAlarm;            /*??????????*/
}PACKED DetectorStatus;
typedef DbArray<DetectorStatus, Byte>  TblDetectorSts; //????????????????????

/*
????:???????????????? 0xA1
????:C6.4
*/
typedef struct
{
    Byte       ucDectorId;         /*??????????*/
    Byte       ucVehCount;         /*????????????????????*/
    Byte       ucLargeCount;       /*????????????*/
    Byte       ucSmallCount;       /*????????????*/
    Byte       ucOccupy;           /*??????*/
    Byte       ucVelocity;         /*????*/
    Byte       ucVehLeng;          /*????????*/
}PACKED DetectorData;
typedef DbArray<DetectorData, Byte>  TblDetectorData;

/*
????:???????????????? 0xA2
????:C6.5
*/
typedef struct
{

    Byte       ucDetectorId;           /*??????????*/
    Byte       ucDetAlmFlag;           /*??????????????*/
    Byte       ucLoopAlmFlag;          /*????????????*/
}PACKED DetAlmSts;
typedef DbArray<DetAlmSts, Byte>  TblDetAlmSts;

/*
????:??????????????
????:C7.1
*/
typedef struct
{
    Byte       ucFlsTimeWhenStart;     /*0xA3 ????????????????*/
    Byte       ucAllRedTimeWhenStart;  /*0xA4 ????????????????*/
    Byte       ucCtrlSts;              /*0xA5 ????????????????????*/
    Byte       ucFlashCtrlMode;        /*0xA6 ??????????????????*/
    Byte       ucAlarm2;               /*0xA7 ??????????2*/
    Byte       ucAlarm1;               /*0xA8 ??????????1*/
    Byte       ucAlmSts;               /*0xA9 ??????????????*/
    Byte       ucRemoteCtrlFlag;       /*0xAA ????????????*/
    Byte       ucFlashFreq;            /*0xAB ????????*/
    Ulong       ulBrtCtrlStrTime;       /*0xAC ????????????????*/
    Ulong       ulBrtCtrlEndTime;       /*0xAD ????????????????*/
}PACKED TblCtrlUnitPara;

/*
????:????????????
????:C8.1
*/
typedef struct
{
    Byte       ucMaxChannel;           /*0xAE??????????????????????*/
    Byte       ucChnGrpCount;          /*0xAF????????*/
}PACKED TblChannelPara;

/*
????:???????????? 0xB0
????:C8.2
*/
typedef struct
{
    Byte       ucChannelId;            /*??????*/
    Byte       ucCtrlSrc;              /*??????????????????????????*/
    Byte       ucAutoFlsCtrlFlag;      /*??????????????????????????????*/
    Byte       ucCtrlType;             /*????????????*/
}PACKED Channel;
typedef DbArray<Channel, Byte>   TblChannel;

/*
????:?????????????? 0xB1
????:C8.3
*/
typedef struct
{
    Byte   ucId;                       /*????????????????1??2*/
    Byte   ucRedSts;                   /*????????*/
    Byte   ucYellowSts;                /*????????*/
    Byte   ucGreenSts;                 /*????????*/
}PACKED ChannelStatus;
typedef DbArray<ChannelStatus, Byte>   TblChannelSts;

/*
????:??????????
????:C9.1
*/
typedef struct
{
    Byte   ucPatternCount;             /*0xB2 ??????????*/
    Byte   ucMaxStgPatternCount;       /*0xB3 ??????????????????*/
    Byte   ucMaxStageCount;            /*0xB4 ??????????*/
    Byte   ucManuCtrlFlag;             /*0xB5 ????????????????*/
    Byte   ucSysCtrlFlag;              /*0xB6 ????????????????*/
    Byte   ucCtrlMode;                 /*0xB7 ????????*/
    Byte   ucGlobalCycle;              /*0xB8 ????????????*/
    Byte   ucPhaseOffset;              /*0xB9 ??????????*/
    Byte   ucStageSts;                 /*0xBA ????????*/
    Byte   ucStepCmd;                  /*0xBB ????????*/
    Byte   ucDegradeMode;              /*0xBC ????????*/
    Byte   ucDegradePattern[14];       /*0xBD ??????????????*/
    Byte   ucStageTime[MAX_STAGE_COUNT];/*0xBE ????????????????????*/
    Byte   ucGreenTime[MAX_STAGE_COUNT];/*0xBF ????????????????????????????????*/
}PACKED CtrlPara;


/*
????:??????????
????:C9.2
*/
typedef struct
{
    Byte   ucPatternId;                 /*??????*/
    Byte   ucCycleTime;                 /*????????*/
    Byte   ucOffset;                    /*??????*/
    Byte   ucCoorPhase;                /*????????*/
    Byte   ucStagePatternId;           /*????????????*/
}PACKED Pattern;
typedef DbArray<Pattern, Byte>  TblPattern;


/*
????:??????????
????:C9.3
*/
typedef struct
{
    Byte   ucStagePatternId;       /*??????????????*/
    Byte   ucStageNo;              /*??????*/
    Uint32  usAllowPhase;           /*????????*/
    Byte   ucGreenTime;            /*????????????*/
    Byte   ucYellowTime;           /*??????????????*/
    Byte   ucRedTime;              /*????????????*/
    Byte   ucOption;               /*????????????*/
}PACKED StagePattern;
typedef DbArray<StagePattern, Ushort>  TblStagePattern;


/*
????????????????
????:??????????
????:C13.1
*/
typedef struct
{
    Byte ucOverlapPhaseId;          /*????????????*/
    Byte ucOperateType;            /*????????*/
    Byte ucIncldPhaseCnt;          /*??????????????*/
    Byte ucIncldPhase[MAX_PHASE_COUNT];/*????????*/
    Byte ucAdjustPhaseCnt;             /*??????????????*/
    Byte ucAdjustPhase[MAX_PHASE_COUNT];/*????????*/
    Byte ucTailGreen;                  /*????????*/
    Byte ucTailYellow;                  /*????????*/
    Byte ucTailRed;                    /*????????*/
}PACKED OverlapPhase;
typedef DbArray<OverlapPhase, Byte>  TblOverlapPhase;

/*
*????????????????????
*/
typedef struct
{
	Byte ucFunType ;  //????????
	Byte ucValue   ;  //?? ??????????????????????????
}PACKED SpecFun;
typedef DbArray<SpecFun, Byte>  TblSpecFun;

/*
*???????????????????? ??????????
*/
typedef struct
{
	Ulong      uiAddTime;        //??????????
	Byte      ucStageCnt;       //????????
	DbString   sStageGreenTime;  //??????????????????
}StageFactTime;
typedef DbArray<StageFactTime, Ulong> TblStageFactTime;

/*
*????????????????????????????
*/
typedef struct
{
	Byte      ucId;             //??id
	Byte      ucWeekType;       //????
	Byte      ucHour;           //??
	Byte      ucMin;            //??
	Byte      ucStageCnt;       //????????
	DbString   sStageGreenTime;  //??????????????????
}AdaptStageTime;
typedef DbArray<AdaptStageTime, Byte> TblAdaptStageTime;

/*
*????????????????
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
??????????????????  ADD:2013 0801 0901

*/
typedef struct
{	
	Byte ucSubChannelId;		//????????????	 ????4????????48??	1---48
	Byte ucIsCheck; 			//???????????????????? 0-?????? 1-????
}PACKED ChannelChk;
typedef DbArray<ChannelChk,Byte>TblChannelChk;



#undef PACKED 
#if !defined(__GNUC__) 
#pragma pack() 
#endif

/*
	??????????????????
*/
class CSqliteRst
{
public:
    CSqliteRst();
    ~CSqliteRst();
    void Clear();
public:
    char**  m_ppResult;  //??????????????????
    int     m_iRow;      //??????
    int     m_iColum;    //??????
};

class CGbtTscDb
{
public:
    CGbtTscDb();
    ~CGbtTscDb();

    /*????????????*/
    bool InitDb(const char* pDbPath);
    /*??????????*/
    void CloseDb();
    /*????????????*/
	void InitDefaultData(); //ADD:20141209
/***********************??????????????***************************/
    /*????????*/
    bool QueryModule(TblModule& tblModule);

   /*??????????ID????*/
    bool QueryModule(Byte uModuleId, Module& sModule);

    /*??????????????*/
    bool QueryModule(const char* sCompany, TblModule& tblModule);

    /*??????????*/
    bool AddModule(Byte uModuleId, Module& sModule);

    /*??????????*/
    bool ModModule(Byte uModuleId, Module& sModule);

    /*??????????*/
    bool DelModule(Byte uModuleId);

    /*??????????*/
    bool IsModuleValid(Byte ucModuleId, Module& sModule);

/***********************????????????????????????????????***************************/
	/*??????????????????*/
	bool QueryStageFactTime(TblStageFactTime& tblStageFactTime);

	/*????????????????????????????*/
	bool QueryStageFactTime(TblStageFactTime& tblStageFactTime,Ulong ulStartTime,Ulong ulEndTime);

	/*??????????????*/
	bool AddStageFactTime(StageFactTime& sStageFactTime);

	/*??????????????*/
	bool DeleteStageFactTime(Ulong ulAddTime);

/*********************????????????????????????????????????***********************/
	/*????????????????????????*/
	bool QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime);

	/*??????????????????????????*/
	bool QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime,Byte ucWeekType);

	/*????ucid????????*/
	bool QueryAdaptStageTime(Byte ucId, AdaptStageTime& sAdaptStageTime);

	/*????ucid????????*/
	bool ModAdaptStageTime(AdaptStageTime& sAdaptStageTime);

	/*????????????????????*/
	bool AddAdaptStageTime(AdaptStageTime& sAdaptStageTime);

	/*????????????????????????*/
	bool DeleteAdaptStageTime();

/*****************??????????????????*****************************/
    /*????????????????????????*/
    bool QueryPlan(TblPlan& tblPlan);
    
    /*??????????????????????*/
    bool QueryPlan(Byte uPlanId, Plan& sPlan);
    
    /*??????????????,??????????*/
    bool AddPlan(Byte ucPlanId, /*??????????*/
                 Plan& sPlan);    /*??????????*/

    /*??????????????*/
    bool DelPlan(Byte ucPlanId);

	bool DelPlan();

    /*??????????????*/
    bool ModPlan(Byte ucPlanId, Plan& sPlan);

    /*??????????????*/
    bool IsPlanValid(Byte ucPlanId,Plan& sPlan);

/*************************??????**********************************/
    /*??????????*/
    bool QuerySchedule(TblSchedule& tblSchedule);

    /*????????????????????*/
    bool QuerySchedule(Byte uScheduleId, TblSchedule& tblSchedule);


    /*????????????????????????????*/
    bool QuerySchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);
    
    /*??????????*/
    bool AddSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);

    /*??????????*/
    bool ModSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);

    /*??????????*/
    bool DelSchedule(Byte uScheduleId, Byte uEvtId);

    /*????????????????????*/
    bool DelSchedule(Byte uScheduleId);

     /*??????????*/
    bool DelSchedule();

    bool IsScheduleValid(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule);
    
/*****************??????????????*****************************/
    /*??????????????*/
    bool QueryPhase(TblPhase& tblPhase);

    /*??????????????*/
    bool QueryPhase(Byte uPhaseId, Phase& sPhase);
    
    /*????????*/
    bool AddPhase(Byte uPhaseId, Phase& sPhase);  
    
    /*??????????*/
    bool ModPhase(Byte uPhaseId, Phase& sPhase);

    /*??????????*/
    bool DelPhase(Byte uPhaseId);
	bool DelPhase();
    bool IsPhaseValid(Byte uPhaseId, Phase& sPhase);
    
/*********************??????????????????******************************************/
    /*??????????????????*/
    bool QueryCollision(TblCollision& tblCollision);

    /*??????????????*/
    bool QueryCollision(Byte uPhaseId, Collision& sCollision);
    
    /*????????????*/
    bool AddCollision(Byte uPhaseId, Collision& sCollision);  
    
    /*??????????????*/
    bool ModCollision(Byte uPhaseId, Collision& sCollision);

    /*??????????????*/
    bool DelCollision(Byte uPhaseId);

    bool IsCollisionValid(Byte uPhaseId, Collision& sCollision);
/**********************????????????????????*************************************/
    /*??????????????*/
    bool QueryDetPara(DetectorPara& detPara);

public:
/******************************????????????????*************************************/
    /*??????????????*/
    bool QueryDetector(TblDetector& tblDetector);

    /*????????????????*/
    bool QueryDetector(Byte uDetectorId, Detector& sDetector);
    
    /*??????????*/
    bool AddDetector(Byte uDetectorId, Detector& sDetector);  
    
    /*????????????*/
    bool ModDetector(Byte uDetectorId, Detector& sDetector);

    /*????????????*/
    bool DelDetector(Byte uDetectorId);

	bool DelDetector();

    bool IsDetectorValid(Byte uDetectorId, Detector& sDetector);

/********************************????????????????????************************************/
	bool QueryCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev);

	bool QueryCntDownDev(TblCntDownDev& tblCntDownDev);

	bool AddCntDownDev(CntDownDev& sCntDownDev);

	bool DelCntDownDev(Byte ucDevId);

	bool ModCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev);

/*******************************??????????????????????????******************************/
	bool QuerySpecFun(SpecFun& sSpecFun);

	bool QuerySpecFun(TblSpecFun& tblSpecFun);

	bool AddSpecFun(Byte ucFunType,Byte ucValue);

	bool ModSpecFun(Byte ucFunType,Byte ucValue);

/*******************************????????????????????***********************************/
	bool QueryDetExtend(DetExtend& sDetExtend);

	bool QueryDetExtend(Byte uDetId, DetExtend& sDetExtend);

	bool QueryDetExtend(TblDetExtend& tblDetExtend);

	bool AddDetExtend(DetExtend sDetExtend);

	bool ModDetExtend(Byte uDetExtendId, DetExtend& sDetExtend);

/*****************************????????????????????**********************************/
	bool QueryAdaptPara(TblAdaptPara& tblAdaptPara);
	
	bool QueryAdaptPara(AdaptPara& sAdaptPara);

	bool AddAdaptPara(AdaptPara sAdaptPara);

	bool ModAdaptPara(AdaptPara& sAdaptPara);

/********************************??????????????*****************************************/
    /*??????????????*/
    bool QueryChannel(TblChannel& tblChannel);

    /*??????????????*/
    bool QueryChannel(Byte uChannelId, Channel& sChannel);
    
    /*????????*/
    bool AddChannel(Byte uChannelId, Channel& sChannel);  

    /*??????????*/
    bool ModChannel(Byte uChannelId, Channel& sChannel);

    /*??????????????*/
    bool DelChannel(Byte uChannelId);

	bool DelChannel();

    bool IsChannelValid(Byte uChannelId, Channel& sChannel);
/********************************??????????????????*****************************************/
    /*??????????????????*/
    bool QueryPattern(TblPattern& tblPattern);

    /*??????????????*/
    bool QueryPattern(Byte uPatternId, Pattern& sPattern);
    
    /*????????????*/
    bool AddPattern(Byte uPatternId, Pattern& sPattern);  
    
    /*??????????????*/
    bool ModPattern(Byte uPatternId, Pattern& sPattern);

    /*??????????????*/
    bool DelPattern(Byte uPatternId);
	bool DelPattern();
    bool IsPatternValid(Byte uPatternId, Pattern& sPattern);  

/********************************??????????????????*****************************************/
    /*??????????????????*/
    bool QueryStagePattern(TblStagePattern& tblStage);

    /*??????????????????*/
    bool QueryStagePattern(Byte uStagePatternId, TblStagePattern& tblStage);

    /*??????????????????????????*/
    bool QueryStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);
    
    /*????????????????*/
    bool AddStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);  
    
    /*??????????????*/
    bool ModStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);

    /*??????????????*/
    bool DelStagePattern(Byte uStagePatternId);

     /*??????????????*/
    bool DelStagePattern(Byte uStagePatternId, Byte uStageNo);

	bool DelStagePattern();

    bool IsStagePatternValid(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage);
    
    
/********************************??????????????????*****************************************/
    /*??????????????????*/
    bool QueryOverlapPhase(TblOverlapPhase& tblFollow);

    /*??????????????*/
    bool QueryOverlapPhase(Byte uOverlapPhase, OverlapPhase& sOverlapPhase);
    
    /*????????????*/
    bool AddOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);  
    
    /*????????????*/
    bool ModOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);

    /*??????????????*/
    bool DelOverlapPhase(Byte uOverlapPhaseId);

	bool DelOverlapPhase();

    bool IsOverlapPhaseValid(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase);
    
/********************************??????????????????*****************************************/
    /*??????????????*/
    bool QueryEventType(TblEventType& tblEvtType);

    /*??????????????????*/
    bool QueryEventType(Byte uEvtTypeId, EventType& sEvtType);
    
    /*??????????????*/
    bool AddEventType(Byte uEvtTypeId, EventType& sEvtType);  
    
    /*??????????????*/
    bool ModEventType(Byte uEvtTypeId, EventType& sEvtType);

    /*??????????????*/
    bool DelEventType(Byte uEvtTypeId);

    /*??????????????*/
    bool DelEventType();
    
    
/********************************??????????????????*****************************************/
    /*??????????????*/
    bool QueryEventLog(TblEventLog& tblLog);

    /*??????????????????*/
    bool QueryEventLog(Byte uEvtTypeId, TblEventLog& tblLog);

    /*????????ID????*/
    bool QueryEventLog(Byte uEvtLogId, EventLog& sEvtLog);

	/*??????????????????????????????????id*/
	bool GetLogMaxMin(Uint32* pMaxId, Uint32* pMinId);
    
    /*????????????*/
    bool AddEventLog(Uint32* pMaxId, Uint32* pMinId, Byte uEvtType, Ulong uEvtValue);

    /*??????????????????????????*/
    bool DelEventLog(Byte uEvtTypeId);

    /*????????*/
    bool DelEventLog();
	 /*????????*/
   bool DelEventLog(Byte uEvtTypeId ,Uint uStartTime ,Uint uEndTime) ;

/******************************??????????????????******************************/

/*??????????????????????*/
bool QueryChannelChk(TblChannelChk & tblChanChk);//(TblEventType& tblEvtType)

/*??????????????????????*/
bool DelChannelChk();

/*??????????????????????*/
bool AddChannelChk(Byte uChanChkId, ChannelChk& sChannelChk);

/*??????????????????????*/
bool ModChannelChk(Byte uChanChkId, ChannelChk& sChannelChk);

/******************************************************************************/

/******************************??????????????????***************************************/
    bool AddVehicleStat(Byte ucDetId , Ulong ulCarTotal , Byte ucOccupancy);

	bool DelVehicleStat(Ulong ulId);

	bool QueryVehileStat(VehicleStat& sVehicleStat);

	bool QueryVehicleStat(TblVehicleStat& tblVehicleStat);

/****************************????????????????????????*******************************/
	bool QueryPhaseToDirec(TblPhaseToDirec& tblPhaseToDirec);

	bool QueryPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool AddPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool ModPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec);

	bool DelPhaseToDirec();

   /*????????????????*/
    bool GetCommonPara(Ushort& usDeviceId, Byte& ucMaxModule, Byte& usSynchTime, Ushort& usSynchFlag);
   
    /*????????????????*/
    bool SetCommonPara(Ushort* usDeviceId, Byte* ucMaxModule, Byte* usSynchTime, Ushort* usSynchFlag);

    /*????????????????*/
    bool GetCommTimePara(Long& lZone, Byte& ucMaxPlanCnt, Byte& ucMaxScheduleCnt, Byte& ucMaxEvtCount);
    
    /*????????????????*/
    bool SetCommTimePara(Long* lZone, Byte* ucMaxPlanCnt, Byte* ucMaxScheduleCnt, Byte* ucMaxEvtCount);
    
    /*????????????*/
    bool GetEventPara(Byte& ucMaxEvtType, Byte& ucMaxLogCount);
    
    /*????????????*/
    bool SetEventPara(Byte* ucMaxEvtType, Byte* ucMaxLogCount);

    /*????????????*/
    bool GetPhasePara(Byte& ucMaxPhaseCount);

    /*????????????*/
    bool SetPhasePara(Byte& ucMaxPhaseCount);

    /*????????????*/
    bool GetChannelPara(Byte& ucMaxChannel);

    /*????????????*/
    bool SetChannelPara(Byte& ucMaxChannel);   
    
   /*????????????????????*/
    bool GetUnitCtrlPara(Byte& ucFlsTimeWhenStart, Byte& ucAllRedTimeWhenStart, Byte& ucRemoteCtrlFlag, Byte& ucFlashFreq);
    
    /*????????????????????*/
    bool SetUnitCtrlPara(Byte* ucFlsTimeWhenStart, Byte* ucAllRedTimeWhenStart, Byte* ucRemoteCtrlFlag, Byte* ucFlashFreq);
  
    /*????????????????*/
    bool GetPatternPara(Byte& ucMaxPatternCount, Byte& ucMaxStgPatternCount, Byte& ucMaxStageCount);
    
    /*????????????????*/
    bool SetPatternPara(Byte* ucMaxPatternCount, Byte* ucMaxStgPatternCount, Byte* ucMaxStageCount);
  
    /*????????????????*/
    bool GetGlobalCycle(Byte& ucGlobalCycle);
   
    /*????????????????*/
    bool SetGlobalCycle(Byte ucGlobalCycle);

    /*??????????????*/
    bool GetCoorPhaseOffset(Byte& ucPhaseOffset);

    /*??????????????*/
    bool SetCoorPhaseOffset(Byte& ucPhaseOffset);
    
    /*????????????*/
    bool GetDegradeCfg(Byte& ucDegradeMode, Byte& ucDegradePattern);

    /*????????????????*/
    bool GetDegradePattern(DbByteArray& tblDegrade);

    /*??????????????*/
    bool GetDegradePattern(Byte ucDegradeMode, Byte& ucDegradePattern);

    /*????????????*/
    bool SetDegradeMode(Byte ucDegradeMode);

    /*????????????????*/
    bool SetDegradePattern(DbByteArray& tblDegrade);

    /*????????????????*/
    bool SetDegradePattern(Byte ucDegradeMode, Byte ucDegradePattern);

    /*????????????????*/
    bool GetCtrlMasterOptFlag(Byte& ucCtrlOptFlag);
    
    /*????????????????*/
    bool SetCtrlMasterOptFlag(Byte ucCtrlOptFlag);
    
     /*????????????????????*/
    bool GetMultiCtrlPara(Ushort& usBaseAddr, Byte& ucDevCount);
    
    /*????????????????????*/
    bool SetMultiCtrlPara(Ushort* usBaseAddr, Byte* ucDevCount);
    
    /*??????????????????????*/
    bool GetEypSerial(char * SysEypSerial);   //201310101344
    /*??????????????????????*/
    bool SetEypSerial(char *passwd);  //201310101350

     /*??????????*/
    bool GetSystemData(const char* sField, Ulong& ulData);

    /*??????????*/
    bool SetSystemData(const char* sField, Ulong ulData);

    /*????*/
    bool GetFieldData(const char* sTable, const char* sField, Ulong& ulData);

    /*????*/
    bool SetFieldData(const char* sTable, const char* sField, Ulong ulData);

    /*????*/
    bool GetFieldSignData(const char* sTable, const char* sField, Long& lData);

    /*????*/
    bool SetFieldSignData(const char* sTable, const char* sField, Long lData);

private:
    /*????*/
    bool Query(const char* sSql, CSqliteRst& sRst);
    /*????????????*/
    bool ExecuteCmd(const char* sSql);
    /*??????????*/
    bool IsTableExist(const char* sTable);
    void AddDefault();

    /*??????????????*/
    bool AddDetPara(DetectorPara& detPara);

    /*????????????*/
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

