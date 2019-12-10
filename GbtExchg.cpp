#include <ace/OS.h>
#include <ace/Log_Msg.h>

#include "GbtExchg.h"

#ifdef SHORT_FILE
#undef SHORT_FILE
#endif

#define SHORT_FILE "GbtExchg.cpp"

namespace GBT_DB
{

const Ushort PLAN_SIZE  = 9;
const Ushort SCHEDULE_SIZE  = 8;
const Ushort EVENTLOG_SIZE = 10;
const Ushort PHASE_SIZE = 12;
const Ushort COLLISION_SIZE = 3;
const Ushort DETECTOR_SIZE = 9;
const Ushort CHANNEL_SIZE = 4;
const Ushort PATTERN_SIZE = 5;
const Ushort STAGEPATTERN_SIZE = 8;
const Ushort CHANNELCHK_SIZE =2;
/*
	数据操作类型枚举
*/
enum
{
    OPERATE_SET_VALUE     = 0,    //设置值
    OPERATE_GET_VALUE     = 1,    //获取值
    OPERATE_GET_LENGTH    = 2     //获取长度
};

/*
	GBT消息处理结果枚举
*/
enum
{
    GBT_MSG_SUCCESS    = 0,      //消息正确
    GBT_MSG_SIZE_LONG  = 1,      //消息长度过长
    GBT_MSG_TYPE_ERROR = 2,      //消息类型错误
    GBT_MSG_OBJ_ERROR  = 3,      //消息对象错误
    GBT_MSG_SIZE_SHORT = 4,      //消息长度过短
    GBT_MSG_ERROR_OTHER = 5      //其他类型错误
};

#define EXCHAGE_ONE_OBJECT_BASE(TYPE,TABLE,FIELD,GETDATA,SETDATA,BASE)\
{\
    Ulong uTempData;\
    int iValSize = sizeof(TYPE);\
    uErrorSts    = GBT_MSG_SUCCESS;\
    uErrorIdx    = 0;\
    if (uDataSize < iValSize)\
    {\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
    if (NULL == m_gTscDb)\
    {\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tDb not exist\n", SHORT_FILE, __LINE__));\
        return -1;\
    }\
    switch (iOprType)\
    {\
    case OPERATE_SET_VALUE:\
        if ((uSubId > 1 && uSubId < 255) || (uIdxFst > 0 && uIdxFst < 255) || 1 != BASE)\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tiOprType=%d, uIdxFst =%d , uSubId= %d, BASE=%d\n", SHORT_FILE, __LINE__,iOprType,uIdxFst,uSubId, BASE));\
            return -1;\
        }\
        UNPACKET_DATA(iValSize, pData, uTempData);\
        if (!SETDATA(TABLE, FIELD, uTempData))\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            return -1;\
        }\
        return iValSize;\
    case OPERATE_GET_VALUE:\
        if ((uSubId > 1 && uSubId < 255) || (uIdxFst > 0 && uIdxFst < 255))\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tiOprType=%d, uIdxFst =%d , uSubId= %d\n", SHORT_FILE, __LINE__,iOprType,uIdxFst,uSubId));\
            return -1;\
        }\
        if (!GETDATA(TABLE, FIELD, uTempData))\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            return -1;\
        }\
        uTempData = (uTempData+(BASE-1))/(BASE);\
        PACKET_DATA(iValSize, pData, uTempData);\
        return iValSize;\
    case OPERATE_GET_LENGTH:\
        return iValSize;\
    default:\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
}

#define EXCHAGE_ONE_SIGN_OBJECT_BASE(TYPE,TABLE,FIELD,GETDATA,SETDATA,BASE)\
{\
    Long uTempData;\
    int iValSize = sizeof(TYPE);\
    uErrorSts = GBT_MSG_SUCCESS;\
    uErrorIdx = 0;\
    if (uDataSize < iValSize)\
    {\
        uErrorSts = GBT_MSG_SIZE_SHORT;\
        return -1;\
    }\
    if (NULL == m_gTscDb)\
    {\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
    switch (iOprType)\
    {\
    case OPERATE_SET_VALUE:\
        if ((uSubId > 1 && uSubId < 255) || (uIdxFst > 0 && uIdxFst < 255) || 1 != BASE)\
        {\
            uErrorSts = GBT_MSG_OBJ_ERROR;\
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tiOprType=%d, uIdxFst =%d , uSubId= %d, BASE=%d\n", SHORT_FILE, __LINE__,iOprType,uIdxFst,uSubId, BASE));\
            return -1;\
        }\
        UNPACKET_DATA(iValSize, pData, uTempData);\
        if (!SETDATA(TABLE, FIELD, uTempData))\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            return -1;\
        }\
        return iValSize;\
    case OPERATE_GET_VALUE:\
        if ((uSubId > 1 && uSubId < 255) || (uIdxFst > 0 && uIdxFst < 255))\
        {\
            uErrorSts = GBT_MSG_OBJ_ERROR;\
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tiOprType=%d, uIdxFst =%d , uSubId= %d\n", SHORT_FILE, __LINE__,iOprType,uIdxFst,uSubId));\
            return -1;\
        }\
        if (!GETDATA(TABLE, FIELD, uTempData))\
        {\
            uErrorSts = GBT_MSG_ERROR_OTHER;\
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to GETDATA\n", SHORT_FILE, __LINE__));\
            return -1;\
        }\
        uTempData = (uTempData+(BASE-1))/(BASE);\
        PACKET_DATA(iValSize, pData, uTempData);\
        return iValSize;\
    case OPERATE_GET_LENGTH:\
        return iValSize;\
    default:\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
}


#define EXCHAGE_SYSTEM_SIGN_OBJECT(TYPE,FIELD)\
EXCHAGE_ONE_SIGN_OBJECT_BASE(TYPE,m_gTableDesc[TBL_SYSTEM].sTblName,FIELD,m_gTscDb->GetFieldSignData,m_gTscDb->SetFieldSignData,1)

#define EXCHANGE_SYSTEM_ONE_OBJ(TYPE,FIELD)\
EXCHAGE_ONE_OBJECT_BASE(TYPE,m_gTableDesc[TBL_SYSTEM].sTblName,FIELD,m_gTscDb->GetFieldData,m_gTscDb->SetFieldData,1)

#define EXCHANGE_CONST_ONE_OBJ(FIELD)\
EXCHAGE_ONE_OBJECT_BASE(Byte,m_gTableDesc[TBL_CONSTANT].sTblName,FIELD,m_gTscDb->GetFieldData,m_gTscDb->SetFieldData,1)

#define EXCHANGE_CONST_ONE_OBJGRP(FIELD)\
EXCHAGE_ONE_OBJECT_BASE(Byte,m_gTableDesc[TBL_CONSTANT].sTblName,FIELD,m_gTscDb->GetFieldData,m_gTscDb->SetFieldData,8)


#define EXCHANGE_ONEIDX_OBJ(GETOBJ, SETOBJ)\
{\
    uErrorSts = GBT_MSG_SUCCESS;\
    uErrorIdx = 0;\
    if (NULL == m_gTscDb)\
    {\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tDb not exist\n", SHORT_FILE, __LINE__));\
        return -1;\
    }\
    switch (iOprType)\
    {\
    case OPERATE_SET_VALUE:\
        if (0 == uIdxFst || 255 == uIdxFst)\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize,uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    uErrorSts = GBT_MSG_ERROR_OTHER;\
                    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t不支持整表单字段设置\n", SHORT_FILE, __LINE__));\
                    return -1;\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t只有一个索引,无法设置\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        else\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uSubId, uErrorSts, uErrorIdx);\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t只有一个索引,无法设置\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        break;\
    case OPERATE_GET_VALUE:\
        if (0 == uIdxFst || 255 == uIdxFst)\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize,uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    uErrorSts = GBT_MSG_ERROR_OTHER;\
                    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t不支持整表单字段查询\n", SHORT_FILE, __LINE__));\
                    return -1;\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t只有一个索引,无法查询\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        else\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uSubId, uErrorSts, uErrorIdx);\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t只有一个索引,无法查询\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        break;\
    default:\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
}

#define EXCHANGE_TWOIDX_OBJ(GETOBJ, SETOBJ)\
{\
    uErrorSts = GBT_MSG_SUCCESS;\
    uErrorIdx = 0;\
    if (NULL == m_gTscDb)\
    {\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
    switch (iOprType)\
    {\
    case OPERATE_SET_VALUE:\
        if (0 == uIdxFst || 255 == uIdxFst)\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize,uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    uErrorSts = GBT_MSG_ERROR_OTHER;\
                    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t不支持整表单字段设置\n", SHORT_FILE, __LINE__));\
                    return -1;\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t第一个索引必须有效\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        else\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uSubId, uErrorSts, uErrorIdx);\
                }\
            }\
            else\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uIdxSnd, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    return SETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uIdxSnd, uSubId, uErrorSts, uErrorIdx);\
                }\
            }\
        }\
        break;\
    case OPERATE_GET_VALUE:\
        if (0 == uIdxFst || 255 == uIdxFst)\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize,uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    uErrorSts = GBT_MSG_ERROR_OTHER;\
                    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t不支持整表单字段查询\n", SHORT_FILE, __LINE__));\
                    return -1;\
                }\
            }\
            else\
            {\
                uErrorSts = GBT_MSG_ERROR_OTHER;\
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\t第一个索引必须有效\n", SHORT_FILE, __LINE__));\
                return -1;\
            }\
        }\
        else\
        {\
            if (0 == uIdxSnd || 255 == uIdxSnd)\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
					uErrorSts = GBT_MSG_ERROR_OTHER;\
					ACE_DEBUG ((LM_DEBUG, "%s:%04d\t不支持整表单字段查询\n", SHORT_FILE, __LINE__));\
					return -1;\
                }\
            }\
            else\
            {\
                if (0 == uSubId || 255 == uSubId)\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uIdxSnd, uErrorSts, uErrorIdx);\
                }\
                else\
                {\
                    return GETOBJ(m_gTscDb, pData, uDataSize, uIdxFst, uIdxSnd, uSubId, uErrorSts, uErrorIdx);\
                }\
            }\
        }\
        break;\
    default:\
        uErrorSts = GBT_MSG_ERROR_OTHER;\
        return -1;\
    }\
}


int Unpacket(Byte* sValueSeq, int iValSize, DbString& sString)
{
    int iPos = 0;
    if (iValSize < 1)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    Ushort usCount = sValueSeq[iPos++];
    if (iValSize < (iPos+usCount))
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -2;
    }

    sString.AllocData(usCount+1);
    char* pData = sString.GetData();
    ACE_OS::memcpy(pData, sValueSeq+iPos, usCount);
    iPos += usCount;

    return iPos;
}
int Packet(DbString& sString, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    Ushort uCount;
    Char* pData = sString.GetData(uCount);

    if (uCount > 0 && '\0' == pData[uCount-1])
    {
        uCount -= 1;
    }
    
    sValueSeq[iPos++] = (Byte)uCount;
    
    if ((iPos+uCount) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    ACE_OS::memcpy(sValueSeq+iPos, pData, uCount);
    iPos += uCount;

    return iPos;
}

int Packet(Module& sModule, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    int iPackSize;
    
    /*模块表ID*/
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sModule.ucModuleId;

    //设备节点
    iPackSize = Packet( sModule.strDevNode, sValueSeq+iPos, iValSize-iPos);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-1);
    }
    iPos += iPackSize;

    //制造商
    iPackSize = Packet(sModule.strCompany, sValueSeq+iPos, iValSize-iPos);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-3);
    }
    iPos += iPackSize;

    //型号
    iPackSize = Packet(sModule.strModel, sValueSeq+iPos, iValSize-iPos);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-5);
    }
    iPos += iPackSize;

    //版本
    iPackSize = Packet(sModule.strVersion, sValueSeq+iPos, iValSize-iPos);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-7);
    }
    iPos += iPackSize;

    /*类型*/
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -10;
    }
    sValueSeq[iPos++] = sModule.ucType;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Module& sModule)
{
    int iPos = 0;
    int iPackSize;
    
    /*模块表ID*/
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sModule.ucModuleId = sValueSeq[iPos++];

    //设备节点
    iPackSize = Unpacket(sValueSeq+iPos, iValSize-iPos, sModule.strDevNode);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-1);
    }
    iPos += iPackSize;

    //制造商
    iPackSize = Unpacket(sValueSeq+iPos, iValSize-iPos, sModule.strCompany);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-3);
    }
    iPos += iPackSize;

    //型号
    iPackSize = Unpacket(sValueSeq+iPos, iValSize-iPos, sModule.strModel);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-5);
    }
    iPos += iPackSize;

    //版本
    iPackSize = Unpacket(sValueSeq+iPos, iValSize-iPos, sModule.strVersion);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return (iPackSize-7);
    }
    iPos += iPackSize;

    /*类型*/
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -10;
    }
    sModule.ucType = sValueSeq[iPos++];
    return iPos;
}


int Unpacket(Byte* sValueSeq, int iValSize, Plan& sPlan)
{
    int iPos = 0;
    if (iValSize < (int)PLAN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    sPlan.ucId = sValueSeq[iPos++];

    UNPACKET_USHORT(sValueSeq+iPos, sPlan.usMonthFlag);
    iPos += sizeof(sPlan.usMonthFlag);

    sPlan.ucWeekFlag = sValueSeq[iPos++];
    UNPACKET_ULONG(sValueSeq+iPos, sPlan.ulDayFlag);
    iPos += sizeof(sPlan.ulDayFlag);
    
    sPlan.ucScheduleId = sValueSeq[iPos++];
    return iPos;
}

int Packet(Plan& sPlan, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)PLAN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    sValueSeq[iPos++] = sPlan.ucId;

    PACKET_USHORT(sValueSeq+iPos, sPlan.usMonthFlag);
    iPos += sizeof(sPlan.usMonthFlag);

    sValueSeq[iPos++] = sPlan.ucWeekFlag;

    PACKET_ULONG(sValueSeq+iPos, sPlan.ulDayFlag);
    iPos += sizeof(sPlan.ulDayFlag);
    
    sValueSeq[iPos++] = sPlan.ucScheduleId;
    return iPos;
}

