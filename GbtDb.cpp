
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   GbtDb.cpp
Date:       2013-1-1
Description:ÐÅºÅ»ú´¦ÀíÓëÊý¾Ý¿â½»»¥ÓÐ¹ØµÄ²Ù×÷
Version:    V1.0
History:
***************************************************************/
#include <ace/OS.h>
#include <ace/Guard_T.h>
#include <ace/Log_Msg.h>

#include "GbtDb.h"
#include "GbtExchg.h"
#include "ComStruct.h"
#include "ComFunc.h"
#include "Dbinstance.h"

#ifndef SHORT_FILE
#define SHORT_FILE "GbtDb.cpp" //MOD: 201309231000ÕûÀíË³Ðò
#endif

namespace GBT_DB  //¶¨ÒåÈ«¾ÖÊý¾Ý¿âÃüÃû¿Õ¼ä
{
	CGbtTscDb*  m_gTscDb = NULL;
	
/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íÃû×Ö·ûÊý×é*******************/
	char* Table_Name[]   = {(char*)"Tbl_System",
                         (char*)"Tbl_Constant",
                         (char*)"Tbl_Module", 
                         (char*)"Tbl_Plan", 
                         (char*)"Tbl_Schedule", 
                         (char*)"Tbl_Phase", 
                         (char*)"Tbl_Collision",
                         (char*)"Tbl_Detector",
                         (char*)"Tbl_Channel", 
                         (char*)"Tbl_Pattern", 
                         (char*)"Tbl_StagePattern",
                         (char*)"Tbl_OverlapPhase",
                         (char*)"Tbl_EventType",
                         (char*)"Tbl_EventLog",
                         (char*)"Tbl_VehicleStat",
                         (char*)"Tbl_CntDownDev",
                         (char*)"Tbl_SpecFun",
						 (char*)"Tbl_DetExtend",
						 (char*)"Tbl_StageFactTime",
						 (char*)"Tbl_AdaptStageTime",
						 (char*)"Tbl_PhaseToDirec",
						 (char*)"Tbl_AdaptPara" ,
						 (char *)"Tbl_LampCheck"   //ADD:2013 0801 0940
						};
/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íÃû×Ö·ûÊý×é*******************/

/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íµÄ×Ö¶ÎÃû×Ö·ûÊý×é*******************/
char* Tbl_LampCheck[] ={
						(char *)"ucId",
						(char *)"ucFlag"	};

char* Tbl_Plan[]     = {(char*)"Id", 
                        (char*)"usMonthFlag", 
                        (char*)"ucWeekFlag", 
                        (char*)"ulDayFlag", 
                        (char*)"ucScheduleId"};

char* Tbl_Schedule[] = {(char*)"ucScheduleId", 
                        (char*)"ucEvtId", 
                        (char*)"ucBgnHour", 
                        (char*)"ucBgnMinute", 
                        (char*)"ucCtrlMode", 
                        (char*)"ucPatternNo", 
                        (char*)"ucAuxOut",
                        (char*)"ucSpecialOut"};

char* Tbl_Module[]   = {(char*)"ucModuleId", 
                        (char*)"sDevNode",
                        (char*)"sCompany",
                        (char*)"sModel",
                        (char*)"sVersion",
                        (char*)"ucType"};

char* Tbl_Phase[]    = {(char*)"ucPhaseId",
                        (char*)"ucPedestrianGreen",
                        (char*)"ucPedestrianClear",
                        (char*)"ucMinGreen",
                        (char*)"ucGreenDelayUnit",
                        (char*)"ucMaxGreen1",
                        (char*)"ucMaxGreen2",
                        (char*)"ucFixGreen",
                        (char*)"ucGreenFlash",
                        (char*)"ucPhaseTypeFlag",
                        (char*)"ucPhaseOption",
                        (char*)"ucExtend"};

char* Tbl_Collision[] = {(char*)"ucPhaseId",
                         (char*)"usCollisonFlag"};

char* Tbl_Detector[]   = {(char*)"ucDetectorId",
                        (char*)"ucPhaseId",
                        (char*)"ucDetFlag",
                        (char*)"ucDirect",
						(char*)"ucValidTime",
                        (char*)"ucOptionFlag",
                        (char*)"usSaturationFlow",
                        (char*)"ucSaturationOccupy"};

char* Tbl_Channel[]   = {(char*)"ucChannelId",
                         (char*)"ucCtrlSrc",
                         (char*)"ucAutoFlsCtrlFlag",
                         (char*)"ucCtrlType"};

char* Tbl_Pattern[]   = {(char*)"ucPatternId",
                        (char*)"ucCycleTime",
                        (char*)"ucOffset",
                        (char*)"ucCoorPhase",
                        (char*)"ucStagePatternId"};

char* Tbl_StagePattern[] = {(char*)"ucStagePatternId",
                          (char*)"ucStageNo",
                          (char*)"usAllowPhase",
                          (char*)"ucGreenTime",
                          (char*)"ucYellowTime",
                          (char*)"ucRedTime",
                          (char*)"ucOption"};

char* TBL_OverlapPhase[] = {(char*)"ucOverlapPhaseId",
                        (char*)"ucOperateType",
                        (char*)"ucIncldPhaseCount",
                        (char*)"sIncldPhase",
                        (char*)"ucAdjustPhaseCount",
                        (char*)"sAdjustPhase",
                        (char*)"ucTailGreen",
                        (char*)"ucTailYellow",
                        (char*)"ucTailRed"};

char* Tbl_EventType[] = {(char*)"ucEvtTypeId",
                        (char*)"ulClearTime",
                        (char*)"sEvtDesc"};

char* Tbl_EventLog[] = {(char*)"ucEventId",
                        (char*)"ucEvtType",
                        (char*)"ulHappenTime",
                        (char*)"ulEvtValue",
                        (char*)"ulEventTime",
                        };

char* Tbl_VehicleStat[] = {(char*)"ulId",
                           (char*)"ucDetId",
						   (char*)"ulCarTotal",
						   (char*)"ucOccupancy",
						   (char*)"ulAddTime"};

char* Tbl_CntDownDev[] = {(char*)"ucDevId",
                          (char*)"usPhase",
						  (char*)"ucOverlapPhase",
						  (char*)"ucMode"
						  };

char* Tbl_SpecFun[] = {(char*)"ucFunType",
					   (char*)"ucValue",};

char* Tbl_DetExtend[] = {(char*)"ucId",
						 (char*)"ucSensi",
						 (char*)"ucGrpNo",
					     (char*)"ucPro",
						 (char*)"ucOcuDefault",
						 (char*)"usCarFlow",
						  (char *)"ucFrency",
						 (char *)"ucGrpData",
						 (char *)"ucGrpDistns"
						 };

char* Tbl_StageFactTime[] = {(char*)"ulAddTime",
						     (char*)"ucStageCnt",
							 (char*)"sStageGreenTime"};

char* Tbl_AdaptStageTime[] = {(char*)"ucId",
							  (char*)"ucWeekType",
							  (char*)"ucHour",
							  (char*)"ucMin",
							  (char*)"ucStageCnt",
							  (char*)"sStageTime"};

char* Tbl_PhaseToDirec[] = {(char*)"ucId",
							(char*)"ucPhase",
							(char*)"ucOverlapPhase",
							(char*)"ucRoadCnt"
							};

char* Tbl_AdaptPara[] = {(char*)"ucType",
						 (char*)"ucFirstPro",
						 (char*)"ucSecPro",
						 (char*)"ucThirdPro",
						 (char*)"ucOcuPro",
						 (char*)"ucCarFlowPro",
						 (char*)"ucSmoothPara"
						};
/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íµÄ×Ö¶ÎÃû×Ö·ûÊý×é*******************/


/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íµÄ´´½¨Óï·¨×Ö·û*******************/
char* Table_Script[] = {
(char*)"create table Tbl_System(\n\
usDeviceId int not null,\n\
ucSynchTime tinyint not null default(0),\n\
usSynchFlag int not null default(0),\n\
lZone int not null default(0),\n\
ucDetDataSeqNo tinyint not null default(1),\n\
ucDetDataCycle tinyint not null default(180),\n\
ucDetPulseSeqNo tinyint not null default(1),\n\
ucDetPulseCycle tinyint not null default(30),\n\
ucFlsTimeWhenStart tinyint not null default(12),\n\
ucAllRedTimeWhenStart tinyint not null default(6),\n\
ucRemoteCtrlFlag tinyint not null default(0),\n\
ucFlashFreq tinyint not null default(1),\n\
ulBrtCtrlBgnTime bigint not null default(0),\n\
ulBrtCtrlEndTime bigint not null default(0),\n\
ucGlobalCycle tinyint not null default(0),\n\
ucCoorPhaseOffset tinyint not null default(0),\n\
ucDegradeMode tinyint not null default(0),\n\
sDegradePattern varchar(256) not null default(' '),\n\
ucCtrlMasterOptFlag tinyint not null default(0),\n\
ucDownloadFlag tinyint not null default(0),\n\
usBaseAddr int not null default(0),\n\
ucSigDevCount tinyint not null default(0),\n\
ucEypDevSerial  varchar(256) not null default(''))",
/*************************************************/
(char*)"create table Tbl_Constant(\n\
ucMaxModule tinyint not null default(16),\n\
ucMaxPlan tinyint not null default(40),\n\
ucMaxSchedule tinyint not null default(16),\n\
ucMaxScheduleEvt tinyint not null default(48),\n\
ucMaxEventType tinyint not null default(255),\n\
ucMaxEventLog tinyint not null default(255),\n\
ucMaxDetector tinyint not null default(48),\n\
ucMaxPhase tinyint not null default(32),\n\
ucMaxChannel tinyint not null default(32),\n\
ucMaxPattern tinyint not null default(32),\n\
ucMaxStagePattern tinyint not null default(16),\n\
ucMaxStage tinyint not null default(16),\n\
ucMaxOverlapPhase tinyint not null default(16))",
/******************************************************/
(char*)"create table Tbl_Module(\n\
ucModuleId tinyint primary key,\n\
sDevNode varchar(256) not null,\n\
sCompany varchar(256) not null,\n\
sModel   varchar(256) not null,\n\
sVersion varchar(256) not null,\n\
ucType   tinyint not null default(0))",
/*********************************/
(char*)"create table Tbl_Plan(\n\
Id tinyint  primary key,\n\
usMonthFlag int not null default(0),\n\
ucWeekFlag  tinyint not null default(0),\n\
ulDayFlag   bigint not null default(0),\n\
ucScheduleId tinyint not null default(0))",
/***************************************/
(char*)"create table Tbl_Schedule(\n\
ucScheduleId tinyint,\n\
ucEvtId tinyint,\n\
ucBgnHour tinyint not null default(0),\n\
ucBgnMinute tinyint not null default(0),\n\
ucCtrlMode tinyint not null default(0),\n\
ucPatternNo tinyint not null default(0),\n\
ucAuxOut tinyint not null default(0),\n\
ucSpecialOut tinyint not null default(0),\n\
CONSTRAINT Schedule_Key PRIMARY KEY (ucScheduleId, ucEvtId))",
/*****************************************************/
(char*)"create table Tbl_Phase(\n\
 ucPhaseId tinyint primary key,\n\
 ucPedestrianGreen tinyint not null default(10),\n\
 ucPedestrianClear tinyint not null default(3),\n\
 ucMinGreen tinyint not null default(15),\n\
 ucGreenDelayUnit tinyint not null default(3),\n\
 ucMaxGreen1 tinyint not null default(45),\n\
 ucMaxGreen2 tinyint not null default(60),\n\
 ucFixGreen tinyint not null default(20),\n\
 ucGreenFlash tinyint not null default(3),\n\
 ucPhaseTypeFlag tinyint not null default(0),\n\
 ucPhaseOption tinyint not null default(0),\n\
 ucExtend tinyint not null default(0))",
 /**********************************************/
(char*)"create table Tbl_Collision(\n\
ucPhaseId tinyint primary key,\n\
usCollisonFlag int not null default(0))",
/***********************************************/
(char*)"create table Tbl_Detector(\n\
ucDetectorId tinyint primary key,\n\
ucPhaseId tinyint not null default(0),\n\
ucDetFlag tinyint not null default(0),\n\
ucDirect tinyint not null default(0),\n\
ucValidTime tinyint not null default(0),\n\
ucOptionFlag tinyint not null default(0),\n\
usSaturationFlow int not null default(0),\n\
ucSaturationOccupy tinyint not null default(0))",
/***************************************************/
(char*)"create table Tbl_Channel(\n\
ucChannelId tinyint primary key\n,\
ucCtrlSrc tinyint not null default(0),\n\
ucAutoFlsCtrlFlag tinyint not null default(0),\n\
ucCtrlType tinyint not null default(0))",
/********************************************************/
(char*)"create table Tbl_Pattern(\n\
ucPatternId tinyint primary key,\n\
ucCycleTime tinyint not null default(0),\n\
ucOffset tinyint not null default(0),\n\
ucCoorPhase tinyint not null default(0),\n\
ucStagePatternId tinyint not null default(0))",
/*************************************************************/
(char*)"create table Tbl_StagePattern(\n\
ucStagePatternId tinyint,\n\
ucStageNo tinyint,\n\
usAllowPhase int not null default(0),\n\
ucGreenTime tinyint not null default(0),\n\
ucYellowTime tinyint not null default(0),\n\
ucRedTime tinyint not null default(0),\n\
ucOption tinyint not null default(0),\n\
CONSTRAINT StagePattern_Key PRIMARY KEY (ucStagePatternId, ucStageNo))",
/*********************************************************************/
(char*)"create table Tbl_OverlapPhase(\n\
ucOverlapPhaseId tinyint primary key,\n\
ucOperateType tinyint not null default(0),\n\
ucIncldPhaseCount tinyint not null default(0),\n\
sIncldPhase varchar(256) not null,\n\
ucAdjustPhaseCount tinyint not null default(0),\n\
sAdjustPhase varchar(256) not null,\n\
ucTailGreen tinyint not null default(0),\n\
ucTailYellow tinyint not null default(0),\n\
ucTailRed tinyint not null default(0))",
/*******************************************************/
(char*)"create table Tbl_EventType(\n\
ucEvtTypeId tinyint primary key,\n\
ulClearTime bigint not null default(0),\n\
sEvtDesc varchar(256) not null,ucLogCount tinyint not null  default(0))",
/*********************************************************/

(char*)"create table Tbl_EventLog(\n\
ucEventId bigint not null,\n\
ucEvtType tinyint not null default(0),\n\
ulHappenTime bigint not null default(0),\n\
ulEvtValue bigint not null default(0),\n\
ulEventTime varchar(20))",

/*******************************************************/
(char*)"create table Tbl_VehicleStat(\n\
						  ulId bigint primary key,\n\
						  ucDetId tinyint not null default(0),\n\
						  ulCarTotal bigint not null default(0),\n\
						  ucOccupancy tinyint not null default(0),\n\
						  ulAddTime bigint not null default(0))",

/*******************************************************/
(char*)"create table Tbl_CntDownDev(\n\
						  ucDevId tinyint not null,\n\
						  usPhase int not null default(0),\n\
						  ucOverlapPhase tinyint not null default(0),\n\
						  ucMode tinyint not null default(0))",

/******************************************************/
(char*)"create table Tbl_SpecFun(\n\
						ucFunType tinyint not null,\n\
						ucValue tinyint not null default(0))",

/******************************************************/
(char*)"create table Tbl_DetExtend(\n\
						ucId tinyint primary key,\n\
						ucSensi tinyint not null default(0),\n\
						ucGrpNo tinyint not null default(0),\n\
						ucPro tinyint not null default(0),\n\
						ucOcuDefault tinyint not null default(0),\n\
						usCarFlow smallint not null default(0),\n\
						ucFrency  tinyint not null default(0),\n\
						ucGrpData smallint not null default(0),\n\
						ucGrpDistns smallint not null default(0))",

/******************************************************/
(char*)"create table Tbl_StageFactTime(\n\
						ulAddTime int primary key,\n\
						ucStageCnt tinyint not null default(0),\n\
						sStageGreenTime varchar(32) not null)",

/******************************************************/
(char*)"create table Tbl_AdaptStageTime(\n\
						ucId INTEGER primary key,\n\
						ucWeekType tinyint not null default(0),\n\
						ucHour tinyint not null default(0),\n\
						ucMin tinyint not null default(0),\n\
						ucStageCnt tinyint not null default(0),\n\
						sStageTime varchar(32) not null)",

/******************************************************/
(char*)"create table Tbl_PhaseToDirec(\n\
						ucId tinyint primary key,\n\
						ucPhase tinyint not null default(0),\n\
						ucOverlapPhase tinyint not null default(0),\n\
						ucRoadCnt tinyint not null default(0))",

/******************************************************/
(char*)"create table Tbl_AdaptPara(\n\
						ucType tinyint not null,\n\
						ucFirstPro tinyint not null default(0),\n\
						ucSecPro tinyint not null default(0),\n\
						ucThirdPro tinyint not null default(0),\n\
						ucOcuPro tinyint not null default(0),\n\
						ucCarFlowPro tinyint not null default(0),\n\
						ucSmoothPara tinyint not null default(0))",
/******************************************************/ //ADD:20130801 09 20

(char *) "create table  Tbl_LampCheck( ucId tinyint primary key, ucFlag tinyint not null default(0))"
 }; 
 /***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±íµÄ´´½¨Óï·¨×Ö·û*******************/
 
 /***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±í½á¹¹ÃèÊöÊý×é*********************/
TableDesc Table_Desc[] = {{TBL_SYSTEM,0,Table_Name[TBL_SYSTEM],NULL},
                          {TBL_CONSTANT, 0, Table_Name[TBL_CONSTANT], NULL},
                          {TBL_MODULE,6,Table_Name[TBL_MODULE],Tbl_Module},
                          {TBL_PLAN,5,Table_Name[TBL_PLAN],Tbl_Plan}, 
                          {TBL_SCHEDULE,8,Table_Name[TBL_SCHEDULE],Tbl_Schedule},
                          {TBL_PHASE,12,Table_Name[TBL_PHASE],Tbl_Phase},
                          {TBL_COLLISION,2,Table_Name[TBL_COLLISION],Tbl_Collision},
                          {TBL_DETECTOR,8,Table_Name[TBL_DETECTOR],Tbl_Detector},
                          {TBL_CHANNEL,4,Table_Name[TBL_CHANNEL],Tbl_Channel},
                          {TBL_PATTERN,5,Table_Name[TBL_PATTERN],Tbl_Pattern},
                          {TBL_STAGEPATTERN,7,Table_Name[TBL_STAGEPATTERN],Tbl_StagePattern},
                          {TBL_OVERLAPPHASE,9,Table_Name[TBL_OVERLAPPHASE],TBL_OverlapPhase},
                          {TBL_EVENTTYPE,3,Table_Name[TBL_EVENTTYPE],Tbl_EventType},
                          {TBL_EVENTLOG,5,Table_Name[TBL_EVENTLOG],Tbl_EventLog},
						  {TBL_VEHICLESTAT,5,Table_Name[TBL_VEHICLESTAT],Tbl_VehicleStat},
						  {TBL_CNTDOWNDEV,4,Table_Name[TBL_CNTDOWNDEV],Tbl_CntDownDev},
						  {TBL_SPECFUN,2,Table_Name[TBL_SPECFUN],Tbl_SpecFun},
						  {TBL_DET_EXTEND,9,Table_Name[TBL_DET_EXTEND],Tbl_DetExtend},
						  {TBL_STAGE_FACTTIME,3,Table_Name[TBL_STAGE_FACTTIME],Tbl_StageFactTime},
						  {TBL_ADAPT_STAGETIME,6,Table_Name[TBL_ADAPT_STAGETIME],Tbl_AdaptStageTime},
						  {TBL_PHASE2DIREC,4,Table_Name[TBL_PHASE2DIREC],Tbl_PhaseToDirec},
						  {TBL_ADAPT_PARA,7,Table_Name[TBL_ADAPT_PARA],Tbl_AdaptPara},
						  {TBL_LAMP_CHECK,2,Table_Name[TBL_LAMP_CHECK],Tbl_LampCheck}
                         };

TableDesc* m_gTableDesc = Table_Desc;
/***************¶¨ÒåÐÅºÅ»úÊý¾Ý¿â°üº¬µÄËùÓÐ±í½á¹¹ÃèÊöÊý×é*********************/

/****************¶¨ÒåGBT±íÊý¾Ý½»»»º¯ÊýÖ¸Õë£¬Êý¾Ý±íº¯Êý******************/
typedef int (* EXCHANGEFUN)(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);

typedef struct
{
    Byte        ucObjId;
    EXCHANGEFUN  pExchgFun;
}ExchangeFunGrp;
ExchangeFunGrp gGroupExchange[] = 
{
    {0x81, ExchangeDeviceId},
    {0x82, ExchangeMaxModule},
    {0x83, ExchangeSynchTime},
    {0x84, ExchangeSynchFlag},
    {0x85, ExchangeModule},
    {0x87, ExchangeZone},
    {0x89, ExchangeMaxPlan},
    {0x8A, ExchangeMaxSchedule},
    {0x8B, ExchangeMaxSegment},
    {0x8D, ExchangePlan},
    {0x8E, ExchangeSchedule},
    {0x8F, ExchangeMaxEvtType},
    {0x90, ExchangeMaxEvtLog},
    {0x91, ExchangeEventType},
    {0x92, ExchangeEventLog},
    {0x93, ExchangeMaxPhase},
    {0x94, ExchangeMaxPhaseGrp},
    {0x95, ExchangePhase},
    {0x97, ExchangeCollision},
    {0x98, ExchangeMaxDetector},
    {0x99, ExchangeMaxDetGrp},
    {0x9A, ExchangeDetDataSeqNo},
    {0x9B, ExchangeDetDataCycle},
    {0x9C, ExchangeActiveDetCount},
    {0x9D, ExchangePulseSeqNo},
    {0x9E, ExchangePulseCycle},
    {0x9F, ExchangeDetector},
    {0xA3, ExchangeFlsTimeWhenStart},
    {0xA4, ExchangeAllRedTimeWhenStart},
    {0xAA, ExchangeRemoteCtrlFlag},
    {0xAB, ExchangeFlashFreq},
    {0xAC, ExchangeBrtCtrlBgnTime},
    {0xAD, ExchangeBrtCtrlEndTime},
    {0xAE, ExchangeMaxChannel},
    {0xAF, ExchangeMaxChanlGrp},
    {0xB0, ExchangeChannel},
    {0xB2, ExchangeMaxPattern},
    {0xB3, ExchangeMaxStagePattern},
    {0xB4, ExchangeMaxStage},
    {0xB8, ExchangeGlobalCycle},
    {0xB9, ExchangeCoorPhaseOffset},
    {0xBC, ExchangeDegradeMode},
    {0xBD, ExchangeDegradePattern},
    {0xC0, ExchangePattern},
    {0xC1, ExchangeStagePattern},
    {0xC2, ExchangeDownloadFlag},
    {0xC3, ExchangeCtrlMasterOptFlag},
    {0xC4, ExchangeBaseAddr},
    {0xC5, ExchangeSigDevCount},
    {0xC6, ExchangeMaxOverlapPhase},
    {0xC7, ExchangeMaxOverlapPhaseGrp},
    {0xC8, ExchangeOverlapPhase},
	//ÒÔÏÂÎªÀ©Õ¹¶ÔÏó
	{0xf1, ExchangeCntDownDev},    //µ¹¼ÆÊ±Éè±¸±í
	{0xfa, ExchangePhaseToDirec},  //ÏàÎ»Óë·½Ïò¶ÔÓ¦±í
	{0xfb, ExchangeAdaptPara},     //×ÔÊÊÓ¦²ÎÊýÖµ
	{0xfc, ExchangeDetExtend},      //¼ì²âÆ÷À©Õ¹±í
	{0xfd, ExchangeAdaptStage},     //×ÔÊÊÓ¦×ÔÑ§Ï°Éú³ÉµÄ½×¶ÎÊ±¼ä
    {0xff, ExchangeChannelChk}          //Í¨µÀµÆÅÝ¼ì²âÅäÖÃ
};
/****************¶¨ÒåGBT±íÊý¾Ý½»»»º¯ÊýÖ¸Õë£¬Êý¾Ý±íº¯Êý******************/


#define GET_CONST_CONST(FIELD, VALUE)\
{\
    Ulong   ulMaxTemp;\
    GetFieldData("Tbl_Constant", FIELD, ulMaxTemp);\
    VALUE = (Byte)ulMaxTemp;\
}

#define CHECK_ID(KIND, ID, RETURN)\
{\
    Ulong  ulMax;\
    GetFieldData("Tbl_Constant", KIND, ulMax);\
    if (0 == ID || ID > ulMax)\
    {\
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tInvalid %s Id(%d)\n", SHORT_FILE, __LINE__,KIND, ID));\
        return RETURN;\
    }\
}


/**************************************************************
Function:       GetFieldsValue
Description:    ´Ó×Ö·û´®ÖÐ·ÖÀë³öÐèÒªµ½×Ö·û			
Input:          sConstant  -- ´ý·ÖÀëµ½×Ö·û³¤³£Á¿  
				uSplit   --   ·ÖÀë×Ö·û    
				sValue   --   ½á¹û±£´æÖ¸Õë     
				uValSize  --  ·ÖÀë¶Ô±È´ÎÊý   
Output:         ÎÞ
Return:         ·ÖÀëµÃµ½×Ö½ÚÊý
***************************************************************/
Byte GetFieldsValue(const char* sConstant, const char uSplit, Byte* sValue, Byte uValSize)
{
    Byte uCount    = 0;
    Byte uVal      = 0;
    size_t uPos     = 0;
    size_t uLength  = ACE_OS::strlen(sConstant);

    while(uPos < uLength && uCount < uValSize)
    {
        if (uSplit == sConstant[uPos])
        {
            uPos += 1;
            sValue[uCount++] = uVal;
            uVal = 0;
            continue;
        }
        else if (sConstant[uPos] >= '0' && sConstant[uPos] <= '9')
        {
            uVal = uVal*10 + (sConstant[uPos] - '0');
			uPos += 1;
            continue;
        }
        else
        {
            break;
        }
    }

    return uCount;
}

/**************************************************************
Function:       ExchangeData
Description:    Êý¾Ý¿âÓÚÉÏÎ»»úÊý¾Ý½»»»·ÖÀë´¦Àí			
Input:          iOprType -- Êý¾Ý´¦ÀíÀàÐÍ£¬²éÑ¯»òÕßÉèÖÃ
				uObjId  -- Êý¾Ý¿âgbt¶ÔÏó±êÖ¾  
				uIdxFst   --  Í·Ë÷ÒýÖµ  
				uIdxSnd   --  ´ÎË÷ÒýÖµ     
				uSubId  --    ×Ó¶ÔÏóÖµ  
				pData   --     ½»»»ÖµÖ¸Õë 
				uDataSize --  Êý¾Ý´óÐ¡
Output:         uErrorSts -- Êý¾Ý´¦Àí´íÎó×´Ì¬
				uErrorIdx -- Êý¾Ý´¦Àí´íÎóË÷Òý
Return:         -1 - º¯Êý´¦Àí´íÎó  
***************************************************************/
int ExchangeData(int iOprType,
                Byte uObjId, 
                Byte uIdxFst, 
                Byte uIdxSnd, 
                Byte uSubId, 
                Byte* pData, 
                int uDataSize,
                Byte& uErrorSts,
                Byte& uErrorIdx)
{
    int i, iGrpSize = sizeof(gGroupExchange) / sizeof(ExchangeFunGrp);
    for (i = 0; i < iGrpSize; i++)
    {
        if (gGroupExchange[i].ucObjId == uObjId)
        {
            return gGroupExchange[i].pExchgFun(iOprType, uIdxFst, uIdxSnd, uSubId, pData, uDataSize, uErrorSts, uErrorIdx);
        }
    }  
    uErrorIdx = 0;
    uErrorSts = 3 ; //GBT_MSG_OBJ_ERROR;
    return -1;
}

/**************************************************************
Function:       CSqliteRst::CSqliteRst
Description:    CSqliteRstÊý¾Ý¿â½á¹û¼¯Àà¹¹Ôìº¯Êý				
Input:          ÎÞ              
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
CSqliteRst::CSqliteRst() : m_ppResult(NULL) , m_iRow(0) , m_iColum(0)
{
	;
}

/**************************************************************
Function:       CSqliteRst::~CSqliteRst
Description:    CSqliteRstÀà	Îö¹¹º¯Êý	
Input:          ÎÞ              
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
CSqliteRst::~CSqliteRst()
{
    Clear();
}


/**************************************************************
Function:       CSqliteRst::Clear
Description:    ÊÍ·ÅÊý¾Ý¿â±íÊý¾Ý¼¯	
Input:          ÎÞ              
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
void CSqliteRst::Clear()
{
    if (NULL != m_ppResult)
    {
        ::sqlite3_free_table(m_ppResult);
        m_ppResult = NULL;
    }
    m_iRow   = 0;
    m_iColum = 0;
}

/**************************************************************
Function:       CGbtTscDb::CGbtTscDb
Description:    CGbtTscDbÐÅºÅ»úÊý¾Ý¿â´¦ÀíÀà¹¹Ôìº¯Êý	
Input:          ÎÞ              
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
CGbtTscDb::CGbtTscDb() : m_pSqlite(NULL)
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Init GbtDb object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CGbtTscDb::~CGbtTscDb
Description:    CGbtTscDbÀà	Îö¹¹º¯Êý	
Input:          ÎÞ              
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
CGbtTscDb::~CGbtTscDb()
{
    CloseDb();
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct GbtDb object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CGbtTscDb::InitDb
Description:    ÐÅºÅ»úÊý¾Ý¿â³õÊ¼»¯
Input:          sDbPath - Êý¾Ý¿âÎÄ¼þ´æ·ÅÂ·¾¶              
Output:         ÎÞ
Return:         true - ³õÊ¼»¯³É¹¦  false -³õÊ¼»¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::InitDb(const char* sDbPath)
{   
    if (NULL != m_pSqlite)
    {
        ::sqlite3_close(m_pSqlite);
    }
    m_pSqlite = NULL;
	ACE_DEBUG ((LM_DEBUG, "%s:%04d@@@@@@InitDb@@@@@ Begin init TSC database!\n", SHORT_FILE, __LINE__));
    if (SQLITE_OK != ::sqlite3_open(sDbPath, &m_pSqlite))
    {      
		return false;
    }
	
    for (int i = 0; i < TABLE_COUNT; i++)        //³õÊ¼»¯Êý¾Ý¿â±í
    {
        if (!IsTableExist(Table_Desc[i].sTblName))
        {
            if (!ExecuteCmd(Table_Script[i]))
            {
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to create %s\n", SHORT_FILE, __LINE__, Table_Desc[i].sTblName));
            }
        }
    }
   // AddDefault();     //Ìí¼Ó²¿·ÖÊý¾Ý¿â±íÄ¬ÈÏÖµ
    InitDefaultData(); //ADD:20141209
    m_gTscDb = this;
	return true;
}


/**************************************************************
Function:       CGbtTscDb::CloseDb
Description:    ¹Ø±ÕÐÅºÅ»úÊý¾Ý¿â
Input:          ÎÞ            
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
void CGbtTscDb::CloseDb()
{
    ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexDb);

    if (NULL != m_pSqlite)
    {
        ::sqlite3_close(m_pSqlite);
    }
    m_pSqlite = NULL;
}

/**************************************************************
Function:       CGbtTscDb::IsTableExist
Description:    ÅÐ¶ÏÊý¾Ý¿â±íÊý¾ÝÊÇ·ñ´æÔÚ
Input:          sTable  -- ±íÃû³Æ            
Output:         ÎÞ
Return:         true - ±í´æÔÚ false -±í²»´æÔÚ»òÕßÎÞÊý¾Ý¼ÇÂ¼
***************************************************************/
bool CGbtTscDb::IsTableExist(const char* sTable)
{
    char  sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
	
    ACE_OS::sprintf(sSql, "select * from sqlite_master where type='table' and name='%s'", sTable);
    if (!Query(sSql, qryRst))
    {
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query %s \n", SHORT_FILE, __LINE__,sTable));
        return false;
    }
    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
    return true;
}

void CGbtTscDb::InitDefaultData()
{
	GBT_DB::TblPlan tblPlan;
	GBT_DB::Plan    sPlan;

	GBT_DB::TblSchedule tblSchedule;
	GBT_DB::Schedule    sSchedule;

	GBT_DB::TblPattern tblPattern;
	GBT_DB::Pattern    sPattern;

	GBT_DB::TblStagePattern tblStage;
	GBT_DB::StagePattern    sStage;
	
	GBT_DB::TblPhase tblPhase;
	GBT_DB::Phase    sPhase;	

	GBT_DB::TblOverlapPhase tblOverlapPhase;
	GBT_DB::OverlapPhase    sOverlapPhase;

	GBT_DB::TblChannel tblChannel;
	GBT_DB::Channel    sChannel;

	GBT_DB::TblDetector tblDetector;
	GBT_DB::Detector    sDetector;

	GBT_DB::TblCollision tblCollision;
	GBT_DB::Collision    sCollision;
	
	GBT_DB::TblModule tblModule;
	GBT_DB::Module    sModule;

	GBT_DB::TblSpecFun tblSpecFun;
	GBT_DB::SpecFun    sSpecFun;

	GBT_DB::TblPhaseToDirec tblPhaseToDirec;
	GBT_DB::PhaseToDirec sPhaseToDirec;	
	
	GBT_DB::TblChannelChk tblChannelChk;
	GBT_DB::ChannelChk    sChannelChk;

	GBT_DB::TblCntDownDev tblCntdownDev;
	GBT_DB::CntDownDev    sCntDownDev;

	char   sSql[LARGE_BUF_SIZE];
    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select * from %s", Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query TBL_SYSTEM\n", SHORT_FILE, __LINE__));
    }

