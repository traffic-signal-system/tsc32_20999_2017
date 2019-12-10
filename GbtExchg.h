#pragma once
#include "GbtDb.h"

namespace GBT_DB
{
int Packet(DbString& sString, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, DbString& sString);

int Packet(Module& sModule, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Module& sModule);

int Packet(Plan& sPlan, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Plan& sPlan);

int Packet(Schedule& sSchedule, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Schedule& sSchedule);

int Packet(EventType& sEvtType, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, EventType& sEvtType);

int Packet(SpecFun& sSpecFun, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, SpecFun& sSpecFun);

int Packet(DetExtend& sDetExtend, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, EventLog& sEvtLog);

int Packet(AdaptStageTime& sAdaptStageTime, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, AdaptStageTime& sAdaptStageTime);

int Packet(EventLog& sEvtLog, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, EventLog& sEvtLog);

int Packet(Phase& sPhase, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Phase& sPhase);

int Packet(Collision& sCollision, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Collision& sCollision); 

int Packet(Detector& sDetector, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Detector& sDetector);

int Packet(Channel& sChannel, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Channel& sChannel);

int Packet(Pattern& sPattern, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, Pattern& sPattern);

int Packet(OverlapPhase& sOverlapPhase, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, OverlapPhase& sOverlapPhase);

int Packet(StagePattern& sStagePat, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, StagePattern& sStagePat);

int Packet(AdaptPara& sAdaptPara, Byte* sValueSeq, int iValSize);
int Unpacket(Byte* sValueSeq, int iValSize, AdaptPara& sAdaptPara);

int Unpacket(Byte* sValueSeq, int iValSize, ChannelChk& sChannelChk);
int Packet(ChannelChk& sChannelChk, Byte* sValueSeq, int iValSize);

int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte& uErrorSts, Byte& uErrorIdx);
int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte& uErrorSts, Byte& uErrorIdx);
int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);


int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte& uErrorSts, Byte& uErrorIdx);
int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);

int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte& uErrorSts, Byte& uErrorIdx);
int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);

int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte& uErrorSts, Byte& uErrorIdx);
int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte& uErrorSts, Byte& uErrorIdx);
int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);


int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte& uErrorSts, Byte& uErrorIdx);
int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte& uErrorSts, Byte& uErrorIdx);
int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);

int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte& uErrorSts, Byte& uErrorIdx);
int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte& uErrorSts, Byte& uErrorIdx);
int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte& uErrorSts, Byte& uErrorIdx);
int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte& uErrorSts, Byte& uErrorIdx);
int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
 
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte& uErrorSts, Byte& uErrorIdx);
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte& uErrorSts, Byte& uErrorIdx);
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);
 

int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx);
int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx);
int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);


int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx);
int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx);
int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte& uErrorSts, Byte& uErrorIdx);
int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte& uErrorSts, Byte& uErrorIdx);
int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte& uErrorSts, Byte& uErrorIdx);
int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte& uErrorSts, Byte& uErrorIdx);
int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte& uErrorSts, Byte& uErrorIdx);
int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte& uErrorSts, Byte& uErrorIdx);
int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte& uErrorSts, Byte& uErrorIdx);
int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte& uErrorSts, Byte& uErrorIdx);
int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte& uErrorSts, Byte& uErrorIdx);
int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte& uErrorSts, Byte& uErrorIdx);
int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int GetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); 
int SetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetFollow(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte& uErrorSts, Byte& uErrorIdx);
int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFollowId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx);

int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx); //ADD:20130801
int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte& uErrorSts, Byte& uErrorIdx); //ADD:20130801
int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte uSubId,Byte& uErrorSts, Byte& uErrorIdx); //ADD:20130801

int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte& uErrorSts, Byte& uErrorIdx); //ADD:20130801
int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte uSubId,Byte& uErrorSts, Byte& uErrorIdx); //ADD:20130801

int ExchangeDeviceId(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxModule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeSynchTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeSynchFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeModule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeZone(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxPlan(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxSchedule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxSegment(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePlan(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeSchedule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxEvtType(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxEvtLog(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeEventType(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeEventLog(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxPhaseGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeCollision(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxDetector(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxDetGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDetDataSeqNo(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDetDataCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeActiveDetCount(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePulseSeqNo(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePulseCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDetector(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeFlsTimeWhenStart(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeAllRedTimeWhenStart(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeRemoteCtrlFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeFlashFreq(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeBrtCtrlBgnTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeBrtCtrlEndTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxChannel(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxChanlGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeChannel(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxPattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxStagePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxStage(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeGlobalCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeCoorPhaseOffset(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDegradeMode(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDegradePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeCtrlMasterOptFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeSigDevCount(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeBaseAddr(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxOverlapPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeMaxOverlapPhaseGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeOverlapPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeStagePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDownloadFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangePhaseToDirec(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeDetExtend(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeAdaptPara(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeCntDownDev(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeAdaptStage(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);
int ExchangeSpecFun(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);


int ExchangeChannelChk(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx);


}