int Packet(Schedule& sSchedule, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)SCHEDULE_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    sValueSeq[iPos++] = sSchedule.ucScheduleId;
    sValueSeq[iPos++] = sSchedule.ucEvtId;
    sValueSeq[iPos++] = sSchedule.ucBgnHour;
    sValueSeq[iPos++] = sSchedule.ucBgnMinute;
    sValueSeq[iPos++] = sSchedule.ucCtrlMode;
    sValueSeq[iPos++] = sSchedule.ucPatternNo;
    sValueSeq[iPos++] = sSchedule.ucAuxOut;
    sValueSeq[iPos++] = sSchedule.ucSpecialOut;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Schedule& sSchedule)
{
    int iPos = 0;
    if (iValSize < (int)SCHEDULE_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
   ACE_DEBUG ((LM_DEBUG, "\r\n%s:%04d  iValSize=%d  \r\n", SHORT_FILE, __LINE__,iValSize));
    sSchedule.ucScheduleId = sValueSeq[iPos++];
    sSchedule.ucEvtId = sValueSeq[iPos++];
    sSchedule.ucBgnHour = sValueSeq[iPos++];
    sSchedule.ucBgnMinute = sValueSeq[iPos++];
    sSchedule.ucCtrlMode = sValueSeq[iPos++];
    sSchedule.ucPatternNo = sValueSeq[iPos++];
    sSchedule.ucAuxOut = sValueSeq[iPos++];
    sSchedule.ucSpecialOut = sValueSeq[iPos++];
    return iPos;
}

int Packet(EventType& sEvtType, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    int iPackSize;

    if ((iPos+1) > iValSize)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }
    sValueSeq[iPos++] = sEvtType.ucEvtTypeId;

    if ((iPos+4) > iValSize)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }

    PACKET_ULONG(sValueSeq+iPos, sEvtType.ulClearTime);
    iPos += 4;

    iPackSize = Packet(sEvtType.strEvtDesc, sValueSeq+iPos, iValSize-iPos);
    if (iPackSize < 0)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }
    iPos += iPackSize;


    if ((iPos+1) > iValSize)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }
    sValueSeq[iPos++] = sEvtType.ucLogCount;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, EventType& sEvtType)
{
    int iPos = 0;
    int iPackSize;
    if (iValSize < (iPos+1))
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sEvtType.ucEvtTypeId = sValueSeq[iPos++];

    if (iValSize < (iPos+4))
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    UNPACKET_ULONG(sValueSeq+iPos, sEvtType.ulClearTime);
    iPos += 4;

    iPackSize = Unpacket(sValueSeq+iPos, iValSize-iPos, sEvtType.strEvtDesc);
    if (iPackSize < 0)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    iPos += iPackSize;


    if (iValSize < (iPos+1))
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sEvtType.ucLogCount = sValueSeq[iPos++];
    return iPos;
}

int Packet(SpecFun& sSpecFun, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sSpecFun.ucFunType;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sSpecFun.ucValue;
	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, SpecFun& sSpecFun)
{
	int iPos = 0;
	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sSpecFun.ucFunType = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sSpecFun.ucValue = sValueSeq[iPos++];

	return iPos;
}

int Packet(DetExtend& sDetExtend, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sDetExtend.ucId;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sDetExtend.ucSensi;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sDetExtend.ucGrpNo;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sDetExtend.ucPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sDetExtend.ucOcuDefault;

	if ((iPos+2) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	PACKET_USHORT(sValueSeq+iPos, sDetExtend.usCarFlow);
	iPos += 2;

	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, DetExtend& sDetExtend)
{
	int iPos = 0;

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sDetExtend.ucId = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sDetExtend.ucSensi = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sDetExtend.ucGrpNo = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sDetExtend.ucPro = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sDetExtend.ucOcuDefault = sValueSeq[iPos++];

	if (iValSize < (iPos+2))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	UNPACKET_USHORT(sValueSeq+iPos, sDetExtend.usCarFlow);
	iPos += 2;

	return iPos;
}

int Packet(AdaptPara& sAdaptPara, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucType;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucFirstPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucSecPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucThirdPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucOcuPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucCarFlowPro;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptPara.ucSmoothPara;

	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, AdaptPara& sAdaptPara)
{
	int iPos = 0;

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucType = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucFirstPro  = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucSecPro = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucThirdPro = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucOcuPro = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucCarFlowPro = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptPara.ucSmoothPara = sValueSeq[iPos++];

	return iPos;
}


int Packet(CntDownDev& sCntDownDev, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sCntDownDev.ucDevId;

	if ((iPos+2) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	PACKET_USHORT(sValueSeq+iPos, sCntDownDev.usPhase);
	iPos += 2;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sCntDownDev.ucOverlapPhase;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sCntDownDev.ucMode;

	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, CntDownDev& sCntDownDev)
{
	int iPos = 0;

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sCntDownDev.ucDevId = sValueSeq[iPos++];

	if (iValSize < (iPos+2))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	UNPACKET_USHORT(sValueSeq+iPos, sCntDownDev.usPhase);
	iPos += 2;

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sCntDownDev.ucOverlapPhase = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sCntDownDev.ucMode = sValueSeq[iPos++];

	return iPos;
}

int Packet(AdaptStageTime& sAdaptStageTime, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;
	Byte  ucStageArray[16] = {0};

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptStageTime.ucId;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptStageTime.ucWeekType;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptStageTime.ucHour;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptStageTime.ucMin;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sAdaptStageTime.ucStageCnt;

	if ((iPos+16) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	GetFieldsValue(sAdaptStageTime.sStageGreenTime.GetData(), ',', ucStageArray, 16);
	ACE_OS::memcpy(sValueSeq+iPos , ucStageArray , 16);
	iPos += 16;

	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, AdaptStageTime& sAdaptStageTime)
{
	int iPos = 0;
	char sStr[64] = {0};

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptStageTime.ucId = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptStageTime.ucWeekType = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptStageTime.ucHour = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptStageTime.ucMin = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sAdaptStageTime.ucStageCnt = sValueSeq[iPos++];

	if (iValSize < (iPos+16))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ACE_OS::sprintf(sStr,"%d,%d,%d,%d,,%d,%d,%d,,%d,%d,%d,%d,%d,%d,%d,%d,%d"
			,sValueSeq[iPos],sValueSeq[iPos+1],sValueSeq[iPos+2],sValueSeq[iPos+3],sValueSeq[iPos+4]
			,sValueSeq[iPos+5],sValueSeq[iPos+6],sValueSeq[iPos+7],sValueSeq[iPos+8],sValueSeq[iPos+9]
			,sValueSeq[iPos+10],sValueSeq[iPos+11],sValueSeq[iPos+12],sValueSeq[iPos+13],sValueSeq[iPos+14]
			,sValueSeq[iPos+15]);
	sAdaptStageTime.sStageGreenTime.SetString(sStr);
	iPos += 16;

	return iPos;
}

int Packet(PhaseToDirec& sPhaseToDirec, Byte* sValueSeq, int iValSize)
{
	int iPos = 0;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sPhaseToDirec.ucId;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sPhaseToDirec.ucPhase;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sPhaseToDirec.ucOverlapPhase;

	if ((iPos+1) > iValSize)
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sValueSeq[iPos++] = sPhaseToDirec.ucRoadCnt;

	return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, PhaseToDirec& sPhaseToDirec)
{
	int iPos = 0;
	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sPhaseToDirec.ucId = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sPhaseToDirec.ucPhase = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sPhaseToDirec.ucOverlapPhase = sValueSeq[iPos++];

	if (iValSize < (iPos+1))
	{
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	sPhaseToDirec.ucRoadCnt = sValueSeq[iPos++];

	return iPos;
}

int Packet(EventLog& sEvtLog, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)EVENTLOG_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    sValueSeq[iPos++] = sEvtLog.ucEventId;
    sValueSeq[iPos++] = sEvtLog.ucEvtType;
    
    PACKET_ULONG(sValueSeq+iPos, sEvtLog.ulHappenTime);
    iPos += 4;

    PACKET_ULONG(sValueSeq+iPos, sEvtLog.ulEvtValue);
    iPos += 4;
    return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, EventLog& sEvtLog)
{
    int iPos = 0;
    if (iValSize < (int)EVENTLOG_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    
    sEvtLog.ucEventId = sValueSeq[iPos++];
    sEvtLog.ucEvtType = sValueSeq[iPos++];
    UNPACKET_ULONG(sValueSeq+iPos, sEvtLog.ulHappenTime);
    iPos += 4;

    UNPACKET_ULONG(sValueSeq+iPos, sEvtLog.ulEvtValue);
    iPos += 4;
    return iPos;
}

int Packet(Phase& sPhase, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)PHASE_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sPhase.ucPhaseId;
    sValueSeq[iPos++] = sPhase.ucPedestrianGreen;
    sValueSeq[iPos++] = sPhase.ucPedestrianClear;
    sValueSeq[iPos++] = sPhase.ucMinGreen;
    sValueSeq[iPos++] = sPhase.ucGreenDelayUnit;
    sValueSeq[iPos++] = sPhase.ucMaxGreen1;
    sValueSeq[iPos++] = sPhase.ucMaxGreen2;
    sValueSeq[iPos++] = sPhase.ucFixGreen;
    sValueSeq[iPos++] = sPhase.ucGreenFlash;
    sValueSeq[iPos++] = sPhase.ucPhaseTypeFlag;
    sValueSeq[iPos++] = sPhase.ucPhaseOption;
    sValueSeq[iPos++] = sPhase.ucExtend;

    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Phase& sPhase)
{
    int iPos = 0;
    if (iValSize < (int)PHASE_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sPhase.ucPhaseId = sValueSeq[iPos++];
    sPhase.ucPedestrianGreen = sValueSeq[iPos++];
    sPhase.ucPedestrianClear = sValueSeq[iPos++];
    sPhase.ucMinGreen = sValueSeq[iPos++];
    sPhase.ucGreenDelayUnit = sValueSeq[iPos++];
    sPhase.ucMaxGreen1 = sValueSeq[iPos++];
    sPhase.ucMaxGreen2 = sValueSeq[iPos++];
    sPhase.ucFixGreen = sValueSeq[iPos++];
    sPhase.ucGreenFlash = sValueSeq[iPos++];
    sPhase.ucPhaseTypeFlag = sValueSeq[iPos++];
    sPhase.ucPhaseOption = sValueSeq[iPos++];
    sPhase.ucExtend = sValueSeq[iPos++];
    return iPos;
}

int Packet(Collision& sCollision, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)COLLISION_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sCollision.ucPhaseId;
    PACKET_ULONG(sValueSeq+iPos, sCollision.usCollisionFlag);
    iPos += 4;

    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Collision& sCollision)
{
    int iPos = 0;
    if (iValSize < (int)COLLISION_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sCollision.ucPhaseId = sValueSeq[iPos++];
    UNPACKET_ULONG(sValueSeq+iPos, sCollision.usCollisionFlag);  
    iPos += 4;

    return iPos;
}

int Packet(Detector& sDetector, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)DETECTOR_SIZE)
    {
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }

    sValueSeq[iPos++] = sDetector.ucDetectorId;
    sValueSeq[iPos++] = sDetector.ucPhaseId;
    sValueSeq[iPos++] = sDetector.ucDetFlag;
    sValueSeq[iPos++] = sDetector.ucDirect;
    sValueSeq[iPos++] = sDetector.ucValidTime;
    sValueSeq[iPos++] = sDetector.ucOptionFlag;
    PACKET_USHORT(sValueSeq+iPos, sDetector.usSaturationFlow);
    iPos += 2;
    sValueSeq[iPos++] = sDetector.ucSaturationOccupy;

    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Detector& sDetector)
{
    int iPos = 0;
    if (iValSize < (int)DETECTOR_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sDetector.ucDetectorId = sValueSeq[iPos++];
    sDetector.ucPhaseId = sValueSeq[iPos++];
    sDetector.ucDetFlag = sValueSeq[iPos++];
    sDetector.ucDirect = sValueSeq[iPos++];
    sDetector.ucValidTime = sValueSeq[iPos++];
    sDetector.ucOptionFlag = sValueSeq[iPos++];
    UNPACKET_USHORT(sValueSeq+iPos, sDetector.usSaturationFlow);
    iPos += 2;
    sDetector.ucSaturationOccupy = sValueSeq[iPos++];
    return iPos;
}



int Packet(ChannelChk& sChannelChk, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)CHANNELCHK_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sValueSeq[iPos++] = sChannelChk.ucSubChannelId;
    sValueSeq[iPos++] = sChannelChk.ucIsCheck;
    
    return iPos;

}

int Unpacket(Byte* sValueSeq, int iValSize, ChannelChk& sChannelChk)
{
		int iPos = 0;
		if (iValSize < (int)CHANNELCHK_SIZE)
		{
#ifdef TSC_DEBUG
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
			return -1;
		}
	
		sChannelChk.ucSubChannelId = sValueSeq[iPos++];
		sChannelChk.ucIsCheck = sValueSeq[iPos++];
		
		return iPos;
}


int Packet(Channel& sChannel, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)CHANNEL_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sValueSeq[iPos++] = sChannel.ucChannelId;
    sValueSeq[iPos++] = sChannel.ucCtrlSrc;
    sValueSeq[iPos++] = sChannel.ucAutoFlsCtrlFlag;
    sValueSeq[iPos++] = sChannel.ucCtrlType;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, Channel& sChannel)
{
    int iPos = 0;
    if (iValSize < (int)CHANNEL_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sChannel.ucChannelId = sValueSeq[iPos++];
    sChannel.ucCtrlSrc = sValueSeq[iPos++];
    sChannel.ucAutoFlsCtrlFlag = sValueSeq[iPos++];
    sChannel.ucCtrlType = sValueSeq[iPos++];
    return iPos;
}

int Packet(Pattern& sPattern, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)PATTERN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sValueSeq[iPos++] = sPattern.ucPatternId;
    sValueSeq[iPos++] = sPattern.ucCycleTime;
    sValueSeq[iPos++] = sPattern.ucOffset;
    sValueSeq[iPos++] = sPattern.ucCoorPhase;
    sValueSeq[iPos++] = sPattern.ucStagePatternId;
    return iPos;
}

int Unpacket(Byte* sValueSeq, int iValSize, Pattern& sPattern)
{
    int iPos = 0;
    if (iValSize < (int)PATTERN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sPattern.ucPatternId = sValueSeq[iPos++];
    sPattern.ucCycleTime = sValueSeq[iPos++];
    sPattern.ucOffset = sValueSeq[iPos++];
    sPattern.ucCoorPhase = sValueSeq[iPos++];
    sPattern.ucStagePatternId = sValueSeq[iPos++];
    return iPos;
}

int Packet(OverlapPhase& sOverlapPhase, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucOverlapPhaseId;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucOperateType;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucIncldPhaseCnt;

    if ((iPos+sOverlapPhase.ucIncldPhaseCnt) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    ACE_OS::memcpy(sValueSeq+iPos, sOverlapPhase.ucIncldPhase, MAX_PHASE_COUNT);
    iPos += MAX_PHASE_COUNT;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucAdjustPhaseCnt;

    if ((iPos+MAX_PHASE_COUNT) > iValSize)
    {
        return -1;
    }
    ACE_OS::memcpy(sValueSeq+iPos, sOverlapPhase.ucAdjustPhase, MAX_PHASE_COUNT);
    iPos += MAX_PHASE_COUNT;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucTailGreen;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucTailYellow;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sValueSeq[iPos++] = sOverlapPhase.ucTailRed;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, OverlapPhase& sOverlapPhase)
{
    int iPos = 0;
    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucOverlapPhaseId = sValueSeq[iPos++];

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucOperateType = sValueSeq[iPos++];

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucIncldPhaseCnt = sValueSeq[iPos++];

    if (sOverlapPhase.ucIncldPhaseCnt > MAX_PHASE_COUNT)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucIncldPhaseCnt(%d) too large\n", SHORT_FILE, __LINE__,sOverlapPhase.ucIncldPhaseCnt));  
#endif
        return -1;
    }

    if ((iPos+MAX_PHASE_COUNT) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    ACE_OS::memcpy(sOverlapPhase.ucIncldPhase, sValueSeq+iPos, MAX_PHASE_COUNT);
    iPos += MAX_PHASE_COUNT;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucAdjustPhaseCnt = sValueSeq[iPos++];

    if (sOverlapPhase.ucAdjustPhaseCnt > MAX_PHASE_COUNT)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucAdjustPhaseCnt(%d) too large\n", SHORT_FILE, __LINE__,sOverlapPhase.ucAdjustPhaseCnt));  
#endif
        return -1;
    }

    if ((iPos+MAX_PHASE_COUNT) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    ACE_OS::memcpy(sOverlapPhase.ucAdjustPhase, sValueSeq+iPos, MAX_PHASE_COUNT);
    iPos += MAX_PHASE_COUNT;

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucTailGreen = sValueSeq[iPos++];

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucTailYellow = sValueSeq[iPos++];

    if ((iPos+1) > iValSize)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    sOverlapPhase.ucTailRed = sValueSeq[iPos++];
    return iPos;
}

int Packet(StagePattern& sStagePat, Byte* sValueSeq, int iValSize)
{
    int iPos = 0;
    if (iValSize < (int)STAGEPATTERN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sValueSeq[iPos++]           = sStagePat.ucStagePatternId;
    sValueSeq[iPos++]           = sStagePat.ucStageNo;
    PACKET_ULONG(sValueSeq+iPos, sStagePat.usAllowPhase);
    iPos += 4;

    sValueSeq[iPos++]           = sStagePat.ucGreenTime ;
    sValueSeq[iPos++]           = sStagePat.ucYellowTime;
    sValueSeq[iPos++]           = sStagePat.ucRedTime;
    sValueSeq[iPos++]           = sStagePat.ucOption;
    return iPos;
}
int Unpacket(Byte* sValueSeq, int iValSize, StagePattern& sStagePat)
{
    int iPos = 0;
    if (iValSize < (int)STAGEPATTERN_SIZE)
    {
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }

    sStagePat.ucStagePatternId = sValueSeq[iPos++];
    sStagePat.ucStageNo = sValueSeq[iPos++];
    UNPACKET_ULONG(sValueSeq+iPos, sStagePat.usAllowPhase);
    iPos += 4;

    sStagePat.ucGreenTime = sValueSeq[iPos++];
    sStagePat.ucYellowTime = sValueSeq[iPos++];
    sStagePat.ucRedTime = sValueSeq[iPos++];
    sStagePat.ucOption = sValueSeq[iPos++];
    return iPos;
}


int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblModule tblModule;
    Module*   pModule;
    Byte   ucCount, i;
    if (!pTscDb->QueryModule(tblModule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryModule\n", SHORT_FILE, __LINE__)); 
#endif
        return -1;
    }

    pModule = tblModule.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pModule[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Module(%d)\n", SHORT_FILE, __LINE__, pModule[i].ucModuleId));  
#endif
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Module sModule;

    if (!pTscDb->QueryModule(uModuleId, sModule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryModule(%d)\n", SHORT_FILE, __LINE__, uModuleId));   
#endif
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sModule, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Module(%d)\n", SHORT_FILE, __LINE__, sModule.ucModuleId));   
#endif
		return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Module sModule;

    if (!pTscDb->QueryModule(uModuleId, sModule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryModule(%d)\n", SHORT_FILE, __LINE__, uModuleId)); 
#endif
        return -1;
    }
    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sModule.ucModuleId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }

        PACKET_UCHAR(pData, sModule.ucModuleId);
        break;

    case 10:
        iPackSize = sizeof(sModule.ucType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }

        PACKET_UCHAR(pData, sModule.ucType);
        break;
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));  
#endif
        return -1;
    }

    return iPackSize;
}

int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Module sModule;

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];

    for (i = 0; i < ucCount; i++)
    {
        sModule.ucModuleId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sModule);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sModule.ucModuleId-1)*m_gTableDesc[TBL_MODULE].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModModule(sModule.ucModuleId, sModule))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModModule(%d)\n", SHORT_FILE, __LINE__, sModule.ucModuleId));   
#endif
			return -1;
        }
    }
    return iPos;
}
int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Module sModule;

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucCount = %d\n", SHORT_FILE, __LINE__, ucCount));
#endif
        return -1;
    }

    for (i = 0; i < ucCount; i++)
    {
        sModule.ucModuleId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sModule);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sModule.ucModuleId-1)*m_gTableDesc[TBL_MODULE].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModModule(sModule.ucModuleId, sModule))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModModule(%d)\n", SHORT_FILE, __LINE__, sModule.ucModuleId));   