    if (qryRst.m_iRow > 0)
    {
        return;
    }

    ACE_OS::sprintf(sSql, "insert into %s(usDeviceId)values(0)",Table_Desc[TBL_SYSTEM].sTblName);
    
    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
    }     
    ACE_OS::sprintf(sSql, "select * from %s", Table_Desc[TBL_CONSTANT].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query TBL_CONSTANT\n", SHORT_FILE, __LINE__));
    }

    if (qryRst.m_iRow <= 0)
    {
        ACE_OS::sprintf(sSql, "insert into %s(ucMaxModule)values(16)",Table_Desc[TBL_CONSTANT].sTblName);     
        if (!ExecuteCmd(sSql))
        {
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        }
    }

	/********************Í¨µÀµÆÅÝ¼ì²âÅäÖÃ±í*****************************/
	(CDbInstance::m_cGbtTscDb).QueryChannelChk(tblChannelChk);
	if(0 == tblChannelChk.GetCount())
	{
		for(int i = 0 ;i<MAX_LAMP;i++)
		{
			sChannelChk.ucSubChannelId= i+1;
			sChannelChk.ucIsCheck = 0 ;
			(CDbInstance::m_cGbtTscDb).AddChannelChk(sChannelChk.ucSubChannelId,sChannelChk);
		}
	}
	/**************Ìí¼ÓÄ¬ÈÏµ÷¶È¼Æ»®²ÎÊý***********/
	(CDbInstance::m_cGbtTscDb).QueryPlan(tblPlan);
	if ( 0 == tblPlan.GetCount() )
	{
		sPlan.ucId         = 1;
		sPlan.usMonthFlag  = 0x1ffe;               
		sPlan.ucWeekFlag   = 0xfe;               
		sPlan.ulDayFlag    = 0xfffffffe;               
		sPlan.ucScheduleId = 1;
		(CDbInstance::m_cGbtTscDb).AddPlan(1,sPlan);   
	}

	/**************Ìí¼ÓÄ¬ÈÏÊ±¶Î±íÊý¾Ý***********/
	(CDbInstance::m_cGbtTscDb).QuerySchedule(tblSchedule);
	if ( 0 == tblSchedule.GetCount() )
	{
		sSchedule.ucScheduleId = 1;                
		sSchedule.ucEvtId      = 1;                     
		sSchedule.ucBgnHour    = 8;                   
		sSchedule.ucBgnMinute  = 0;                
		sSchedule.ucCtrlMode   = 0;  
		sSchedule.ucPatternNo  = 1;              
		sSchedule.ucAuxOut     = 0;                   
		sSchedule.ucSpecialOut = 0;               
		(CDbInstance::m_cGbtTscDb).AddSchedule(sSchedule.ucScheduleId,sSchedule.ucEvtId,sSchedule);
	}

	(CDbInstance::m_cGbtTscDb).QueryPattern(tblPattern);
	if ( 0 == tblPattern.GetCount() )
	{
		sPattern.ucPatternId = 1; 
		sPattern.ucCycleTime = 120; 
		sPattern.ucOffset    = 30;      
		sPattern.ucCoorPhase = 10;             
		sPattern.ucStagePatternId = 1;  
		(CDbInstance::m_cGbtTscDb).AddPattern(sPattern.ucPatternId,sPattern);
		sPattern.ucPatternId = 2;
		sPattern.ucStagePatternId = 2;		
		(CDbInstance::m_cGbtTscDb).AddPattern(sPattern.ucPatternId,sPattern);
		sPattern.ucPatternId = 3;
		sPattern.ucStagePatternId = 3;		
		(CDbInstance::m_cGbtTscDb).AddPattern(sPattern.ucPatternId,sPattern);
		sPattern.ucPatternId = 15;
		sPattern.ucStagePatternId = 15;		
		(CDbInstance::m_cGbtTscDb).AddPattern(sPattern.ucPatternId,sPattern);
		sPattern.ucPatternId = 16;
		sPattern.ucStagePatternId = 16;		
		(CDbInstance::m_cGbtTscDb).AddPattern(sPattern.ucPatternId,sPattern);	
	
	}

	/**************½×¶ÎÅäÊ±·½°¸±í***********/
	(CDbInstance::m_cGbtTscDb).QueryStagePattern(tblStage);
	if ( 0 == tblStage.GetCount() )
	{  //½×¶ÎÅäÊ±1 Ê®×ÖÂ·¿Ú
		sStage.ucStagePatternId = 1;      
		sStage.ucStageNo        = 1;            
		sStage.usAllowPhase     = 32771; //±±×óÖ± Î÷ÈËÐÐ
		sStage.ucGreenTime      = 15;           
		sStage.ucYellowTime     = 3;         
		sStage.ucRedTime        = 0;             
		sStage.ucOption         = 1;
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 2;            
		sStage.usAllowPhase     = 56;  //¶«×óÖ±±±ÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 3;            
		sStage.usAllowPhase     = 896; //ÄÏ×óÖ± ¶«ÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 4;            
		sStage.usAllowPhase     = 14336; //Î÷×óÖ±ÄÏÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
      //½×¶ÎÅäÊ±2 Ê®×ÖÂ·¿Ú			
	
		sStage.ucStagePatternId = 2;      
		sStage.ucStageNo        = 1;            
		sStage.usAllowPhase     = 257; //ÄÏ±±×ó
		sStage.ucGreenTime      = 20;           
		sStage.ucYellowTime     = 3;         
		sStage.ucRedTime        = 0;             
		sStage.ucOption         = 1;
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 2;            
		sStage.usAllowPhase     = 33410;  //ÄÏ±±Ö± ¶«Î÷ÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 3;            
		sStage.usAllowPhase     = 4112; //¶«Î÷×ó
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 4;            
		sStage.usAllowPhase     = 10280; //¶«Î÷Ö± ÄÏ±±ÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

 		//¶¡×ÖÂ·¿Ú¶«Î÷±±
		sStage.ucStagePatternId = 3;      
		sStage.ucStageNo        = 1;            
		sStage.usAllowPhase     = 8224; //¶«Î÷Ö±
		sStage.ucGreenTime      = 20;           
		sStage.ucYellowTime     = 3;         
		sStage.ucRedTime        = 0;             
		sStage.ucOption         = 1;
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 2;            
		sStage.usAllowPhase     = 12288;  //Î÷×óÖ±
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 3;            
		sStage.usAllowPhase     = 1; //±±×ó±ß
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
		//Ä¬ÈÏ¿ª»ú¼ì²âÃ¿¿é°åµÆÉ«·½°¸
		sStage.ucStagePatternId = 15;      
		sStage.ucStageNo        = 1;            
		sStage.usAllowPhase     = 15; //µÚÒ»¿é°å ±±×óÖ±ÓÒÈËÐÐ
		sStage.ucGreenTime      = 7;           
		sStage.ucYellowTime     = 3;         
		sStage.ucRedTime        = 0;             
		sStage.ucOption         = 1;
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 2;            
		sStage.usAllowPhase     = 240;  //µÚ¶þ¿é°å¶«×óÖ±ÓÒÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 3;            
		sStage.usAllowPhase     = 3840; //µÚÈý¿é°åÄÏ×óÖ±ÓÒÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
		sStage.ucStageNo        = 4;            
		sStage.usAllowPhase     = 61440;  //µÚËÄ¿é°åÎ÷×óÖ±ÓÒÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 5;            
		sStage.usAllowPhase     = 983040; //¶þ´Î¹ý½Ö±±¶«ÄÏÎ÷
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
			sStage.ucStageNo    = 6;            
		sStage.usAllowPhase     = 15728640; //µ÷Í·±±¶«ÄÏÎ÷
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
		sStage.ucStageNo        = 7;            
		sStage.usAllowPhase     = 251658240;  //ÌØÊâ±±¶«ÄÏÎ÷
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 8;            
		sStage.usAllowPhase     = 4026531840; //ÆäËû±±¶«ÄÏÎ÷
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		

		//Ä¬ÈÏÈËÐÐ¹ý½Ö ·½°¸16
		sStage.ucStagePatternId = 16;      
		sStage.ucStageNo        = 1;            
		sStage.usAllowPhase     = 8224; //¶«Î÷Ö±
		sStage.ucGreenTime      = 15;           
		sStage.ucYellowTime     = 3;         
		sStage.ucRedTime        = 0;             
		sStage.ucOption         = 1;
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);

		sStage.ucStageNo        = 2;  
		sStage.ucGreenTime      = 15;           
		sStage.ucYellowTime     = 0;  
		sStage.usAllowPhase     = 32896;  //ÄÏ±±ÈËÐÐ
		(CDbInstance::m_cGbtTscDb).AddStagePattern(sStage.ucStagePatternId,
			sStage.ucStageNo, sStage);
		
	}

	/**************ÏàÎ»±í***********/
	(CDbInstance::m_cGbtTscDb).QueryPhase(tblPhase);
	if ( 0 == tblPhase.GetCount() )
	{
		sPhase.ucPhaseId         = 1;                 
		sPhase.ucPedestrianGreen = 0;           
		sPhase.ucPedestrianClear = 0;          
		sPhase.ucMinGreen        = 15;                
		sPhase.ucGreenDelayUnit  = 8;          
		sPhase.ucMaxGreen1       = 40;                
		sPhase.ucMaxGreen2       = 60;                
		sPhase.ucFixGreen        = 7;                 
		sPhase.ucGreenFlash      = 2; 
		sPhase.ucPhaseTypeFlag   = 32;    //32 µ¯ÐÔÏàÎ»0x40´ý¶¨ÏàÎ»0x80¹Ì¶¨ÏàÎ»
		sPhase.ucPhaseOption     = 0;               
		sPhase.ucExtend          = 0;
		sPhase.ucPedestrianGreen = 0; 

		for ( int i=1; i<(MAX_PHASE+1); i++ )
		{
			sPhase.ucPhaseId = i; 
			
			if (i==4 ||i==8 || i==12 || i==16||i==17 ||i==18 || i==19 ||i==20) 
			{  //Ä¬ÈÏÈËÐÐÏàÎ» 4 8 12 16 21 22 23 24
				sPhase.ucPhaseOption = 2; 
			}
			else
			{				
				sPhase.ucPhaseOption = 1; //Ä¬ÈÏÏàÎ»ÆôÓÃ£¬0ÎªÎ´Öª
			}					
			(CDbInstance::m_cGbtTscDb).AddPhase(sPhase.ucPhaseId, sPhase);
		}
	}

	/**************Í¨µÀ±í***********/
	(CDbInstance::m_cGbtTscDb).QueryChannel(tblChannel);
	ACE_OS::memset(&sChannel,0,sizeof(GBT_DB::Channel));
	sChannel.ucCtrlType = 1;
	if ( 0 == tblChannel.GetCount() )
	{
		for (int i=0; i<MAX_CHANNEL; i++ )
		{
			sChannel.ucChannelId = 1 + i; 
			sChannel.ucCtrlSrc   = 1 + i; 
			Byte setChannelId = sChannel.ucChannelId ;
			
			if (setChannelId==4 ||setChannelId==8 ||setChannelId==12 || setChannelId==16 ||
				setChannelId==17||setChannelId==18|| setChannelId==19||setChannelId==20)
				sChannel.ucCtrlType  = CHANNEL_FOOT;
			else
				sChannel.ucCtrlType  = CHANNEL_VEHICLE;
			(CDbInstance::m_cGbtTscDb).AddChannel(sChannel.ucChannelId,sChannel);
		}
	}
	
	/**************************Ìí¼ÓÄ¬ÈÏÓ²¼þÄ£¿é************************/
	(CDbInstance::m_cGbtTscDb).QueryModule(tblModule);

	if ( 0 == tblModule.GetCount() )
	{		
		sModule.ucModuleId = 1;
		sModule.strDevNode.SetString("DET-0-1");
		sModule.strModel.SetString("hd");
		sModule.strVersion.SetString("V1.0");   
		sModule.ucType = 2;
		(CDbInstance::m_cGbtTscDb).ModModule(sModule.ucModuleId,  sModule);

		sModule.ucModuleId = 2;
		sModule.strDevNode.SetString("DET-1-17");							
		sModule.strModel.SetString("hd");
		sModule.strVersion.SetString("V1.0");   
		sModule.ucType = 2;
		(CDbInstance::m_cGbtTscDb).ModModule(sModule.ucModuleId,  sModule);
		sModule.ucModuleId = 3;
		sModule.strDevNode.SetString("DET-2-33");
		sModule.strModel.SetString("hd");
		sModule.strVersion.SetString("V1.0");   
		sModule.ucType = 2;
		(CDbInstance::m_cGbtTscDb).ModModule(sModule.ucModuleId,  sModule);

		sModule.ucModuleId = 4;
		sModule.strDevNode.SetString("DET-3-65");							
		sModule.strModel.SetString("hd");
		sModule.strVersion.SetString("V1.0");   
		sModule.ucType = 2;
		(CDbInstance::m_cGbtTscDb).ModModule(sModule.ucModuleId,  sModule);
	}

	/************************Ìí¼ÓÄ¬ÈÏÌØ¶¨¹¦ÄÜ**************************/
	(CDbInstance::m_cGbtTscDb).QuerySpecFun(tblSpecFun);

	if ( tblSpecFun.GetCount() == 0 )
	{
		sSpecFun.ucFunType = FUN_SERIOUS_FLASH + 1;
		sSpecFun.ucValue   = 0;
		(CDbInstance::m_cGbtTscDb).ModSpecFun(sSpecFun.ucFunType , sSpecFun.ucValue);
		
		sSpecFun.ucFunType = FUN_COUNT_DOWN + 1;
		sSpecFun.ucValue   = COUNTDOWN_STUDY;
		(CDbInstance::m_cGbtTscDb).ModSpecFun(sSpecFun.ucFunType , sSpecFun.ucValue);

		sSpecFun.ucFunType = FUN_GPS + 1;
		(CDbInstance::m_cGbtTscDb).ModSpecFun(sSpecFun.ucFunType , sSpecFun.ucValue);
 
		sSpecFun.ucFunType = FUN_MSG_ALARM + 1;
		(CDbInstance::m_cGbtTscDb).ModSpecFun(sSpecFun.ucFunType , sSpecFun.ucValue);

		
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CROSS_TYPE     + 1) , 0);  //0-tsc 1-psc1  2-psc2
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_STAND_STAGEID  + 1) , 1);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CORSS1_STAGEID + 1) , 2);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CROSS2_STAGEID + 1) , 3);

	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_TEMPERATURE    + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_VOLTAGE        + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_DOOR           + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_COMMU_PARA     + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_PORT_LOW       + 1) , 59);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_PORT_HIGH      + 1) , 21);

	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_PRINT_FLAG    + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_PRINT_FLAGII  + 1) , 0);
		(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CAM          + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_3G            + 1) , 0);
	   (CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_WIRELESSBTN   + 1) , 0);
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CNTTYPE      + 1) , 3);
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_LIGHTCHECK   + 1) , 0);
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_GPS_INTERVAL + 1) , 1);
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_WIRELESSBTN_TIMEOUT + 1) , 30);		
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_CROSSSTREET_TIMEOUT + 1) , 10);		
	(CDbInstance::m_cGbtTscDb).ModSpecFun((FUN_RS485_BITRATE + 1) , 0); //ADD 150129
		
	}


	/*******************¡¤Ìí¼ÓÄ¬ÈÏ·½ÏòÓÚÏàÎ»¶ÔÓ¦²ÎÊý*******************/
	(CDbInstance::m_cGbtTscDb).QueryPhaseToDirec(tblPhaseToDirec);

	if ( tblPhaseToDirec.GetCount() == 0 )
	{
		//Ä¬ÈÏÌí¼Ó32¸ö·½Ïò¶ÔÓ¦32¸öÏàÎ»ºÍ32¸öÍ¨µÀ
		//ÏÂÃæÃ¿ÅÅÊÇÒ»¸ö·½ÏòµÄ³µµÀ·½ÏòID,·Ö±ðÊÇ
		//×ó,  Ö±, ÓÒ, ÈËÐÐ, ¶þ´Î¹ý½Ö, µ÷Í·,ÆäËû, ÌØÊâ
		Byte direcId[32] = {0x1,0x2,0x4,0x8,       //±±×óÖ±ÓÒÈËÐÐ  
					      0x41,0x42,0x44,0x48,   //¶«×óÖ±ÓÒÈËÐÐ
				               0x81,0x82,0x84,0x88,   //ÄÏ×óÖ±ÓÒÈËÐÐ
					      0xc1,0xc2,0xc4,0xc8,   //Î÷×óÓÒÖ®ÈËÐÐ
					      0x18,0x58,0x98,0xd8,   //±±¶«ÄÏÎ÷¶þ´Î¹ý½							
					      0x0,0x40,0x80,0xc0,    //±±¶«ÄÏÎ÷µ÷Í·
					      0x7,0x47,0x87,0xc7,    //±±¶«ÄÏÎ÷ÌØÊâ,Êµ¼ÊÉÏ¶ÔÓ¦µÄÊÇ±±¶«ÄÏÎ÷µÄ×óÖ±ÓÒÖ±ÐÐ·½ÏòID
					      0x5,0x45,0x85,0xc5} ;   //±±¶«ÄÏÎ÷ÆäËû
						
		               
		for (Byte i=0; i<MAX_PHASE; i++)
		{
			sPhaseToDirec.ucId    = direcId[i];
			sPhaseToDirec.ucPhase = i + 1;
			sPhaseToDirec.ucOverlapPhase = 0;
			sPhaseToDirec.ucRoadCnt      = 1;
			(CDbInstance::m_cGbtTscDb).AddPhaseToDirec(sPhaseToDirec.ucId,sPhaseToDirec);
		}
	}

	/**************¸úËæÏàÎ»±í***********/
	(CDbInstance::m_cGbtTscDb).QueryOverlapPhase(tblOverlapPhase);
	ACE_OS::memset(&sOverlapPhase,0,sizeof(GBT_DB::OverlapPhase));
	if ( 0 == tblOverlapPhase.GetCount() )
	{
		sOverlapPhase.ucOverlapPhaseId = 1;         
		sOverlapPhase.ucIncldPhaseCnt  = 0;
		sOverlapPhase.ucTailGreen      = 0;
		sOverlapPhase.ucTailYellow     = 0;
		sOverlapPhase.ucTailRed        = 0;
		(sOverlapPhase.ucIncldPhase)[0] = 0; 
		(sOverlapPhase.ucIncldPhase)[1] = 0;
		(CDbInstance::m_cGbtTscDb).AddOverlapPhase(sOverlapPhase.ucOverlapPhaseId,sOverlapPhase);
	}

	/**************³åÍ»ÏàÎ»±í***********/
	(CDbInstance::m_cGbtTscDb).QueryCollision(tblCollision);
	ACE_OS::memset(&sCollision , 0 , sizeof(GBT_DB::Collision) );
	if ( 0 == tblCollision.GetCount() )
	{
		for(Byte index = 0 ;index<MAX_CONFLICT_PHASE;index++)
		{
			sCollision.ucPhaseId       = index+1;
			sCollision.usCollisionFlag = 0x0;
			(CDbInstance::m_cGbtTscDb).AddCollision(sCollision.ucPhaseId, sCollision);
		}
	}

	/*******************Ìí¼ÓÄ¬ÈÏ¼ì²âÆ÷²ÎÊý*******************/
	(CDbInstance::m_cGbtTscDb).QueryDetector(tblDetector);
	if ( tblDetector.GetCount() == 0 )
	{			
		sDetector.ucPhaseId = 1 ;
		sDetector.ucDirect = 1 ;
		sDetector.ucDetFlag = 129 ;
	(CDbInstance::m_cGbtTscDb).AddDetector(1,sDetector); // Ä¬ÈÏÌí¼ÓÒ»¸ö¼ì²âÆ÷
		
}
   	/*******************Ìí¼ÓÄ¬ÈÏµ¹¼ÆÊ±Éè±¸±í*******************/
	(CDbInstance::m_cGbtTscDb).QueryCntDownDev(tblCntdownDev);
	if (tblCntdownDev.GetCount() == 0 )
	{	
		for(Byte index = 0 ;index< MAX_CNTDOWNDEV;index++)
		{
			sCntDownDev.ucDevId = index+1 ;
			sCntDownDev.usPhase = index+1 ;
			sCntDownDev.ucOverlapPhase = 0 ;
			sCntDownDev.ucMode= 0x57 ;
			(CDbInstance::m_cGbtTscDb).AddCntDownDev(sCntDownDev); // Ä¬ÈÏÌí¼ÓÒ»¸ö¼ì²âÆ÷
		}
	
	}

}