#endif
			return -1;
        }
    }
    return iPos;
}
int SetModule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uModuleId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Module sModule;

    if (!pTscDb->QueryModule(uModuleId, sModule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uModuleId*m_gTableDesc[TBL_MODULE].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryModule(%d)\n", SHORT_FILE, __LINE__, uModuleId));   
#endif
        return -1;
    }
    switch (uSubId)
    {
    case 10:
        iPackSize = sizeof(sModule.ucType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uModuleId*m_gTableDesc[TBL_MODULE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sModule.ucType);
        break;
    
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uModuleId*m_gTableDesc[TBL_MODULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));  
#endif
        return -1;
    }

    if (!pTscDb->ModModule(uModuleId, sModule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModModule(%d)\n", SHORT_FILE, __LINE__, uModuleId));  
#endif
        return -1;
    }
    return iPackSize;
}

int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    int iPos = 0;
    Byte uCount;
    Plan sPlan = {0};

    if (uDataSize <= 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    uCount = pData[iPos++];

    if (0 == uCount)
    {
        return iPos;
    }

    iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPlan);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
	iPos += iPackSize;

    if (sPlan.ucId != uPlanId)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsPlan.ucId(%d) != uPlanId\n", SHORT_FILE, __LINE__, sPlan.ucId, uPlanId));
#endif
		return -1;
    }

    if (!pTscDb->ModPlan(uPlanId, sPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPlan(%d)\n", SHORT_FILE, __LINE__, sPlan.ucId));   
#endif
        return -1;
    }

    return iPos;
}

int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Plan sPlan = {0};

    if (!pTscDb->QueryPlan(uPlanId, sPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPlan(%d)\n", SHORT_FILE, __LINE__, uPlanId)); 
#endif
        return -1;
    }

    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sPlan.usMonthFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_USHORT(pData, sPlan.usMonthFlag);
        break;

    case 3:
        iPackSize = sizeof(sPlan.ucWeekFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPlan.ucWeekFlag);
        break;
    case 4:
        iPackSize = sizeof(sPlan.ulDayFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_ULONG(pData, sPlan.ulDayFlag);
        break;
    case 5:
        iPackSize = sizeof(sPlan.ucScheduleId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPlan.ucScheduleId);
        break;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uPlanId*m_gTableDesc[TBL_PLAN].iFieldCount + uSubId;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId)); 
#endif
        return -1;
    }

    if (!pTscDb->ModPlan(uPlanId, sPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPlan(%d)\n", SHORT_FILE, __LINE__, sPlan.ucId));   
#endif
        return -1;
    }

    return iPackSize;
}

int SetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Plan sPlan = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if(uDataSize != ucCount*0x9+0x1)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
   }
	if (!pTscDb->DelPlan())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
        sPlan.ucId = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPlan);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sPlan.ucId-1)*m_gTableDesc[TBL_PLAN].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Plan\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddPlan(sPlan.ucId, sPlan))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPlan(%d)\n", SHORT_FILE, __LINE__, sPlan.ucId)); 
#endif
            return -1;
        }
    }
    return iPos;
}

int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Plan sPlan;

    if (uDataSize <= 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    
    pData[iPos++] = 1;

    if (!pTscDb->QueryPlan(uPlanId, sPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPlan(%d)\n", SHORT_FILE, __LINE__, uPlanId));  
#endif
        return -1;
    }

    iPackSize = Packet(sPlan, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Plan(%d)\n", SHORT_FILE, __LINE__, uPlanId)); 
#endif
        return -1;
    }

    return iPackSize;
}

int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPlanId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Plan sPlan;
    if (!pTscDb->QueryPlan(uPlanId, sPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPlan(%d)\n", SHORT_FILE, __LINE__, uPlanId));  
#endif
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sPlan.ucId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sPlan.ucId);
        break;
    case 2:
        iPackSize = sizeof(sPlan.usMonthFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_USHORT(pData, sPlan.usMonthFlag);
        break;

    case 3:
        iPackSize = sizeof(sPlan.ucWeekFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sPlan.ucWeekFlag);
        break;
    case 4:
        iPackSize = sizeof(sPlan.ulDayFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_ULONG(pData, sPlan.ulDayFlag);
        break;
    case 5:
        iPackSize = sizeof(sPlan.ucScheduleId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sPlan.ucScheduleId);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));  
#endif
        return -1;
    }

    return iPackSize;
}

int GetPlan(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblPlan tblPlan;
    Plan*   pPlan;
    Byte   ucCount, i;
    if (!pTscDb->QueryPlan(tblPlan))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPlan\n", SHORT_FILE, __LINE__));
#endif
        return -1;
    }
    pPlan = tblPlan.GetData(ucCount);

    if (uDataSize <= 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    
    pData[iPos++] = ucCount;

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pPlan[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Plan(%d)\n", SHORT_FILE, __LINE__, pPlan[i].ucId));   
#endif
			return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}


/*******************************获取通道灯泡检测配置表内容***************************************************/
int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    	 int iPos = 0;
		int iPackSize;		
		ChannelChk * pChannelChk;
		TblChannelChk tblChannelChk;
		
		Byte   ucCount, i;
		if (!pTscDb->QueryChannelChk(tblChannelChk))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryChannelChk\n", SHORT_FILE, __LINE__));
#endif
			return -1;
		}
	
		pChannelChk = tblChannelChk.GetData(ucCount);
	
		if (uDataSize <= 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			return -1;
		}
		
		pData[iPos++] = ucCount; //表查询结果行数
	
		for (i = 0; i < ucCount; i++)
		{
			iPackSize = Packet(pChannelChk[i], pData+iPos, uDataSize-iPos);
	
			if (iPackSize < 0)
			{
				uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
				ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet ChannelChk(%d)\n", SHORT_FILE, __LINE__, pPlan[i].ucId));   
#endif
				return -1;
			}
			iPos += iPackSize;
		}
	
		return iPos;
}

int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte& uErrorSts, Byte& uErrorIdx) //ADD:20130801
{return 0 ;}
int GetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte uSubId,Byte& uErrorSts, Byte& uErrorIdx) //ADD:20130801
{return 0 ;}
int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte& uErrorSts, Byte& uErrorIdx) //ADD:20130801
{return 0 ;}
int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucSubChanenlId,Byte uSubId,Byte& uErrorSts, Byte& uErrorIdx) //ADD:20130801
{return 0 ;}


int SetChannelChk(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	
    int iPos = 0;
    int iPackSize = 0;
  //  Plan sPlan = {0};
	ChannelChk sChannelChk={0};
    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];//获取上位机发送过来的要设置的行数

	if (!pTscDb->DelChannelChk()) //需要先删除原始数据
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
       // sPlan.ucId = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sChannelChk);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sChannelChk.ucSubChannelId-1)*m_gTableDesc[TBL_LAMP_CHECK].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Plan\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddChannelChk(sChannelChk.ucSubChannelId, sChannelChk))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ChannelChk(%d)\n", SHORT_FILE, __LINE__, sPlan.ucId)); 
#endif
            return -1;
        }
    }
    return iPos;

}


/*******************************************************************/



int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Byte uScheduleCount, uEvtCount;
    Schedule sSchedule = {0};

    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__)); 
#endif
        return -1;
    }

    uScheduleCount = pData[iPos++];
    uEvtCount = pData[iPos++];

    if (1 != uScheduleCount || 1 != uEvtCount)
    {
        uErrorSts = GBT_MSG_OBJ_ERROR;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tuScheduleCount = %d, uEvtCount = %d\n", SHORT_FILE, __LINE__, uScheduleCount, uEvtCount));  
#endif
        return -1;
    }

    iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sSchedule);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }

    iPos += iPackSize;
    if (0 == sSchedule.ucScheduleId
        || 0 == sSchedule.ucEvtId)
    {
        pTscDb->DelSchedule(uScheduleId, uEvtId);
        return iPos;
    }

    if (sSchedule.ucScheduleId != uScheduleId
        || sSchedule.ucEvtId != uEvtId)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uScheduleId= %d, sSchedule.ucScheduleId = %d, uEvtId = %d, sSchedule.ucEvtId = %d\n", SHORT_FILE, __LINE__, uScheduleId, sSchedule.ucScheduleId, uEvtId, sSchedule.ucEvtId));  
#endif
        return -1;
    }

    if (!pTscDb->ModSchedule(uScheduleId, uEvtId, sSchedule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModSchedule(%d, %d)\n", SHORT_FILE, __LINE__, sSchedule.ucScheduleId, sSchedule.ucEvtId));
#endif
        return -1;
    }
    return iPos;
}

int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Schedule sSchedule = {0};

    if (!pTscDb->QuerySchedule(uScheduleId, uEvtId, sSchedule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySchedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, uEvtId));   
#endif
        return -1;
    }

    switch (uSubId)
    {
    case 3:
        iPackSize = sizeof(sSchedule.ucBgnHour);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucBgnHour);
        break;

    case 4:
        iPackSize = sizeof(sSchedule.ucBgnMinute);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__)); 
#endif
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucBgnMinute);
        break;
    case 5:
        iPackSize = sizeof(sSchedule.ucCtrlMode);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));  
#endif
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucCtrlMode);
        break;
    case 6:
        iPackSize = sizeof(sSchedule.ucPatternNo);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));   
#endif
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucPatternNo);
        break;
    case 7:
        iPackSize = sizeof(sSchedule.ucAuxOut);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;

#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));  
#endif
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucAuxOut);
        break;
    case 8:
        iPackSize = sizeof(sSchedule.ucSpecialOut);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__)); 
#endif
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sSchedule.ucSpecialOut);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));  
#endif
        return -1;
    }

    if (!pTscDb->ModSchedule(uScheduleId, uEvtId, sSchedule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;

        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModSchedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, uEvtId)); 
        return -1;
    }
    return iPackSize;
}

int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Byte i, uScheduleCount, uEvtCount;
    Schedule sSchedule = {0};

    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));  
#endif
        return -1;
    }
    
    uScheduleCount = pData[iPos++];
    uEvtCount      = pData[iPos++];

    if (1 != uScheduleCount)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tuScheduleCount = %d\n", SHORT_FILE, __LINE__, uScheduleCount));
#endif
        return -1;
    }
    /*删除*/
    if (0 == uEvtCount)
    {
        pTscDb->DelSchedule(uScheduleId);
        return iPos;
    }

    for (i = 0; i < uEvtCount; i++) 
    {
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sSchedule);
        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Plan\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (sSchedule.ucScheduleId != uScheduleId)
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = uScheduleId*m_gTableDesc[TBL_SCHEDULE].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsSchedule.ucScheduleId(%d) != uScheduleId(%d)\n", SHORT_FILE, __LINE__, sSchedule.ucScheduleId, uScheduleId));
#endif
            return -1;
        }

        if (!pTscDb->ModSchedule(uScheduleId, sSchedule.ucEvtId, sSchedule))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModSchedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, sSchedule.ucEvtId));   
#endif
            return -1;
        }
    }

    return iPos;
}

int SetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0x0;
    int iPackSize = 0x0;
    Ushort  j , uScheduleCount ;
    Schedule sSchedule = {0};
 
    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tBuffer no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }
    	Byte Hbit = pData[iPos++] ;
   	Byte  Lbit = pData[iPos++] ; 
   	 uScheduleCount = (Ushort)((Hbit<<0x8) + Lbit);  //时段表行数

       if(uScheduleCount >= 0x300)
	   	uScheduleCount =( (uDataSize-iPos)/0x8) ; //兼容原来的时段表数处理
	   	
  //ACE_OS::printf("\r\n\n%s:%d StagePatterns count=%d uDataSize=%d !\r\n",__FILE__,__LINE__,uScheduleCount,uDataSize);
    if(uDataSize != uScheduleCount*0x8+0x2)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
   }
	//pTscDb->DelSchedule();  //先执行删除操作一遍
	if (!pTscDb->DelSchedule())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}
        for (j = 0; j < uScheduleCount; j++)
        {
            iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sSchedule);			
            if (iPackSize < 0)
            {
                uErrorSts = GBT_MSG_SIZE_SHORT;					
                return -1;
            }           
	if (!pTscDb->AddSchedule(sSchedule.ucScheduleId, sSchedule.ucEvtId, sSchedule))
	{		
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}
	 iPos += iPackSize;
      }  
    return iPos;
}
int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Schedule sSchedule;
    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer no enough\n", SHORT_FILE, __LINE__));  
#endif
        return -1;
    }

    if (!pTscDb->QuerySchedule(uScheduleId, uEvtId, sSchedule))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySchedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, uEvtId));   
#endif
        return iPos;
    }

    pData[iPos++] = 0x0;
    pData[iPos++] = 0x1;

    iPackSize = Packet(sSchedule, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {    
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet schedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, uEvtId));  
#endif
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}

int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte uEvtId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Schedule sSchedule;
    if (!pTscDb->QuerySchedule(uScheduleId, uEvtId, sSchedule))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySchedule(%d, %d)\n", SHORT_FILE, __LINE__, uScheduleId, uEvtId));   
#endif
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sSchedule.ucScheduleId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucScheduleId);
        break;
    case 2:
        iPackSize = sizeof(sSchedule.ucEvtId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucEvtId);
        break;

    case 3:
        iPackSize = sizeof(sSchedule.ucBgnHour);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucBgnHour);
        break;

    case 4:
        iPackSize = sizeof(sSchedule.ucBgnMinute);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucBgnMinute);
        break;

    case 5:
        iPackSize = sizeof(sSchedule.ucCtrlMode);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucCtrlMode);
        break;

    case 6:
        iPackSize = sizeof(sSchedule.ucPatternNo);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucPatternNo);
        break;

    case 7:
        iPackSize = sizeof(sSchedule.ucAuxOut);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucAuxOut);
        break;

    case 8:
        iPackSize = sizeof(sSchedule.ucSpecialOut);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sSchedule.ucSpecialOut);
        break;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
#endif
        return -1;
    }

    return iPackSize;
}

int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uScheduleId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblSchedule    tblSchedule;
    Schedule*   pSchedule;
    Ushort   usCount, i;

    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }

    if (!pTscDb->QuerySchedule(uScheduleId, tblSchedule))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySchedule(%d)\n", SHORT_FILE, __LINE__, uScheduleId));
#endif
        return iPos;
    }

    pSchedule = tblSchedule.GetData(usCount);

    pData[iPos++] = 0x0;
    pData[iPos++] = (Byte)usCount;
    for (i = 0; i < usCount; i++)
    {
        iPackSize = Packet(pSchedule[i], pData+iPos, uDataSize-iPos);
        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Schedule(%d, %d)\n", SHORT_FILE, __LINE__, pSchedule[i].ucScheduleId, pSchedule[i].ucEvtId));   
#endif
            return -1;
        }
        iPos += iPackSize;
    }
    return iPos;
}

int GetSchedule(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
  //  Ulong ulTemp;
    Ushort k,uCount;
    //Byte uMaxSchedule = 16;
    //Byte uMaxScheduleEvt = 48;

    TblSchedule  tblSchedule;
    Schedule*    pSchedule;
  //  Schedule     sSchedule;
/*
    if (pTscDb->GetFieldData(m_gTableDesc[TBL_CONSTANT].sTblName, "ucMaxSchedule", ulTemp))
    {
        uMaxSchedule = (Byte)ulTemp;
	
    }

    if (pTscDb->GetFieldData(m_gTableDesc[TBL_CONSTANT].sTblName, "ucMaxScheduleEvt", ulTemp))
    {
        uMaxScheduleEvt = (Byte)ulTemp;
    }
*/
    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }

    if (!pTscDb->QuerySchedule(tblSchedule))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySchedule\n", SHORT_FILE, __LINE__)); 
#endif
        return iPos;
    }	   

        pSchedule = tblSchedule.GetData(uCount);
	pData[iPos++] =(Byte)( (uCount>>0x8) &0xff) ;      //存储时段表条数的高8位
 	pData[iPos++] =(Byte)( uCount &0xff);                     //存储是时段表条数低8位

for(k = 0; k< uCount ;k++)
{
	  iPackSize = Packet(pSchedule[k], pData+iPos, uDataSize-iPos);
            if (iPackSize < 0)
            {
                uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
#endif
                return -1;
            }
            iPos += iPackSize;}
    	  return iPos;
}

int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Byte i, uEvtCount;
    EventType sEventType;

    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }

    uEvtCount = pData[iPos++];

    if (0 == uEvtCount)
    {
        pTscDb->DelEventType();
        return iPos;
    }
    for (i =0; i < uEvtCount; i++)
    {
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sEventType);
        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sEventType.ucEvtTypeId-1)*m_gTableDesc[TBL_EVENTTYPE].iFieldCount + 1;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
#endif
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModEventType(sEventType.ucEvtTypeId, sEventType))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
#ifdef TSC_DEBUG
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModEventType(%d)\n", SHORT_FILE, __LINE__, sEventType.ucEvtTypeId)); 
#endif
            return -1;
        }
    }

    return iPos;
}
int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Byte uEvtCount;
    EventType sEventType;

    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));  
#endif
        return -1;
    }

    uEvtCount = pData[iPos++];
    if (0 == uEvtCount)
    {
        pTscDb->DelEventType(uEventId);
        return iPos;
    }

    if (1 != uEvtCount)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tuEvtCount = %d\n", SHORT_FILE, __LINE__, uEvtCount)); 
#endif
        return -1;
    }
    iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sEventType);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
#endif
        return -1;
    }
	iPos += iPackSize;
    if (0 == sEventType.ucEvtTypeId)
    {
        pTscDb->DelEventType(uEventId);
        return iPos;
    }

    if (sEventType.ucEvtTypeId != uEventId)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uEventId*m_gTableDesc[TBL_EVENTTYPE].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsEventType.ucEvtTypeId(%d) != %d\n", SHORT_FILE, __LINE__, sEventType.ucEvtTypeId, uEventId));   
#endif
        return -1;
    }

    if (!pTscDb->ModEventType(uEventId, sEventType))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPlan(%d)\n", SHORT_FILE, __LINE__, sEventType.ucEvtTypeId));   
#endif
		return -1;
    }
    return iPos;
}

int SetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    EventType sEventType;

    if (!pTscDb->QueryEventType(uEventId, sEventType))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uEventId*m_gTableDesc[TBL_EVENTTYPE].iFieldCount + 1;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventType(%d)\n", SHORT_FILE, __LINE__, uEventId));  
#endif
        return -1;
    }

    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sEventType.ulClearTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uEventId*m_gTableDesc[TBL_EVENTTYPE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_ULONG(pData, sEventType.ulClearTime);
        break;

    case 5:
        iPackSize = sizeof(sEventType.ucLogCount);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        PACKET_UCHAR(pData, sEventType.ucLogCount);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uEventId*m_gTableDesc[TBL_EVENTTYPE].iFieldCount + uSubId;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));  
#endif
        return -1;
    }

    if (!pTscDb->ModEventType(uEventId, sEventType))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModEventType(%d)\n", SHORT_FILE, __LINE__, uEventId)); 
#endif
        return -1;
    }
    return iPackSize;
}

int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblEventType tblEvt;
    EventType*   pEventType;
    Byte   ucCount, i;
    
    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer no enough\n", SHORT_FILE, __LINE__));  