/**************************************************************
Function:       CGbtTscDb::Query
Description:    Êý¾Ý±í²éÑ¯
Input:          sSql  -- Êý¾Ý²éÑ¯Óï¾ä            
Output:         sRst  -- Êý¾Ý²éÑ¯½á¹û±£´æ¶ÔÏó
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::Query(const char* sSql, CSqliteRst& sRst)
{
    char* sErrMsg = NULL;
    ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexDb);

    if (NULL == m_pSqlite)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t m_pSqlite== NULL\n", SHORT_FILE, __LINE__));
		return false;
    }
    sRst.Clear();

    if (SQLITE_OK != ::sqlite3_get_table(m_pSqlite, sSql, &(sRst.m_ppResult), &sRst.m_iRow, &sRst.m_iColum, &sErrMsg))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFiled to sqlite3_get_table\n sql = %s\n", SHORT_FILE, __LINE__, sSql));
        if (NULL != sErrMsg)
        {
			ACE_DEBUG ((LM_DEBUG, "ErrMsg = %s\n", sErrMsg));
			::sqlite3_free(sErrMsg);
        }
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::ExecuteCmd
Description:    Ö´ÐÐÊý¾Ý¿âsqlÓï¾ä
Input:          sSql  -- Êý¾Ý²éÑ¯Óï¾ä            
Output:         ÎÞ
Return:         true - Óï¾äÖ´ÐÐ³É¹¦ false -Óï¾äÖ´ÐÐÊ§°Ü
***************************************************************/
bool CGbtTscDb::ExecuteCmd(const char *sSql)
{
    char* sErrMsg = NULL;
    ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexDb);

    if (SQLITE_OK != ::sqlite3_exec(m_pSqlite, sSql, NULL, NULL, &sErrMsg))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to sqlite3_exec\n sql = %s\n", SHORT_FILE, __LINE__, sSql));
        if (NULL != sErrMsg)
        {
            ACE_DEBUG ((LM_DEBUG, "ErrMsg = %s\n", sErrMsg));
			::sqlite3_free(sErrMsg);
        }
        return false;
    }
	if (NULL != sErrMsg)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFree sErrMsg\n", SHORT_FILE, __LINE__));
		::sqlite3_free(sErrMsg);
	}

    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPlan
Description:    ²éÑ¯¼Æ»®·½°¸±í£¬²¢±£´æµ½½á¹¹ÌåÖÐ
Input:          ÎÞ            
Output:         tblPlan  -- ¼Æ»®·½°¸Àà¶ÔÏó
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryPlan(TblPlan& tblPlan)
{
    int    i;
	char*  pStop;
    Byte uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Plan*   pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
               				 Table_Desc[TBL_PLAN].sTblName, 
               				 Table_Desc[TBL_PLAN].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PLAN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PLAN].sTblName, qryRst.m_iColum,Table_Desc[TBL_PLAN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
    
    uCount = (Byte)qryRst.m_iRow;
    if (!tblPlan.AllocData(uCount))
    {
        return false;
    }

    pData = tblPlan.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    { 
        pData[i].ucId           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].usMonthFlag    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucWeekFlag     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
	pData[i].ulDayFlag      = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3], &pStop, 10);
        pData[i].ucScheduleId   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);    
    }
    
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPlan
Description:    ¸ù¾Ý¼Æ»®IDHºÅ²éÑ¯¼Æ»®·½°¸±í£¬²¢±£´æµ½½á¹¹ÌåÖÐ
Input:          uPlanId  -- ¼Æ»®·½°¸ºÅ            
Output:         sPlan  --   ±£´æ¼Æ»®·½°¸½á¹¹Ìå
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryPlan(Byte uPlanId, Plan& sPlan)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
                			Table_Desc[TBL_PLAN].sTblName, 
                			Table_Desc[TBL_PLAN].sFieldName[0], uPlanId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PLAN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PLAN].sTblName,  	 qryRst.m_iColum,Table_Desc[TBL_PLAN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
 
    sPlan.ucId           = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sPlan.usMonthFlag    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sPlan.ucWeekFlag     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sPlan.ulDayFlag      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    sPlan.ucScheduleId   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);    
    return true;
}


/**************************************************************
Function:       CGbtTscDb::AddPlan
Description:    Ìí¼Óµ÷¶È¼Æ»®±í
Input:          uPlanId  -- ¼Æ»®·½°¸ºÅ  
				sPlan  --   ´ýÌí¼Ó·½°¸½á¹¹Ìå          
Output:         ÎÞ
Return:         true - Ìí¼Ó³É¹¦ false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddPlan(Byte ucPlanId, Plan& sPlan)   
{
    char   sSql[LARGE_BUF_SIZE];
    
    if (!IsPlanValid(ucPlanId, sPlan))
    {
     //   ACE_DEBUG ((LM_DEBUG, "%s:%04d\tInvalid Plan\n", SHORT_FILE, __LINE__));
        return true;
    }

    ACE_OS::sprintf(sSql, "insert into %s(%s,%s,%s,%s,%s)values(%u,%u,%u,%lu,%u)",
                   			Table_Desc[TBL_PLAN].sTblName,
                  			Table_Desc[TBL_PLAN].sFieldName[0], 
                  			Table_Desc[TBL_PLAN].sFieldName[1], 
                   			Table_Desc[TBL_PLAN].sFieldName[2], 
                   			Table_Desc[TBL_PLAN].sFieldName[3], 
                  			Table_Desc[TBL_PLAN].sFieldName[4], 
                   			ucPlanId, 
                  			sPlan.usMonthFlag, 
                   			sPlan.ucWeekFlag, 
                   			sPlan.ulDayFlag, 
                   			sPlan.ucScheduleId);

    if (!ExecuteCmd(sSql))
    {
      //  ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelPlan
Description:    É¾³ýÊ±»ùµ÷¶È±í
Input:          uPlanId  -- ¼Æ»®·½°¸ºÅ      
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPlan(Byte ucPlanId)
{
    char   sSql[SMALL_BUF_SIZE];

    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
                  		 Table_Desc[TBL_PLAN].sTblName,
                  		 Table_Desc[TBL_PLAN].sFieldName[0], ucPlanId);
    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelPlan
Description:    É¾³ýÈ«²¿Ê±»ùµ÷¶È±í
Input:          ÎÞ     
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPlan()
{
	char   sSql[SMALL_BUF_SIZE];

	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_PLAN].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModPlan
Description:    ÐÞ¸ÄÊ±»ùµ÷¶È±í¼ÇÂ¼£¬Èô²»´æÔÚÔòÌí¼Ó
Input:          uPlanId  -- ¼Æ»®·½°¸ºÅ  
				sPlan  --   ´ýÐÞ¸Ä·½°¸½á¹¹Ìå        
Output:         ÎÞ
Return:         true - ÐÞ¸Ä³É¹¦ false -ÐÞ¸ÄÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModPlan(Byte ucPlanId, Plan& sPlan)
{
	Plan sTemp;
	if (!QueryPlan(ucPlanId, sTemp))
	{
		return AddPlan(ucPlanId, sPlan);
	}

	if (!IsPlanValid(ucPlanId, sPlan))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tInvalid Plan\n", SHORT_FILE, __LINE__));
		return false;
	}

	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%lu, %s=%u where %s=%u",
							Table_Desc[TBL_PLAN].sTblName,
							Table_Desc[TBL_PLAN].sFieldName[1], sPlan.usMonthFlag,
							Table_Desc[TBL_PLAN].sFieldName[2], sPlan.ucWeekFlag,
							Table_Desc[TBL_PLAN].sFieldName[3], sPlan.ulDayFlag,
							Table_Desc[TBL_PLAN].sFieldName[4], sPlan.ucScheduleId,
							Table_Desc[TBL_PLAN].sFieldName[0], ucPlanId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::IsPlanValid
Description:    ÅÐ¶Ï·½°¸ºÅÊÇ·ñºÏ·¨
Input:          uPlanId  -- ¼Æ»®·½°¸ºÅ  
				sPlan  --   ·½°¸½á¹¹Ìå        
Output:         ÎÞ
Return:         true - ºÏ·¨ false -²»ºÏ·¨
***************************************************************/
bool CGbtTscDb::IsPlanValid(Byte ucPlanId, Plan& sPlan)
{
    CHECK_ID(CONST_PLAN, ucPlanId, false);
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QuerySchedule
Description:    ²éÑ¯Ê±¶Î±í£¬²¢±£´æ²éÑ¯½á¹û
Input:          tblSchedule -- Ê±¶Î±í¶ÔÏó   
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QuerySchedule(TblSchedule& tblSchedule)
{
    int    i;
    Ushort uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Schedule*   pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s,%s", 
                			Table_Desc[TBL_SCHEDULE].sTblName, 
                			Table_Desc[TBL_SCHEDULE].sFieldName[0], 
                			Table_Desc[TBL_SCHEDULE].sFieldName[1]);
    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_SCHEDULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SCHEDULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_SCHEDULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
    
    uCount = (Ushort)qryRst.m_iRow;
    if (!tblSchedule.AllocData(uCount))
    {
        return false;
    }

    pData = tblSchedule.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    { 
        pData[i].ucScheduleId = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucEvtId      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucBgnHour    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucBgnMinute  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucCtrlMode   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucPatternNo  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].ucAuxOut     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
        pData[i].ucSpecialOut = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 7]);
	}
    
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QuerySchedule
Description:    ¸ù¾ÝÊ±¶ÎºÅ²éÑ¯Ê±¶Î±í£¬²¢±£´æ²éÑ¯½á¹û
Input:          uScheduleId -- Ê±¶Î±íºÅ
				tblSchedule -- Ê±¶Î±í¶ÔÏó   
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QuerySchedule(Byte uScheduleId, TblSchedule& tblSchedule)
{
    int    i;
    Ushort uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Schedule*   pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u order by %s , %s", 
               			 Table_Desc[TBL_SCHEDULE].sTblName, 
               			 Table_Desc[TBL_SCHEDULE].sFieldName[0],
               			 uScheduleId,
               			 Table_Desc[TBL_SCHEDULE].sFieldName[2],
				 Table_Desc[TBL_SCHEDULE].sFieldName[3]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_SCHEDULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SCHEDULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_SCHEDULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
    
    uCount = (Ushort)qryRst.m_iRow;
    if (!tblSchedule.AllocData(uCount))
    {
        return false;
    }

    pData = tblSchedule.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    { 
        pData[i].ucScheduleId = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucEvtId      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucBgnHour    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucBgnMinute  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucCtrlMode   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucPatternNo  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].ucAuxOut     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
        pData[i].ucSpecialOut = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 7]);
    }
    
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QuerySchedule
Description:    ¸ù¾ÝÊ±¶ÎºÅºÍÊÂ¼þºÅ²éÑ¯Ê±¶Î±í£¬²¢±£´æ²éÑ¯½á¹û
Input:          uScheduleId -- Ê±¶Î±íºÅ
				uEvtId      -- ÊÂ¼þºÅ
				sSchedule -- Ê±¶Î±í¼ÇÂ¼½á¹¹Ìå   
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QuerySchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u and %s=%u", 
               				 Table_Desc[TBL_SCHEDULE].sTblName, 
               				 Table_Desc[TBL_SCHEDULE].sFieldName[0],
               				 uScheduleId,
               				 Table_Desc[TBL_SCHEDULE].sFieldName[1], uEvtId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_SCHEDULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SCHEDULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_SCHEDULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
    
    sSchedule.ucScheduleId 	  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sSchedule.ucEvtId      		  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sSchedule.ucBgnHour  		  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sSchedule.ucBgnMinute 	  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    sSchedule.ucCtrlMode   	  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    sSchedule.ucPatternNo 	  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
    sSchedule.ucAuxOut     		  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);
    sSchedule.ucSpecialOut 	  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 7]);

    return true;
}


/**************************************************************
Function:       CGbtTscDb::AddSchedule
Description:    Ìí¼ÓÊ±¶Î±í
Input:          uScheduleId -- Ê±¶Î±íºÅ
				uEvtId      -- ÊÂ¼þºÅ
				sSchedule --   Ê±¶Î±í¼ÇÂ¼½á¹¹Ìå   
Output:         ÎÞ
Return:         true - Ìí¼Ó³É¹¦ false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule)
{
    char   sSql[SMALL_BUF_SIZE];

    if (!IsScheduleValid(uScheduleId, uEvtId, sSchedule))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tSchedule invalid\n", SHORT_FILE, __LINE__));
        return true;  //ºöÂÔÕâÌõ
    }
	/*
	if ( 0 == uEvtId  || uEvtId >0x30) //ÊÂ¼þidÎª0²»´¦Àí
	{		
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to AddSchedule(%d, %d)\n", SHORT_FILE, __LINE__, sSchedule.ucScheduleId, sSchedule.ucEvtId));  
		return true;
	}
       */
    ACE_OS::sprintf(sSql, "insert into %s(%s,%s,%s,%s,%s,%s,%s,%s)values(%u,%u,%u,%u,%u,%u,%u,%u)",
                  		   Table_Desc[TBL_SCHEDULE].sTblName,
                  		   Table_Desc[TBL_SCHEDULE].sFieldName[0],
                  	 	   Table_Desc[TBL_SCHEDULE].sFieldName[1],
                  		   Table_Desc[TBL_SCHEDULE].sFieldName[2],
                  		   Table_Desc[TBL_SCHEDULE].sFieldName[3], 
                   		   Table_Desc[TBL_SCHEDULE].sFieldName[4], 
                  		   Table_Desc[TBL_SCHEDULE].sFieldName[5], 
                   		   Table_Desc[TBL_SCHEDULE].sFieldName[6], 
                   		   Table_Desc[TBL_SCHEDULE].sFieldName[7],
                   		   uScheduleId, uEvtId, sSchedule.ucBgnHour, 
                   		   sSchedule.ucBgnMinute, sSchedule.ucCtrlMode, 
                  	       sSchedule.ucPatternNo, sSchedule.ucAuxOut, 
                  		   sSchedule.ucSpecialOut);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true; //ºöÂÔÕâÌõ
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::ModSchedule
Description:    ÐÞ¸ÄÊ±¶Î±í
Input:          uScheduleId -- Ê±¶Î±íºÅ
				uEvtId      -- ÊÂ¼þºÅ
				sSchedule --   Ê±¶Î±í¼ÇÂ¼½á¹¹Ìå   
Output:         ÎÞ
Return:         true - ÐÞ¸Ä³É¹¦ false -ÐÞ¸ÄÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModSchedule(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule)
{
    Schedule  sTemp;

	if ( 0 == uEvtId ) 
	{
		return true;
	}

    if (!QuerySchedule(uScheduleId, uEvtId, sTemp))
    {
        return AddSchedule(uScheduleId, uEvtId, sSchedule);
    }

    char   sSql[SMALL_BUF_SIZE];
    if (!IsScheduleValid(uScheduleId, uEvtId, sSchedule))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tSchedule invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    ACE_OS::sprintf(sSql, "update %s set %s=%u,%s=%u,%s=%u,%s=%u,%s=%u,%s=%u where %s=%u and %s=%u",
                   			Table_Desc[TBL_SCHEDULE].sTblName,
                  		         Table_Desc[TBL_SCHEDULE].sFieldName[2], sSchedule.ucBgnHour,
                   			Table_Desc[TBL_SCHEDULE].sFieldName[3], sSchedule.ucBgnMinute,
                  			Table_Desc[TBL_SCHEDULE].sFieldName[4], sSchedule.ucCtrlMode,
                  			Table_Desc[TBL_SCHEDULE].sFieldName[5], sSchedule.ucPatternNo,
                  			Table_Desc[TBL_SCHEDULE].sFieldName[6], sSchedule.ucAuxOut,
                  			Table_Desc[TBL_SCHEDULE].sFieldName[7], sSchedule.ucSpecialOut,
                   			Table_Desc[TBL_SCHEDULE].sFieldName[0], uScheduleId,
                   			Table_Desc[TBL_SCHEDULE].sFieldName[1], uEvtId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    
    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelSchedule
Description:    É¾³ýÊ±¶Î±í
Input:          uScheduleId -- Ê±¶Î±íºÅ
				uEvtId      -- ÊÂ¼þºÅ
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelSchedule(Byte uScheduleId, Byte uEvtId)
{
    char   sSql[SMALL_BUF_SIZE];

    ACE_OS::sprintf(sSql, "delete from %s where %s=%u and %s=%u",
                  		   Table_Desc[TBL_SCHEDULE].sTblName,
                   		   Table_Desc[TBL_SCHEDULE].sFieldName[0], uScheduleId,
                   		   Table_Desc[TBL_SCHEDULE].sFieldName[1], uEvtId);
    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelSchedule
Description:    ¸ù¾ÝÊ±¶ÎºÅÉ¾³ýÊ±¶Î±í
Input:          uScheduleId -- Ê±¶Î±íºÅ
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelSchedule(Byte uScheduleId)
{
    char   sSql[SMALL_BUF_SIZE];

    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
                  		   Table_Desc[TBL_SCHEDULE].sTblName,
                  		   Table_Desc[TBL_SCHEDULE].sFieldName[0], uScheduleId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelSchedule
Description:    É¾³ýÈ«²¿Ê±¶Î±í
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelSchedule()
{
    char   sSql[SMALL_BUF_SIZE];

    ACE_OS::sprintf(sSql, "delete from %s",Table_Desc[TBL_SCHEDULE].sTblName);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::IsScheduleValid
Description:    ÅÐ¶ÏÊäÊ±¶Î±íÊý¾ÝÊÇ·ñºÏ·¨
Input:          uScheduleId -- Ê±¶Î±íºÅ
				uEvtId      -- ÊÂ¼þºÅ
				sSchedule --   Ê±¶Î±í¼ÇÂ¼½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÊäÈëºÏ·¨ false -ÊäÈë²»ºÏ·¨
***************************************************************/
bool CGbtTscDb::IsScheduleValid(Byte uScheduleId, Byte uEvtId, Schedule& sSchedule)
{
    CHECK_ID(CONST_SCHEDULE,uScheduleId, false);
    
	if ( uEvtId != 0 )
	{
		CHECK_ID(CONST_SCHEDULEEVT,uEvtId, false);
	}

    if (sSchedule.ucBgnHour >= 24)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucBgnHour = %d, invalid\n", SHORT_FILE, __LINE__,sSchedule.ucBgnHour));
        return false;
    }

    if (sSchedule.ucBgnMinute >= 60)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucBgnMinute = %d, invalid\n", SHORT_FILE, __LINE__,sSchedule.ucBgnMinute));
        return false;
    }

    if (sSchedule.ucCtrlMode > 13)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucCtrlMode = %d, invalid\n", SHORT_FILE, __LINE__,sSchedule.ucCtrlMode));
        return false;
    }
    if (0 != sSchedule.ucPatternNo && 255 != sSchedule.ucPatternNo && 254 != sSchedule.ucPatternNo)
    {
        CHECK_ID(CONST_PATTERN,sSchedule.ucPatternNo, false);
    }

    return true;
}
 
/**************************************************************
Function:       CGbtTscDb::QueryModule
Description:    ÐÅºÅ»úÄ£¿é²éÑ¯£¬²¢±£´æ
Input:          tblModule -- Ä£¿éÀà¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryModule(TblModule& tblModule)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Module* pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
               			   Table_Desc[TBL_MODULE].sTblName,
               			   Table_Desc[TBL_MODULE].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_MODULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_MODULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_MODULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblModule.AllocData(uCount))
    {
        return false;
    }

    pData = tblModule.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucModuleId = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);

        if (!pData[i].strDevNode.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]))
        {
            return false;
        }
        
        if (!pData[i].strCompany.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]))
        {
            return false;
        }

        if (!pData[i].strModel.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]))
        {
            return false;
        }

        if (!pData[i].strVersion.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]))
        {
            return false;
        }

        pData[i].ucType = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryModule
Description:    ¸ù¾ÝÐÅºÅ»úÄ£¿é±íID²éÑ¯£¬²¢±£´æ
Input:          uModuleId -- Ä£¿éID
				sModule   -- Ä£¿é½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryModule(Byte uModuleId, Module& sModule)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
               			   Table_Desc[TBL_MODULE].sTblName,
               			   Table_Desc[TBL_MODULE].sFieldName[0], uModuleId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_MODULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_MODULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_MODULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sModule.ucModuleId = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);

    if (!sModule.strDevNode.SetString(qryRst.m_ppResult[qryRst.m_iColum + 1]))
    {
        return false;
    }
    
    if (!sModule.strCompany.SetString(qryRst.m_ppResult[qryRst.m_iColum + 2]))
    {
        return false;
    }

    if (!sModule.strModel.SetString(qryRst.m_ppResult[qryRst.m_iColum + 3]))
    {
        return false;
    }

    if (!sModule.strVersion.SetString(qryRst.m_ppResult[qryRst.m_iColum + 4]))
    {
        return false;
    }

    sModule.ucType = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum+5]);
   
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryModule
Description:    ¸ù¾ÝÉú²úÉÌ²éÑ¯Ä£¿é£¬²¢±£´æ
Input:          sCompany -- Éú²úÉÌÃû³Æ×Ö·û´®
				tblModule   -- Ä£¿éÀà¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryModule(const char* sCompany, TblModule& tblModule)
{
    int    i;
    Byte  uCount;
    char   sSql[LARGE_BUF_SIZE];
    CSqliteRst  qryRst;
    Module* pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s where %s='%s' order by %s", 
              			   Table_Desc[TBL_MODULE].sTblName,
                		   Table_Desc[TBL_MODULE].sFieldName[2], 
                		   sCompany,
               			   Table_Desc[TBL_MODULE].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_MODULE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_MODULE].sTblName, qryRst.m_iColum,Table_Desc[TBL_MODULE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblModule.AllocData(uCount))
    {
        return false;
    }

    pData = tblModule.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucModuleId = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);

        if (!pData[i].strDevNode.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]))
        {
            return false;
        }
        
        if (!pData[i].strCompany.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]))
        {
            return false;
        }

        if (!pData[i].strModel.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]))
        {
            return false;
        }

        if (!pData[i].strVersion.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]))
        {
            return false;
        }

        pData[i].ucType = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::AddModule
Description:    Ìí¼ÓÄ£¿é±í£¬²¢±£´æ
Input:          uModuleId -- Ä£¿éIDºÅ
				sModule   -- ´ý±£´æÄ£¿é½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼Ó³É¹¦ false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddModule(Byte uModuleId, Module& sModule)
{
    Ushort uCount;
    char   sSql[LARGE_BUF_SIZE];

    if (!IsModuleValid(uModuleId, sModule))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsModule invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    ACE_OS::sprintf(sSql, "insert into %s(%s,%s,%s,%s,%s,%s)values(%u,'%s','%s','%s','%s',%u)",
                  		   Table_Desc[TBL_MODULE].sTblName,
                   		   Table_Desc[TBL_MODULE].sFieldName[0],
                   		   Table_Desc[TBL_MODULE].sFieldName[1], 
                   		   Table_Desc[TBL_MODULE].sFieldName[2], 
                   		   Table_Desc[TBL_MODULE].sFieldName[3], 
                   		   Table_Desc[TBL_MODULE].sFieldName[4], 
                   		   Table_Desc[TBL_MODULE].sFieldName[5],
                   		   uModuleId, 
                  		   sModule.strDevNode.GetData(uCount), 
                   		   sModule.strCompany.GetData(uCount), 
                  		   sModule.strModel.GetData(uCount), 
                  		   sModule.strVersion.GetData(uCount), 
                   		   sModule.ucType);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}



/**************************************************************
Function:       CGbtTscDb::ModModule
Description:    ÐÞ¸ÄÄ£¿é±í£¬²¢±£´æ
Input:          uModuleId -- Ä£¿éIDºÅ
				sModule   -- ´ý±£´æÄ£¿é½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸Ä³É¹¦ false -ÐÞ¸ÄÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModModule(Byte uModuleId, Module& sModule)
{
    Module sTemp;
    Ushort uCount;
    if (!QueryModule(uModuleId, sTemp))
    {
        return AddModule(uModuleId, sModule);
    }

    if (!IsModuleValid(uModuleId, sModule))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsModule invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    char   sSql[LARGE_BUF_SIZE];
    CHECK_ID(CONST_MODULE, uModuleId, false);
    ACE_OS::sprintf(sSql, "update %s set %s='%s', %s='%s', %s='%s', %s='%s', %s=%u where %s=%u",
                  		   Table_Desc[TBL_MODULE].sTblName,
                   		   Table_Desc[TBL_MODULE].sFieldName[1], 
                   		   sModule.strDevNode.GetData(uCount),
                  		   Table_Desc[TBL_MODULE].sFieldName[2], 
                  		   sModule.strCompany.GetData(uCount),
                   		   Table_Desc[TBL_MODULE].sFieldName[3], 
                   		   sModule.strModel.GetData(uCount),
                   		   Table_Desc[TBL_MODULE].sFieldName[4], 
                   		   sModule.strVersion.GetData(uCount),
                  		   Table_Desc[TBL_MODULE].sFieldName[5], 
                  		   sModule.ucType,
                  		   Table_Desc[TBL_MODULE].sFieldName[0], 
                  		   uModuleId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::DelModule
Description:    ¸ù¾ÝÄ£¿éIDºÅÉ¾³ýÄ£¿é±í¼ÇÂ¼
Input:          uModuleId -- Ä£¿éIDºÅ
Output:         ÎÞ
Return:         true - É¾³ý³É¹¦ false -É¾³ýÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelModule(Byte uModuleId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
                   		   Table_Desc[TBL_MODULE].sTblName,
                   		   Table_Desc[TBL_MODULE].sFieldName[0], 
                   		   uModuleId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::IsModuleValid
Description:    ÅÐ¶ÏÄ£¿éÊý¾ÝÊÇ·ñºÏ·¨
Input:          uModuleId -- Ä£¿éIDºÅ
				sModule   -- Ä£¿é½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Êý¾ÝÐ£ÑéºÏ·¨ false -Êý¾ÝÐ£Ñé·Ç·¨
***************************************************************/
bool CGbtTscDb::IsModuleValid(Byte uModuleId, Module& sModule)
{
    CHECK_ID(CONST_MODULE, uModuleId, false);

    if (sModule.ucType <= 0 || sModule.ucType > 3)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucType invalid, ucType = %d\n", SHORT_FILE, __LINE__, sModule.ucType));
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryStageFactTime
Description:    ²éÑ¯È«²¿½×¶ÎÂÌµÆ±í£¬²¢±£´æ
Input:          tblStageFactTime -- ½×¶ÎÂÌµÆÀà¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryStageFactTime(TblStageFactTime& tblStageFactTime)
{
	int    i;
	Ulong  uCount;
	char   sSql[SMALL_BUF_SIZE] = {0};
	CSqliteRst  qryRst;
	StageFactTime* pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
					       Table_Desc[TBL_STAGE_FACTTIME].sTblName,
						   Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_STAGE_FACTTIME].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", 
							  SHORT_FILE, __LINE__, Table_Desc[TBL_STAGE_FACTTIME].sTblName
							 , qryRst.m_iColum,Table_Desc[TBL_STAGE_FACTTIME].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblStageFactTime.AllocData(uCount))
	{
		return false;
	}

	pData = tblStageFactTime.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].uiAddTime = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);

		pData[i].ucStageCnt = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);

		if (!pData[i].sStageGreenTime.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]))
		{
			return false;
		}
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryStageFactTime
Description:    ²éÑ¯ÌØ¶¨Ê±¼ä¶ÎÀïµÄ½×¶ÎÂÌµÆ±í
Input:          tblStageFactTime -- ½×¶ÎÂÌµÆÀà¶ÔÏó
				ulStartTime      -- ÆðÊ¼Ê±¼ä
				ulEndTime        -- ½áÊøÊ±¼ä
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryStageFactTime(TblStageFactTime& tblStageFactTime,Ulong ulStartTime,Ulong ulEndTime)
{
	int    i;
	Ulong  uCount;
	char   sSql[SMALL_BUF_SIZE] = {0};
	CSqliteRst  qryRst;
	StageFactTime* pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s where %s>%lu and %s<%lu", 
						  Table_Desc[TBL_STAGE_FACTTIME].sTblName,
						  Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0],
						  ulStartTime,
						  Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0],
						  ulEndTime);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_STAGE_FACTTIME].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", 
							  SHORT_FILE, __LINE__, Table_Desc[TBL_STAGE_FACTTIME].sTblName
					          , qryRst.m_iColum,Table_Desc[TBL_STAGE_FACTTIME].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblStageFactTime.AllocData(uCount))
	{
		return false;
	}

	pData = tblStageFactTime.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].uiAddTime = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);

		pData[i].ucStageCnt = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);

		if (!pData[i].sStageGreenTime.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]))
		{
			return false;
		}
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::AddStageFactTime
Description:    Ìí¼Ó½×¶ÎÂÌµÆ±í
Input:          sStageFactTime  -- ½×¶ÎÂÌµÆ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddStageFactTime(StageFactTime& sStageFactTime)
{
	Ushort uCount;
	Ulong   uTime;
	CSqliteRst  qryRst;
	char   sSql[LARGE_BUF_SIZE] = {0};

	//ÅÐ¶Ï×îÐ¡µÄÈÕÆÚ ´æ´¢µÄ×î³¤1¸öÔÂ
	ACE_OS::sprintf(sSql, "select MIN(%s) from %s", 
						  Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0],
					           Table_Desc[TBL_STAGE_FACTTIME].sTblName);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if ( NULL == qryRst.m_ppResult[qryRst.m_iColum] )
	{
		uTime = sStageFactTime.uiAddTime;
	}
	else
	{
		uTime = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	}
	
	if ( sStageFactTime.uiAddTime - uTime > MAX_STAGEFACT_CNT * 24 * 3600 )
	{
		DeleteStageFactTime(uTime);
	}

	ACE_OS::memset(sSql,0,LARGE_BUF_SIZE);
	ACE_OS::sprintf(sSql, "insert into %s(%s,%s,%s)values(%lu,%u,'%s')",
						   Table_Desc[TBL_STAGE_FACTTIME].sTblName,
						   Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0], 
						   Table_Desc[TBL_STAGE_FACTTIME].sFieldName[1], 
						   Table_Desc[TBL_STAGE_FACTTIME].sFieldName[2],
						   sStageFactTime.uiAddTime,
						   sStageFactTime.ucStageCnt,
						   sStageFactTime.sStageGreenTime.GetData(uCount)
							);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::DeleteStageFactTime