#endif
        return -1;
    }
    if (!pTscDb->QueryEventType(tblEvt))
    {
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventType\n", SHORT_FILE, __LINE__));   
        return iPos;
    }
    pEventType = tblEvt.GetData(ucCount);

    pData[iPos++] = ucCount;
    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pEventType[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet EventType(%d)\n", SHORT_FILE, __LINE__, pEventType[i].ucEvtTypeId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    EventType   sEventType;
    
    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    if (!pTscDb->QueryEventType(uEventId, sEventType))
    {
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventType\n", SHORT_FILE, __LINE__));   
        return iPos;
    }

    pData[iPos++] = 1;
   
    iPackSize = Packet(sEventType, pData+iPos, uDataSize-iPos);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet EventType(%d)\n", SHORT_FILE, __LINE__, uEventId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}

int GetEventType(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEventId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    Ushort uCount;
    int iPackSize;
    EventType sEventType;
    if (!pTscDb->QueryEventType(uEventId, sEventType))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventType(%d)\n", SHORT_FILE, __LINE__, uEventId));   
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sEventType.ucEvtTypeId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sEventType.ucEvtTypeId);
        break;
    case 2:
        iPackSize = sizeof(sEventType.ulClearTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_ULONG(pData, sEventType.ulClearTime);
        break;

    case 3:
        uCount = sEventType.strEvtDesc.GetCount();
        iPackSize = sizeof(Byte);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, uCount);
        break;

    case 5:
        iPackSize = sizeof(sEventType.ucLogCount);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_UCHAR(pData, sEventType.ucLogCount);
        break;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblEventLog tblEvt;
    EventLog*   pEventLog;
    unsigned int uiCount, i;

    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryEventLog(tblEvt))
    {
        pData[iPos++] = 0;

        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventLog\n", SHORT_FILE, __LINE__));   
        return iPos;
    }

    pEventLog = tblEvt.GetData(uiCount);
	
	PACKET_ULONG(pData+iPos, uiCount);
	iPos += 4;   						//四个字节存储日志数目
    for (i = 0; i < uiCount; i++)
    {
        iPackSize = Packet(pEventLog[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet EventLog(%d)\n", SHORT_FILE, __LINE__, pEventLog[i].ucEventId));   
			continue;	//在实际应用过程中，有发现一条数据出现太短，其它都没有。所以这里不进行返回并不加入
            //return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}

int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblEventLog tblEvt;
    EventLog*   pEventLog;
    unsigned int uiCount, i;
	ACE_DEBUG((LM_DEBUG,"%s:%d,uEvtTypeId:%d 	sizeleft:%d	 \n",__FILE__,__LINE__,uEvtTypeId,uDataSize));
    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryEventLog(uEvtTypeId, tblEvt))
    {
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventLog\n", SHORT_FILE, __LINE__));   
        return iPos;
    }

    pEventLog = tblEvt.GetData(uiCount);
	PACKET_ULONG(pData+iPos, uiCount);
	iPos += 4;

    for (i = 0; i < uiCount; i++)
    {
        iPackSize = Packet(pEventLog[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet EventLog(%d)\n", SHORT_FILE, __LINE__, pEventLog[i].ucEventId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}

int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int         iPos = 0;
    int         iPackSize;
    EventLog    sEventLog;
	ACE_DEBUG((LM_DEBUG,"%s:%d,uEvtTypeId:%d uEvtLogId:%d	sizeleft:%d	 \n",__FILE__,__LINE__,uEvtTypeId,uEvtLogId,uDataSize));
    if (uDataSize < 1)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryEventLog(uEvtLogId, sEventLog))
    {
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventLog(%d)\n", SHORT_FILE, __LINE__, uEvtLogId));   
        
        return iPos;
    }

	PACKET_ULONG(pData+iPos, 1);
	iPos += 4;
    iPackSize = Packet(sEventLog, pData+iPos, uDataSize-iPos);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet EventLog(%d)\n", SHORT_FILE, __LINE__, sEventLog.ucEventId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}

int GetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    EventLog sEventLog;
	ACE_DEBUG((LM_DEBUG,"%s:%d,uEvtTypeId:%d uEvtLogId:%d	uSubId:%d sizeleft:%d	 \n",__FILE__,__LINE__,uEvtTypeId,uEvtLogId,uSubId,uDataSize));
    if (!pTscDb->QueryEventLog(uEvtLogId, sEventLog))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryEventLog(%d)\n", SHORT_FILE, __LINE__, uEvtLogId));   
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sEventLog.ucEventId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sEventLog.ucEventId);
        break;
    case 2:
        iPackSize = sizeof(sEventLog.ucEvtType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sEventLog.ucEvtType);
        break;

    case 3:
        iPackSize = sizeof(sEventLog.ulHappenTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        
        PACKET_ULONG(pData, sEventLog.ulHappenTime);
        break;

    case 4:
        iPackSize = sizeof(sEventLog.ulEvtValue);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }
        PACKET_ULONG(pData, sEventLog.ulEvtValue);
        break;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int     iPos = 0;
    Byte   uCount;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    uCount    = pData[iPos++];
    if (0 == uCount)
    {
        pTscDb->DelEventLog();
        return iPos;
    }

    uErrorSts = GBT_MSG_ERROR_OTHER;
    uErrorIdx = 0;
    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t event log can not be modify\n", SHORT_FILE, __LINE__));   
    return -1;
}
int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte& uErrorSts, Byte& uErrorIdx)
{
   uErrorSts = GBT_MSG_ERROR_OTHER;
    uErrorIdx = 0;;
    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t event log can not be modify\n", SHORT_FILE, __LINE__));   
    return -1;
}


int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte& uErrorSts, Byte& uErrorIdx)
{
    uErrorSts = GBT_MSG_ERROR_OTHER;
    uErrorIdx = 0;;
    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t event log can not be modify\n", SHORT_FILE, __LINE__));   
    return -1;
}


int SetEventLog(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uEvtTypeId, Byte uEvtLogId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    uErrorSts = GBT_MSG_ERROR_OTHER;
    uErrorIdx = 0;
    ACE_DEBUG ((LM_DEBUG, "%s:%04d\t event log can not be modify\n", SHORT_FILE, __LINE__));   
    return -1;
}
 
int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblPhase tblPhase;
    Phase*   pPhase;
    Byte   ucCount, i;
    if (!pTscDb->QueryPhase(tblPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPhase\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pPhase = tblPhase.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pPhase[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Phase(%d)\n", SHORT_FILE, __LINE__, pPhase[i].ucPhaseId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Phase sPhase;

    if (!pTscDb->QueryPhase(uPhaseId, sPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPhase(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sPhase, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Phase(%d)\n", SHORT_FILE, __LINE__, sPhase.ucPhaseId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Phase sPhase;

    if (!pTscDb->QueryPhase(uPhaseId, sPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPhase(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sPhase.ucPhaseId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucPhaseId);
        break;

    case 2:
        iPackSize = sizeof(sPhase.ucPedestrianGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucPedestrianGreen);
        break;
    case 3:
        iPackSize = sizeof(sPhase.ucPedestrianClear);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucPedestrianClear);
        break;
    case 4:
        iPackSize = sizeof(sPhase.ucMinGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucMinGreen);
        break;
    case 5:
        iPackSize = sizeof(sPhase.ucGreenDelayUnit);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucGreenDelayUnit);
        break;
    case 6:
        iPackSize = sizeof(sPhase.ucMaxGreen1);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucMaxGreen1);
        break;
    case 7:
        iPackSize = sizeof(sPhase.ucMaxGreen2);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucMaxGreen2);
        break;
    case 8:
        iPackSize = sizeof(sPhase.ucFixGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucFixGreen);
        break;
    case 9:
        iPackSize = sizeof(sPhase.ucGreenFlash);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucGreenFlash);
        break;
    case 10:
        iPackSize = sizeof(sPhase.ucPhaseTypeFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucPhaseTypeFlag);
        break;
    case 11:
        iPackSize = sizeof(sPhase.ucPhaseOption);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucPhaseOption);
        break;
    case 12:
        iPackSize = sizeof(sPhase.ucExtend);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPhase.ucExtend);
        break;
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Phase sPhase = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if(uDataSize != ucCount*0xc+0x1)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
   }
	if (!pTscDb->DelPhase())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPhase(%d)\n", SHORT_FILE, __LINE__, sPhase.ucPhaseId));   
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
        sPhase.ucPhaseId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPhase);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sPhase.ucPhaseId-1)*m_gTableDesc[TBL_PHASE].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddPhase(sPhase.ucPhaseId, sPhase))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPhase(%d)\n", SHORT_FILE, __LINE__, sPhase.ucPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Phase sPhase = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        return -1;
    }

    for (i = 0; i < ucCount; i++)
    {
        sPhase.ucPhaseId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPhase);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sPhase.ucPhaseId-1)*m_gTableDesc[TBL_PHASE].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModPhase(sPhase.ucPhaseId, sPhase))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPhase(%d)\n", SHORT_FILE, __LINE__, sPhase.ucPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Phase sPhase = {0};

    if (!pTscDb->QueryPhase(uPhaseId, sPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPhase(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sPhase.ucPedestrianGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucPedestrianGreen);
        break;
    case 3:
        iPackSize = sizeof(sPhase.ucPedestrianClear);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucPedestrianClear);
        break;
    case 4:
        iPackSize = sizeof(sPhase.ucMinGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucMinGreen);
        break;
    case 5:
        iPackSize = sizeof(sPhase.ucGreenDelayUnit);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucGreenDelayUnit);
        break;
    case 6:
        iPackSize = sizeof(sPhase.ucMaxGreen1);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucMaxGreen1);
        break;
    case 7:
        iPackSize = sizeof(sPhase.ucMaxGreen2);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucMaxGreen2);
        break;
    case 8:
        iPackSize = sizeof(sPhase.ucFixGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucFixGreen);
    case 9:
        iPackSize = sizeof(sPhase.ucGreenFlash);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucGreenFlash);
        break;
    case 10:
        iPackSize = sizeof(sPhase.ucPhaseTypeFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucPhaseTypeFlag);
        break;
    case 11:
        iPackSize = sizeof(sPhase.ucPhaseOption);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucPhaseOption);
        break;
    case 12:
        iPackSize = sizeof(sPhase.ucExtend);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPhase.ucExtend);
        break;
    
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uPhaseId*m_gTableDesc[TBL_PHASE].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModPhase(uPhaseId, sPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPhase(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    return iPackSize;
}

int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblCollision tblCollision;
    Collision*   pCollision;
    Byte   ucCount, i;
    if (!pTscDb->QueryCollision(tblCollision))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryCollision\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pCollision = tblCollision.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pCollision[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Collision(%d)\n", SHORT_FILE, __LINE__, pCollision[i].ucPhaseId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Collision sCollision;

    if (!pTscDb->QueryCollision(uPhaseId, sCollision))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryCollision(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sCollision, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Collision(%d)\n", SHORT_FILE, __LINE__, sCollision.ucPhaseId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Collision sCollision;

    if (!pTscDb->QueryCollision(uPhaseId, sCollision))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryCollision(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sCollision.ucPhaseId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sCollision.ucPhaseId);
        break;

    case 2:
        iPackSize = sizeof(sCollision.usCollisionFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_USHORT(pData, sCollision.usCollisionFlag);
        break;
 
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Collision sCollision = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];

    for (i = 0; i < ucCount; i++)
    {
        sCollision.ucPhaseId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sCollision);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sCollision.ucPhaseId-1)*m_gTableDesc[TBL_COLLISION].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Collision\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModCollision(sCollision.ucPhaseId, sCollision))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModCollision(%d)\n", SHORT_FILE, __LINE__, sCollision.ucPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Collision sCollision = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        return -1;
    }

     for (i = 0; i < ucCount; i++)
    {
        sCollision.ucPhaseId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sCollision);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sCollision.ucPhaseId-1)*m_gTableDesc[TBL_COLLISION].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Collision\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModCollision(sCollision.ucPhaseId, sCollision))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModCollision(%d)\n", SHORT_FILE, __LINE__, sCollision.ucPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetCollision(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Collision sCollision = {0};

    if (!pTscDb->QueryCollision(uPhaseId, sCollision))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uPhaseId*m_gTableDesc[TBL_COLLISION].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryCollision(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sCollision.usCollisionFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPhaseId*m_gTableDesc[TBL_COLLISION].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_USHORT(pData, sCollision.usCollisionFlag);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uPhaseId*m_gTableDesc[TBL_COLLISION].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModCollision(uPhaseId, sCollision))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModCollision(%d)\n", SHORT_FILE, __LINE__, uPhaseId));   
        return -1;
    }
    return iPackSize;
}

int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblDetector tblDetector;
    Detector*   pDetector;
    Byte   ucCount, i;
    if (!pTscDb->QueryDetector(tblDetector))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetector\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pDetector = tblDetector.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pDetector[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Detector(%d)\n", SHORT_FILE, __LINE__, pDetector[i].ucDetectorId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Detector sDetector;

    if (!pTscDb->QueryDetector(uDetId, sDetector))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetector(%d)\n", SHORT_FILE, __LINE__, uDetId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sDetector, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Detector(%d)\n", SHORT_FILE, __LINE__, sDetector.ucDetectorId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Detector sDetector;

    if (!pTscDb->QueryDetector(uDetId, sDetector))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetector(%d)\n", SHORT_FILE, __LINE__, uDetId));   
        return -1;
    }
    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sDetector.ucDetectorId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucDetectorId);
        break;

    case 2:
        iPackSize = sizeof(sDetector.ucPhaseId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucPhaseId);
        break;
 
    case 3:
        iPackSize = sizeof(sDetector.ucDetFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucDetFlag);
        break;
    case 4:
        iPackSize = sizeof(sDetector.ucDirect);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucDirect);
        break;
    case 5:
        iPackSize = sizeof(sDetector.ucValidTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucValidTime);
        break;
    case 6:
        iPackSize = sizeof(sDetector.ucOptionFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucOptionFlag);
        break;
    case 7:
        iPackSize = sizeof(sDetector.usSaturationFlow);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_USHORT(pData, sDetector.usSaturationFlow);
        break;
    case 8:
        iPackSize = sizeof(sDetector.ucSaturationOccupy);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sDetector.ucSaturationOccupy);
        break;
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Detector sDetector = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
   // ACE_OS::printf("\n\n\n%s:%d ucCount=%d uDataSize=%d\r\n\n\n\n",__FILE__,__LINE__,ucCount,uDataSize);
     if(uDataSize != ucCount*0x9+0x1)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
    }
	if (!pTscDb->DelDetector())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetector(%d)\n", SHORT_FILE, __LINE__, sDetector.ucDetectorId));   
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
        sDetector.ucDetectorId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sDetector);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sDetector.ucDetectorId-1)*m_gTableDesc[TBL_DETECTOR].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Detector\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddDetector(sDetector.ucDetectorId, sDetector))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetector(%d)\n", SHORT_FILE, __LINE__, sDetector.ucDetectorId));   
            return -1;
        }
    }
    return iPos;
}
int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Detector sDetector = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        return -1;
    }

     for (i = 0; i < ucCount; i++)
    {
        sDetector.ucPhaseId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sDetector);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sDetector.ucDetectorId-1)*m_gTableDesc[TBL_DETECTOR].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Detector\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModDetector(sDetector.ucDetectorId, sDetector))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetector(%d)\n", SHORT_FILE, __LINE__, sDetector.ucDetectorId));   
            return -1;
        }
    }
    return iPos;
}
int SetDetector(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uDetectorId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Detector sDetector = {0};

    if (!pTscDb->QueryDetector(uDetectorId, sDetector))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetector(%d)\n", SHORT_FILE, __LINE__, uDetectorId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sDetector.ucPhaseId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucPhaseId);
        break;
    case 3:
        iPackSize = sizeof(sDetector.ucDetFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucDetFlag);
        break;
     case 4:
        iPackSize = sizeof(sDetector.ucDirect);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucDirect);
        break;
     case 5:
        iPackSize = sizeof(sDetector.ucValidTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucValidTime);
        break;
    case 6:
        iPackSize = sizeof(sDetector.ucOptionFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucOptionFlag);
        break;
    case 7:
        iPackSize = sizeof(sDetector.usSaturationFlow);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_USHORT(pData, sDetector.usSaturationFlow);
        break;
    case 8:
        iPackSize = sizeof(sDetector.ucSaturationOccupy);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sDetector.ucSaturationOccupy);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uDetectorId*m_gTableDesc[TBL_DETECTOR].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModDetector(uDetectorId, sDetector))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetector(%d)\n", SHORT_FILE, __LINE__, uDetectorId));   
        return -1;
    }
    return iPackSize;
}

int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblChannel tblChannel;
    Channel*   pChannel;
    Byte   ucCount, i;
    if (!pTscDb->QueryChannel(tblChannel))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryChannel\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pChannel = tblChannel.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pChannel[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Detector(%d)\n", SHORT_FILE, __LINE__, pChannel[i].ucChannelId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Channel sChannel;

    if (!pTscDb->QueryChannel(uChannelId, sChannel))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryChannel(%d)\n", SHORT_FILE, __LINE__, uChannelId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sChannel, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Channel(%d)\n", SHORT_FILE, __LINE__, sChannel.ucChannelId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Channel sChannel;

    if (!pTscDb->QueryChannel(uChannelId, sChannel))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryChannel(%d)\n", SHORT_FILE, __LINE__, uChannelId));   
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sChannel.ucChannelId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sChannel.ucChannelId);
        break;

    case 2:
        iPackSize = sizeof(sChannel.ucCtrlSrc);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sChannel.ucCtrlSrc);
        break;
 
    case 3:
        iPackSize = sizeof(sChannel.ucAutoFlsCtrlFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sChannel.ucAutoFlsCtrlFlag);
        break;
    case 4:
        iPackSize = sizeof(sChannel.ucCtrlType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sChannel.ucCtrlType);
        break;
   
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Channel sChannel = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if(uDataSize != ucCount*0x4+0x1)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
    }
if (!pTscDb->DelChannel())
{
	uErrorSts = GBT_MSG_ERROR_OTHER;
	uErrorIdx = 0;
	//ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModChannel(%d)\n", SHORT_FILE, __LINE__, sChannel.ucChannelId));   
	return -1;
}

    for (i = 0; i < ucCount; i++)
    {
        sChannel.ucChannelId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sChannel);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sChannel.ucChannelId-1)*m_gTableDesc[TBL_CHANNEL].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Channel\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddChannel(sChannel.ucChannelId, sChannel))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModChannel(%d)\n", SHORT_FILE, __LINE__, sChannel.ucChannelId));   
            return -1;
        }
    }
    return iPos;
}
int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Channel sChannel = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        return -1;
    }

     for (i = 0; i < ucCount; i++)
    {
        sChannel.ucChannelId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sChannel);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sChannel.ucChannelId-1)*m_gTableDesc[TBL_CHANNEL].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Channel\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModChannel(sChannel.ucChannelId, sChannel))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModChannel(%d)\n", SHORT_FILE, __LINE__, sChannel.ucChannelId));   
            return -1;
        }
    }
    return iPos;
}
int SetChannel(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uChannelId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Channel sChannel = {0};

    if (!pTscDb->QueryChannel(uChannelId, sChannel))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uChannelId*m_gTableDesc[TBL_CHANNEL].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryChannel(%d)\n", SHORT_FILE, __LINE__, uChannelId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sChannel.ucCtrlSrc);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uChannelId*m_gTableDesc[TBL_CHANNEL].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sChannel.ucCtrlSrc);
        break;
    case 3:
        iPackSize = sizeof(sChannel.ucAutoFlsCtrlFlag);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uChannelId*m_gTableDesc[TBL_CHANNEL].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sChannel.ucAutoFlsCtrlFlag);
        break;
     case 4:
        iPackSize = sizeof(sChannel.ucCtrlType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uChannelId*m_gTableDesc[TBL_CHANNEL].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sChannel.ucCtrlType);
        break;
    
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uChannelId*m_gTableDesc[TBL_CHANNEL].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModChannel(uChannelId, sChannel))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModChannel(%d)\n", SHORT_FILE, __LINE__, uChannelId));   
        return -1;
    }
    return iPackSize;
}

int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblPattern tblPattern;
    Pattern*   pPattern;
    Byte   ucCount, i;
    if (!pTscDb->QueryPattern(tblPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPattern\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pPattern = tblPattern.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pPattern[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Detector(%d)\n", SHORT_FILE, __LINE__, pPattern[i].ucPatternId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Pattern sPattern;

    if (!pTscDb->QueryPattern(uPatternId, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPattern(%d)\n", SHORT_FILE, __LINE__, uPatternId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sPattern, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Pattern(%d)\n", SHORT_FILE, __LINE__, sPattern.ucPatternId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    Pattern sPattern;

    if (!pTscDb->QueryPattern(uPatternId, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPattern(%d)\n", SHORT_FILE, __LINE__, uPatternId));   
        return -1;
    }
    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sPattern.ucPatternId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPattern.ucPatternId);
        break;

    case 2:
        iPackSize = sizeof(sPattern.ucCycleTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPattern.ucCycleTime);
        break;
 
    case 3:
        iPackSize = sizeof(sPattern.ucOffset);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPattern.ucOffset);
        break;
    case 4:
        iPackSize = sizeof(sPattern.ucCoorPhase);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPattern.ucCoorPhase);
        break;

    case 5:
        iPackSize = sizeof(sPattern.ucStagePatternId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sPattern.ucStagePatternId);
        break;
   
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Pattern sPattern = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
      if(uDataSize != ucCount*0x5+0x1)
    {
	  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	  uErrorIdx = 0;
       	 return -1;
    }
	if (!pTscDb->DelPattern())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPattern(%d)\n", SHORT_FILE, __LINE__, sPattern.ucPatternId));   
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
        sPattern.ucPatternId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPattern);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sPattern.ucPatternId-1)*m_gTableDesc[TBL_PATTERN].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddPattern(sPattern.ucPatternId, sPattern))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPattern(%d)\n", SHORT_FILE, __LINE__, sPattern.ucPatternId));   
            return -1;
        }
    }
    return iPos;
}
int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    Pattern sPattern = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        return -1;
    }

    for (i = 0; i < ucCount; i++)
    {
        sPattern.ucPatternId  = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPattern);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sPattern.ucPatternId-1)*m_gTableDesc[TBL_PATTERN].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->ModPattern(sPattern.ucPatternId, sPattern))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPattern(%d)\n", SHORT_FILE, __LINE__, sPattern.ucPatternId));   
            return -1;
        }
    }
    return iPos;
}
int SetPattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uPatternId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    Pattern sPattern = {0};

    if (!pTscDb->QueryPattern(uPatternId, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryPattern(%d)\n", SHORT_FILE, __LINE__, uPatternId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sPattern.ucCycleTime);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPattern.ucCycleTime);
        break;
    case 3:
        iPackSize = sizeof(sPattern.ucOffset);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPattern.ucOffset);
        break;
    case 4:
        iPackSize = sizeof(sPattern.ucCoorPhase);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPattern.ucCoorPhase);
        break;

    case 5:
        iPackSize = sizeof(sPattern.ucStagePatternId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + uSubId;
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sPattern.ucStagePatternId);
        break;
    
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uPatternId*m_gTableDesc[TBL_PATTERN].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModPattern(uPatternId, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModPattern(%d)\n", SHORT_FILE, __LINE__, uPatternId));   
        return -1;
    }
    return iPackSize;
}

int SetDetDelicacy(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetDetDelicacy(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*软件参数表
*/
int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblSpecFun tblSpecFun;
	SpecFun*   pSpecFun;
	Byte   ucCount, i;
	if (!pTscDb->QuerySpecFun(tblSpecFun))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySpecFun\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pSpecFun = tblSpecFun.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pSpecFun[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
//			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, pOverlapPhase[i].ucOverlapPhaseId));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucFunType, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	SpecFun sSpecFun;

	sSpecFun.ucFunType = ucFunType;
	if ( !pTscDb->QuerySpecFun(sSpecFun) )
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QuerySpecFun(%d)\n", SHORT_FILE, __LINE__, ucFunType));   
		return -1;
	}

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = 1; 

	iPackSize = Packet(sSpecFun, pData+iPos, uDataSize-iPos);

	if (iPackSize < 0)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet Phase(%d)\n", SHORT_FILE, __LINE__, sSpecFun.ucFunType));   
		return -1;
	}
	iPos += iPackSize;
	return iPos;

}

int GetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte ucFunType, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*软件参数表
*/
int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize = 0;
	SpecFun sSpecFun = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];

	for (i = 0; i < ucCount; i++)
	{
		sSpecFun.ucFunType = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sSpecFun);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sSpecFun.ucFunType-1)*m_gTableDesc[TBL_SPECFUN].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->ModSpecFun(sSpecFun.ucFunType, sSpecFun.ucValue))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
//			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetDelicacy(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
			return -1;
		}
	}
	return iPos;
}

int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uFunType, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos         = 0;
	int iPackSize    = 0;
	SpecFun sSpecFun = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		return -1;
	}
	ucCount = pData[iPos++];
	if (ucCount != 1)
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		return -1;
	}

	for (i = 0; i < ucCount; i++)
	{
		sSpecFun.ucFunType  = 0;
		iPackSize           = Unpacket(pData+iPos, uDataSize-iPos, sSpecFun);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sSpecFun.ucFunType-1)*m_gTableDesc[TBL_SPECFUN].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket EventType\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if ( !pTscDb->ModSpecFun(sSpecFun.ucFunType, sSpecFun.ucValue) )
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModSpecFun(%d)\n", SHORT_FILE, __LINE__, sSpecFun.ucFunType));   
			return -1;
		}
	}

	return iPos;
}

int SetSpecFun(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*检测器扩展表
*/
int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblDetExtend tblDetExtend;
	DetExtend*   pDetExtend;
	Byte   ucCount, i;
	if (!pTscDb->QueryDetExtend(tblDetExtend))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetDelicacy\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pDetExtend = tblDetExtend.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pDetExtend[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
//			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, pOverlapPhase[i].ucOverlapPhaseId));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*检测器扩展整表设置
*/
int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize = 0;
	DetExtend sDetExtend = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];

	for (i = 0; i < ucCount; i++)
	{
		sDetExtend.ucId = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sDetExtend);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sDetExtend.ucId-1)*m_gTableDesc[TBL_DET_EXTEND].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->ModDetExtend(sDetExtend.ucId, sDetExtend))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
//			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetDelicacy(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
			return -1;
		}
	}
	return iPos;
}

int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetDetExtend(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblAdaptPara tblAdaptPara;
	AdaptPara*   pAdaptPara;
	Byte   ucCount, i;
	if (!pTscDb->QueryAdaptPara(tblAdaptPara))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryAdaptPara\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pAdaptPara = tblAdaptPara.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pAdaptPara[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet pAdaptPara(%d)\n", SHORT_FILE, __LINE__, pAdaptPara[i].ucType));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize = 0;
	AdaptPara sAdaptPara = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];

	for (i = 0; i < ucCount; i++)
	{
		sAdaptPara.ucType = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sAdaptPara);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sAdaptPara.ucType-1)*m_gTableDesc[TBL_ADAPT_PARA].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->ModAdaptPara(sAdaptPara))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetDelicacy(%d)\n", SHORT_FILE, __LINE__, sAdaptPara.ucType));   
			return -1;
		}
	}
	return iPos;
}

int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetAdaptPara(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*倒计时设备表
*/
int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblCntDownDev tblCntDownDev;
	CntDownDev*   pCntDownDev;
	Byte   ucCount, i;
	if (!pTscDb->QueryCntDownDev(tblCntDownDev))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetDelicacy\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pCntDownDev = tblCntDownDev.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pCntDownDev[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet CntDownDev(%d)\n", SHORT_FILE, __LINE__, pCntDownDev[i].ucDevId));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{	
	int iPos = 0;
	int iPackSize = 0;
	CntDownDev sCntDownDev = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];

	for (i = 0; i < ucCount; i++)
	{
		sCntDownDev.ucDevId = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sCntDownDev);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sCntDownDev.ucDevId-1)*m_gTableDesc[TBL_CNTDOWNDEV].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->ModCntDownDev(sCntDownDev.ucDevId, sCntDownDev))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModCntDownDev(%d)\n", SHORT_FILE, __LINE__, sCntDownDev.ucDevId)); 
			return -1;
		}
	}
	return iPos;
}