Description:    É¾³ý½×¶ÎÂÌµÆ±í
Input:          ulAddTime  -- ½×¶ÎÂÌµÆÊ±¼ä
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DeleteStageFactTime(Ulong ulAddTime)
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s where %s=%lu",
						   Table_Desc[TBL_STAGE_FACTTIME].sTblName,
						   Table_Desc[TBL_STAGE_FACTTIME].sFieldName[0], 
						   ulAddTime);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}



/**************************************************************
Function:       CGbtTscDb::QueryAdaptStageTime
Description:    ²éÑ¯È«²¿×ÔÊÊÓ¦½×¶ÎÂÌµÆ±í
Input:          tblAdaptStageTime  -- ×ÔÊÊÓ¦½×¶ÎÂÌµÆ±íÀà¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	AdaptStageTime* pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
					       Table_Desc[TBL_ADAPT_STAGETIME].sTblName,
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_ADAPT_STAGETIME].sTblName, qryRst.m_iColum,Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if ( !tblAdaptStageTime.AllocData(uCount) )
	{
		return false;
	}

	pData = tblAdaptStageTime.GetData(uCount);
	for ( i = 0; i < qryRst.m_iRow; i++ )
	{
		pData[i].ucId       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucWeekType = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucHour     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucMin      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ucStageCnt = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);

		if (!pData[i].sStageGreenTime.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]))
		{
			return false;
		}
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryAdaptStageTime
Description:    ¸ù¾ÝÐÇÆÚ²éÑ¯È«²¿½×¶ÎÂÌµÆ±í
Input:          tblAdaptStageTime  -- ×ÔÊÊÓ¦½×¶ÎÂÌµÆ±íÀà¶ÔÏó
				ucWeekType  -- ÐÇÆÚ
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryAdaptStageTime(TblAdaptStageTime& tblAdaptStageTime,Byte ucWeekType)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	AdaptStageTime* pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%u order by %s , %s", 
						   Table_Desc[TBL_ADAPT_STAGETIME].sTblName,
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[1],
						   ucWeekType,
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[2],
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[3]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_ADAPT_STAGETIME].sTblName, qryRst.m_iColum,Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if ( !tblAdaptStageTime.AllocData(uCount) )
	{
		return false;
	}

	pData = tblAdaptStageTime.GetData(uCount);
	for ( i = 0; i < qryRst.m_iRow; i++ )
	{
		pData[i].ucId       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucWeekType = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucHour     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucMin      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ucStageCnt = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);

		if (!pData[i].sStageGreenTime.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]))
		{
			return false;
		}
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::AddAdaptStageTime
Description:    Ìí¼Ó×ÔÊÊÓ¦½×¶ÎÂÌµÆ±íÊý¾Ý¼ÇÂ¼
Input:          sAdaptStageTime  -- ×ÔÊÊÓ¦½×¶ÎÂÌµÆ½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddAdaptStageTime(AdaptStageTime& sAdaptStageTime)
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "insert into %s(%s,%s,%s,%s,%s)values(%u,%u,%u,%u,'%s')",
						   Table_Desc[TBL_ADAPT_STAGETIME].sTblName,
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[1], 
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[2], 
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[3], 
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[4], 
						   Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[5],
						   sAdaptStageTime.ucWeekType, 
						   sAdaptStageTime.ucHour,
						   sAdaptStageTime.ucMin,
						   sAdaptStageTime.ucStageCnt,
						   sAdaptStageTime.sStageGreenTime.GetData()
							);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryAdaptStageTime
Description:    ¸ù¾Ýucid²éÑ¯×ÔÊÊÓ¦½×¶ÎÂÌµÆ±íÊý¾Ý¼ÇÂ¼
Input:          ucId  -- IDºÅ
				sAdaptStageTime  -- ×ÔÊÊÓ¦½×¶ÎÂÌµÆ½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryAdaptStageTime(Byte ucId, AdaptStageTime& sAdaptStageTime)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%u ", 
						  Table_Desc[TBL_ADAPT_STAGETIME].sTblName,
						  Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[0],
						  ucId);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_ADAPT_STAGETIME].sTblName, qryRst.m_iColum,Table_Desc[TBL_ADAPT_STAGETIME].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}
	sAdaptStageTime.ucId       		= ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sAdaptStageTime.ucWeekType          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sAdaptStageTime.ucHour                   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
	sAdaptStageTime.ucMin                     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sAdaptStageTime.ucStageCnt            = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);

	if ( !sAdaptStageTime.sStageGreenTime.SetString(qryRst.m_ppResult[qryRst.m_iColum + 5]) )
	{
		return false;
	}
	
	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModAdaptStageTime
Description:    ÐÞ¸Ä×ÔÊÊÓ¦½×¶ÎÂÌµÆ±íÊý¾Ý¼ÇÂ¼
Input:          sAdaptStageTime  -- ×ÔÊÊÓ¦½×¶ÎÂÌµÆ½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModAdaptStageTime(AdaptStageTime& sAdaptStageTime)
{
	AdaptStageTime sTemp;
	if ( !QueryAdaptStageTime(sAdaptStageTime.ucId, sTemp) )
	{
		return AddAdaptStageTime(sAdaptStageTime);
	}

	char   sSql[LARGE_BUF_SIZE];
	
	ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u, %s=%u, %s='%s' where %s=%u",
							Table_Desc[TBL_ADAPT_STAGETIME].sTblName,
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[1], 
							sAdaptStageTime.ucWeekType,
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[2], 
							sAdaptStageTime.ucHour,
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[3], 
							sAdaptStageTime.ucMin,
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[4], 
							sAdaptStageTime.ucStageCnt,
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[5], 
							sAdaptStageTime.sStageGreenTime.GetData(),
							Table_Desc[TBL_ADAPT_STAGETIME].sFieldName[0], 
							sAdaptStageTime.ucId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::DeleteAdaptStageTime
Description:    É¾³ýÈ«²¿×ÔÊÊÓ¦½×¶ÎÂÌµÆ±í
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DeleteAdaptStageTime()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",  Table_Desc[TBL_ADAPT_STAGETIME].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPhase
Description:    ÏàÎ»²éÑ¯
Input:          tblPhase  -- ÏàÎ»±íÀà¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryPhase(TblPhase& tblPhase)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Phase* pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
               			   Table_Desc[TBL_PHASE].sTblName,
               			   Table_Desc[TBL_PHASE].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PHASE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PHASE].sTblName, qryRst.m_iColum,Table_Desc[TBL_PHASE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblPhase.AllocData(uCount))
    {
        return false;
    }

    pData = tblPhase.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucPhaseId          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucPedestrianGreen  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucPedestrianClear  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucMinGreen         = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucGreenDelayUnit   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucMaxGreen1        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].ucMaxGreen2        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
        pData[i].ucFixGreen         = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 7]);
        pData[i].ucGreenFlash       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 8]);
        pData[i].ucPhaseTypeFlag    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 9]);
        pData[i].ucPhaseOption      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 10]);
        pData[i].ucExtend           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 11]);

    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅÏàÎ»²éÑ¯
Input:          uPhaseId  -- ÏàÎ»ID
				sPhase   -- ÏàÎ»½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryPhase(Byte uPhaseId, Phase& sPhase)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
                		  Table_Desc[TBL_PHASE].sTblName,
                		  Table_Desc[TBL_PHASE].sFieldName[0], uPhaseId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PHASE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PHASE].sTblName, qryRst.m_iColum,Table_Desc[TBL_PHASE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sPhase.ucPhaseId          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sPhase.ucPedestrianGreen  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sPhase.ucPedestrianClear  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sPhase.ucMinGreen         = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    sPhase.ucGreenDelayUnit   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    sPhase.ucMaxGreen1        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
    sPhase.ucMaxGreen2        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);
    sPhase.ucFixGreen         = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 7]);
    sPhase.ucGreenFlash       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 8]);
    sPhase.ucPhaseTypeFlag    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 9]);
    sPhase.ucPhaseOption      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 10]);
    sPhase.ucExtend           = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 11]);
 
    return true;
}
  
/**************************************************************
Function:       CGbtTscDb::AddPhase
Description:    Ìí¼ÓÏàÎ»±í¼ÇÂ¼
Input:          uPhaseId  -- ÏàÎ»ID
				sPhase   -- ´ýÌí¼ÓÏàÎ»½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddPhase(Byte uPhaseId, Phase& sPhase)
{
    char   sSql[LARGE_BUF_SIZE];
    if (!IsPhaseValid(uPhaseId, sPhase))
    {
     //   ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsPhase invalid\n", SHORT_FILE, __LINE__));
        return true;
    }
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)values(%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
				           Table_Desc[TBL_PHASE].sTblName,
				           Table_Desc[TBL_PHASE].sFieldName[0],
				           Table_Desc[TBL_PHASE].sFieldName[1], 
				           Table_Desc[TBL_PHASE].sFieldName[2], 
				           Table_Desc[TBL_PHASE].sFieldName[3],
				           Table_Desc[TBL_PHASE].sFieldName[4],
				           Table_Desc[TBL_PHASE].sFieldName[5], 
				           Table_Desc[TBL_PHASE].sFieldName[6], 
				           Table_Desc[TBL_PHASE].sFieldName[7], 
				           Table_Desc[TBL_PHASE].sFieldName[8], 
				           Table_Desc[TBL_PHASE].sFieldName[9], 
				           Table_Desc[TBL_PHASE].sFieldName[10], 
				           Table_Desc[TBL_PHASE].sFieldName[11], 
				           uPhaseId,
				           sPhase.ucPedestrianGreen,
				           sPhase.ucPedestrianClear,
				           sPhase.ucMinGreen, 
				           sPhase.ucGreenDelayUnit, 
				           sPhase.ucMaxGreen1,
				           sPhase.ucMaxGreen2,
				           sPhase.ucFixGreen,
				           sPhase.ucGreenFlash,
				           sPhase.ucPhaseTypeFlag,
				           sPhase.ucPhaseOption,
				           sPhase.ucExtend);

    if (!ExecuteCmd(sSql))
    {
       // ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }
    return true;
}
   
/**************************************************************
Function:       CGbtTscDb::ModPhase
Description:    ÐÞ¸ÄÏàÎ»±í¼ÇÂ¼Êý¾Ý
Input:          uPhaseId  -- ÏàÎ»ID
				sPhase   -- ´ýÐÞ¸ÄÏàÎ»½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModPhase(Byte uPhaseId, Phase& sPhase)
{
    Phase sTemp;
    if (!QueryPhase(uPhaseId, sTemp))
    {
        return AddPhase(uPhaseId, sPhase);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsPhaseValid(uPhaseId, sPhase))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsPhase invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u, %s=%u, %s=%u, %s=%u, %s=%u, %s=%u,%s=%u, %s=%u, %s=%u where %s=%u",
				           Table_Desc[TBL_PHASE].sTblName,
				           Table_Desc[TBL_PHASE].sFieldName[1], sPhase.ucPedestrianGreen,
				           Table_Desc[TBL_PHASE].sFieldName[2], sPhase.ucPedestrianClear,
				           Table_Desc[TBL_PHASE].sFieldName[3], sPhase.ucMinGreen,
				           Table_Desc[TBL_PHASE].sFieldName[4], sPhase.ucGreenDelayUnit,
				           Table_Desc[TBL_PHASE].sFieldName[5], sPhase.ucMaxGreen1,
				           Table_Desc[TBL_PHASE].sFieldName[6], sPhase.ucMaxGreen2,
				           Table_Desc[TBL_PHASE].sFieldName[7], sPhase.ucFixGreen,
				           Table_Desc[TBL_PHASE].sFieldName[8], sPhase.ucGreenFlash,
				           Table_Desc[TBL_PHASE].sFieldName[9], sPhase.ucPhaseTypeFlag,
				           Table_Desc[TBL_PHASE].sFieldName[10], sPhase.ucPhaseOption,
				           Table_Desc[TBL_PHASE].sFieldName[11], sPhase.ucExtend,
				           Table_Desc[TBL_PHASE].sFieldName[0], uPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅÉ¾³ýÏàÎ»±í¼ÇÂ¼Êý¾Ý
Input:          uPhaseId  -- ÏàÎ»ID
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPhase(Byte uPhaseId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_PHASE].sTblName,
				           Table_Desc[TBL_PHASE].sFieldName[0], uPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelPhase
Description:    É¾³ýÈ«²¿ÏàÎ»±í¼ÇÂ¼Êý¾Ý
Input:          uPhaseId  -- ÏàÎ»ID
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPhase()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_PHASE].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::IsPhaseValid
Description:    ÅÐ¶ÏÏàÎ»±íÊý¾ÝºÏ·¨ÐÔ
Input:          uPhaseId  -- ÏàÎ»ID
				sPhase    -- ÏàÎ»Êý¾Ý½á¹¹Ìå
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨ false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsPhaseValid(Byte uPhaseId, Phase& sPhase)
{
    CHECK_ID(CONST_PHASE, uPhaseId, false);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryCollision
Description:    ²éÑ¯ÏàÎ»³åÍ»±íÊý¾Ý
Input:          tblCollision  -- ÏàÎ»³åÍ»±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯ Êý¾Ý³É¹¦false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryCollision(TblCollision& tblCollision)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Collision*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_COLLISION].sTblName,
						    Table_Desc[TBL_COLLISION].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_COLLISION].iFieldCount)
    {
#ifndef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_COLLISION].sTblName, qryRst.m_iColum,Table_Desc[TBL_COLLISION].iFieldCount));
#endif
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblCollision.AllocData(uCount))
    {
        return false;
    }

    pData = tblCollision.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucPhaseId          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].usCollisionFlag    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
    }
    return true;
}

/*0—6¨´0—00‰60ˆ3¨¤0ˆ20†30†20‡30…5¨¦0ˆ50…4*/
bool CGbtTscDb::QueryCollision(Byte uPhaseId, Collision& sCollision)
{
     char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
                Table_Desc[TBL_COLLISION].sTblName,
                Table_Desc[TBL_COLLISION].sFieldName[0], uPhaseId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_COLLISION].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_COLLISION].sTblName, qryRst.m_iColum,Table_Desc[TBL_COLLISION].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sCollision.ucPhaseId          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sCollision.usCollisionFlag    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);

    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddCollision
Description:    ¸ù¾ÝÏàÎ»IDºÅÌí¼ÓÏàÎ»³åÍ»±íÊý¾Ý
Input:          uPhaseId  -- ³åÍ»ÏàÎ»IDºÅ
				sCollision --³åÍ»ÏàÎ»½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddCollision(Byte uPhaseId, Collision& sCollision)
{
    char   sSql[LARGE_BUF_SIZE];
    if (!IsCollisionValid(uPhaseId, sCollision))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tCollision invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    ACE_OS::sprintf(sSql, "insert into %s(%s, %s)values(%u,%d)",
				           Table_Desc[TBL_COLLISION].sTblName,
				           Table_Desc[TBL_COLLISION].sFieldName[0],
				           Table_Desc[TBL_COLLISION].sFieldName[1], 
				           uPhaseId,
				           sCollision.usCollisionFlag);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::ModCollision
Description:    ÐÞ¸ÄÏàÎ»³åÍ»±íÊý¾Ý
Input:          uPhaseId  -- ³åÍ»ÏàÎ»IDºÅ
				sCollision --³åÍ»ÏàÎ»½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModCollision(Byte uPhaseId, Collision& sCollision)
{
    Collision sTemp;
    if (!QueryCollision(uPhaseId, sTemp))
    {
        return AddCollision(uPhaseId, sCollision);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsCollisionValid(uPhaseId, sCollision))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tCollision invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%d where %s=%u",
				           Table_Desc[TBL_COLLISION].sTblName,
				           Table_Desc[TBL_COLLISION].sFieldName[1], sCollision.usCollisionFlag,
				           Table_Desc[TBL_COLLISION].sFieldName[0], uPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelCollision
Description:    ¸ù¾Ý³åÍ»ÏàÎ»IDHºÅÉ¾³ýÏàÎ»³åÍ»±íÊý¾Ý¼ÇÂ¼
Input:          uPhaseId  -- ³åÍ»ÏàÎ»IDºÅ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelCollision(Byte uPhaseId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_COLLISION].sTblName,
				           Table_Desc[TBL_COLLISION].sFieldName[0], 
				           uPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

bool CGbtTscDb::IsCollisionValid(Byte uPhaseId, Collision& sCollision)
{
    CHECK_ID(CONST_PHASE, uPhaseId, false);

    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryDetPara
Description:    ²éÑ¯¼ì²âÆ÷²ÎÊý
Input:          detPara  -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯²ÎÊý³É¹¦  false -²éÑ¯²ÎÊýÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetPara(DetectorPara& detPara)
{
    Ulong   ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxDetector", ulTemp);
    detPara.uDetCount = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxDetector", ulTemp);
    detPara.uMaxDetGrpCount = (Byte)(ulTemp+7)/8;
    
    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucDetDataSeqNo", ulTemp);
    detPara.ucDataSeqNo = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucDetDataCycle", ulTemp);
    detPara.ucDataCycle = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucDetPulseSeqNo", ulTemp);
    detPara.ucPulseSeqNo = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucDetPulseCycle", ulTemp);
    detPara.ucPulseCycle = (Byte)ulTemp;

    detPara.uDetCount = 0;
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryDetector
Description:    ²éÑ¯¼ì²âÆ÷ÅäÖÃ²ÎÊý
Input:          tblDetector  -- ¼ì²âÆ÷ÅäÖÃ²ÎÊý±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetector(TblDetector& tblDetector)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Detector*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_DETECTOR].sTblName,
						    Table_Desc[TBL_DETECTOR].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_DETECTOR].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_DETECTOR].sTblName, qryRst.m_iColum,Table_Desc[TBL_DETECTOR].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblDetector.AllocData(uCount))
    {
        return false;
    }

    pData = tblDetector.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucDetectorId   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucPhaseId      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucDetFlag      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucDirect       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ucValidTime    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucOptionFlag   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].usSaturationFlow   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
        pData[i].ucSaturationOccupy = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 7]);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryDetector
Description:    ¸ù¾Ý¼ì²âÆ÷IDºÅ²éÑ¯¼ì²âÆ÷ÅäÖÃ²ÎÊý
Input:          uDetectorId  -- ¼ì²âÆ÷IDºÅ
				sDetector    -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetector(Byte uDetectorId, Detector& sDetector)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
						    Table_Desc[TBL_DETECTOR].sTblName,
						    Table_Desc[TBL_DETECTOR].sFieldName[0], 
						    uDetectorId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_DETECTOR].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_DETECTOR].sTblName, qryRst.m_iColum,Table_Desc[TBL_DETECTOR].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sDetector.ucDetectorId   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sDetector.ucPhaseId      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sDetector.ucDetFlag      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sDetector.ucDirect       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sDetector.ucValidTime    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    sDetector.ucOptionFlag   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
    sDetector.usSaturationFlow   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);
    sDetector.ucSaturationOccupy = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 7]);
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddDetector
Description:    ¸ù¾Ý¼ì²âÆ÷IDºÅÌí¼Ó¼ì²âÆ÷ÅäÖÃ²ÎÊý
Input:          uDetectorId  -- ¼ì²âÆ÷IDºÅ
				sDetector    -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddDetector(Byte uDetectorId, Detector& sDetector)
{
    char   sSql[LARGE_BUF_SIZE];
    if (!IsDetectorValid(uDetectorId, sDetector))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tDetector invalid\n", SHORT_FILE, __LINE__));
        return true;
    }
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s, %s ,%s)values(%u,%u,%u,%u,%u,%d,%u,%u)",
				           Table_Desc[TBL_DETECTOR].sTblName,
				           Table_Desc[TBL_DETECTOR].sFieldName[0],
				           Table_Desc[TBL_DETECTOR].sFieldName[1], 
				           Table_Desc[TBL_DETECTOR].sFieldName[2],
				           Table_Desc[TBL_DETECTOR].sFieldName[3], 
				           Table_Desc[TBL_DETECTOR].sFieldName[4],
				           Table_Desc[TBL_DETECTOR].sFieldName[5], 
				           Table_Desc[TBL_DETECTOR].sFieldName[6], 
						   Table_Desc[TBL_DETECTOR].sFieldName[7], 
				           uDetectorId,
				           sDetector.ucPhaseId,
				           sDetector.ucDetFlag,
				           sDetector.ucDirect,
						   sDetector.ucValidTime,
				           sDetector.ucOptionFlag ,
				           sDetector.usSaturationFlow ,
				           sDetector.ucSaturationOccupy);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::ModDetector
Description:    ¸ù¾Ý¼ì²âÆ÷IDºÅÐÞ¸Ä¼ì²âÆ÷ÅäÖÃ²ÎÊý
Input:          uDetectorId  -- ¼ì²âÆ÷IDºÅ
				sDetector    -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModDetector(Byte uDetectorId, Detector& sDetector)
{
    Detector sTemp;
    if (!QueryDetector(uDetectorId, sTemp))
    {
        return AddDetector(uDetectorId, sDetector);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsDetectorValid(uDetectorId, sDetector))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tDetector invalid\n", SHORT_FILE, __LINE__));
        return false;
    }

    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u, %s=%u, %s=%d, %s=%u, %s=%u where %s=%u",
				           Table_Desc[TBL_DETECTOR].sTblName,
				           Table_Desc[TBL_DETECTOR].sFieldName[1], sDetector.ucPhaseId,
				           Table_Desc[TBL_DETECTOR].sFieldName[2], sDetector.ucDetFlag,
				           Table_Desc[TBL_DETECTOR].sFieldName[3], sDetector.ucDirect,
						   Table_Desc[TBL_DETECTOR].sFieldName[4], sDetector.ucValidTime,
				           Table_Desc[TBL_DETECTOR].sFieldName[5], sDetector.ucOptionFlag,
				           Table_Desc[TBL_DETECTOR].sFieldName[6], sDetector.usSaturationFlow,
				           Table_Desc[TBL_DETECTOR].sFieldName[7], sDetector.ucSaturationOccupy,
				           Table_Desc[TBL_DETECTOR].sFieldName[0], uDetectorId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelDetector
Description:    ¸ù¾Ý¼ì²âÆ÷IDºÅÉ¾³ý¼ì²âÆ÷ÅäÖÃ²ÎÊý
Input:          uDetectorId  -- ¼ì²âÆ÷IDºÅ
				sDetector    -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelDetector(Byte uDetectorId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_DETECTOR].sTblName,
				           Table_Desc[TBL_DETECTOR].sFieldName[0], 
				           uDetectorId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelDetector
Description:    ÅÐ¶Ï¼ì²âÆ÷±íÊý¾ÝºÏ·¨ÐÔ
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelDetector()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_DETECTOR].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::IsDetectorValid
Description:    É¾³ýÈ«²¿¼ì²âÆ÷ÅäÖÃ²ÎÊý±í¼ÇÂ¼
Input:          uDetectorId  -- ¼ì²âÆ÷IDºÅ
				sDetector    -- ¼ì²âÆ÷²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨  false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsDetectorValid(Byte uDetectorId, Detector& sDetector)
{
   //  CHECK_ID(CONST_DETECTOR, uDetectorId, false);
    if(uDetectorId > MAX_DETECTOR ||uDetectorId ==0x0 )
	return false ;
    if (sDetector.ucPhaseId > MAX_PHASE || sDetector.ucSaturationOccupy > 200)
    {
    	return false ;
        //CHECK_ID(CONST_PHASE, sDetector.ucPhaseId, false);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryDetExtend
Description:    ²éÑ¯¼ì²âÆ÷À©Õ¹ÅäÖÃ±íÊý¾Ý
Input:          tblDetExtend -- ¼ì²âÆ÷À©Õ¹ÅäÖÃ±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetExtend(TblDetExtend& tblDetExtend)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	DetExtend*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
							Table_Desc[TBL_DET_EXTEND].sTblName,
							Table_Desc[TBL_DET_EXTEND].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_DET_EXTEND].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_DET_EXTEND].sTblName, qryRst.m_iColum,Table_Desc[TBL_DET_EXTEND].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblDetExtend.AllocData(uCount))
	{
		return false;
	}

	pData = tblDetExtend.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ucId         = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucSensi      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucGrpNo      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucPro        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ucOcuDefault = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
		pData[i].usCarFlow    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryDetExtend
Description:    ¸ù¾ÝÀ©Õ¹½á¹¹Ìå²éÑ¯¼ì²âÆ÷À©Õ¹ÅäÖÃ±íÊý¾Ý
Input:          sDetExtend -- ¼ì²âÆ÷À©Õ¹ÅäÖÃ½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetExtend(DetExtend& sDetExtend)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
							Table_Desc[TBL_DET_EXTEND].sTblName,
							Table_Desc[TBL_DET_EXTEND].sFieldName[0], 
							sDetExtend.ucId);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_DET_EXTEND].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_DET_EXTEND].sTblName, qryRst.m_iColum,Table_Desc[TBL_DET_EXTEND].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	sDetExtend.ucId         = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sDetExtend.ucSensi      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sDetExtend.ucGrpNo      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
	sDetExtend.ucPro        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sDetExtend.ucOcuDefault = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
	sDetExtend.usCarFlow    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
	
	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryDetExtend
Description:    ¸ù¾ÝÀ©Õ¹IDºÅ²éÑ¯¼ì²âÆ÷À©Õ¹ÅäÖÃ±íÊý¾Ý
Input:          uDetId    -- À©Õ¹IDºÅ
				sDetExtend -- ¼ì²âÆ÷À©Õ¹ÅäÖÃ½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryDetExtend(Byte uDetId, DetExtend& sDetExtend)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
							Table_Desc[TBL_DET_EXTEND].sTblName,
							Table_Desc[TBL_DET_EXTEND].sFieldName[0], 
							uDetId);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_DET_EXTEND].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_DET_EXTEND].sTblName, qryRst.m_iColum,Table_Desc[TBL_DET_EXTEND].iFieldCount));
		return false;
	}

	if ( qryRst.m_iRow <= 0 )
	{
		return false;
	}

	sDetExtend.ucId         = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sDetExtend.ucSensi      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sDetExtend.ucGrpNo      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
	sDetExtend.ucPro        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sDetExtend.ucOcuDefault = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
	sDetExtend.usCarFlow    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);

	return true;
}

/**************************************************************
Function:       CGbtTscDb::AddDetExtend
Description:    Ìí¼Ó¼ì²âÆ÷À©Õ¹ÅäÖÃ±íÊý¾Ý
Input:         	sDetExtend -- ¼ì²âÆ÷À©Õ¹ÅäÖÃ½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÀ©Õ¹ÅäÖÃ¼ÇÂ¼³É¹¦  false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddDetExtend(DetExtend sDetExtend)
{
	char   sSql[LARGE_BUF_SIZE] = {0};
	
	ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s,%s,%s,%s)values(%u,%u,%u,%u,%u,%u,%u,%u,%u)",
							Table_Desc[TBL_DET_EXTEND].sTblName,
							Table_Desc[TBL_DET_EXTEND].sFieldName[0],
							Table_Desc[TBL_DET_EXTEND].sFieldName[1], 
							Table_Desc[TBL_DET_EXTEND].sFieldName[2],
							Table_Desc[TBL_DET_EXTEND].sFieldName[3],
							Table_Desc[TBL_DET_EXTEND].sFieldName[4],
							Table_Desc[TBL_DET_EXTEND].sFieldName[5],
							Table_Desc[TBL_DET_EXTEND].sFieldName[6],
							Table_Desc[TBL_DET_EXTEND].sFieldName[7],
							Table_Desc[TBL_DET_EXTEND].sFieldName[8],
							sDetExtend.ucId,
							sDetExtend.ucSensi,
							sDetExtend.ucGrpNo,
							sDetExtend.ucPro,
							sDetExtend.ucOcuDefault,
							sDetExtend.usCarFlow,
		
							sDetExtend.ucFrency,
							sDetExtend.ucGrpData,
							sDetExtend.ucGrpDistns
							);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModDetExtend
Description:    Ìí¼Ó¼ì²âÆ÷À©Õ¹ÅäÖÃ±íÊý¾Ý
Input:         	sDetExtend -- ¼ì²âÆ÷À©Õ¹ÅäÖÃ½á¹¹Ìå
				uDetExtendId -- À©Õ¹ÅäÖÃIDHºÅ
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÀ©Õ¹ÅäÖÃ¼ÇÂ¼³É¹¦  false -ÐÞ¸ÄÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModDetExtend(Byte uDetExtendId, DetExtend& sDetExtend)
{
	DetExtend sTemp;
	if (!QueryDetExtend(uDetExtendId, sTemp))
	{
		sDetExtend.ucId = uDetExtendId;
		return AddDetExtend(sDetExtend);
	}

	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "update %s set %s=%u,%s=%u,%s=%u,%s=%u,%s=%u ,%s=%u,%s=%u,%s=%u where %s=%u",
							Table_Desc[TBL_DET_EXTEND].sTblName,
							Table_Desc[TBL_DET_EXTEND].sFieldName[3], sDetExtend.ucPro,
							Table_Desc[TBL_DET_EXTEND].sFieldName[1], sDetExtend.ucSensi,
							Table_Desc[TBL_DET_EXTEND].sFieldName[2], sDetExtend.ucGrpNo,
							Table_Desc[TBL_DET_EXTEND].sFieldName[4], sDetExtend.ucOcuDefault,
							Table_Desc[TBL_DET_EXTEND].sFieldName[5], sDetExtend.usCarFlow,
							Table_Desc[TBL_DET_EXTEND].sFieldName[6], sDetExtend.ucFrency,
							Table_Desc[TBL_DET_EXTEND].sFieldName[7], sDetExtend.ucGrpData,
							Table_Desc[TBL_DET_EXTEND].sFieldName[8], sDetExtend.ucGrpDistns,
							Table_Desc[TBL_DET_EXTEND].sFieldName[0], uDetExtendId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryAdaptPara
Description:    ²éÑ¯×ÔÊÊÓ¦²ÎÊý±íÊý¾Ý
Input:         	tblAdaptPara -- ×ÔÊÊÓ¦²ÎÊý±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryAdaptPara(TblAdaptPara& tblAdaptPara)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	AdaptPara*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", Table_Desc[TBL_ADAPT_PARA].sTblName,
						  Table_Desc[TBL_ADAPT_PARA].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_ADAPT_PARA].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_ADAPT_PARA].sTblName, qryRst.m_iColum,Table_Desc[TBL_ADAPT_PARA].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblAdaptPara.AllocData(uCount))
	{
		return false;
	}

	pData = tblAdaptPara.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ucType       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucFirstPro   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucSecPro     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucThirdPro   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ucOcuPro     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
		pData[i].ucCarFlowPro = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
		pData[i].ucSmoothPara = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryAdaptPara
Description:    ¸ù¾Ý×ÔÊÊÓ¦²ÎÊý½á¹¹ÌåÊý¾Ý²éÑ¯×ÔÊÊÓ¦²ÎÊý±íÊý¾Ý
Input:         	sAdaptPara -- ×ÔÊÊÓ¦²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryAdaptPara(AdaptPara& sAdaptPara)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
							Table_Desc[TBL_ADAPT_PARA].sTblName,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[0], 
							sAdaptPara.ucType);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_ADAPT_PARA].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_ADAPT_PARA].sTblName, qryRst.m_iColum,Table_Desc[TBL_ADAPT_PARA].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	sAdaptPara.ucType       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sAdaptPara.ucFirstPro   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sAdaptPara.ucSecPro     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
	sAdaptPara.ucThirdPro   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sAdaptPara.ucOcuPro     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
	sAdaptPara.ucCarFlowPro = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
	sAdaptPara.ucSmoothPara = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);

	return true;
}


/**************************************************************
Function:       CGbtTscDb::AddAdaptPara
Description:    Ìí¼Ó×ÔÊÊÓ¦²ÎÊý±íÊý¾Ý
Input:         	sAdaptPara -- ×ÔÊÊÓ¦²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddAdaptPara(AdaptPara sAdaptPara)
{
	char   sSql[LARGE_BUF_SIZE] = {0};

	ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s, %s)values(%u,%u,%u,%u,%u,%u,%u)",
							Table_Desc[TBL_ADAPT_PARA].sTblName,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[0],
							Table_Desc[TBL_ADAPT_PARA].sFieldName[1], 
							Table_Desc[TBL_ADAPT_PARA].sFieldName[2],
							Table_Desc[TBL_ADAPT_PARA].sFieldName[3],
							Table_Desc[TBL_ADAPT_PARA].sFieldName[4],
							Table_Desc[TBL_ADAPT_PARA].sFieldName[5],
							Table_Desc[TBL_ADAPT_PARA].sFieldName[6],
							sAdaptPara.ucType,
							sAdaptPara.ucFirstPro,
							sAdaptPara.ucSecPro,
							sAdaptPara.ucThirdPro,
							sAdaptPara.ucOcuPro,
							sAdaptPara.ucCarFlowPro,
							sAdaptPara.ucSmoothPara);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::ModAdaptPara
Description:    ÐÞ¸Ä×ÔÊÊÓ¦²ÎÊý±íÊý¾Ý
Input:         	sAdaptPara -- ´ýÐÞ¸Ä×ÔÊÊÓ¦²ÎÊý½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModAdaptPara(AdaptPara& sAdaptPara)
{
	AdaptPara sTemp;
	sTemp.ucType = sAdaptPara.ucType;
	if ( !QueryAdaptPara(sTemp) )
	{
		return AddAdaptPara(sAdaptPara);
	}

	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "update %s set %s=%u,%s=%u,%s=%u,%s=%u,%s=%u,%s=%u where %s=%u",
							Table_Desc[TBL_ADAPT_PARA].sTblName,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[1], sAdaptPara.ucFirstPro,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[2], sAdaptPara.ucSecPro,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[3], sAdaptPara.ucThirdPro,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[4], sAdaptPara.ucOcuPro,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[5], sAdaptPara.ucCarFlowPro,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[6], sAdaptPara.ucSmoothPara,
							Table_Desc[TBL_ADAPT_PARA].sFieldName[0], sAdaptPara.ucType);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryCntDownDev
Description:    µ¹¼ÆÊ±Éè±¸±íÊý¾Ý²éÑ¯
Input:         	tblCntDownDev -- µ¹¼ÆÊ±Éè±¸±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryCntDownDev(TblCntDownDev& tblCntDownDev)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	CntDownDev*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
							Table_Desc[TBL_CNTDOWNDEV].sTblName,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_CNTDOWNDEV].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CNTDOWNDEV].sTblName, qryRst.m_iColum,Table_Desc[TBL_CNTDOWNDEV].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblCntDownDev.AllocData(uCount))
	{
		return false;
	}

	pData = tblCntDownDev.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ucDevId        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].usPhase        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucOverlapPhase = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucMode         = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
	}
	return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryCntDownDev
Description:    ¸ù¾ÝÉè±¸ºÅ²éÑ¯µ¹¼ÆÊ±Éè±¸±íÊý¾Ý
Input:         	ucDevId -- Éè±¸IDºÅ
				sCntDownDev -- µ¹¼ÆÊ±Éè±¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
							Table_Desc[TBL_CNTDOWNDEV].sTblName,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[0], 
							ucDevId);

	if ( !Query(sSql, qryRst) )
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_CNTDOWNDEV].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CNTDOWNDEV].sTblName, qryRst.m_iColum,Table_Desc[TBL_CNTDOWNDEV].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	sCntDownDev.ucDevId        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sCntDownDev.usPhase        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sCntDownDev.ucOverlapPhase = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sCntDownDev.ucMode         = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);

	return true;
}

/**************************************************************
Function:       CGbtTscDb::AddCntDownDev
Description:    Ìí¼Óµ¹¼ÆÊ±Éè±¸±íÊý¾Ý
Input:         	sCntDownDev -- µ¹¼ÆÊ±Éè±¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddCntDownDev(CntDownDev& sCntDownDev)
{
	char   sSql[LARGE_BUF_SIZE];
	
	ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s)values(%u,%u,%u,%u)",
							Table_Desc[TBL_CNTDOWNDEV].sTblName,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[0],
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[1], 
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[2],
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[3],
							sCntDownDev.ucDevId,
							sCntDownDev.usPhase,
							sCntDownDev.ucOverlapPhase,
							sCntDownDev.ucMode);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::DelCntDownDev
Description:    ¸ù¾ÝÉè±¸ºÅÉ¾³ýµ¹¼ÆÊ±Éè±¸±íÊý¾Ý¼ÇÂ¼
Input:         	ucDevId -- µ¹¼ÆÊ±Éè±¸ºÅ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelCntDownDev(Byte ucDevId)
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
							Table_Desc[TBL_CNTDOWNDEV].sTblName,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[0], 
							ucDevId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModCntDownDev
Description:    ¸ù¾ÝÉè±¸ºÅÐÞ¸Äµ¹¼ÆÊ±Éè±¸±íÊý¾Ý¼ÇÂ¼
Input:         	ucDevId -- µ¹¼ÆÊ±Éè±¸ºÅ
				sCntDownDev -- µ¹¼ÆÊ±Éè±¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModCntDownDev(Byte ucDevId, CntDownDev& sCntDownDev)
{
	CntDownDev sTemp;
	if (!QueryCntDownDev(ucDevId, sTemp))
	{
		return AddCntDownDev(sCntDownDev);
	}

	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u where %s=%u",
							Table_Desc[TBL_CNTDOWNDEV].sTblName,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[1] , sCntDownDev.usPhase,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[2] , sCntDownDev.ucOverlapPhase,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[3] , sCntDownDev.ucMode,
							Table_Desc[TBL_CNTDOWNDEV].sFieldName[0] , sCntDownDev.ucDevId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QuerySpecFun
Description:    ¸ù¾ÝÌØÊâ¹¦ÄÜ½á¹¹Ìå²éÑ¯ÌØÊâ¹¦ÄÜ±í
Input:         	sSpecFun -- ÌØÊâ¹¦ÄÜ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QuerySpecFun(SpecFun& sSpecFun)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
							Table_Desc[TBL_SPECFUN].sTblName,
							Table_Desc[TBL_SPECFUN].sFieldName[0], 
							sSpecFun.ucFunType);

	if ( !Query(sSql, qryRst) )
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if ( qryRst.m_iColum != Table_Desc[TBL_SPECFUN].iFieldCount )
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SPECFUN].sTblName, qryRst.m_iColum,Table_Desc[TBL_SPECFUN].iFieldCount));
		return false;
	}

	if ( qryRst.m_iRow <= 0 )
	{
		return false;
	}

	sSpecFun.ucFunType = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sSpecFun.ucValue   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QuerySpecFun
Description:    ²éÑ¯È«²¿ÌØÊâ¹¦ÄÜ±íÊý¾Ý
Input:         	tblSpecFun -- ÌØÊâ¹¦ÄÜ±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QuerySpecFun(TblSpecFun& tblSpecFun)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	SpecFun*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
							Table_Desc[TBL_SPECFUN].sTblName,
							Table_Desc[TBL_SPECFUN].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_SPECFUN].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SPECFUN].sTblName, qryRst.m_iColum,Table_Desc[TBL_SPECFUN].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblSpecFun.AllocData(uCount))
	{
		return false;
	}

	pData = tblSpecFun.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ucFunType = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucValue   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::AddSpecFun
Description:    Ìí¼ÓÌØÊâ¹¦ÄÜ±íÊý¾Ý
Input:         	tblSpecFun -- ÌØÊâ¹¦ÄÜ±í¶ÔÏó
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddSpecFun(Byte ucFunType,Byte ucValue)
{
	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "insert into %s(%s, %s)values(%u,%u)",
							Table_Desc[TBL_SPECFUN].sTblName,
							Table_Desc[TBL_SPECFUN].sFieldName[0],
							Table_Desc[TBL_SPECFUN].sFieldName[1],
							ucFunType,
							ucValue);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModSpecFun
Description:    ¸ù¾ÝÌØÊâ¹¦ÄÜÀàÐÍÐÞ¸ÄÌØÊâ¹¦ÄÜ±íÊý¾Ý
Input:         	ucFunType -- ÌØÊâ¹¦ÄÜÀàÐÍ
				ucValue   -- ÐÞ¸ÄÖµ
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModSpecFun(Byte ucFunType,Byte ucValue)
{
	SpecFun sTemp;
	sTemp.ucFunType = ucFunType;
	if ( !CGbtTscDb::QuerySpecFun(sTemp) )
	{
		return AddSpecFun(ucFunType,ucValue);
	}

	char   sSql[LARGE_BUF_SIZE] = {0};

	ACE_OS::sprintf(sSql, "update %s set %s=%u where %s=%u",
							Table_Desc[TBL_SPECFUN].sTblName,
							Table_Desc[TBL_SPECFUN].sFieldName[1] , 
							ucValue,
							Table_Desc[TBL_SPECFUN].sFieldName[0] , 
							ucFunType);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryChannel
Description:    ÐÅºÅ»úÍ¨µÀ²éÑ¯
Input:         	tblChannel -- Í¨µÀÊý¾Ý±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryChannel(TblChannel& tblChannel)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Channel*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_CHANNEL].sTblName,
						    Table_Desc[TBL_CHANNEL].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_CHANNEL].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CHANNEL].sTblName, qryRst.m_iColum,Table_Desc[TBL_CHANNEL].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblChannel.AllocData(uCount))
    {
        return false;
    }

    pData = tblChannel.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucChannelId        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucCtrlSrc          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucAutoFlsCtrlFlag  = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucCtrlType         = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryChannel
Description:    ¸ù¾ÝÍ¨µÀIDHºÅÐÅºÅ»úÍ¨µÀ²éÑ¯
Input:         	uChannelId -- Í¨µÀIDºÅ
				sChannel   -- Í¨µÀ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryChannel(Byte uChannelId, Channel& sChannel)
{
     char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
						    Table_Desc[TBL_CHANNEL].sTblName,
						    Table_Desc[TBL_CHANNEL].sFieldName[0], 
						    uChannelId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_CHANNEL].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CHANNEL].sTblName, qryRst.m_iColum,Table_Desc[TBL_CHANNEL].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sChannel.ucChannelId    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sChannel.ucCtrlSrc      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sChannel.ucAutoFlsCtrlFlag = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sChannel.ucCtrlType     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddChannel
Description:    Ìí¼ÓÐÅºÅ»úÍ¨µÀ±í¼ÇÂ¼Êý¾Ý
Input:         	uChannelId -- Í¨µÀIDºÅ
				sChannel   -- Í¨µÀ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddChannel(Byte uChannelId, Channel& sChannel)
{
    char   sSql[LARGE_BUF_SIZE];
    if (!IsChannelValid(uChannelId, sChannel))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tChannel invalid\n", SHORT_FILE, __LINE__));
        return true;
    }
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s)values(%u,%u,%u,%u)",
				           Table_Desc[TBL_CHANNEL].sTblName,
				           Table_Desc[TBL_CHANNEL].sFieldName[0],
				           Table_Desc[TBL_CHANNEL].sFieldName[1], 
				           Table_Desc[TBL_CHANNEL].sFieldName[2],
				           Table_Desc[TBL_CHANNEL].sFieldName[3],
				           uChannelId,
				           sChannel.ucCtrlSrc,
				           sChannel.ucAutoFlsCtrlFlag,
				           sChannel.ucCtrlType);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::ModChannel
Description:    ÐÞ¸ÄÐÅºÅ»úÍ¨µÀ±í¼ÇÂ¼Êý¾Ý
Input:         	uChannelId -- Í¨µÀIDºÅ
				sChannel   -- ´ýÐÞ¸ÄÍ¨µÀ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModChannel(Byte uChannelId, Channel& sChannel)
{
    Channel sTemp;
    if (!QueryChannel(uChannelId, sTemp))
    {
        return AddChannel(uChannelId, sChannel);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsChannelValid(uChannelId, sChannel))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tChannel invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u where %s=%u",
				           Table_Desc[TBL_CHANNEL].sTblName,
				           Table_Desc[TBL_CHANNEL].sFieldName[1], sChannel.ucCtrlSrc,
				           Table_Desc[TBL_CHANNEL].sFieldName[2], sChannel.ucAutoFlsCtrlFlag,
				           Table_Desc[TBL_CHANNEL].sFieldName[3], sChannel.ucCtrlType,
				           Table_Desc[TBL_CHANNEL].sFieldName[0], uChannelId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelChannel
Description:    ¸ù¾ÝÍ¨µÀIDºÅÉ¾³ýÐÅºÅ»úÍ¨µÀ±í¼ÇÂ¼Êý¾Ý
Input:         	uChannelId -- Í¨µÀIDºÅ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelChannel(Byte uChannelId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_CHANNEL].sTblName,
				           Table_Desc[TBL_CHANNEL].sFieldName[0], 
				           uChannelId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelChannel
Description:    É¾³ýÈ«²¿ÐÅºÅ»úÍ¨µÀ±í¼ÇÂ¼Êý¾Ý
Input:         	ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelChannel()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_CHANNEL].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::IsChannelValid
Description:    ÅÐ¶ÏÐÅºÅ¼°Í¨µÀÊý¾ÝºÏ·¨ÐÔ
Input:         	uChannelId -- Í¨µÀIDºÅ
				sChannel   -- ´ýÐÞ¸ÄÍ¨µÀ½á¹¹ÌåÊý¾Ý
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨ false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsChannelValid(Byte uChannelId, Channel& sChannel)
{
    CHECK_ID(CONST_CHANNEL, uChannelId, false);

    if (sChannel.ucCtrlSrc > MAX_CHANNEL||sChannel.ucCtrlType > 0x4)
    {
     //   ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucCtrlSrc=%d,invalid\n", SHORT_FILE, __LINE__, sChannel.ucCtrlSrc));
        return false;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPattern
Description:    ²éÑ¯ÅäÊ±·½°¸
Input:          tblPattern  -- ÅäÊ±·½°¸±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryPattern(TblPattern& tblPattern)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    Pattern*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_PATTERN].sTblName,
						    Table_Desc[TBL_PATTERN].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PATTERN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PATTERN].sTblName, qryRst.m_iColum,Table_Desc[TBL_PATTERN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblPattern.AllocData(uCount))
    {
        return false;
    }

    pData = tblPattern.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucPatternId        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucCycleTime        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ucOffset           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        pData[i].ucCoorPhase        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucStagePatternId   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPattern
Description:    ¸ù¾ÝÅäÊ±·½°¸ºÅ²éÑ¯ÅäÊ±·½°¸±íÊý¾Ý
Input:          uPatternId  -- ÅäÊ±·½°¸ºÅ
				sPattern    -- ÅäÊ±·½°¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryPattern(Byte uPatternId, Pattern& sPattern)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%d", 
						    Table_Desc[TBL_PATTERN].sTblName,
						    Table_Desc[TBL_PATTERN].sFieldName[0], uPatternId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_PATTERN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PATTERN].sTblName, qryRst.m_iColum,Table_Desc[TBL_PATTERN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sPattern.ucPatternId        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sPattern.ucCycleTime        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sPattern.ucOffset           = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    sPattern.ucCoorPhase        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    sPattern.ucStagePatternId   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddPattern
Description:    ¸ù¾ÝÅäÊ±·½°¸ºÅÌí¼ÓÅäÊ±·½°¸±íÊý¾Ý
Input:          uPatternId  -- ÅäÊ±·½°¸ºÅ
				sPattern    -- ÅäÊ±·½°¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddPattern(Byte uPatternId, Pattern& sPattern)
{
    char   sSql[LARGE_BUF_SIZE];
    if (!IsPatternValid(uPatternId, sPattern))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tPattern invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s)values(%u,%u,%u,%u,%u)",
				           Table_Desc[TBL_PATTERN].sTblName,
				           Table_Desc[TBL_PATTERN].sFieldName[0],
				           Table_Desc[TBL_PATTERN].sFieldName[1], 
				           Table_Desc[TBL_PATTERN].sFieldName[2],
				           Table_Desc[TBL_PATTERN].sFieldName[3],
				           Table_Desc[TBL_PATTERN].sFieldName[4],
				           uPatternId,
				           sPattern.ucCycleTime,
				           sPattern.ucOffset,
				           sPattern.ucCoorPhase,
				           sPattern.ucStagePatternId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::ModPattern
Description:    ¸ù¾ÝÅäÊ±·½°¸ºÅÐÞ¸ÄÅäÊ±·½°¸±íÊý¾Ý
Input:          uPatternId  -- ÅäÊ±·½°¸ºÅ
				sPattern    -- ÅäÊ±·½°¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModPattern(Byte uPatternId, Pattern& sPattern)
{
    Pattern sTemp;
    if (!QueryPattern(uPatternId, sTemp))
    {
        return AddPattern(uPatternId, sPattern);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsPatternValid(uPatternId, sPattern))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tPattern invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u, %s=%u where %s=%u",
				           Table_Desc[TBL_PATTERN].sTblName,
				           Table_Desc[TBL_PATTERN].sFieldName[1], sPattern.ucCycleTime,
				           Table_Desc[TBL_PATTERN].sFieldName[2], sPattern.ucOffset,
				           Table_Desc[TBL_PATTERN].sFieldName[3], sPattern.ucCoorPhase,
				           Table_Desc[TBL_PATTERN].sFieldName[4], sPattern.ucStagePatternId,
				           Table_Desc[TBL_PATTERN].sFieldName[0], uPatternId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelPattern
Description:    ¸ù¾ÝÅäÊ±·½°¸ºÅÉ¾³ýÅäÊ±·½°¸±íÊý¾Ý
Input:          uPatternId  -- ÅäÊ±·½°¸ºÅ
				sPattern    -- ÅäÊ±·½°¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPattern(Byte uPatternId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
                   Table_Desc[TBL_PATTERN].sTblName,
                   Table_Desc[TBL_PATTERN].sFieldName[0], uPatternId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelPattern
Description:    É¾³ýÈ«²¿ÅäÊ±·½°¸±íÊý¾Ý
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPattern()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_PATTERN].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::IsPatternValid
Description:    ÅÐ¶ÏÅäÊ±·½°¸Êý¾ÝºÏ·¨ÐÔ
Input:          uPatternId  -- ÅäÊ±·½°¸ºÅ
				sPattern    -- ÅäÊ±·½°¸½á¹¹Ìå
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨ false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsPatternValid(Byte uPatternId, Pattern& sPattern)
{
    CHECK_ID(CONST_PATTERN, uPatternId, false);

    if (sPattern.ucCoorPhase > 16)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucCoorPhase=%d,invalid\n", SHORT_FILE, __LINE__, sPattern.ucCoorPhase));
        return false;
    }
    if (sPattern.ucStagePatternId > 16)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucStagePatternId=%d,invalid\n", SHORT_FILE, __LINE__, sPattern.ucStagePatternId));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryStagePattern
Description:    ²éÑ¯½×¶ÎÅäÊ±±íÊý¾Ý
Input:          tblStagePattern  -- ½×¶ÎÅäÊ±±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryStagePattern(TblStagePattern& tblStagePattern)
{
    int    i;
    Ushort  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    StagePattern*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s, %s", 
						    					Table_Desc[TBL_STAGEPATTERN].sTblName,
						   				         Table_Desc[TBL_STAGEPATTERN].sFieldName[0],
						 				         Table_Desc[TBL_STAGEPATTERN].sFieldName[1]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }
    //ACE_DEBUG ((LM_DEBUG, "%s:%d Get StagePatterns count =%d \n", __FILE__, __LINE__,uCount));   

    if (qryRst.m_iColum != Table_Desc[TBL_STAGEPATTERN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_STAGEPATTERN].sTblName, qryRst.m_iColum,Table_Desc[TBL_STAGEPATTERN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Ushort)(qryRst.m_iRow);   //Mod:20151224 Byte->Ushort
    
    if (!tblStagePattern.AllocData(uCount))
    {
        return false;
    }
  
  //ACE_DEBUG ((LM_DEBUG, "%s:%d Get StagePatterns count =%d \n", __FILE__, __LINE__,uCount));   
    pData = tblStagePattern.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucStagePatternId   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucStageNo          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);		
       pData[i].usAllowPhase       = ACE_OS::strtoll(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2],NULL,10);		
        pData[i].ucGreenTime        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucYellowTime       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucRedTime          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].ucOption           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryStagePattern
Description:    ¸ù¾Ý½×¶ÎÅäÊ±ºÅ²éÑ¯½×¶ÎÅäÊ±±íÊý¾Ý
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				tblStagePattern  -- ½×¶ÎÅäÊ±±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryStagePattern(Byte uStagePatternId, TblStagePattern& tblStagePattern)
{
    int    i;
    Ushort  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    StagePattern*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u order by %s", 
						    Table_Desc[TBL_STAGEPATTERN].sTblName,
						    Table_Desc[TBL_STAGEPATTERN].sFieldName[0], 
						    uStagePatternId,
						    Table_Desc[TBL_STAGEPATTERN].sFieldName[1]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_STAGEPATTERN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_STAGEPATTERN].sTblName, qryRst.m_iColum,Table_Desc[TBL_STAGEPATTERN].iFieldCount));
        return false;
    }

    uCount = (Ushort)qryRst.m_iRow;  //Mod:20151224 Byte->Ushort
    if (!tblStagePattern.AllocData(uCount))
    {
        return false;
    }

    pData = tblStagePattern.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucStagePatternId   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucStageNo          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].usAllowPhase		= ACE_OS::strtoll(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2],NULL,10);
        pData[i].ucGreenTime        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
        pData[i].ucYellowTime       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        pData[i].ucRedTime          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5]);
        pData[i].ucOption           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryStagePattern
Description:    ¸ù¾Ý½×¶ÎÅäÊ±±íºÅ£¬½×¶ÎºÅ²éÑ¯½×¶ÎÅäÊ±±íÊý¾Ý
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				uStageNo         -- ½×¶ÎºÅ
				sStagePattern    -- ½×¶ÎÅäÊ±½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStagePattern)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u and %s=%u", 
						    Table_Desc[TBL_STAGEPATTERN].sTblName,
						    Table_Desc[TBL_STAGEPATTERN].sFieldName[0], 
						    uStagePatternId,
						    Table_Desc[TBL_STAGEPATTERN].sFieldName[1], 
						    uStageNo);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_STAGEPATTERN].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_STAGEPATTERN].sTblName, qryRst.m_iColum,Table_Desc[TBL_STAGEPATTERN].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
 
    sStagePattern.ucStagePatternId   		 = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sStagePattern.ucStageNo          		 = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sStagePattern.usAllowPhase			 = ACE_OS::strtoll(qryRst.m_ppResult[qryRst.m_iColum + 2],NULL,10);
    sStagePattern.ucGreenTime     	          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
    sStagePattern.ucYellowTime    	          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    sStagePattern.ucRedTime        	          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 5]);
    sStagePattern.ucOption        		          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);
   
    return true;
}

/**************************************************************
Function:       CGbtTscDb::AddStagePattern
Description:    Ìí¼Ó½×¶ÎÅäÊ±±íÊý¾Ý¼ÇÂ¼
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				uStageNo         -- ½×¶ÎºÅ
				sStagePattern    -- ½×¶ÎÅäÊ±½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStagePattern)
{
    char   sSql[LARGE_BUF_SIZE];
        /*
	if ( 0 == uStageNo )  //½×¶ÎºÅ²»ÄÜÎª0 IsStagePatternValidÒÑ¾­ÓÐÑéÖ¤
	{
		return true;
	}
      */
     
    if (!IsStagePatternValid(uStagePatternId, uStageNo, sStagePattern))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t StagePatternId=%d StageNo=%d   invalid\n", __FILE__, __LINE__,uStagePatternId, uStageNo));
        return true;  //Ö±½ÓºöÂÔÕâÌõ´¦Àí
    }   
   
 //  ACE_OS::printf("%s:%d Add StagePatternId=%d StageNo=%d !\r\n",__FILE__,__LINE__,uStagePatternId,uStageNo);
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s, %s)values(%u,%u,%u,%u,%u,%u,%u)",
				           Table_Desc[TBL_STAGEPATTERN].sTblName,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[0],
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[1], 
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[2],
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[3],
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[4],
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[5],
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[6],
				           uStagePatternId,
				           uStageNo,
				           sStagePattern.usAllowPhase,
				           sStagePattern.ucGreenTime,
				           sStagePattern.ucYellowTime,
				           sStagePattern.ucRedTime,
				           sStagePattern.ucOption);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::ModStagePattern
Description:    ¸ù¾Ý½×¶ÎÅäÊ±ºÅ£¬½×¶ÎºÅÐÞ¸ÄÅäÊ±±íÊý¾Ý¼ÇÂ¼
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				uStageNo         -- ½×¶ÎºÅ
				sStagePattern    -- ½×¶ÎÅäÊ±½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModStagePattern(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage)
{
    StagePattern sTemp;
    if (!QueryStagePattern(uStagePatternId, uStageNo, sTemp))
    {
        return AddStagePattern(uStagePatternId, uStageNo,sStage);
    }

    char   sSql[LARGE_BUF_SIZE];
    if (!IsStagePatternValid(uStagePatternId, uStageNo, sStage))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tStagePattern invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s=%u, %s=%u, %s=%u where %s=%u and %s=%u",
				           Table_Desc[TBL_STAGEPATTERN].sTblName,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[2], sStage.usAllowPhase,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[3], sStage.ucGreenTime,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[4], sStage.ucYellowTime,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[5], sStage.ucRedTime,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[6], sStage.ucOption,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[0], uStagePatternId,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[1], uStageNo);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelStagePattern
Description:    ¸ù¾Ý½×¶ÎÅäÊ±ºÅÉ¾³ýÅäÊ±±íÊý¾Ý¼ÇÂ¼
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelStagePattern(Byte uStagePatternId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_STAGEPATTERN].sTblName,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[0], 
				           uStagePatternId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelStagePattern
Description:    ¸ù¾Ý½×¶ÎÅäÊ±ºÅ£¬½×¶ÎºÅÉ¾³ýÅäÊ±±íÊý¾Ý¼ÇÂ¼
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				uStageNo         -- ½×¶ÎºÅ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelStagePattern(Byte uStagePatternId, Byte uStageNo)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u and %s=%u",
				           Table_Desc[TBL_STAGEPATTERN].sTblName,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[0], 
				           uStagePatternId,
				           Table_Desc[TBL_STAGEPATTERN].sFieldName[1], 
				           uStageNo);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelStagePattern
Description:    É¾³ýÈ«²¿ÅäÊ±±íÊý¾Ý¼ÇÂ¼
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelStagePattern()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_STAGEPATTERN].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::IsStagePatternValid
Description:    ÅÐ¶Ï½×¶ÎÅäÊ±Êý¾ÝºÏ·¨ÐÔ
Input:          uStagePatternId  -- ½×¶ÎÅäÊ±ºÅ
				uStageNo         -- ½×¶ÎºÅ
				sStagePattern    -- ½×¶ÎÅäÊ±½á¹¹Ìå
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨ false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsStagePatternValid(Byte uStagePatternId, Byte uStageNo, StagePattern& sStage)
{
    if (uStagePatternId == 17)
		return true ;
    CHECK_ID(CONST_STAGEPATTERN, uStagePatternId, false);
    CHECK_ID(CONST_STAGE, uStageNo, false);

    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryOverlapPhase
Description:    ²éÑ¯¸úËæÏàÎ»
Input:         tblOverlapPhase -- ¸úËæÏàÎ»±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryOverlapPhase(TblOverlapPhase& tblOverlapPhase)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    OverlapPhase*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_OVERLAPPHASE].sTblName,
						    Table_Desc[TBL_OVERLAPPHASE].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_OVERLAPPHASE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_OVERLAPPHASE].sTblName, qryRst.m_iColum,Table_Desc[TBL_OVERLAPPHASE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblOverlapPhase.AllocData(uCount))
    {
        return false;
    }

    pData = tblOverlapPhase.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucOverlapPhaseId    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucOperateType      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);

        pData[i].ucIncldPhaseCnt    = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
        GetFieldsValue(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3], ',', pData[i].ucIncldPhase, MAX_PHASE_COUNT);

        pData[i].ucAdjustPhaseCnt   = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4]);
        GetFieldsValue(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 5], ',', pData[i].ucAdjustPhase, MAX_PHASE_COUNT);

        pData[i].ucTailGreen        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 6]);
        pData[i].ucTailYellow       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 7]);
        pData[i].ucTailRed          = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 8]); 
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryOverlapPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅ²éÑ¯¸úËæÏàÎ»±íÊý¾Ý
Input:         uOverlapPhaseId -- ¸úËæÏàÎ»ID
				sOverlapPhase -- ¸úËæÏàÎ»½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
						    Table_Desc[TBL_OVERLAPPHASE].sTblName,
						    Table_Desc[TBL_OVERLAPPHASE].sFieldName[0], 
						    uOverlapPhaseId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_OVERLAPPHASE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_OVERLAPPHASE].sTblName, qryRst.m_iColum,Table_Desc[TBL_OVERLAPPHASE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sOverlapPhase.ucOverlapPhaseId    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sOverlapPhase.ucOperateType      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);

    sOverlapPhase.ucIncldPhaseCnt    = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
    GetFieldsValue(qryRst.m_ppResult[qryRst.m_iColum + 3], ',', sOverlapPhase.ucIncldPhase, MAX_PHASE_COUNT);

    sOverlapPhase.ucAdjustPhaseCnt   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 4]);
    GetFieldsValue(qryRst.m_ppResult[qryRst.m_iColum + 5], ',', sOverlapPhase.ucAdjustPhase, MAX_PHASE_COUNT);

    sOverlapPhase.ucTailGreen        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 6]);
    sOverlapPhase.ucTailYellow       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 7]);
    sOverlapPhase.ucTailRed          = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 8]); 
   
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddOverlapPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅÌí¼Ó¸úËæÏàÎ»±íÊý¾Ý
Input:          uOverlapPhaseId -- ¸úËæÏàÎ»ID
				sOverlapPhase -- ¸úËæÏàÎ»½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase)
{
    Byte  i;
    char   sIncldPhase[SMALL_BUF_SIZE];
    char   sAdjustPhase[SMALL_BUF_SIZE];
    char   sSql[LARGE_BUF_SIZE];

    if(!IsOverlapPhaseValid(uOverlapPhaseId, sOverlapPhase))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tOverlapPhase invalid\n", SHORT_FILE, __LINE__));
        return true;
    }

    if (sOverlapPhase.ucIncldPhaseCnt > MAX_PHASE_COUNT || sOverlapPhase.ucAdjustPhaseCnt > MAX_PHASE_COUNT)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s sOverlapPhase.ucIncldPhaseCnt = %d, sOverlapPhase.ucAdjustPhaseCnt = %d\n", SHORT_FILE, __LINE__, sOverlapPhase.ucIncldPhaseCnt, sOverlapPhase.ucAdjustPhaseCnt));
        return true;
    }

    ACE_OS::memset(sIncldPhase, 0, SMALL_BUF_SIZE);
    for (i = 0; i < MAX_PHASE_COUNT; i++)
    {
        ACE_OS::sprintf(sSql, "%d,", sOverlapPhase.ucIncldPhase[i]);
        ACE_OS::strcat(sIncldPhase, sSql);
    }

    ACE_OS::memset(sAdjustPhase, 0, SMALL_BUF_SIZE);
    for (i = 0; i < MAX_PHASE_COUNT; i++)
    {
        ACE_OS::sprintf(sSql, "%d,", sOverlapPhase.ucAdjustPhase[i]);
        ACE_OS::strcat(sAdjustPhase, sSql);
    }
    
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s, %s, %s, %s, %s, %s, %s)values(%u,%u,%u,'%s',%u,'%s',%u,%u,%u)",
				           Table_Desc[TBL_OVERLAPPHASE].sTblName,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[0],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[1], 
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[2],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[3],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[4],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[5],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[6],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[7],
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[8],
				           uOverlapPhaseId,
				           sOverlapPhase.ucOperateType,
				           sOverlapPhase.ucIncldPhaseCnt,
				           sIncldPhase,
				           sOverlapPhase.ucAdjustPhaseCnt,
				           sAdjustPhase,
				           sOverlapPhase.ucTailGreen,
				           sOverlapPhase.ucTailYellow,
				           sOverlapPhase.ucTailRed);


    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::ModOverlapPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅÐÞ¸Ä¸úËæÏàÎ»±íÊý¾Ý
Input:          uOverlapPhaseId -- ¸úËæÏàÎ»ID
				sOverlapPhase -- ¸úËæÏàÎ»½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModOverlapPhase(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase)
{
    OverlapPhase sTemp;
    if (!QueryOverlapPhase(uOverlapPhaseId, sTemp))
    {
        return AddOverlapPhase(uOverlapPhaseId, sOverlapPhase);
    }

    Byte  i;
    char   sIncldPhase[SMALL_BUF_SIZE];
    char   sAdjustPhase[SMALL_BUF_SIZE];
    char   sSql[LARGE_BUF_SIZE];

    if(!IsOverlapPhaseValid(uOverlapPhaseId, sOverlapPhase))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tOverlapPhase invalid\n", SHORT_FILE, __LINE__));
        return false;
    }
    if (sOverlapPhase.ucIncldPhaseCnt > MAX_PHASE_COUNT  || sOverlapPhase.ucAdjustPhaseCnt > MAX_PHASE_COUNT)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s sOverlapPhase.ucIncldPhaseCnt = %d, sOverlapPhase.ucAdjustPhaseCnt = %d\n", SHORT_FILE, __LINE__, sOverlapPhase.ucIncldPhaseCnt, sOverlapPhase.ucAdjustPhaseCnt));
        return false;
    }
    ACE_OS::memset(sIncldPhase, 0, SMALL_BUF_SIZE);
    for (i = 0; i < MAX_PHASE_COUNT; i++)
    {
        ACE_OS::sprintf(sSql, "%d,", sOverlapPhase.ucIncldPhase[i]);
        ACE_OS::strcat(sIncldPhase, sSql);
    }
    ACE_OS::memset(sAdjustPhase, 0, SMALL_BUF_SIZE);
    for (i = 0; i < MAX_PHASE_COUNT; i++)
    {
        ACE_OS::sprintf(sSql, "%d,", sOverlapPhase.ucAdjustPhase[i]);
        ACE_OS::strcat(sAdjustPhase, sSql);
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u, %s='%s', %s=%u, %s='%s', %s=%u, %s=%u, %s=%u where %s=%u",
				           Table_Desc[TBL_OVERLAPPHASE].sTblName,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[1], sOverlapPhase.ucOperateType,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[2], sOverlapPhase.ucIncldPhaseCnt,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[3], sIncldPhase,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[4], sOverlapPhase.ucAdjustPhaseCnt,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[5], sAdjustPhase,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[6], sOverlapPhase.ucTailGreen,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[7], sOverlapPhase.ucTailYellow,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[8], sOverlapPhase.ucTailRed,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[0], uOverlapPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelOverlapPhase
Description:    ¸ù¾ÝÏàÎ»IDºÅÉ¾³ý¸úËæÏàÎ»±íÊý¾Ý
Input:          uOverlapPhaseId -- ¸úËæÏàÎ»ID
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelOverlapPhase(Byte uOverlapPhaseId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_OVERLAPPHASE].sTblName,
				           Table_Desc[TBL_OVERLAPPHASE].sFieldName[0], uOverlapPhaseId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelOverlapPhase
Description:    É¾³ýÈ«²¿¸úËæÏàÎ»±íÊý¾Ý
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelOverlapPhase()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",	Table_Desc[TBL_OVERLAPPHASE].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}


/**************************************************************
Function:       CGbtTscDb::IsOverlapPhaseValid
Description:   ÅÐ¶Ï¸úËæÏàÎ»±íÊý¾ÝºÏ·¨ÐÔ
Input:          uOverlapPhaseId -- ¸úËæÏàÎ»ID
				sOverlapPhase -- ¸úËæÏàÎ»½á¹¹Ìå
Output:         ÎÞ
Return:         true - Êý¾ÝºÏ·¨ false -Êý¾Ý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsOverlapPhaseValid(Byte uOverlapPhaseId, OverlapPhase& sOverlapPhase)
{
    CHECK_ID(CONST_OVAERLAPPHASE, uOverlapPhaseId, false);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryEventType
Description:   ²éÑ¯ÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          tblEvtType -- Êý¾ÝÀàÐÍ±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryEventType(TblEventType& tblEvtType)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    TblEventLog tblLog;
    EventType*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s order by %s", 
						    Table_Desc[TBL_EVENTTYPE].sTblName,
						    Table_Desc[TBL_EVENTTYPE].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }


    if (qryRst.m_iColum != Table_Desc[TBL_EVENTTYPE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTTYPE].sTblName, qryRst.m_iColum,Table_Desc[TBL_EVENTTYPE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblEvtType.AllocData(uCount))
    {
        return false;
    }

    pData = tblEvtType.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucEvtTypeId        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ulClearTime        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].strEvtDesc.SetString(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);

        pData[i].ucLogCount = 0;
        if (QueryEventLog(pData[i].ucEvtTypeId, tblLog))
        {
            pData[i].ucLogCount = tblLog.GetCount();
        }
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryEventType
Description:   ¸ù¾ÝÈÕÖ¾ÀàÐÍID²éÑ¯ÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          uEvtTypeId -- ÈÕÖ¾ÀàÐÍID
				sEvtType   -- ÈÕÖ¾ÀàÐÍ½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryEventType(Byte uEvtTypeId, EventType& sEvtType)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    TblEventLog tblLog;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
						    Table_Desc[TBL_EVENTTYPE].sTblName,
						    Table_Desc[TBL_EVENTTYPE].sFieldName[0], 
						    uEvtTypeId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_EVENTTYPE].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTTYPE].sTblName, qryRst.m_iColum,Table_Desc[TBL_EVENTTYPE].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    sEvtType.ucEvtTypeId   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sEvtType.ulClearTime   = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sEvtType.strEvtDesc.SetString(qryRst.m_ppResult[qryRst.m_iColum + 2]);

    sEvtType.ucLogCount = 0;
    if (QueryEventLog(sEvtType.ucEvtTypeId, tblLog))
    {
        sEvtType.ucLogCount = tblLog.GetCount();
    }
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::AddEventType
Description:   Ìí¼ÓÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          uEvtTypeId -- ÈÕÖ¾ÀàÐÍID
				sEvtType   -- ÈÕÖ¾ÀàÐÍ½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼Ó³É¹¦ false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddEventType(Byte uEvtTypeId, EventType& sEvtType)
{
    Ushort uCount;
    char   sSql[LARGE_BUF_SIZE];
    CHECK_ID(CONST_EVENTTYPE, uEvtTypeId, false);
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s)values(%u, %lu,'%s')",
				           Table_Desc[TBL_EVENTTYPE].sTblName,
				           Table_Desc[TBL_EVENTTYPE].sFieldName[0],
				           Table_Desc[TBL_EVENTTYPE].sFieldName[1], 
				           Table_Desc[TBL_EVENTTYPE].sFieldName[2],
				           uEvtTypeId,
				           sEvtType.ulClearTime,
				           sEvtType.strEvtDesc.GetData(uCount));

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::ModEventType
Description:    ¸ù¾ÝÈÕÖ¾ÀàÐÍIDºÅÐÞ¸ÄÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          uEvtTypeId -- ÈÕÖ¾ÀàÐÍID
				sEvtType   -- ÈÕÖ¾ÀàÐÍ½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼Ó³É¹¦ false -Ìí¼ÓÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModEventType(Byte uEvtTypeId, EventType& sEvtType)
{
    EventType sTemp;
    if (!QueryEventType(uEvtTypeId, sTemp))
    {
        return AddEventType(uEvtTypeId, sEvtType);
    }

    char   sSql[LARGE_BUF_SIZE];
    CHECK_ID(CONST_EVENTTYPE, uEvtTypeId, false);
    ACE_OS::sprintf(sSql, "update %s set %s=%lu, %s='%s' where %s=%u",
				           Table_Desc[TBL_EVENTTYPE].sTblName,
				           Table_Desc[TBL_EVENTTYPE].sFieldName[1], sEvtType.ulClearTime,
				           Table_Desc[TBL_EVENTTYPE].sFieldName[2], sEvtType.strEvtDesc.GetData(),
				           Table_Desc[TBL_EVENTTYPE].sFieldName[0], uEvtTypeId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (0 == sEvtType.ucLogCount)
    {
        DelEventLog(sEvtType.ucEvtTypeId);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelEventType
Description:    ¸ù¾ÝÈÕÖ¾ÀàÐÍIDºÅÉ¾³ýÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          uEvtTypeId -- ÈÕÖ¾ÀàÐÍID
Output:         ÎÞ
Return:         true - É¾³ý¼ÇÂ¼³É¹¦ false -É¾³ý¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::DelEventType(Byte uEvtTypeId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_EVENTTYPE].sTblName,
				           Table_Desc[TBL_EVENTTYPE].sFieldName[0], 
				           uEvtTypeId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelEventType
Description:    É¾³ýÈ«²¿ÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ý¼ÇÂ¼³É¹¦ false -É¾³ý¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::DelEventType()
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s", Table_Desc[TBL_EVENTTYPE].sTblName);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelEventType
Description:    É¾³ýÈ«²¿ÈÕÖ¾ÀàÐÍ±í¼ÇÂ¼
Input:          uEvtTypeId  - ÊÂ¼þÀàÐÍ
				uStartTime  - ¿ªÊ¼Ê±¼ä
				uEndTime    - ½áÊøÊ±¼ä
Output:         ÎÞ
Return:         true - É¾³ý¼ÇÂ¼³É¹¦ false -É¾³ý¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::DelEventLog(Byte uEvtTypeId ,Uint uStartTime ,Uint uEndTime)
{
	char   sSql[LARGE_BUF_SIZE] = {0};
	if(uStartTime >uEndTime)		
		return false ;
	else if(uStartTime == 0xFFFFFFFF && uEndTime == 0xFFFFFFFF && uEvtTypeId == 0xFF)
		ACE_OS::sprintf(sSql, "delete from %s  ", Table_Desc[TBL_EVENTLOG].sTblName);
	else if(uStartTime == 0xFFFFFFFF && uEndTime == 0xFFFFFFFF && uEvtTypeId != 0xFF)
		ACE_OS::sprintf(sSql, "delete from %s where %s = %u ", 
								Table_Desc[TBL_EVENTLOG].sTblName,
								Table_Desc[TBL_EVENTLOG].sFieldName[1],
								uEvtTypeId);
	else 
    	ACE_OS::sprintf(sSql, "delete from %s where %s = %u  and (%s >= %u and %s <= %u)",
    							Table_Desc[TBL_EVENTLOG].sTblName,
    							Table_Desc[TBL_EVENTLOG].sFieldName[1],
    							uEvtTypeId ,
    							Table_Desc[TBL_EVENTLOG].sFieldName[2],
    							uStartTime,
    							Table_Desc[TBL_EVENTLOG].sFieldName[2],
    							uEndTime);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
	return true ;

}


/**************************************************************
Function:       CGbtTscDb::QueryEventLog
Description:    ÊÂ¼þÈÕÖ¾±í¼ÇÂ¼²éÑ¯
Input:          tblLog -- ÈÕÖ¾¼ÇÂ¼±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯¼ÇÂ¼³É¹¦ false -²éÑ¯¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryEventLog(TblEventLog& tblLog)
{
    unsigned int i;
	Uint   uiCount;
    char*  pStop;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    EventLog*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s ", 
						    Table_Desc[TBL_EVENTLOG].sTblName
							
						    );

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_EVENTLOG].iFieldCount)
    {
        //ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTLOG].sTblName, qryRst.m_iColum,Table_Desc[TBL_EVENTLOG].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uiCount = (unsigned int)qryRst.m_iRow;
    if (!tblLog.AllocData(uiCount))
    {
        return false;
    }

    pData = tblLog.GetData(uiCount);
    for (i = 0; i < uiCount; i++)
    {
        pData[i].ucEventId       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucEvtType       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ulHappenTime    = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2], &pStop, 10);
        pData[i].ulEvtValue      = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3], &pStop, 10);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryEventLog
Description:    ¸ù¾ÝÊÂ¼þÀàÐÍ²éÑ¯ÊÂ¼þÈÕÖ¾±í¼ÇÂ¼
Input:          uEvtTypeId -- ÈÕÖ¾ÀàÐÍIDºÅ
				tblLog     -- ÈÕÖ¾±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯¼ÇÂ¼³É¹¦ false -²éÑ¯¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryEventLog(Byte uEvtTypeId, TblEventLog& tblLog)
{
    unsigned int i;
    unsigned int uiCount;
    char*  pStop;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    EventLog*  pData = NULL;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u ", 
						    Table_Desc[TBL_EVENTLOG].sTblName,
						    Table_Desc[TBL_EVENTLOG].sFieldName[1], 
						    uEvtTypeId
						    );

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_EVENTLOG].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTLOG].sTblName, qryRst.m_iColum,Table_Desc[TBL_EVENTLOG].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uiCount = (unsigned int)qryRst.m_iRow;
    if (!tblLog.AllocData(uiCount))
    {
        return false;
    }

    pData = tblLog.GetData(uiCount);
    for (i = 0; i < uiCount; i++)
    {
        pData[i].ucEventId       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucEvtType       = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
        pData[i].ulHappenTime    = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2], &pStop, 10);
        pData[i].ulEvtValue      = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3], &pStop, 10);
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryEventLog
Description:    ¸ù¾ÝÊÂ¼þIDHºÅ²éÑ¯ÊÂ¼þÈÕÖ¾±í¼ÇÂ¼
Input:          uEvtLogId -- ÈÕÖ¾IDºÅ
				sEvtLog   -- ÈÕÖ¾½á¹¹Ìå
Output:         ÎÞ
Return:         true - ²éÑ¯¼ÇÂ¼³É¹¦ false -²éÑ¯¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryEventLog(Byte uEvtLogId, EventLog& sEvtLog)
{
    char*  pStop;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;

    ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
						    Table_Desc[TBL_EVENTLOG].sTblName,
						    Table_Desc[TBL_EVENTLOG].sFieldName[0], 
						    uEvtLogId);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != Table_Desc[TBL_EVENTLOG].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTLOG].sTblName, qryRst.m_iColum,Table_Desc[TBL_EVENTLOG].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
  
    sEvtLog.ucEventId       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    sEvtLog.ucEvtType       = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
    sEvtLog.ulHappenTime    = ACE_OS::strtoul(qryRst.m_ppResult[qryRst.m_iColum + 2], &pStop, 10);
    sEvtLog.ulEvtValue      = ACE_OS::strtoul(qryRst.m_ppResult[qryRst.m_iColum + 3], &pStop, 10);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetLogMaxMin
Description:    »ñÈ¡ÈÕÖ¾±í×î´óºÍ×îÐ¡ID
Input:          ÎÞ
Output:         pMaxId --×î´óÈÕÖ¾ºÅÊý¾ÝÖ¸Õë
				pMinId --×îÐ¡ÈÕÖ¾ºÅÊý¾ÝÖ¸Õë
Return:         true - ²éÑ¯³É¹¦ false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetLogMaxMin(Uint32* pMaxId, Uint32* pMinId)
{
	char   sSql[LARGE_BUF_SIZE] = {0};
	CSqliteRst  qryRst;

	ACE_OS::memset(sSql, 0, LARGE_BUF_SIZE);

	ACE_OS::sprintf(sSql, "select max(%s) from %s"
							,Table_Desc[TBL_EVENTLOG].sFieldName[0]
							,Table_Desc[TBL_EVENTLOG].sTblName);
	
	if (!Query(sSql, qryRst))
	{
		
		return false;
	}
	if ( NULL == qryRst.m_ppResult[qryRst.m_iColum] )
	{
		*pMaxId = 0;
	}
	else
	{
		*pMaxId = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	}
	
	if ( 0 == *pMaxId )
	{
		*pMinId = 1;
		
		return true;
	}

	ACE_OS::memset(sSql, 0, LARGE_BUF_SIZE);
	ACE_OS::sprintf(sSql, "select %s from %s where %s=(select min(%s) from %s)"
							,Table_Desc[TBL_EVENTLOG].sFieldName[0]
							,Table_Desc[TBL_EVENTLOG].sTblName
							,Table_Desc[TBL_EVENTLOG].sFieldName[2]
							,Table_Desc[TBL_EVENTLOG].sFieldName[2]
							,Table_Desc[TBL_EVENTLOG].sTblName);
	if (!Query(sSql, qryRst))
	{
		
		return false;
	}
	if ( NULL == qryRst.m_ppResult[qryRst.m_iColum] )
	{
		*pMinId = 1;
	}
	else
	{
		*pMinId = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	}
	ACE_DEBUG ((LM_DEBUG, "%s:%04d pmaxid =%d pminid= %d\n", SHORT_FILE, __LINE__,*pMaxId,*pMinId));
	return true;
}

/**************************************************************
Function:       CGbtTscDb::AddEventLog
Description:    Ìí¼ÓÈÕÖ¾±í¼ÇÂ¼
Input:          pMaxId --×î´óÈÕÖ¾ºÅÊý¾ÝÖ¸Õë
				pMinId --×îÐ¡ÈÕÖ¾ºÅÊý¾ÝÖ¸Õë
				uEvtType -- ÈÕÖ¾ÀàÐÍ
				uEvtValue-- ÈÕÖ¾Öµ
Output:         ÎÞ
Return:         true - Ìí¼ÓÈÕÖ¾³É¹¦ false -Ìí¼ÓÈÕÖ¾Ê§°Ü
***************************************************************/
bool CGbtTscDb::AddEventLog(Uint32* pMaxId, Uint32* pMinId, Byte uEvtType, Ulong uEvtValue)
{	
	Uint32 uiCurEventId = 0;
    time_t tCurrent = ACE_OS::time(NULL);
	tCurrent = tCurrent + 8*3600;
	char   sSql[LARGE_BUF_SIZE] = {0};
	CSqliteRst  qryRst;

	ACE_OS::memset(sSql, 0 , LARGE_BUF_SIZE);
	if ( *pMaxId < 255 )
	{
		uiCurEventId = *pMaxId + 1;
		ACE_OS::sprintf(sSql, "insert into %s values( %u, %u, %lu, %lu,datetime('now','localtime'))",
								Table_Desc[TBL_EVENTLOG].sTblName,
								uiCurEventId,
								uEvtType,
								tCurrent,
								uEvtValue);
	   *pMaxId += 1;
	}
	else  
	{
		uiCurEventId = *pMinId;
		ACE_OS::sprintf(sSql, "update %s set %s=%u,%s=%u,%s=%lu where %s=%u",
			Table_Desc[TBL_EVENTLOG].sTblName,
			Table_Desc[TBL_EVENTLOG].sFieldName[1],
			uEvtType,
			Table_Desc[TBL_EVENTLOG].sFieldName[2],
			(int)tCurrent,
			Table_Desc[TBL_EVENTLOG].sFieldName[3],
			uEvtValue,
			Table_Desc[TBL_EVENTLOG].sFieldName[0],
			uiCurEventId);

		uiCurEventId++;

		*pMinId = uiCurEventId > 255 ? 1 : uiCurEventId; 
	}

    if ( !ExecuteCmd(sSql) )
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    //ACE_DEBUG ((LM_DEBUG, "%s:%d uEvtValue = %d\n",__FILE__,__LINE__,uEvtValue));
    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelEventLog
Description:    ¸ù¾ÝÈÕÖ¾ÀàÐÍIDÉ¾³ý¶ÔÓ¦ÀàÐÍÈÕÖ¾±í¼ÇÂ¼
Input:          uEvtTypeId  -- ÈÕÖ¾ÀàÐÍID
Output:         ÎÞ
Return:         true - É¾³ýÈÕÖ¾³É¹¦ false -É¾³ýÈÕÖ¾Ê§°Ü
***************************************************************/
bool CGbtTscDb::DelEventLog(Byte uEvtTypeId)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s where %s=%u",
				           Table_Desc[TBL_EVENTLOG].sTblName,
				           Table_Desc[TBL_EVENTLOG].sFieldName[1], 
				           uEvtTypeId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::DelEventLog
Description:    É¾³ýÈ«²¿¶ÔÓ¦ÀàÐÍÈÕÖ¾±í¼ÇÂ¼
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÈÕÖ¾³É¹¦ false -É¾³ýÈÕÖ¾Ê§°Ü
***************************************************************/
bool CGbtTscDb::DelEventLog()
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s", Table_Desc[TBL_EVENTLOG].sTblName);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}




/**************************************************************
Function:       CGbtTscDb::QueryChannelChk
Description:    ²éÑ¯Í¨µÀµÆÅÝ¼ì²âÅäÖÃ±í ADD:201308011101
Input:          tblChanChk  -- Í¨µÀµÆÅÝ¼ì²âÅäÖÃ±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯Êý¾Ý³É¹¦ false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryChannelChk(TblChannelChk & tblChanChk)
{
    int    i;
    Byte  uCount;
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;   
	ChannelChk* pData =NULL ;

    ACE_OS::sprintf(sSql, "select * from %s order by %s",  
    						Table_Desc[TBL_LAMP_CHECK].sTblName, 
    						Table_Desc[TBL_LAMP_CHECK].sFieldName[0]);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }


    if (qryRst.m_iColum != Table_Desc[TBL_LAMP_CHECK].iFieldCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_LAMP_CHECK].sTblName, qryRst.m_iColum,Table_Desc[TBL_LAMP_CHECK].iFieldCount));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    uCount = (Byte)qryRst.m_iRow;
    if (!tblChanChk.AllocData(uCount))
    {
        return false;
    }

    pData = tblChanChk.GetData(uCount);
    for (i = 0; i < qryRst.m_iRow; i++)
    {
        pData[i].ucSubChannelId        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
        pData[i].ucIsCheck             = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);       
    
    }
    return true;
}


/**************************************************************
Function:       CGbtTscDb::QueryChannelChk
Description:    É¾³ýÍ¨µÀµÆÅÝ¼ì²âÅäÖÃ±í ADD:201308011401
Input:          ÎÞ
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelChannelChk()
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "delete from %s",Table_Desc[TBL_LAMP_CHECK].sTblName);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
    return true;
}
    

/**************************************************************
Function:       CGbtTscDb::AddChannelChk
Description:    Ìí¼ÓÍ¨µÀµÆÅÝ¼ì²âÅäÖÃ±í ADD:201308011501
Input:          uChanChkId  -- ÐÅºÅµÆÍ¨µÀIDºÅ
				sChannelChk -- Í¨µÀµÆÅÝ¼ì²â½á¹¹Ìå
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddChannelChk(Byte uChanChkId, ChannelChk& sChannelChk)
{
   
    char   sSql[LARGE_BUF_SIZE];
   
    ACE_OS::sprintf(sSql, "insert into %s(%s, %s)values(%u, %u)",
				           Table_Desc[TBL_LAMP_CHECK].sTblName,
				           Table_Desc[TBL_LAMP_CHECK].sFieldName[0],
				           Table_Desc[TBL_LAMP_CHECK].sFieldName[1],                   
				           uChanChkId,
				           sChannelChk.ucIsCheck );
                 

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return true;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::ModChannelChk
Description:    ÐÞ¸ÄÍ¨µÀµÆÅÝ¼ì²âÅäÖÃ±í ADD:201308011548
Input:          uChanChkId  -- ÐÅºÅµÆÍ¨µÀIDºÅ
				sChannelChk -- Í¨µÀµÆÅÝ¼ì²â½á¹¹Ìå
Output:         ÎÞ
Return:         true - ÐÞ¸ÄÊý¾Ý³É¹¦ false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModChannelChk(Byte uChanChkId, ChannelChk& sChannelChk)
{  
    char   sSql[LARGE_BUF_SIZE];
   if(uChanChkId <1 || uChanChkId>MAX_LAMP)
   		return false;
    ACE_OS::sprintf(sSql, "update %s set %s=%u where %s=%u",
				           Table_Desc[TBL_LAMP_CHECK].sTblName,
				           Table_Desc[TBL_LAMP_CHECK].sFieldName[1], 
				           sChannelChk.ucIsCheck,                   
				           Table_Desc[TBL_LAMP_CHECK].sFieldName[0], 
				           uChanChkId);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }
  
    return true;
}


/**************************************************************
Function:       CGbtTscDb::AddVehicleStat
Description:    Ìí¼Ó³µÁ¾Í³¼Æ¼ÇÂ¼
Input:          ucDetId     - ¼ì²âÆ÷id
*       		ulCarTotal  - ³µÁ÷Á¿
*      		    ucOccupancy - Õ¼ÓÐÂÊ
Output:         ÎÞ
Return:         true - Ìí¼ÓÊý¾Ý³É¹¦ false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddVehicleStat(Byte ucDetId,Ulong ulCarTotal , Byte ucOccupancy)
{
	int iMaxId = 0;
	int iDelId = 0;
	int iCurId = 0;
	time_t tCurrent = ACE_OS::time(NULL);
	char   sSql[LARGE_BUF_SIZE] = {0};	

	CSqliteRst  qryRst;	

	ACE_OS::memset(sSql, 0, LARGE_BUF_SIZE);
	ACE_OS::sprintf(sSql, "select max(%s) from %s"
							,Table_Desc[TBL_VEHICLESTAT].sFieldName[0]
							,Table_Desc[TBL_VEHICLESTAT].sTblName);
	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}
	if ( NULL == qryRst.m_ppResult[qryRst.m_iColum] )
	{
		iMaxId = 0;
	}
	else
	{
		iMaxId = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	}
	//ACE_DEBUG ((LM_DEBUG, "%s:%04d iMaxEventId:%d\n", SHORT_FILE, __LINE__,iMaxEventId));

	if ( iMaxId < 300 )
	{
		iCurId = iMaxId + 1;
	}
	else  //ÐèÒªÉ¾³ýÊý¾Ý
	{
		ACE_OS::memset(sSql, 0, LARGE_BUF_SIZE);
		ACE_OS::sprintf(sSql, "select %s from %s where %s=(select min(%s) from %s)"
								,Table_Desc[TBL_VEHICLESTAT].sFieldName[0]
								,Table_Desc[TBL_VEHICLESTAT].sTblName
								,Table_Desc[TBL_VEHICLESTAT].sFieldName[4]
								,Table_Desc[TBL_VEHICLESTAT].sFieldName[4]
								,Table_Desc[TBL_VEHICLESTAT].sTblName);
		if (!Query(sSql, qryRst))
		{
			//ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
			return false;
		}
		if ( NULL == qryRst.m_ppResult[qryRst.m_iColum] )
		{
			iDelId = 1;
		}
		else
		{
			iDelId = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
		}
		//ACE_DEBUG ((LM_DEBUG, "%s:%04d iDelEventId:%d\n", SHORT_FILE, __LINE__,iDelEventId));
 
		DelVehicleStat(iDelId);
		iCurId = iDelId;
	}

	ACE_OS::memset(sSql, 0, LARGE_BUF_SIZE);

	ACE_OS::sprintf(sSql, "insert into %s values( %u, %u, %lu, %u, %lu)",
							Table_Desc[TBL_VEHICLESTAT].sTblName,		
							iCurId,
							ucDetId,
							ulCarTotal,
							ucOccupancy,
							tCurrent);


	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::DelVehicleStat
Description:    ¸ù¾ÝulIdÉ¾³ý³µÁ¾¼ÇÂ¼
Input:          ulId     - ³µÁ¾ulId
Output:         ÎÞ
Return:         true - É¾³ýÊý¾Ý³É¹¦ false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelVehicleStat(Ulong ulId)
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s where %s=%lu",
							Table_Desc[TBL_VEHICLESTAT].sTblName,
							Table_Desc[TBL_VEHICLESTAT].sFieldName[0], 
							ulId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryVehileStat
Description:    ²éÑ¯×î½üµÄ³µÁ¾¼ÇÂ¼
Input:          ÎÞ
Output:         sVehicleStat     - ³µÁ¾¼ÇÂ¼½á¹¹Ìå
Return:         true - ²éÑ¯¼ÇÂ¼³É¹¦ false -²éÑ¯¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryVehileStat(VehicleStat& sVehicleStat)
{
	char*  pStop;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=(select max(%s) from %s)", 
							Table_Desc[TBL_VEHICLESTAT].sTblName,
							Table_Desc[TBL_VEHICLESTAT].sFieldName[4],
							Table_Desc[TBL_VEHICLESTAT].sFieldName[4],
							Table_Desc[TBL_VEHICLESTAT].sTblName);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_VEHICLESTAT].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_EVENTLOG].sTblName, qryRst.m_iColum,Table_Desc[TBL_VEHICLESTAT].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	sVehicleStat.ulId        = ACE_OS::strtoul(qryRst.m_ppResult[qryRst.m_iColum], &pStop, 10);
	sVehicleStat.ucDetId     = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum+1]);
	sVehicleStat.ulCarTotal  = ACE_OS::strtoul(qryRst.m_ppResult[qryRst.m_iColum + 2], &pStop, 10);
	sVehicleStat.ucOccupancy = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);
	sVehicleStat.ulAddtime   = ACE_OS::strtoul(qryRst.m_ppResult[qryRst.m_iColum + 4], &pStop, 10);

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryVehileStat
Description:    ²éÑ¯È«²¿µÄ³µÁ¾¼ÇÂ¼
Input:          tblVehicleStat   -- ³µÁ¾¼ÇÂ¼±í¶ÔÏó
Output:         ÎÞ
Return:         true - ²éÑ¯¼ÇÂ¼³É¹¦ false -²éÑ¯¼ÇÂ¼Ê§°Ü
***************************************************************/
bool CGbtTscDb::QueryVehicleStat(TblVehicleStat& tblVehicleStat)
{
	int    i;
	Ulong  uCount;
	char*  pStop;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	VehicleStat*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s, %s", 
							Table_Desc[TBL_VEHICLESTAT].sTblName,
							Table_Desc[TBL_VEHICLESTAT].sFieldName[4],
							Table_Desc[TBL_VEHICLESTAT].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_VEHICLESTAT].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_VEHICLESTAT].sTblName, qryRst.m_iColum,Table_Desc[TBL_VEHICLESTAT].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Ulong)qryRst.m_iRow;
	if (!tblVehicleStat.AllocData(uCount))
	{
		return false;
	}

	pData = tblVehicleStat.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ulId        = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum], &pStop, 10);
		pData[i].ucDetId     = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum+1]);
		pData[i].ulCarTotal  = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2], &pStop, 10);
		pData[i].ucOccupancy = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
		pData[i].ulAddtime   = ACE_OS::strtoul(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 4], &pStop, 10);
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::GetCommonPara
Description:    »ñÈ¡¹«¹²ÅäÖÃÏà¹Ø²ÎÊý
Input:          ÎÞ
Output:         usDeviceId   -- Éè±¸ID
				ucMaxModule  -- ×î´óÄ£¿éÊý
				ucSynchTime  -- Í¬²½Ê±¼ä
				usSynchFlag  -- Í¬²½±êÖ¾