int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetCntDownDev(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*自适应控制阶段时间
*/
int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblAdaptStageTime tblAdaptStageTime;
	AdaptStageTime*   pAdaptStageTime;
	Byte   ucCount, i;
	if (!pTscDb->QueryAdaptStageTime(tblAdaptStageTime))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryDetDelicacy\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pAdaptStageTime = tblAdaptStageTime.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pAdaptStageTime[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, pAdaptStageTime[i].ucId));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*设置自适应阶段表
*/
int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize = 0;
	AdaptStageTime sAdaptStageTime;

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];

	for (i = 0; i < ucCount; i++)
	{
		sAdaptStageTime.ucId = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sAdaptStageTime);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sAdaptStageTime.ucId-1)*m_gTableDesc[TBL_ADAPT_STAGETIME].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->ModAdaptStageTime(sAdaptStageTime))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetDelicacy(%d)\n", SHORT_FILE, __LINE__, sAdaptStageTime.ucId));   
			return -1;
		}
	}
	return iPos;
}

int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetAdaptStage(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

/*
*方向与相位对应表整表获取
*/
int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize;
	TblPhaseToDirec tblPhaseToDirec;
	PhaseToDirec*   pPhaseToDirec;
	Byte   ucCount, i;

	if (!pTscDb->QueryPhaseToDirec(tblPhaseToDirec))
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryOverlapPhase\n", SHORT_FILE, __LINE__));   
		return -1;
	}

	pPhaseToDirec = tblPhaseToDirec.GetData(ucCount);
	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
		return -1;
	}
	pData[iPos++] = ucCount; 

	for (i = 0; i < ucCount; i++)
	{
		iPackSize = Packet(pPhaseToDirec[i], pData+iPos, uDataSize-iPos);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
//			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, pOverlapPhase[i].ucOverlapPhaseId));   
			return -1;
		}
		iPos += iPackSize;
	}

	return iPos;
}

int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	int iPos = 0;
	int iPackSize = 0;
	PhaseToDirec sPhaseToDirec = {0};

	Byte ucCount, i;

	if ((iPos+1) > uDataSize)
	{
		uErrorSts = GBT_MSG_SIZE_SHORT;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
		return -1;
	}
	ucCount = pData[iPos++];
          if(uDataSize != ucCount*0x4+0x1)
  	 {
		  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
		  uErrorIdx = 0;
		//  SndMsgLog(LOG_TYPE_GREEN_CONFIG,3,0,0,0); 
       		 return -1;
  	 }
	if (!pTscDb->DelPhaseToDirec())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}

	for (i = 0; i < ucCount; i++)
	{
		sPhaseToDirec.ucId = 0;
		iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPhaseToDirec);

		if (iPackSize < 0)
		{
			uErrorSts = GBT_MSG_SIZE_SHORT;
			uErrorIdx = (sPhaseToDirec.ucId-1)*m_gTableDesc[TBL_PHASE2DIREC].iFieldCount + 1;

			ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
			return -1;
		}
		iPos += iPackSize;

		if (!pTscDb->AddPhaseToDirec(sPhaseToDirec.ucId, sPhaseToDirec))
		{
			uErrorSts = GBT_MSG_ERROR_OTHER;
			uErrorIdx = 0;
			//ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModDetDelicacy(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
			return -1;
		}
	}
	return iPos;
}

int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int SetPhaseToDirec(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
	return 0;
}

int GetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    TblOverlapPhase tblOverlapPhase;
    OverlapPhase*   pOverlapPhase;
    Byte   ucCount, i;
    if (!pTscDb->QueryOverlapPhase(tblOverlapPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryOverlapPhase\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pOverlapPhase = tblOverlapPhase.GetData(ucCount);
    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = ucCount; 

    for (i = 0; i < ucCount; i++)
    {
        iPackSize = Packet(pOverlapPhase[i], pData+iPos, uDataSize-iPos);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, pOverlapPhase[i].ucOverlapPhaseId));   
            return -1;
        }
        iPos += iPackSize;
    }

    return iPos;
}
int GetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    OverlapPhase sOverlapPhase;

    if (!pTscDb->QueryOverlapPhase(uOverlapPhaseId, sOverlapPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to OverlapPhase(%d)\n", SHORT_FILE, __LINE__, uOverlapPhaseId));   
        return -1;
    }

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Buffer is not enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1; 
 
    iPackSize = Packet(sOverlapPhase, pData+iPos, uDataSize-iPos);

    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Packet OverlapPhase(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    OverlapPhase sOverlapPhase;

    if (!pTscDb->QueryOverlapPhase(uOverlapPhaseId, sOverlapPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryOverlapPhase(%d)\n", SHORT_FILE, __LINE__, uOverlapPhaseId));   
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sOverlapPhase.ucOverlapPhaseId);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucOverlapPhaseId);
        break;

    case 2:
        iPackSize = sizeof(sOverlapPhase.ucOperateType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucOperateType);
        break;
   case 3:
        iPackSize = sizeof(sOverlapPhase.ucIncldPhaseCnt);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucIncldPhaseCnt);
        break;

    case 4:
        iPackSize = MAX_PHASE_COUNT;
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        ACE_OS::memcpy(pData, sOverlapPhase.ucIncldPhase, iPackSize);
        break;
    case 5:
        iPackSize = sizeof(sOverlapPhase.ucIncldPhaseCnt);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucIncldPhaseCnt);
        break;
    
    case 6:
        iPackSize = MAX_PHASE_COUNT;
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        ACE_OS::memcpy(pData, sOverlapPhase.ucAdjustPhase, iPackSize);
        break;
    case 7:
        iPackSize = sizeof(sOverlapPhase.ucTailGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucTailGreen);
        break;

    case 8:
        iPackSize = sizeof(sOverlapPhase.ucTailYellow);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucTailYellow);
        break;

    case 9:
        iPackSize = sizeof(sOverlapPhase.ucTailRed);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            return -1;
        }

        PACKET_UCHAR(pData, sOverlapPhase.ucTailRed);
        break;
   
     default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    return iPackSize;
}

int SetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    OverlapPhase sOverlapPhase = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
        return -1;
    }
    ucCount = pData[iPos++];
     if(uDataSize != ucCount*0x47+0x1)
  	 {
		  uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
       		 return -1;
  	 }
	if (!pTscDb->DelOverlapPhase())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModOverlapPhase(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
		return -1;
	}

    for (i = 0; i < ucCount; i++)
    {
        sOverlapPhase.ucOverlapPhaseId = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sOverlapPhase);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sOverlapPhase.ucOverlapPhaseId-1)*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (!pTscDb->AddOverlapPhase(sOverlapPhase.ucOverlapPhaseId, sOverlapPhase))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModOverlapPhase(OverlapPhaseId =%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize = 0;
    OverlapPhase sOverlapPhase = {0};

    Byte ucCount, i;

    if ((iPos+1) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        return -1;
    }
    ucCount = pData[iPos++];
    if (ucCount != 1)
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tucCount = %d\n", SHORT_FILE, __LINE__, ucCount));
        return -1;
    }

    for (i = 0; i < ucCount; i++)
    {
        sOverlapPhase.ucOverlapPhaseId = 0;
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sOverlapPhase);

        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sOverlapPhase.ucOverlapPhaseId-1)*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + 1;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to Unpacket Pattern\n", SHORT_FILE, __LINE__));
            return -1;
        }
        iPos += iPackSize;

        if (sOverlapPhase.ucOverlapPhaseId != uOverlapPhaseId)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = (sOverlapPhase.ucOverlapPhaseId-1)*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + 1;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tsOverlapPhase.ucOverlapPhaseId(%d) != uPatternId(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId, uOverlapPhaseId));   
            return -1;
        }

        if (!pTscDb->ModOverlapPhase(sOverlapPhase.ucOverlapPhaseId, sOverlapPhase))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModOverlapPhase(%d)\n", SHORT_FILE, __LINE__, sOverlapPhase.ucOverlapPhaseId));   
            return -1;
        }
    }
    return iPos;
}
int SetOverlapPhase(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uOverlapPhaseId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize = 0;
    OverlapPhase sOverlapPhase = {0};

    if (!pTscDb->QueryOverlapPhase(uOverlapPhaseId, sOverlapPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + 1;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryOverlapPhase(%d)\n", SHORT_FILE, __LINE__, uOverlapPhaseId));   
        return -1;
    }
    switch (uSubId)
    {
    case 2:
        iPackSize = sizeof(sOverlapPhase.ucOperateType);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucOperateType);
        break;

    case 3:
        iPackSize = sizeof(sOverlapPhase.ucIncldPhaseCnt);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucIncldPhaseCnt);
        break;

    case 4:
        iPackSize = MAX_PHASE_COUNT;
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        ACE_OS::memcpy(sOverlapPhase.ucIncldPhase, pData, iPackSize);
        break;

    case 5:
        iPackSize = sizeof(sOverlapPhase.ucAdjustPhaseCnt);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucAdjustPhaseCnt);
        break;

    case 6:
        iPackSize = MAX_PHASE_COUNT;
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;

            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        ACE_OS::memcpy(sOverlapPhase.ucAdjustPhase, pData, iPackSize);
        break;
    case 7:
        iPackSize = sizeof(sOverlapPhase.ucTailGreen);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucTailGreen);
        break;
    case 8:
        iPackSize = sizeof(sOverlapPhase.ucTailYellow);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucTailYellow);
        break;
    case 9:
        iPackSize = sizeof(sOverlapPhase.ucTailRed);
        if (uDataSize < iPackSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbuffer not enough\n", SHORT_FILE, __LINE__));
            return -1;
        }
        
        UNPACKET_UCHAR(pData, sOverlapPhase.ucTailRed);
        break;
   
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        uErrorIdx = uOverlapPhaseId*m_gTableDesc[TBL_OVERLAPPHASE].iFieldCount + uSubId;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModOverlapPhase(uOverlapPhaseId, sOverlapPhase))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModOverlapPhase(%d)\n", SHORT_FILE, __LINE__, uOverlapPhaseId));   
        return -1;
    }
    return iPackSize;
}
int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
 //   Ulong ulTemp;
    Ushort k,uCount;
   // Byte uMaxStage = 16;
    //Byte uMaxStagePattern = 16;
    TblStagePattern  tblStagePat;
    StagePattern* pStagePattern;
    //StagePattern  sPattern;
/*
    if (pTscDb->GetFieldData(m_gTableDesc[TBL_CONSTANT].sTblName, "ucMaxStage", ulTemp))
    {
        uMaxStage = (Byte)ulTemp;
    }

    if (pTscDb->GetFieldData(m_gTableDesc[TBL_CONSTANT].sTblName, "ucMaxStagePattern", ulTemp))
    {
        uMaxStagePattern = (Byte)ulTemp;
    }
*/
    if ((iPos+2) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryStagePattern(tblStagePat))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryStagePattern\n", SHORT_FILE, __LINE__));   
        return iPos;
    } 
    pStagePattern = tblStagePat.GetData(uCount);
    
    //ACE_DEBUG ((LM_DEBUG, "%s:%d Get StagePatterns count =%d \n", SHORT_FILE, __LINE__,uCount));   
    pData[iPos++] = (Byte)((uCount>>0x8)&0xff) ;	  //存储时段表条数的高8位
    pData[iPos++] = (Byte)(uCount &0xff);			//存储是时段表条数低8位
	
	 //  k= 0x0 ;
	for(k = 0; k< uCount ;k++)
	{
		// sSchedule = pSchedule[k];
		  iPackSize = Packet(pStagePattern[k], pData+iPos, uDataSize-iPos);
				if (iPackSize < 0)
				{
					uErrorSts = GBT_MSG_SIZE_SHORT;
#ifdef TSC_DEBUG
					ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));	 
#endif
					return -1;
				}
				iPos += iPackSize;
	}
	
    return iPos;
}

int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;;
    Ushort i,uCount;
 
    TblStagePattern  tblStagePat;
    StagePattern* pStagePattern;

    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryStagePattern(uStagePatId, tblStagePat))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryStagePattern\n", SHORT_FILE, __LINE__));   
        return iPos;
    }

    pStagePattern = tblStagePat.GetData(uCount);

    if ((iPos+2) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    pData[iPos++] = 1;
    pData[iPos++] = (Byte)uCount;
    for (i = 0; i < uCount; i++)
    {
        iPackSize = Packet(pStagePattern[i], pData+iPos, uDataSize-iPos);
        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        iPos += iPackSize;
    }
    
    return iPos;
}