Return:         true - ²éÑ¯²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::GetCommonPara(Ushort& usDeviceId, Byte& ucMaxModule, Byte& ucSynchTime, Ushort& usSynchFlag)
{
    Ulong ulTemp;
    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "usDeviceId", ulTemp);
    usDeviceId  = (Ushort)ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxModule", ulTemp);
    ucMaxModule  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucSynchTime", ulTemp);
    ucSynchTime  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucSynchFlag", ulTemp);
    usSynchFlag  = (Ushort)ulTemp;

    return true;
}
   
/**************************************************************
Function:       CGbtTscDb::SetCommonPara
Description:    ÉèÖÃ¹«¹²ÅäÖÃÏà¹Ø²ÎÊý
Input:          usDeviceId   -- Éè±¸IDÊý¾ÝÖ¸Õë
				ucMaxModule  -- ×î´óÄ£¿éÊýÖ¸Õë
				ucSynchTime  -- Í¬²½Ê±¼äÖ¸Õë
				usSynchFlag  -- Í¬²½±êÖ¾Ö¸Õë
Output:         ÎÞ
Return:         true - ²éÑ¯²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::SetCommonPara(Ushort* usDeviceId, Byte* ucMaxModule, Byte* ucSynchTime, Ushort* usSynchFlag)
{
    if (NULL == usDeviceId && NULL == ucMaxModule && NULL == ucSynchTime && NULL == usSynchFlag)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (NULL != ucMaxModule)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxModule", *ucMaxModule))
        {
            return false;
        }
    }

    if (NULL != usDeviceId)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "usDeviceId", *usDeviceId))
        {
            return false;
        }
    }

    if (NULL != ucSynchTime)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucSynchTime", *ucSynchTime))
        {
            return false;
        }
    }

    if (NULL != usSynchFlag)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "usSynchFlag", *usSynchFlag))
        {
            return false;
        }
    }
 
    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetCommTimePara
Description:    »ñÈ¡¹«¹²Ê±¼ä²ÎÊý
Input:          ÎÞ
Output:         lZone  		 		-- Ê±Çø
				ucMaxPlanCnt    	-- ×î´óÊ±»ùÊý
				ucMaxScheduleCnt    -- ×î´óÊ±¶Î±íÊý
				ucMaxEvtCount  		-- ×î´óÊÂ¼þÊý
Return:         true - ²éÑ¯²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::GetCommTimePara(Long& lZone, Byte& ucMaxPlanCnt, Byte& ucMaxScheduleCnt, Byte& ucMaxEvtCount)
{
    Ulong ulTemp;

    GetFieldSignData(Table_Desc[TBL_SYSTEM].sTblName, "lZone", lZone);

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxPlan", ulTemp);
    ucMaxPlanCnt  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxSchedule", ulTemp);
    ucMaxScheduleCnt  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxScheduleEvt", ulTemp);
    ucMaxEvtCount  = (Byte)ulTemp;

    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::SetCommTimePara
Description:    ÉèÖÃ¹«¹²Ê±¼ä²ÎÊý
Input:          lZone  		 		-- Ê±ÇøÊý¾ÝÖ¸Õë
				ucMaxPlanCnt    	-- ×î´óÊ±»ùÊýÖ¸Õë
				ucMaxScheduleCnt    -- ×î´óÊ±¶Î±íÊýÖ¸Õë
				ucMaxEvtCount  		-- ×î´óÊÂ¼þÊýÖ¸Õë
Output:         ÎÞ
Return:         true - ÉèÖÃ¹«¹²Ê±¼ä²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::SetCommTimePara(Long* lZone, Byte* ucMaxPlan, Byte* ucMaxSchedule, Byte* ucMaxScheduleEvt)
{
    if (NULL == lZone && NULL == ucMaxPlan && NULL == ucMaxSchedule && NULL == ucMaxScheduleEvt)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (NULL != ucMaxPlan)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxPlan", *ucMaxPlan))
        {
            return false;
        }
    }

    if (NULL != ucMaxSchedule)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxSchedule", *ucMaxSchedule))
        {
            return false;
        }
    }

    if (NULL != ucMaxScheduleEvt)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxScheduleEvt", *ucMaxScheduleEvt))
        {
            return false;
        }
    }
    if (NULL != lZone)
    {
        if (!SetFieldSignData(Table_Desc[TBL_SYSTEM].sTblName, "lZone", *lZone))
        {
            return false;
        }
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetEventPara
Description:    »ñÈ¡ÊÂ¼þ²ÎÊý
Input:          ÎÞ	
Output:         ucMaxEventType  -- ×î´óÊÂ¼þÀàÐÍ
				ucMaxEventLog   -- ×î´óÈÕÖ¾Êý
Return:         true - »ñÈ¡ÈÕÖ¾²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::GetEventPara(Byte& ucMaxEventType, Byte& ucMaxEventLog)
{
    Ulong ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxEventType", ulTemp);
    ucMaxEventType  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxEventLog", ulTemp);
    ucMaxEventLog  = (Byte)ulTemp;

    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetEventPara
Description:    ÉèÖÃÊÂ¼þ²ÎÊý
Input:          ucMaxEventType  -- ×î´óÊÂ¼þÀàÐÍÊý¾ÝÖ¸Õë
				ucMaxEventLog   -- ×î´óÈÕÖ¾ÊýÊý¾ÝÖ¸Õë
Output:         ÎÞ
Return:         true - ÉèÖÃÈÕÖ¾²ÎÊý³É¹¦  false -ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetEventPara(Byte* ucMaxEventType, Byte* ucMaxEventLog)
{
    if (NULL == ucMaxEventType && NULL == ucMaxEventLog)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (NULL != ucMaxEventType)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxEventType", *ucMaxEventType))
        {
            return false;
        }
    }

    if (NULL != ucMaxEventLog)
    {
        if (!SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxEventLog", *ucMaxEventLog))
        {
            return false;
        }
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetPhasePara
Description:    »ñÈ¡ÏàÎ»²ÎÊý
Input:          ÎÞ
Output:         ucMaxPhaseCount --  ×î´óÏàÎ»Êý
Return:         true - »ñÈ¡ÏàÎ»²ÎÊý³É¹¦ 
***************************************************************/
bool CGbtTscDb::GetPhasePara(Byte& ucMaxPhaseCount)
{
    Ulong ulTemp;

    if (!GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxPhase", ulTemp))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GetFieldData(%s, ucMaxPhase)\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CONSTANT].sTblName));
        return false;
    }
    
    ucMaxPhaseCount = (Byte)ulTemp;
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetPhasePara
Description:    ÉèÖÃÏàÎ»²ÎÊý
Input:          ucMaxPhaseCount --  ×î´óÏàÎ»Êý
Output:         ÎÞ
Return:         true - ÉèÖÃÏàÎ»²ÎÊý³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetPhasePara(Byte& ucMaxPhaseCount)
{
   return SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxPhase", ucMaxPhaseCount);
}

/**************************************************************
Function:       CGbtTscDb::GetChannelPara
Description:    »ñÈ¡Í¨µÀ²ÎÊý
Input:          ÎÞ
Output:         ucMaxChannel --  ×î´óÍ¨µÀÊý
Return:         true - »ñÈ¡Í¨µÀ²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetChannelPara(Byte& ucMaxChannel)
{
    Ulong ulTemp;

    if (!GetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxChannel", ulTemp))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GetFieldData(%s, ucMaxChannel)\n", SHORT_FILE, __LINE__, Table_Desc[TBL_CONSTANT].sTblName));
        return false;
    }
    
    ucMaxChannel = (Byte)ulTemp;
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetChannelPara
Description:    ÉèÖÃÍ¨µÀ²ÎÊý
Input:          ucMaxChannel --  ×î´óÍ¨µÀÊý
Output:         ÎÞ
Return:         true - ÉèÖÃÍ¨µÀ²ÎÊý³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetChannelPara(Byte& ucMaxChannel)
{
    return SetFieldData(Table_Desc[TBL_CONSTANT].sTblName, "ucMaxChannel", ucMaxChannel);
}

/**************************************************************
Function:       CGbtTscDb::GetUnitCtrlPara
Description:    »ñÈ¡µ¥ÔªÅäÖÃ²ÎÊý
Input:          ucFlsTimeWhenStart  -- ¿ª»ú»ÆÉÁÊ±¼ä
				ucAllRedTimeWhenStart -- ¿ª»úÈ«ºìÊ±¼ä
				ucRemoteCtrlFlag      -- Ô¶³Ì¿ØÖÆ±ê¼Ç
				ucFlashFreq           -- »ÆÉÁÆµÂÊ
Output:         ÎÞ
Return:         true - »ñÈ¡µ¥Ôª²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetUnitCtrlPara(Byte& ucFlsTimeWhenStart, Byte& ucAllRedTimeWhenStart, Byte& ucRemoteCtrlFlag, Byte& ucFlashFreq)
{
    Ulong ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucFlsTimeWhenStart", ulTemp);
    ucFlsTimeWhenStart  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucAllRedTimeWhenStart", ulTemp);
    ucAllRedTimeWhenStart  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucRemoteCtrlFlag", ulTemp);
    ucRemoteCtrlFlag  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucFlashFreq", ulTemp);
    ucFlashFreq  = (Byte)ulTemp;
    
    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::SetUnitCtrlPara
Description:    ÉèÖÃµ¥ÔªÅäÖÃ²ÎÊý
Input:          ÎÞ
Output:         ucFlsTimeWhenStart  -- ¿ª»ú»ÆÉÁÊ±¼äÖ¸Õë
				ucAllRedTimeWhenStart -- ¿ª»úÈ«ºìÊ±¼äÖ¸Õë
				ucRemoteCtrlFlag      -- Ô¶³Ì¿ØÖÆ±ê¼ÇÖ¸Õë
				ucFlashFreq           -- »ÆÉÁÆµÂÊÖ¸Õë
Return:         true - ÉèÖÃµ¥Ôª²ÎÊý³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetUnitCtrlPara(Byte* ucFlsTimeWhenStart, Byte* ucAllRedTimeWhenStart, Byte* ucRemoteCtrlFlag, Byte* ucFlashFreq)
{
    if (NULL == ucFlsTimeWhenStart && NULL == ucAllRedTimeWhenStart && NULL == ucRemoteCtrlFlag && NULL == ucFlashFreq)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (NULL != ucFlsTimeWhenStart)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucFlsTimeWhenStart", *ucFlsTimeWhenStart))
        {
            return false;
        }
    }

    if (NULL != ucAllRedTimeWhenStart)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucAllRedTimeWhenStart", *ucAllRedTimeWhenStart))
        {
            return false;
        }
    }

    if (NULL != ucRemoteCtrlFlag)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucRemoteCtrlFlag", *ucRemoteCtrlFlag))
        {
            return false;
        }
    }
    if (NULL != ucFlashFreq)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucFlashFreq", *ucFlashFreq))
        {
            return false;
        }
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetPatternPara
Description:    »ñÈ¡ÅäÊ±·½°¸²ÎÊý
Input:          ÎÞ
Output:         ucMaxPattern  -- ×î´ó·½°¸Êý
				ucMaxStagePattern -- ×î´ó½×¶ÎÅäÊ±Êý
				ucMaxStage      -- ×î´ó½×¶ÎÊý
Return:         true - »ñÈ¡ÅäÊ±·½°¸²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetPatternPara(Byte& ucMaxPattern, Byte& ucMaxStagePattern, Byte& ucMaxStage)
{
    Ulong ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxPattern", ulTemp);
    ucMaxPattern  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxStagePattern", ulTemp);
    ucMaxStagePattern  = (Byte)ulTemp;

    GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxStage", ulTemp);
    ucMaxStage  = (Byte)ulTemp;

    return true;
}
    
/**************************************************************
Function:       CGbtTscDb::SetPatternPara
Description:    ÉèÖÃÅäÊ±·½°¸²ÎÊý
Input:          ucMaxPattern  -- ×î´ó·½°¸ÊýÖ¸Õë
				ucMaxStagePattern -- ×î´ó½×¶ÎÅäÊ±ÊýÖ¸Õë
				ucMaxStage      -- ×î´ó½×¶ÎÊýÖ¸Õë
Output:         ÎÞ
Return:         true - ÉèÖÃÅäÊ±·½°¸²ÎÊý³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetPatternPara(Byte* ucMaxPattern, Byte* ucMaxStagePattern, Byte* ucMaxStage)
{
    if (NULL == ucMaxPattern && NULL == ucMaxStagePattern && NULL == ucMaxStage)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (NULL != ucMaxPattern)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxPattern", *ucMaxPattern))
        {
            return false;
        }
    }

    if (NULL != ucMaxStagePattern)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxStagePattern", *ucMaxStagePattern))
        {
            return false;
        }
    }

    if (NULL != ucMaxStage)
    {
        if (!SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucMaxStage", *ucMaxStage))
        {
            return false;
        }
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetGlobalCycle
Description:    »ñÈ¡¹«¹²ÖÜÆÚÊ±³¤
Input:          ÎÞ
Output:         ucGlobalCycle  --  ¹«¹²ÖÜÆÚÊ±³¤
Return:         true - »ñÈ¡¹«¹²ÖÜÆÚÊ±³¤³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetGlobalCycle(Byte& ucGlobalCycle)
{
    Ulong ulTemp;

    if (!GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucGlobalCycle", ulTemp))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GetFieldData(%s, ucGlobalCycle)\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }
    
    ucGlobalCycle = (Byte)ulTemp;
    return true;
}
   
/**************************************************************
Function:       CGbtTscDb::SetGlobalCycle
Description:    ÉèÖÃ¹«¹²ÖÜÆÚÊ±³¤
Input:          ucGlobalCycle  --  ¹«¹²ÖÜÆÚÊ±³¤
Output:         ÎÞ
Return:         true - ÉèÖÃ¹«¹²ÖÜÆÚÊ±³¤³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetGlobalCycle(Byte ucGlobalCycle)
{
    return SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucGlobalCycle", ucGlobalCycle);
}

/**************************************************************
Function:       CGbtTscDb::GetCoorPhaseOffset
Description:    »ñÈ¡Ð­Í¬ÏàÎ»²î
Input:          ÎÞ
Output:         ucPhaseOffset  --  Ð­Í¬ÏàÎ»²î
Return:         true - »ñÈ¡Ð­Í¬ÏàÎ»²î³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetCoorPhaseOffset(Byte& ucPhaseOffset)
{
    Ulong ulTemp;

    if (!GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucCoorPhaseOffset", ulTemp))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GetFieldData(%s, ucCoorPhaseOffset)\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }
    
    ucPhaseOffset = (Byte)ulTemp;
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetCoorPhaseOffset
Description:    ÉèÖÃÐ­Í¬ÏàÎ»²î
Input:          ucPhaseOffset  --  Ð­Í¬ÏàÎ»²î
Output:         ÎÞ
Return:         true - ÉèÖÃÐ­Í¬ÏàÎ»²î³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetCoorPhaseOffset(Byte& ucPhaseOffset)
{
    return SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucCoorPhaseOffset", ucPhaseOffset);
}

/**************************************************************
Function:       CGbtTscDb::GetDegradeCfg
Description:    »ñÈ¡½µ¼¶ÅäÖÃ
Input:          ÎÞ
Output:         ucDegradeMode  --  ½µ¼¶ÅäÖÃÄ£Ê½
				ucDegradePattern  - ½µ¼¶ÅäÖÃ·½°¸
Return:         true - »ñÈ¡½µ¼¶ÅäÖÃ³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetDegradeCfg(Byte& ucDegradeMode, Byte& ucDegradePattern)
{
    char   sSql[SMALL_BUF_SIZE];
    Byte  ucPatternArray[14] = {0};

    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select ucDegradeMode, sDegradePattern from %s",Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != 2)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= 3\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
  
    ucDegradeMode      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);

    GetFieldsValue(qryRst.m_ppResult[qryRst.m_iColum+1], ',', ucPatternArray, 14);
    if (ucDegradeMode < 14)
    {
        ucDegradePattern = ucPatternArray[ucDegradeMode];
    }
    else
    {
        ucDegradePattern = 0;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetDegradePattern
Description:    »ñÈ¡½µ¼¶ÅäÖÃ·½°¸±íÊý¾Ý
Input:          ÎÞ
Output:         tblDegrade  -- ½µ¼¶·½°¸±í¶ÔÏó
Return:         true - »ñÈ¡½µ¼¶ÅäÖÃ·½°¸±í³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetDegradePattern(DbByteArray& tblDegrade)
{
    char   sSql[SMALL_BUF_SIZE];
    Byte  ucPatternArray[14] = {0};

    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select sDegradePattern from %s",Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != 1)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= 3\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
  

    GetFieldsValue(qryRst.m_ppResult[qryRst.m_iColum], ',', ucPatternArray, 14);
    tblDegrade.SetData(ucPatternArray, 14);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetDegradePattern
Description:    ¸ù¾Ý½µ¼¶Ä£Ê½»ñÈ¡½µ¼¶ÅäÖÃ·½°¸
Input:          ucDegradeMode  -- ½µ¼¶Ä£Ê½
Output:         ucDegradePattern  -- ½µ¼¶ÅäÖÃ·½°¸
Return:         true - »ñÈ¡½µ¼¶ÅäÖÃ·½°¸  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetDegradePattern(Byte ucDegradeMode, Byte& ucDegradePattern)
{
    DbByteArray tblByte;
    Byte* pData;
    Byte  uCount;
    if (!GetDegradePattern(tblByte))
    {
        return false;
    }
    pData = tblByte.GetData(uCount);

    if (ucDegradeMode >= uCount)
    {
        return false;
    }

    ucDegradePattern = pData[ucDegradeMode];
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetDegradeMode
Description:    ÉèÖÃ½µ¼¶ÅäÖÃÄ£Ê½
Input:          ucDegradeMode  -- ½µ¼¶Ä£Ê½
Output:         ÎÞ
Return:         true - ÉèÖÃ½µ¼¶ÅäÖÃÄ£Ê½³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetDegradeMode(Byte ucDegradeMode)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "update %s set ucDegradeMode = %u", Table_Desc[TBL_SYSTEM].sTblName, ucDegradeMode);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetDegradePattern
Description:    ÉèÖÃ½µ¼¶ÅäÖÃ·½°¸
Input:          tblDegrade  -- ½µ¼¶·½°¸±í¶ÔÏó
Output:         ÎÞ
Return:         true - ÉèÖÃ½µ¼¶ÅäÖÃ·½°¸³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetDegradePattern(DbByteArray& tblDegrade)
{
    char   sValue[SMALL_BUF_SIZE] = {0};
    char   sSql[SMALL_BUF_SIZE];
    size_t uLength;
    Byte* pData;
    Byte  uCount, i;
    pData = tblDegrade.GetData(uCount);

    if (uCount > 14)
    {
        uCount = 14;
    }

    if (uCount <= 0)
    {
        return false;
    }


    for (i=0; i<uCount;i++)
    {
        uLength = ACE_OS::strlen(sValue);
        ACE_OS::sprintf(sValue+uLength, "%u,", pData[i]);
    }

    ACE_OS::sprintf(sSql, "update %s set sDegradePattern='%s'", Table_Desc[TBL_SYSTEM].sTblName, sValue);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetDegradePattern
Description:    ¸ù¾Ý½µ¼¶Ä£Ê½ÉèÖÃ½µ¼¶ÅäÖÃ·½°¸
Input:          ucDegradeMode  -- ½µ¼¶Ä£Ê½
				ucDegradePattern -- ½µ¼¶·½°¸
Output:         ÎÞ
Return:         true - ÉèÖÃ½µ¼¶ÅäÖÃ·½°¸³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetDegradePattern(Byte ucDegradeMode, Byte ucDegradePattern)
{
    DbByteArray tblDegrade;
    Byte* pData;
    Byte  uCount;
   

    if (!GetDegradePattern(tblDegrade))
    {
        return false;
    }

    pData = tblDegrade.GetData(uCount);
    if (ucDegradeMode >= uCount)
    {
        return false;
    }

    pData[ucDegradeMode] = ucDegradePattern;

    return SetDegradePattern(tblDegrade);
}

/**************************************************************
Function:       CGbtTscDb::GetCtrlMasterOptFlag
Description:    »ñÈ¡¿ØÖÆÖ÷»ú²ÎÊý
Input:          ÎÞ
Output:         ucCtrlOptFlag   -- ¿ØÖÆÖ÷»ú²ÎÊý
Return:         true - »ñÈ¡²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetCtrlMasterOptFlag(Byte& ucCtrlOptFlag)
{
    Ulong ulTemp;

    if (!GetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucCtrlMasterOptFlag", ulTemp))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GetFieldData(%s, ucCtrlMasterOptFlag)\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }
    
    ucCtrlOptFlag = (Byte)ulTemp;
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetCtrlMasterOptFlag
Description:    ÉèÖÃ¿ØÖÆÖ÷»ú²ÎÊý
Input:          ÎÞ
Output:         ucCtrlOptFlag   -- ¿ØÖÆÖ÷»ú²ÎÊý
Return:         true - »ñÈ¡²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::SetCtrlMasterOptFlag(Byte ucCtrlOptFlag)
{
    return SetFieldData(Table_Desc[TBL_SYSTEM].sTblName, "ucCtrlMasterOptFlag", ucCtrlOptFlag);
}

/**************************************************************
Function:       CGbtTscDb::GetMultiCtrlPara
Description:    »ñÈ¡¶àÂ·¿ÚÐÅºÅ»ú²ÎÊý
Input:          ÎÞ
Output:         usBaseAddr   -- »ù×¼µØÖ·
				ucDevCount   -- Éè±¸ÐÅºÅ»úÊý
Return:         true - »ñÈ¡²ÎÊý³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetMultiCtrlPara(Ushort& usBaseAddr, Byte& ucDevCount)
{
    char   sSql[SMALL_BUF_SIZE];

    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select usBaseAddr,ucSigDevCount from %s", Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iColum != 2)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= 3\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }
  
    usBaseAddr      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    ucDevCount      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum+1]);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetMultiCtrlPara
Description:    ÉèÖÃ¶àÂ·¿ÚÐÅºÅ»ú²ÎÊý
Input:          usBaseAddr   -- »ù×¼µØÖ·²ÎÊýÖ¸Õë
				ucDevCount   -- Éè±¸ÐÅºÅ»úÊý²ÎÊýÖ¸Õë
Output:         ÎÞ
Return:         true - ÉèÖÃ²ÎÊý³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetMultiCtrlPara(Ushort* usBaseAddr, Byte* ucDevCount)
{
    if (NULL == usBaseAddr && NULL == ucDevCount)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tAll input is null\n", SHORT_FILE, __LINE__));
        return false;
    }

    bool   bFirst = true;
    char   sSql[LARGE_BUF_SIZE];
    size_t uLength;
    ACE_OS::sprintf(sSql, "update %s set", Table_Desc[TBL_SYSTEM].sTblName);

    if (NULL != usBaseAddr)
    {
        uLength = ACE_OS::strlen(sSql);
        if (bFirst)
        {
            bFirst = false;
            ACE_OS::sprintf(sSql+uLength, " usBaseAddr=%u", *usBaseAddr);
        }
        else
        {
            ACE_OS::sprintf(sSql+uLength, ",usBaseAddr=%u", *usBaseAddr);
        }
    }

    if (NULL != ucDevCount)
    {
        uLength = ACE_OS::strlen(sSql);
        if (bFirst)
        {
            bFirst = false;
            ACE_OS::sprintf(sSql+uLength, " ucSigDevCount=%u", *ucDevCount);
        }
        else
        {
            ACE_OS::sprintf(sSql+uLength, ",ucSigDevCount=%u", *ucDevCount);
        }
    }
    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::SetEypSerial
Description:    ÉèÖÃ±£´æ¼ÓÃÜÑéÖ¤×Ö·û´®
Input:          ÎÞ
Output:         ÎÞ
Return:         true - ÉèÖÃ¼ÓÃÜÑéÖ¤ÐòºÅ³É¹¦  false - ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetEypSerial(char * passwd)
{
     char   sSql[LARGE_BUF_SIZE];  
    //char SysEypDevId[32]={0};
   //	if(GetSysEnyDevId(SysEypDevId)==-1)
   		//return false ;	
   	//ACE_DEBUG ((LM_DEBUG, "%s:%d SysEypDevId =%s\n",__FILE__, __LINE__,SysEypDevId));  
   	
   	 ACE_OS::sprintf(sSql, "update %s set ucEypDevSerial='%s'",Table_Desc[TBL_SYSTEM].sTblName,  passwd);

   	 if (!ExecuteCmd(sSql))
   	 {
      		  ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
       		 return false;
  	  }
   	 return true;
    
}

/**************************************************************
Function:       CGbtTscDb::GetEypSerial
Description:    »ñÈ¡¼ÓÃÜÑéÖ¤×Ö·û´®
Input:          SysEypSerial- ¼ÓÃÜ°¡ÐòÁÐºÅ×Ö·ûÖ¸Õë
Output:         ÎÞ
Return:         true - »ñÈ¡¼ÓÃÜÑéÖ¤ÐòºÅ³É¹¦  false - »ñÈ¡Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetEypSerial(char * SysEypSerial)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select ucEypDevSerial from %s", Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    if (qryRst.m_iColum != 1)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tcolum number != 1\n", SHORT_FILE, __LINE__, qryRst.m_iColum));
        return false;
    } 
    
    ACE_OS::strcpy(SysEypSerial,qryRst.m_ppResult[qryRst.m_iColum]);
    return true;
}


/**************************************************************
Function:       CGbtTscDb::AddDefault
Description:    Êý¾Ý¿âÌí¼ÓÈ«¾ÖÄ£Ê½²ÎÊý
Input:          ÎÞ
Output:         ÎÞ
Return:         ÎÞ
***************************************************************/
void CGbtTscDb::AddDefault()
{
    char   sSql[LARGE_BUF_SIZE];
    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select * from %s", Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
    }
    if (qryRst.m_iRow > 0)
    {
        return;
    }

    ACE_OS::sprintf(sSql, "insert into %s(usDeviceId)values(0)",Table_Desc[TBL_SYSTEM].sTblName);
    
    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
    }     
    ACE_OS::sprintf(sSql, "select * from %s", Table_Desc[TBL_CONSTANT].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
    }

    if (qryRst.m_iRow <= 0)
    {
        ACE_OS::sprintf(sSql, "insert into %s(ucMaxModule)values(16)",Table_Desc[TBL_CONSTANT].sTblName);     
        if (!ExecuteCmd(sSql))
        {
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        }
    }

    return ;
}

/**************************************************************
Function:       CGbtTscDb::GetSystemData
Description:    ²éÑ¯ÏµÍ³±í
Input:          sField  -- ²éÑ¯×Ö¶ÎÖ¸Õë
Output:         ulData  --  ²éÑ¯Öµ
Return:         true -- ²éÑ¯³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetSystemData(const char* sField, Ulong& ulData)
{
    char   sSql[SMALL_BUF_SIZE];
    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select %s from %s", sField, Table_Desc[TBL_SYSTEM].sTblName);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    if (qryRst.m_iColum != 1)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= 3\n", SHORT_FILE, __LINE__, Table_Desc[TBL_SYSTEM].sTblName));
        return false;
    }
  
    ulData  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetSystemData
Description:    ÉèÖÃÏµÍ³±í
Input:          sField  -- ÉèÖÃ×Ö¶ÎÖ¸Õë
				ulData  -- ÉèÖÃÖµ
Output:         
Return:         true -- ÉèÖÃÏµÍ³±í³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::SetSystemData(const char* sField, Ulong ulData)
{
    char   sSql[LARGE_BUF_SIZE];
    ACE_OS::sprintf(sSql, "update %s set %s=%lu", Table_Desc[TBL_SYSTEM].sTblName, sField, ulData);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetFieldData
Description:    ²éÑ¯Êý¾Ý±í×Ö¶ÎÖµ
Input:          sTable  -- Êý¾Ý±íÃû×Ö·û´®
				sField  -- ×Ö¶ÎÃû
Output:         ulData  -- ×Ö¶ÎÖµ
Return:         true -- ²éÑ¯±í×Ö¶Î³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetFieldData(const char* sTable, const char* sField, Ulong& ulData)
{
    char   sSql[SMALL_BUF_SIZE];

    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select %s from %s", sField, sTable);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    if (qryRst.m_iColum != 1)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tcolum number != 1\n", SHORT_FILE, __LINE__, qryRst.m_iColum));
        return false;
    }
  
    ulData  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetFieldData
Description:    ÉèÖÃÊý¾Ý±í×Ö¶ÎÖµ
Input:          sTable  -- Êý¾Ý±íÃû×Ö·û´®
				sField  -- ×Ö¶ÎÃû
				ulData  -- ×Ö¶ÎÖµ
Output:         ÎÞ
Return:         true -- ²éÉèÖÃ±í×Ö¶Î³É¹¦  false -ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetFieldData(const char* sTable, const char* sField, Ulong ulData)
{
    char   sSql[LARGE_BUF_SIZE];

    if (0 == ACE_OS::strcmp(sTable, "Tbl_Constant"))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tTable(%s) Field(%s) can not be mod\n", SHORT_FILE, __LINE__, sTable, sField));
        return false;
    }
    else
    {
        if (!IsSysParaValid(sField, ulData))
        {
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s = %u, invalid\n", SHORT_FILE, __LINE__, sField, ulData));
            return false;
        }
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%lu", sTable, sField, ulData);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::GetFieldSignData
Description:    ²éÑ¯Êý¾Ý±í×Ö¶ÎÖµ
Input:          sTable  -- Êý¾Ý±íÃû×Ö·û´®
				sField  -- ×Ö¶ÎÃû
Output:         lData  -- ×Ö¶ÎÖµ
Return:         true -- ²éÑ¯±í×Ö¶Î³É¹¦  false -²éÑ¯Ê§°Ü
***************************************************************/
bool CGbtTscDb::GetFieldSignData(const char* sTable, const char* sField, Long& lData)
{
    char   sSql[SMALL_BUF_SIZE];

    CSqliteRst  qryRst;
    ACE_OS::sprintf(sSql, "select %s from %s", sField, sTable);

    if (!Query(sSql, qryRst))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
        return false;
    }

    if (qryRst.m_iRow <= 0)
    {
        return false;
    }

    if (qryRst.m_iColum != 1)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tcolum number != 1\n", SHORT_FILE, __LINE__, qryRst.m_iColum));
        return false;
    }
  
    lData  = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
    return true;
}

/**************************************************************
Function:       CGbtTscDb::SetFieldSignData
Description:    ÉèÖÃÊý¾Ý±í×Ö¶ÎÖµ
Input:          sTable  -- Êý¾Ý±íÃû×Ö·û´®
				sField  -- ×Ö¶ÎÃû
				lData  -- ×Ö¶ÎÖµ
Output:         ÎÞ
Return:         true -- ²éÉèÖÃ±í×Ö¶Î³É¹¦  false -ÉèÖÃÊ§°Ü
***************************************************************/
bool CGbtTscDb::SetFieldSignData(const char* sTable, const char* sField, Long lData)
{
    char   sSql[LARGE_BUF_SIZE];

    if (0 == ACE_OS::strcmp(sTable, "Tbl_Constant"))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tTable(%s) Field(%s) can not be mod\n", SHORT_FILE, __LINE__, sTable, sField));
        return false;
    }
    else
    {
        if (!IsSysParaValid(sField, lData))
        {
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s = %d, invalid\n", SHORT_FILE, __LINE__, sField, lData));
            return false;
        }
    }
    ACE_OS::sprintf(sSql, "update %s set %s=%ld", sTable, sField, lData);

    if (!ExecuteCmd(sSql))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
        return false;
    }

    return true;
}


/**************************************************************
Function:       CGbtTscDb::IsSysParaValid
Description:    ÅÐ¶ÏÉèÖÃµ½ÏµÍ³²ÎÊýÊÇ·ñºÏ·¨
Input:          sField  -- ²ÎÊýÀàÐÍ
				ulValue  -- ²ÎÊýÖµ				
Output:         ÎÞ
Return:         true -- ²ÎÊýºÏ·¨  false -²ÎÊý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsSysParaValid(const char* sField, Ulong& ulValue)
{
    if (0 == ACE_OS::strcmp(sField, SYS_DEGRADE_MODE))
    {
        if (ulValue > 13)
        {
            return false;
        }
        return true;
    }
    else if (0 == ACE_OS::strcmp(sField, SYS_BASE_ADDR))
    {
        if (ulValue > 8192)
        {
            return false;
        }
        return true;
    }
    else if (0 == ACE_OS::strcmp(sField, SYS_SIGDEV_COUNT))
    {
        if (ulValue > 8 || 0 == ulValue)
        {
            return false;
        }
        return true;
    }

    return true;
}

/**************************************************************
Function:       CGbtTscDb::IsSysParaValid
Description:    ÅÐ¶ÏÉèÖÃµ½ÏµÍ³²ÎÊýÊÇ·ñºÏ·¨
Input:          sField  -- ²ÎÊýÀàÐÍ
				lValue  -- ²ÎÊýÖµ				
Output:         ÎÞ
Return:         true -- ²ÎÊýºÏ·¨  false -²ÎÊý·Ç·¨
***************************************************************/
bool CGbtTscDb::IsSysParaValid(const char* sField, Long& lValue)
{
    if (0 == ACE_OS::strcmp(sField, SYS_ZONE))
    {
        if (lValue > 43200 || lValue < -43200)
        {
            return false;
        }
        return true;
    }
    return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPhaseToDirec
Description:    ²éÑ¯ÏàÎ»·½Ïò±íÊý¾Ý
Input:          tblPhaseToDirec - ÏàÎ»·½Ïò±í¶ÔÏó			
Output:         ÎÞ
Return:         true -- ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryPhaseToDirec(TblPhaseToDirec& tblPhaseToDirec)
{
	int    i;
	Byte  uCount;
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;
	PhaseToDirec*  pData = NULL;

	ACE_OS::sprintf(sSql, "select * from %s order by %s", 
							Table_Desc[TBL_PHASE2DIREC].sTblName,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[0]);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_PHASE2DIREC].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PHASE2DIREC].sTblName, qryRst.m_iColum,Table_Desc[TBL_PHASE2DIREC].iFieldCount));
		return false;
	}

	if (qryRst.m_iRow <= 0)
	{
		return false;
	}

	uCount = (Byte)qryRst.m_iRow;
	if (!tblPhaseToDirec.AllocData(uCount))
	{
		return false;
	}

	pData = tblPhaseToDirec.GetData(uCount);
	for (i = 0; i < qryRst.m_iRow; i++)
	{
		pData[i].ucId           = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum]);
		pData[i].ucPhase        = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 1]);
		pData[i].ucOverlapPhase = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 2]);
		pData[i].ucRoadCnt      = ACE_OS::atoi(qryRst.m_ppResult[(i+1)*qryRst.m_iColum + 3]);
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::QueryPhaseToDirec
Description:    ¸ù¾ÝucId²éÑ¯ÏàÎ»·½Ïò±íÊý¾Ý
Input:          ucId - ucIdºÅ			
Output:         sPhaseToDirec  -- ÏàÎ»·½ÏòÊý¾Ý½á¹¹Ìå
Return:         true -- ²éÑ¯Êý¾Ý³É¹¦  false -²éÑ¯Êý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::QueryPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec)
{
	char   sSql[SMALL_BUF_SIZE];
	CSqliteRst  qryRst;

	ACE_OS::sprintf(sSql, "select * from %s where %s=%u", 
							Table_Desc[TBL_PHASE2DIREC].sTblName,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[0], 
							ucId);

	if (!Query(sSql, qryRst))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Query\n", SHORT_FILE, __LINE__));
		return false;
	}

	if (qryRst.m_iColum != Table_Desc[TBL_PHASE2DIREC].iFieldCount)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t%s colum number(%d)!= %d\n", SHORT_FILE, __LINE__, Table_Desc[TBL_PHASE2DIREC].sTblName, qryRst.m_iColum,Table_Desc[TBL_PHASE2DIREC].iFieldCount));
		return false;
	}

	if ( qryRst.m_iRow <= 0 )
	{
		return false;
	}

	sPhaseToDirec.ucId           = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum]);
	sPhaseToDirec.ucPhase        = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 1]);
	sPhaseToDirec.ucOverlapPhase = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 2]);
	sPhaseToDirec.ucRoadCnt      = ACE_OS::atoi(qryRst.m_ppResult[qryRst.m_iColum + 3]);

	return true;
}

/**************************************************************
Function:       CGbtTscDb::AddPhaseToDirec
Description:    Ìí¼ÓÏàÎ»·½Ïò±íÊý¾Ý¼ÇÂ¼
Input:          ucId - ucIdºÅ			
Output:         sPhaseToDirec  -- ÏàÎ»·½ÏòÊý¾Ý½á¹¹Ìå
Return:         true -- Ìí¼ÓÊý¾Ý³É¹¦  false -Ìí¼ÓÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::AddPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec)
{
	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "insert into %s(%s, %s, %s , %s)values(%u,%u,%u,%u)",
							Table_Desc[TBL_PHASE2DIREC].sTblName,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[0],
							Table_Desc[TBL_PHASE2DIREC].sFieldName[1], 
							Table_Desc[TBL_PHASE2DIREC].sFieldName[2],
							Table_Desc[TBL_PHASE2DIREC].sFieldName[3],
							ucId,
							sPhaseToDirec.ucPhase,
							sPhaseToDirec.ucOverlapPhase,
							sPhaseToDirec.ucRoadCnt);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return true;
	}
	return true;
}

/**************************************************************
Function:       CGbtTscDb::ModPhaseToDirec
Description:    ÐÞ¸ÄÏàÎ»·½Ïò±íÊý¾Ý¼ÇÂ¼
Input:          ucId - ucIdºÅ			
Output:         sPhaseToDirec  -- ÏàÎ»·½ÏòÊý¾Ý½á¹¹Ìå
Return:         true -- ÐÞ¸ÄÊý¾Ý³É¹¦  false -ÐÞ¸ÄÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::ModPhaseToDirec(Byte ucId, PhaseToDirec& sPhaseToDirec)
{
	PhaseToDirec sTemp;
	if ( !QueryPhaseToDirec(ucId, sTemp) )
	{
		return AddPhaseToDirec(ucId, sPhaseToDirec);
	}

	char   sSql[LARGE_BUF_SIZE];

	ACE_OS::sprintf(sSql, "update %s set %s=%u, %s=%u , %s=%u where %s=%u",
							Table_Desc[TBL_PHASE2DIREC].sTblName,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[1], sPhaseToDirec.ucPhase,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[2], sPhaseToDirec.ucOverlapPhase,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[3], sPhaseToDirec.ucRoadCnt,
							Table_Desc[TBL_PHASE2DIREC].sFieldName[0], ucId);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

/**************************************************************
Function:       CGbtTscDb::DelPhaseToDirec
Description:    É¾³ýÈ«²¿ÏàÎ»ÓÚ·½Ïò±íÊý¾Ý¼ÇÂ¼
Input:          ÎÞ		
Output:         ÎÞ
Return:         true -- É¾³ýÊý¾Ý³É¹¦  false -É¾³ýÊý¾ÝÊ§°Ü
***************************************************************/
bool CGbtTscDb::DelPhaseToDirec()
{
	char   sSql[LARGE_BUF_SIZE];
	ACE_OS::sprintf(sSql, "delete from %s",Table_Desc[TBL_PHASE2DIREC].sTblName);

	if (!ExecuteCmd(sSql))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ExecuteCmd\n", SHORT_FILE, __LINE__));
		return false;
	}

	return true;
}

}