int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;;
   
    StagePattern sStagePattern;

    if (uDataSize < 2)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    if (!pTscDb->QueryStagePattern(uStagePatId, uStageId, sStagePattern))
    {
        pData[iPos++] = 0;
        pData[iPos++] = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryStagePattern\n", SHORT_FILE, __LINE__));   
        return iPos;
    }

    if ((iPos+2) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    pData[iPos++] = 1;
    pData[iPos++] = 1;
   
    iPackSize = Packet(sStagePattern, pData+iPos, uDataSize-iPos);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    iPos += iPackSize;
    return iPos;
}
int GetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize, iPos = 0;
   
    StagePattern sStagePattern;

    if (!pTscDb->QueryStagePattern(uStagePatId, uStageId, sStagePattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryStagePattern\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    switch (uSubId)
    {
    case 1:
        iPackSize = sizeof(sStagePattern.ucStagePatternId);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucStagePatternId);
        break;
    case 2:
        iPackSize = sizeof(sStagePattern.ucStageNo);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucStageNo);
        break;
    case 3:
        iPackSize = sizeof(sStagePattern.usAllowPhase);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_USHORT(pData, sStagePattern.usAllowPhase);
        break;
    case 4:
        iPackSize = sizeof(sStagePattern.ucGreenTime);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucGreenTime);
        break;
    case 5:
        iPackSize = sizeof(sStagePattern.ucYellowTime);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucYellowTime);
        break;

    case 6:
        iPackSize = sizeof(sStagePattern.ucRedTime);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucRedTime);
        break;

    case 7:
        iPackSize = sizeof(sStagePattern.ucOption);
        if ((iPos+iPackSize) > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        PACKET_UCHAR(pData, sStagePattern.ucOption);
        break;

    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }
    return iPackSize;
}
int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Ushort j;
    //Byte uMaxStage = 16;
    //Byte uMaxStagePattern = 16;
    Ushort  uScheduleCount =0x0 ;
    StagePattern  sPattern;
    StagePattern  sTempPattern;
    if ((iPos+2) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    //uMaxStagePattern = pData[iPos++];
    //uMaxStage        = pData[iPos++];
    	Byte Hbit = pData[iPos++] ;
   	Byte  Lbit = pData[iPos++] ; 
   	 uScheduleCount = (Ushort)((Hbit<<0x8) + Lbit);  //阶段配时表行数
      
       if(uScheduleCount >= 0xfe)  //254 兼容原来上位机
	   	uScheduleCount =0xff ;//( (uDataSize-iPos)/0xA) ; //兼容原来的时段表数处理	
	/// ACE_OS::printf("\r\n\n%s:%d StagePatterns count=%d uDataSize=%d !\r\n",__FILE__,__LINE__,uScheduleCount,uDataSize);
	  if(uDataSize != uScheduleCount*0xa+0x2)
   	 {
	 	 uErrorSts = GBT_MSG_ERROR_OTHER;  //字节数不正确
	 	 uErrorIdx = 0;
       	 	return -1;
   	 }
	if (!pTscDb->DelStagePattern())
	{
		uErrorSts = GBT_MSG_ERROR_OTHER;
		uErrorIdx = 0;
		return -1;
	}
	
        for (j = 0; j < uScheduleCount; j++)
        {
            iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPattern);			
            if (iPackSize < 0)
            {
                uErrorSts = GBT_MSG_SIZE_SHORT;					
                return -1;
            }   
	if (!pTscDb->AddStagePattern(sPattern.ucStagePatternId, sPattern.ucStageNo, sPattern))
            {
                uErrorSts = GBT_MSG_ERROR_OTHER;
                uErrorIdx = 0;
                ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModStagePattern\n", SHORT_FILE, __LINE__));   
                return -1;
            }
            iPos += iPackSize;
      }  
//      ACE_DEBUG ((LM_DEBUG, "%s:%d Set stagepatterns %d\n", SHORT_FILE, __LINE__,uScheduleCount));	
     return iPos;
}

int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Ushort i;
    Byte uMaxStage = 16;
    Byte uMaxStagePattern = 16;
    StagePattern  sPattern;

    if ((iPos+2) > uDataSize)
    { 
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    uMaxStagePattern    = pData[iPos++];
    uMaxStage           = pData[iPos++];

    if (1 != uMaxStagePattern)
    {
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t 1 != uMaxStagePattern(%d)\n", SHORT_FILE, __LINE__, uMaxStagePattern));   
        return -1;
    }

    
    for (i = 0; i < uMaxStage; i++)
    {
        iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPattern);
        if (iPackSize < 0)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }

        if (uStagePatId != sPattern.ucStagePatternId)
        {
            uErrorSts = GBT_MSG_OBJ_ERROR;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tuStagePatId != sPattern.ucStagePatternId\n", SHORT_FILE, __LINE__));   
            return -1;
        }

        if (!pTscDb->ModStagePattern(sPattern.ucStagePatternId, sPattern.ucStageNo, sPattern))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            uErrorIdx = 0;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModStagePattern\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        iPos += iPackSize;
    }
    
    return iPos;
}

int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPos = 0;
    int iPackSize;
    Byte uMaxStage = 16;
    Byte uMaxStagePattern = 16;
    StagePattern  sPattern;

    if ((iPos+2) > uDataSize)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    uMaxStagePattern    = pData[iPos++];
    uMaxStage           = pData[iPos++];

    if (1 != uMaxStagePattern || 1 != uMaxStage)
    {
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t 1 != uMaxStagePattern(%d)\n", SHORT_FILE, __LINE__, uMaxStagePattern));   
        return -1;
    }

    iPackSize = Unpacket(pData+iPos, uDataSize-iPos, sPattern);
    if (iPackSize < 0)
    {
        uErrorSts = GBT_MSG_SIZE_SHORT;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tbufer is no enough\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    iPos += iPackSize;
    
    if (0 == sPattern.ucStagePatternId
        || 0 == sPattern.ucStageNo)
    {
        pTscDb->DelStagePattern(uStagePatId, uStageId);
        return iPos;
    }

    if (uStagePatId != sPattern.ucStagePatternId
        || uStageId != sPattern.ucStageNo)
    {
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t sPattern.ucStagePatternId=%d, sPattern.ucStageNo=%d\n", SHORT_FILE, __LINE__,sPattern.ucStagePatternId, sPattern.ucStageNo));   
        return -1;
    }

    if (!pTscDb->ModStagePattern(sPattern.ucStagePatternId, sPattern.ucStageNo, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModStagePattern\n", SHORT_FILE, __LINE__));   
        return -1;
    }

    return iPos;
}

int SetStagePattern(CGbtTscDb* pTscDb, Byte* pData, int uDataSize, Byte uStagePatId, Byte uStageId, Byte uSubId, Byte& uErrorSts, Byte& uErrorIdx)
{
    int iPackSize;
    StagePattern  sPattern;

    if (!pTscDb->QueryStagePattern(uStagePatId, uStageId, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to QueryStagePattern(%d, %d)\n", SHORT_FILE, __LINE__, uStagePatId, uStageId));   
        return -1;
    }
  
    switch (uSubId)
    {
    case 3:
        iPackSize = sizeof (sPattern.usAllowPhase);
        if (iPackSize > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        UNPACKET_USHORT(pData, sPattern.usAllowPhase);
        break;

    case 4:
        iPackSize = sizeof (sPattern.ucGreenTime);
        if (iPackSize > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        UNPACKET_UCHAR(pData, sPattern.ucGreenTime);
        break;
    case 5:
        iPackSize = sizeof (sPattern.ucYellowTime);
        if (iPackSize > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        UNPACKET_UCHAR(pData, sPattern.ucYellowTime);
        break;
    case 6:
        iPackSize = sizeof (sPattern.ucRedTime);
        if (iPackSize > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        UNPACKET_UCHAR(pData, sPattern.ucRedTime);
        break;
    case 7:
        iPackSize = sizeof (sPattern.ucOption);
        if (iPackSize > uDataSize)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));   
            return -1;
        }
        UNPACKET_UCHAR(pData, sPattern.ucOption);
        break;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t uSubId=%d\n", SHORT_FILE, __LINE__, uSubId));   
        return -1;
    }

    if (!pTscDb->ModStagePattern(sPattern.ucStagePatternId, sPattern.ucStageNo, sPattern))
    {
        uErrorSts = GBT_MSG_ERROR_OTHER;
        uErrorIdx = 0;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\tFailed to ModStagePattern\n", SHORT_FILE, __LINE__));   
        return -1;
    }
    return iPackSize;
}



int ExchangeDeviceId(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Ushort, SYS_DEVICEID);
}

int ExchangeMaxModule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_MODULE);
}

int ExchangeSynchTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_SYNCHTIME);
}

int ExchangeSynchFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Ushort, SYS_SYNCHFLAG);
}

int ExchangeModule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetModule, SetModule);
}

int ExchangeZone(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHAGE_SYSTEM_SIGN_OBJECT(Long, SYS_ZONE);
}

int ExchangeMaxPlan(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_PLAN);
}

int ExchangeMaxSchedule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_SCHEDULE);
}

int ExchangeMaxSegment(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_SCHEDULEEVT);
}

int ExchangePlan(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetPlan, SetPlan);
}

int ExchangeSchedule(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_TWOIDX_OBJ(GetSchedule, SetSchedule);
}

int ExchangeMaxEvtType(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_EVENTTYPE);
}

int ExchangeMaxEvtLog(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_EVENTLOG);
}

int ExchangeEventType(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetEventType, SetEventType);
}

int ExchangeEventLog(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_TWOIDX_OBJ(GetEventLog, SetEventLog);
}

int ExchangeMaxPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_PHASE);
}

int ExchangeMaxPhaseGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJGRP(CONST_PHASE);
}
int ExchangePhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetPhase, SetPhase);
}

int ExchangeCollision(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetCollision, SetCollision);
}

int ExchangeMaxDetector(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_DETECTOR);
}

int ExchangeMaxDetGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJGRP(CONST_DETECTOR);
}
int ExchangeDetDataSeqNo(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_DETDATA_SEQNO);
}
int ExchangeDetDataCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_DETDATA_CYCLE);
}
int ExchangeActiveDetCount(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    return -1;
}
int ExchangePulseSeqNo(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_DETPULSE_SEQNO);
}
int ExchangePulseCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte,SYS_DETPULSE_CYCLE);
}
int ExchangeDetector(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetDetector, SetDetector);
}

int ExchangeFlsTimeWhenStart(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte,SYS_FLSTIME_START);
}
int ExchangeAllRedTimeWhenStart(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_REDTIME_START);
}

int ExchangeRemoteCtrlFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte,SYS_REMOTECTRL_FLAG);
}

int ExchangeFlashFreq(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_FLASH_FREQ);
}

int ExchangeBrtCtrlBgnTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Ulong, SYS_BRTTIME_BEGIN);
}
int ExchangeBrtCtrlEndTime(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Ulong, SYS_BRTTIME_END);
}

int ExchangeMaxChannel(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_CHANNEL);
}
int ExchangeMaxChanlGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJGRP(CONST_CHANNEL);
}

int ExchangeChannel(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetChannel, SetChannel);
}

int ExchangeMaxPattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_PATTERN);
}
int ExchangeMaxStagePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_STAGEPATTERN);
}
int ExchangeMaxStage(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_STAGE);
}
int ExchangeGlobalCycle(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_GLOBAL_CYCLE);
}
int ExchangeCoorPhaseOffset(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_PHASE_OFFSET);
}
int ExchangeDegradeMode(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Char,SYS_DEGRADE_MODE);
}
int ExchangeDegradePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    DbByteArray sPattern;
    Byte* pPattern;
    Byte  ucCount;
    switch (iOprType)
    {
    case OPERATE_SET_VALUE:
        ucCount = 14;
        if (uDataSize < ucCount)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));  
            return -1;
        }
        sPattern.SetData(pData, ucCount);
        if(!m_gTscDb->SetDegradePattern(sPattern))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Failed to SetDegradePattern\n", SHORT_FILE, __LINE__));  
            return -1;
        }
        return ucCount;

    case OPERATE_GET_VALUE:
        if(!m_gTscDb->GetDegradePattern(sPattern))
        {
            uErrorSts = GBT_MSG_ERROR_OTHER;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t Failed to GetDegradePattern\n", SHORT_FILE, __LINE__));  
            return -1;
        }
        pPattern = sPattern.GetData(ucCount);
        if (uDataSize < ucCount)
        {
            uErrorSts = GBT_MSG_SIZE_SHORT;
            ACE_DEBUG ((LM_DEBUG, "%s:%04d\t buffer not enough\n", SHORT_FILE, __LINE__));  
            return -1;
        }
        ACE_OS::memcpy(pData, pPattern, ucCount);
        return ucCount;
    default:
        uErrorSts = GBT_MSG_OBJ_ERROR;
        ACE_DEBUG ((LM_DEBUG, "%s:%04d\t iOprType = %d\n", SHORT_FILE, __LINE__, iOprType));  
        return -1;
    }
}
int ExchangeCtrlMasterOptFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_CTRL_OPTFLAG);
}

int ExchangeSigDevCount(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_SIGDEV_COUNT);
}
int ExchangeBaseAddr(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Ushort, SYS_BASE_ADDR);
}

int ExchangePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetPattern, SetPattern);
}

int ExchangeMaxOverlapPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJ(CONST_OVAERLAPPHASE);
}
int ExchangeMaxOverlapPhaseGrp(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_CONST_ONE_OBJGRP(CONST_OVAERLAPPHASE);
}

int ExchangeOverlapPhase(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_ONEIDX_OBJ(GetOverlapPhase, SetOverlapPhase);
}

int ExchangeSpecFun(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetSpecFun, SetSpecFun);
}

int ExchangeDetExtend(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetDetExtend, SetDetExtend);
}

int ExchangeAdaptPara(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetAdaptPara, SetAdaptPara);
}

int ExchangeCntDownDev(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetCntDownDev, SetCntDownDev);
}

int ExchangeAdaptStage(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetAdaptStage, SetAdaptStage);
}

int ExchangePhaseToDirec(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
	EXCHANGE_ONEIDX_OBJ(GetPhaseToDirec, SetPhaseToDirec);
}

int ExchangeStagePattern(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_TWOIDX_OBJ(GetStagePattern, SetStagePattern);
}

int ExchangeDownloadFlag(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx)
{
    EXCHANGE_SYSTEM_ONE_OBJ(Byte, SYS_DOWNLOAD_FLAG);
}

int ExchangeChannelChk(int iOprType, Byte uIdxFst, Byte uIdxSnd, Byte uSubId, Byte* pData, int uDataSize, Byte& uErrorSts, Byte& uErrorIdx) //ADD?20130801
{
	EXCHANGE_ONEIDX_OBJ(GetChannelChk, SetChannelChk);   //ADD:2013 08 01 14 30

}
}
