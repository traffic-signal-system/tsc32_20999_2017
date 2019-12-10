/********************************************************************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   TscKernelGb20999-2017.cpp
Date:       2019-5-1
Description: 国标GB20999-2017附录协议实现核心类
Version:    V1.0
History:
**********************************************************************************************************************/
#include "TscKernelGb20999-2017.h"
#include "Configure.h"
#include "Define.h"
#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Date_Time.h"
#include "GbtDb.h"
#include "GbtMsgQueue.h"
#include "Detector.h"
#include "DbInstance.h"
#include "TscMsgQueue.h"
#include "ComFunc.h"
#include "GbtExchg.h"
#include <exception>

/***************************************************************************************************************
Function:       CTscKernelGb20999::CTscKernelGb20999()
Description:    信号机核心CTscKernelGb20999类构造函数.
                        初始化核心变量值.
Input:             无
Output:         无
Return:         无
****************************************************************************************************************/

CTscKernelGb20999::CTscKernelGb20999()
{
    //ACE_OS::memset(&TscGB25280TscConfig , 0 , sizeof(sGB25280TscConfig) );
    TscGB25280TscConfig.TscCommunicationStatus  =  TSC_COMMUICATION_OFFLINE ;                               //初始化为离线状态
    Configure::CreateInstance()->GetInteger("Location","AreaCode",TscGB25280TscConfig.TscAreaCode);         //获取配置文件区域号
    Configure::CreateInstance()->GetInteger("Location","CrossCode",TscGB25280TscConfig.TscCrossCode);          //获取配置文件路口号
    Configure::CreateInstance()->GetString("APPDESCRIP","IdentifyCode",TscGB25280TscConfig.IdentifyCode);   //获取配置文件控制指令验证码
    Configure::CreateInstance()->GetString("COMMUNICATION","ClientIp",TscGB25280TscConfig.TscClientIp);      //获取配置文件上位端IP
    if(Configure::CreateInstance()->GetInteger("COMMUNICATION","port",TscGB25280TscConfig.iPort)==-1)       //获取配置文件上位端端口
        TscGB25280TscConfig.iPort= 8801 ;

    TscGB25280TscConfig.bGetQueryResponse = false ;  //判断是否收到查询应答

    TscGB25280TscConfig.bUpTrafficFlow = false ;   //是否自动上传交通流
    TscGB25280TscConfig.bUpTscVerUp  = false ;    //是否自动上传信号机版本
    TscGB25280TscConfig.bUpTscError  =false ;       //是否自动上传信号机故障
    TscGB25280TscConfig.bUpLampColor =false ;   //是否自动上传灯色状态
    TscGB25280TscConfig.bUpTscWorkStatus =false ;  //是否自动上传工作状态

    ACE_DEBUG((LM_DEBUG,"%s:%d Init CTscKernelGb20999 object ok !\n",__FILE__,__LINE__));
    //ACE_OS::printf("%s:%d TscClientIp=%s areacode=%d crosscode=%d port=%d \r\n",__FILE__,__LINE__,TscGB25280TscConfig.TscClientIp.c_str(),TscGB25280TscConfig.TscAreaCode,TscGB25280TscConfig.TscCrossCode,TscGB25280TscConfig.iPort);
}

/***************************************************************************************************************
Function:       CTscKernelGb20999::~CTscKernelGb20999()
Description:    信号机核心CTscKernelGb20999类析构函数.
Input:             无
Output:         无
Return:         无
****************************************************************************************************************/
CTscKernelGb20999::~CTscKernelGb20999()
{
    ACE_DEBUG((LM_DEBUG,"%s:%d Destruct CTscKernelGb20999 object ok !\n",__FILE__,__LINE__));
}

/***************************************************************************************************************
Function:       CTscKernelGb20999::CreateInstance()
Description:    返回静态单例CTscKernelGb20999类对象指针.
Input:             无
Output:         无
Return:         CTscKernelGb20999对象指针.
****************************************************************************************************************/
CTscKernelGb20999* CTscKernelGb20999::CreateInstance()
{
    static CTscKernelGb20999 TscKernelGb20999;
    return &TscKernelGb20999;
}

/***************************************************************************************************************
Function:       CTscKernelGb20999::GetGbFrameCheckCode
Description:   产生协议数据的校验码  (当前默认没有启用，协议校验码为常量0x0)
Input:             协议数据链表指针
Output:         无
Return:         协议数据校验码
****************************************************************************************************************/
Byte  CTscKernelGb20999::GetGbFrameCheckCode(CLinklistGb209992017Data  *pListFrameTableData)
{
    if (pListFrameTableData == NULL)
        return 0x0;
    Byte ucCheckData, ucTempData;
    int ByteGetRest = pListFrameTableData->Getelem(0x1, &ucCheckData);

    if (ByteGetRest != -1)
    {
        for (int ucIndex = 0x2; ucIndex <= pListFrameTableData->LIST_GetCount(); ucIndex++)
        {
            pListFrameTableData->Getelem(ucIndex, &ucTempData);
            ucCheckData = (ucCheckData) ^ (ucTempData);
        }
        return ucCheckData;
    }
    return 0x0;
}

/***************************************************************************************************************
Function:       CTscKernelGb20999::BytesSendConstructedGB25580FrameData
Description:    通过协议数据结构体构造协议发送数据帧
Input:         Gb20099FrameDataSource   协议数据表结构体
Return:         协议发送数据帧链表指针
****************************************************************************************************************/
CLinklistGb209992017Data* CTscKernelGb20999::BytesSendConstructedGB25580FrameData(SGB209992017FrameTableData Gb20099FrameDataSource)
{
    try
    {
        CLinklistGb209992017Data* GbSourceTableData = new CLinklistGb209992017Data();
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucFrameBeginFlag); //帧头
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucFrameLength);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucRecvFlag);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucDataLinkCode);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucAreaCode);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucRoadCode & 0xFF);
        GbSourceTableData->LIST_Add((Gb20099FrameDataSource.ucRoadCode >> 8) & 0xFF);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucOperaType);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucObjCode);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucReserveDara[0]);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucReserveDara[1]);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucReserveDara[2]);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucReserveDara[3]);
        GbSourceTableData->LIST_Add(Gb20099FrameDataSource.ucReserveDara[4]);

        if (Gb20099FrameDataSource.pTableData != NULL)
        {
            for (Uint index = 0x0; index < Gb20099FrameDataSource.uiTableDataLen; index++)
            {
                GbSourceTableData->LIST_Add(Gb20099FrameDataSource.pTableData[index]);
            }
            delete []Gb20099FrameDataSource.pTableData;
        }
        //  Byte objetc = 0x0 ;
        //   GbSourceTableData->Getelem(9,&objetc);
        //ACE_Date_Time tvTime(ACE_OS::gettimeofday());
        //ACE_OS::printf("\r\n%s:%d  Send Table Data obj=%d Time: %ld:%ld:%ld\r\n", __FILE__,__LINE__,objetc,tvTime.hour(),tvTime.minute(),tvTime.second());

        int SourceListcount = GbSourceTableData->LIST_GetCount();
        CLinklistGb209992017Data* GbConstructedTableData = new CLinklistGb209992017Data();

        int ByteGetRest;
        Byte ByteNode;
        for (int index = 1; index <= SourceListcount; index++)
        {
            ByteGetRest = GbSourceTableData->Getelem(index, &ByteNode);

            if (ByteGetRest != -1)
            {
                if (ByteNode == 0xc0)
                {
                    GbConstructedTableData->LIST_Add(0xDB);
                    GbConstructedTableData->LIST_Add(0xDC);
                }
                else if (ByteNode == 0xdb)
                {
                    GbConstructedTableData->LIST_Add(0xDB);
                    GbConstructedTableData->LIST_Add(0xDD);
                }
                else
                {
                    GbConstructedTableData->LIST_Add(ByteNode);
                }
            }

        }
        delete GbSourceTableData;
        CLinklistGb209992017Data* GbConstructedFrameData = new CLinklistGb209992017Data();

        GbConstructedFrameData->LIST_Add(0x7E);   //添加协议帧头.
        Byte TempData;
        for (int index = 1; index <= GbConstructedTableData->LIST_GetCount(); index++)
        {
            if (GbConstructedTableData->Getelem(index, &TempData) != -1)
                GbConstructedFrameData->LIST_Add(TempData);
        }
        GbConstructedFrameData->LIST_Add(0x0);    //添加协议校验码,统一为0x0.
        GbConstructedFrameData->LIST_Add(0x7D); //添加协议帧尾.

        // ACE_OS::printf("\r\n%s:%d Construced Send Frame Data ByteNum=%d  Time: %ld:%ld:%ld\r\n", __FILE__,__LINE__,GbConstructedFrameData->LIST_GetCount(),tvTime.hour(),tvTime.minute(),tvTime.second());
        // GbConstructedFrameData->LIST_Print();   //打印最终构造的协议发送数据帧.
        delete GbConstructedTableData;
        return GbConstructedFrameData;
    }
    catch (...)
    {
        ACE_OS::printf("\r\n%s:%d Constructed Send Gb25280data exception!\r\n",__FILE__,__LINE__);
        return NULL ;
    }

}

/***************************************************************************************************************
Function:       CTscKernelGb20999::BytesRecvDecodeGB25580FrameData
Description:    RecvedGbFrameData -上位端发送过来的协议数据字节数组
                       RecvDataLen -上位端发送过来的协议数据字节长度
Input:         Gb20099FrameDataSource   协议数据表结构体
Out:                无
Return:         通过解析得到的协议表数据链表指针
****************************************************************************************************************/
CLinklistGb209992017Data* CTscKernelGb20999::BytesRecvDecodeGB25580FrameData(Byte RecvedGbFrameData[],Uint RecvDataLen)
{
    try
    {
        Uint RecvTableDataLen = 0x0;
        Byte RecvDataCheckCode = 0x0;
        if (RecvedGbFrameData[RecvDataLen - 0x3] == 0xDB && RecvedGbFrameData[RecvDataLen - 0x2] == 0xDC)
            RecvDataCheckCode = 0xC0;
        else if (RecvedGbFrameData[RecvDataLen - 0x3] == 0xDB && RecvedGbFrameData[RecvDataLen - 0x2] == 0xDD)
            RecvDataCheckCode = 0xDB;
        else
            RecvDataCheckCode = RecvedGbFrameData[RecvDataLen - 0x2];    //获取校验码

        if (RecvDataCheckCode == 0xC0 || RecvDataCheckCode == 0xDB)
            RecvTableDataLen = RecvDataLen - 0x4;
        else
            RecvTableDataLen = RecvDataLen - 0x3;       //判断数据表长度

        CLinklistGb209992017Data* GbTableDataList = new CLinklistGb209992017Data();
        for (Uint index = 0x0; index < RecvTableDataLen; index++)
        {
            GbTableDataList->LIST_Add(RecvedGbFrameData[index + 0x1]);
        }
        CLinklistGb209992017Data* GbDecodeTableDataList = new CLinklistGb209992017Data();
        Byte tempdata1, tempdata2 = 0x0;
        for (Uint posindex = 0x1; posindex <= RecvTableDataLen; posindex++)
        {
            if (GbTableDataList->Getelem(posindex, &tempdata1) == -1)
            {
                break;
            }

            if (posindex == RecvTableDataLen)
            {
                tempdata2 = tempdata1;
            }
            else
            {
                if (GbTableDataList->Getelem(posindex + 1, &tempdata2) == -1)
                    break;
            }
            if (tempdata1 == 0xDB && tempdata2 == 0xDC)
            {
                GbDecodeTableDataList->LIST_Add(0xC0);
                posindex++;
            }
            else if (tempdata1 == 0xDB && tempdata2 == 0xDD)
            {
                GbDecodeTableDataList->LIST_Add(0xDB);
                posindex++;
            }
            else
            {
                GbDecodeTableDataList->LIST_Add(tempdata1);
            }
        }

        /* 打印解析后的协议表数据
        ACE_Date_Time tvTime(ACE_OS::gettimeofday());
        Byte objetc= 0x0;
        GbDecodeTableDataList->Getelem(9,&objetc);
        if(objetc != 0x1)
        {
            ACE_OS::printf("\r\n%s:%d  Decode Recved Table Data obj=%d  ByteNum=%d  Time: %ld:%ld:%ld\r\n", __FILE__,__LINE__,objetc,GbDecodeTableDataList->LIST_GetCount(),tvTime.hour(),tvTime.minute(),tvTime.second());
            GbDecodeTableDataList->LIST_Print();
        }
        */
        delete GbTableDataList;
        return GbDecodeTableDataList;
    }
    catch (...)
    {
        printf("\r\nDecode Received Gb25280data exception!\r\n");
        return NULL ;
    }

}

/***************************************************************************************************************
 Function:       CTscKernelGb20999::GetTscCommunicationStatus
 Description:    获取信号机离线/联机状态
 Input:         无
 Out:                无
 Return:         信号机离线/在线   0x0-离线 0x1-联机
 ****************************************************************************************************************/
Byte CTscKernelGb20999::GetTscCommunicationStatus()
{
    return TscGB25280TscConfig.TscCommunicationStatus ;
}

/***************************************************************************************************************
   Function:    CTscKernelGb20999::ConstruectOnlineRequestReply
   Description:   构造信号机联机请求数据
   Input:           无
   Out:            BytesArrayLen -生成数据字节长度
   Return:          信号机联机请求数据字节数组指针
   ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectOnlineRequest(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_COMMUNICATIONPROC_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_REQ ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_ONLINE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;  //默认保留数据字节值为0x30
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.pTableData = NULL ;
    Gb20099FrameDataSource.uiTableDataLen = 0x0 ;

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
   Function:    CTscKernelGb20999::ConstruectOnlineQuery
   Description:   构造信号机联机查询数据
   Input:           无
   Out:          BytesArrayLen -  生成数据字节长度
   Return:          信号机联机查询数据字节数组指针
   ****************************************************************************************************************/
Byte* CTscKernelGb20999:: ConstruectOnlineQuery(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_COMMUNICATIONPROC_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_REQ ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_ONLINE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.pTableData = NULL ;
    Gb20099FrameDataSource.uiTableDataLen = 0x0 ;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
   Function:    CTscKernelGb20999::ConstruectTrafficFlowSelfReport
   Description:   构造信号机交通流自动上传数据
   Input:           无
   Out:           BytesArrayLen -  生成数据字节长度
   Return:          信号机交通流自动上传数据字节数组指针
   ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectTrafficFlowSelfReport(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_COMMUNICATIONPROC_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SELF_REPORT ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TRAFFIC_FLOW_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0x30*6+1 ;        //  默认上传48个检测器交通流
    Gb20099FrameDataSource.pTableData = new Byte[Gb20099FrameDataSource.uiTableDataLen];
    Gb20099FrameDataSource.pTableData[0] = 0x30;
    SDetectorData sDecTrafficFlow[48];
    CDetector::CreateInstance()->GetDetData(sDecTrafficFlow);
    for(int index= 0x0; index<0x30; index++)
    {
        Gb20099FrameDataSource.pTableData[index*6+1] = sDecTrafficFlow[index].ucId;
        Gb20099FrameDataSource.pTableData[index*6+2] = sDecTrafficFlow[index].ucVolume;
        Gb20099FrameDataSource.pTableData[index*6+3] = 0x0;
        Gb20099FrameDataSource.pTableData[index*6+4] = 0x0;
        Gb20099FrameDataSource.pTableData[index*6+5] = 0x0;
        Gb20099FrameDataSource.pTableData[index*6+6] = 0x0;
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
   Function:    CTscKernelGb20999::ConstruectWorkStatusSelfReply
   Description:   构造信号机工作状态自动上传数据
   Input:           无
   Out:           BytesArrayLen -  生成数据字节长度
   Return:          信号机工作状态自动上传数据字节数组指针
   ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectWorkStatusSelfReply(int & BytesArrayLen)
{
    CManaKernel * pManakernel = CManaKernel::CreateInstance();
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SELF_REPORT ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_WORKSTATUS_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0x6;
    Gb20099FrameDataSource.pTableData = new Byte[0x6];
    Uint AllowPhase = pManakernel->m_pRunData->sStageStepInfo[pManakernel->m_pRunData->ucStepNo].uiAllowPhase;
    Gb20099FrameDataSource.pTableData[0] = (Byte)(AllowPhase&0xFF);                                           //当前阶段相位号
    Gb20099FrameDataSource.pTableData[1] = (Byte)((AllowPhase>>0x8)&0xFF);                             //当前阶段相位号
    Gb20099FrameDataSource.pTableData[2] =(Byte)((AllowPhase>>0x10)&0xFF);                            //当前阶段相位号
    Gb20099FrameDataSource.pTableData[3] = (Byte)((AllowPhase>>0x18)&0xFF);                           //当前阶段相位号
    Gb20099FrameDataSource.pTableData[4] = pManakernel->m_pRunData->ucTimePatternId;    //当前配时方案号
    Gb20099FrameDataSource.pTableData[5] = pManakernel->m_pRunData->uiCtrl;                      //当前信号机控制方式

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
  Function:       CTscKernelGb20999::ConstruectLampColorStatusSelfReply
  Description:   构造信号机灯色状态自动上传数据
  Input:          无
  Out:               BytesArrayLen -  生成数据字节长度
  Return:         信号机灯色状态自动上传数据字节数组指针
  ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectLampColorStatusSelfReply(int & BytesArrayLen)
{
    CManaKernel * pManakernel = CManaKernel::CreateInstance();
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SELF_REPORT ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_LAMPCOLOR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0xC;
    Gb20099FrameDataSource.pTableData = new Byte[Gb20099FrameDataSource.uiTableDataLen];

    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Byte *pLampColor=pManakernel->m_pRunData->sStageStepInfo[pManakernel->m_pRunData->ucStepNo].ucLampOn;
    for ( Byte  i=0; i<MAX_LAMP; i++ )
    {
        if ( pLampColor[i] == 0 )  //非灯亮
        {
            continue;
        }
        switch ( i%3 )
        {
            case LAMP_COLOR_RED:          // 灯组红灯亮
                Gb20099FrameDataSource.pTableData[i/12] =0x3<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            case LAMP_COLOR_YELLOW:  // 灯组黄灯亮
                Gb20099FrameDataSource.pTableData[i/12] =0x2<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            case LAMP_COLOR_GREEN:    // 灯组绿灯亮
                Gb20099FrameDataSource.pTableData[i/12] =0x1<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            default:
                break;
        }
    }
    Gb20099FrameDataSource.pTableData[8]   =  0x0;
    Gb20099FrameDataSource.pTableData[9]   =  0x0;
    Gb20099FrameDataSource.pTableData[10] = 0x0;
    Gb20099FrameDataSource.pTableData[11] = 0x0;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
 Function:       CTscKernelGb20999::ConstruectTimeQueryReply
 Description:   构造信号机时间查询应答数据
 Input:      无
 Out:             BytesArrayLen -    生成数据字节长度
 Return:         信号机时间查询应答数据字节数组指针
 ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectTimeQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_CURRENTTIME_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0x4;
    Gb20099FrameDataSource.pTableData = new Byte[Gb20099FrameDataSource.uiTableDataLen];

    ACE_Time_Value tvCurTime = ACE_OS::gettimeofday();
    Uint iTotalSec   = (Uint)tvCurTime.sec();        //utc time
    iTotalSec -=8*3600;
    Gb20099FrameDataSource.pTableData[0]=(Byte)( iTotalSec &0xFF);

    Gb20099FrameDataSource.pTableData[1]=(Byte)( iTotalSec>>8 &0xFF);

    Gb20099FrameDataSource.pTableData[2]=(Byte)( iTotalSec>>16 &0xFF);

    Gb20099FrameDataSource.pTableData[3]=(Byte)( iTotalSec>>24 &0xFF);

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:     CTscKernelGb20999::ConstruectTimeSetReply
Description:   构造信号机时间设置应答数据
Input:        无
Out:           BytesArrayLen -    生成数据字节长度
Return:       信号机时间设置应答数据字节数组指针
****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectTimeSetReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_CURRENTTIME_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;       //五个保留字节对应设置指令验证码:ABCDE
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen = 0x0;
    Gb20099FrameDataSource.pTableData = NULL;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}



/***************************************************************************************************************
  Function:       CTscKernelGb20999::ConstruectLampColorStatusReply
  Description:   构造信号机灯色状态应答数据
  Input:             无
  Out:            BytesArrayLen -    生成数据字节长度
  Return:         信号机灯色状态应答数据字节数组指针
  ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectLampColorStatusReply(int & BytesArrayLen)
{
    CManaKernel * pManakernel = CManaKernel::CreateInstance();
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_LAMPCOLOR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0xC;
    Gb20099FrameDataSource.pTableData = new Byte[Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Byte *pLampColor=pManakernel->m_pRunData->sStageStepInfo[pManakernel->m_pRunData->ucStepNo].ucLampOn;
    for ( int i=0; i<MAX_LAMP; i++ )
    {
        if ( pLampColor[i] == 0 )  //非灯亮
        {
            continue;
        }
        switch ( i%3 )
        {
            case LAMP_COLOR_RED:
                Gb20099FrameDataSource.pTableData[i/12] =0x3<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            case LAMP_COLOR_YELLOW:
                Gb20099FrameDataSource.pTableData[i/12] =0x2<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            case LAMP_COLOR_GREEN:
                Gb20099FrameDataSource.pTableData[i/12] =0x1<<2*((i/3)%4) |Gb20099FrameDataSource.pTableData[i/12];
                break;
            default:
                break;
        }
    }

    Gb20099FrameDataSource.pTableData[8]   =  0x0;
    Gb20099FrameDataSource.pTableData[9]   =  0x0;
    Gb20099FrameDataSource.pTableData[10] = 0x0;
    Gb20099FrameDataSource.pTableData[11] = 0x0;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
 Function:    CTscKernelGb20999::ConstruectWorkStatusReply
 Description:    构造信号机工作状态应答数据
 Input:      无
 Out:         BytesArrayLen -    生成数据字节长度
 Return:      信号机工作状态应答数据字节数组指针
 ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectWorkStatusReply(int & BytesArrayLen)
{
    CManaKernel * pManakernel = CManaKernel::CreateInstance();
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_WORKSTATUS_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0x6;
    Gb20099FrameDataSource.pTableData = new Byte[0x6];
    Uint AllowPhase = pManakernel->m_pRunData->sStageStepInfo[pManakernel->m_pRunData->ucStepNo].uiAllowPhase;
    Gb20099FrameDataSource.pTableData[0] = (Byte)(AllowPhase&0xFF);                                           //当前阶段相位号
    Gb20099FrameDataSource.pTableData[1] = (Byte)((AllowPhase>>0x8)&0xFF);                             //当前阶段相位号
    Gb20099FrameDataSource.pTableData[2] =(Byte)((AllowPhase>>0x10)&0xFF);                            //当前阶段相位号
    Gb20099FrameDataSource.pTableData[3] = (Byte)((AllowPhase>>0x18)&0xFF);                           //当前阶段相位号
    Gb20099FrameDataSource.pTableData[4] = pManakernel->m_pRunData->ucTimePatternId;    //当前配时方案号
    Gb20099FrameDataSource.pTableData[5] = pManakernel->m_pRunData->uiCtrl;                      //当前信号机控制方式

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
 Function:    CTscKernelGb20999::ConstruectLampGrpQueryReply
 Description:    构造信号机信号灯组查询应答数据
 Input:      无
 Out:         BytesArrayLen -    生成数据字节长度
 Return:      信号机信号灯组查询应答数据字节数组指针
 ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectLampGrpQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_LANPGROUP_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    GBT_DB::TblChannel  tblChannel;
    GBT_DB::Channel*  pChannel;
    Byte  iindex;
    unsigned char  ucCountChannel;
    if((CDbInstance::m_cGbtTscDb).QueryChannel(tblChannel)==false)
        return NULL;
    pChannel  = tblChannel.GetData(ucCountChannel);
    Gb20099FrameDataSource.uiTableDataLen = ucCountChannel*12+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = ucCountChannel;
    int iindex2 = 0x0;
    for(iindex = 0x0 ; iindex<ucCountChannel; iindex++)
    {
        Gb20099FrameDataSource.pTableData[++iindex2]=pChannel[iindex].ucChannelId;
        Gb20099FrameDataSource.pTableData[++iindex2]=pChannel[iindex].ucCtrlSrc;
        Gb20099FrameDataSource.pTableData[++iindex2]= pChannel[iindex].ucAutoFlsCtrlFlag;
        Gb20099FrameDataSource.pTableData[++iindex2]=pChannel[iindex].ucCtrlType;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}



/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectLampGrpSetReply
  Description:   构造信号机信号灯组设置应答数据,应答数据包含设置的数据内容.
  Input:         pData -设置内容数据指针
            Len -     设置内容数据长度
  Out:        BytesArrayLen -    生成数据字节长度
  Return:         信号机信号灯组设置应答数据字节数组指针
  ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectLampGrpSetReply(int & BytesArrayLen,Byte*pData,int Len)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_LANPGROUP_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen = Len;
    Gb20099FrameDataSource.pTableData =new Byte[Len];
    for(int index =0x0; index< Len; index++)
        (Gb20099FrameDataSource.pTableData)[index] = pData[14+index];

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}


/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectPlanPatternQueryReply
  Description:    构造信号机调度计划查询应答数据.
  Input:      无
  Out:         BytesArrayLen -    生成数据字节长度.
  Return:          信号机调度计划查询应答数据字节数组指针.
  ****************************************************************************************************************/
Byte*  CTscKernelGb20999::ConstruectPlanPatternQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_PLAN_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;

    GBT_DB::TblPlan tblPlan;
    GBT_DB::TblSchedule tblSchedule;
    GBT_DB::Plan*   pPlan;
    GBT_DB::Schedule* pSchedule;
    Byte   ucCountPlan,ucPlanCount, iindexi,iindexy;
    Ushort ucCountSchedule,ucCountSum ;
    ucPlanCount = 0x0;
    iindexy = 0x0;
    if((CDbInstance::m_cGbtTscDb).QueryPlan(tblPlan)==false)
        return NULL;
    if((CDbInstance::m_cGbtTscDb).QuerySchedule(tblSchedule)==false)
        return NULL;
    pPlan         = tblPlan.GetData(ucCountPlan);
    pSchedule = tblSchedule.GetData(ucCountSchedule);
    ucCountSum = tblPlan.GetCount()+tblSchedule.GetCount();
    Gb20099FrameDataSource.uiTableDataLen =(tblPlan.GetCount()+tblSchedule.GetCount())*12+0x1; // ucPlanCount*12+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = ucCountSum;
    for(iindexi=0x0; iindexi<ucCountPlan; iindexi++)
    {
        Gb20099FrameDataSource.pTableData[++iindexy]  =0x1 ;                        //计划表
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPlan[iindexi].ucId; //调度号
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPlan[iindexi].usMonthFlag&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =(pPlan[iindexi].usMonthFlag>>8)&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPlan[iindexi].ucWeekFlag;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPlan[iindexi].ulDayFlag&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =(pPlan[iindexi].ulDayFlag>>8)&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =(pPlan[iindexi].ulDayFlag>>16)&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =(pPlan[iindexi].ulDayFlag>>24)&0xff;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPlan[iindexi].ucScheduleId;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;

    }
    for(iindexi=0x0; iindexi<ucCountSchedule; iindexi++)
    {
        Gb20099FrameDataSource.pTableData[++iindexy]   =0x2 ; //时段表
        Gb20099FrameDataSource.pTableData[++iindexy]   = pSchedule[iindexi].ucScheduleId;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucEvtId;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucBgnHour;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucBgnMinute;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucCtrlMode;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucPatternNo;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucAuxOut;
        Gb20099FrameDataSource.pTableData[++iindexy]  =pSchedule[iindexi].ucSpecialOut;
        Gb20099FrameDataSource.pTableData[++iindexy]   =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]    =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]    =0x0;
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectPlanPatternSetReply
  Description:   构造信号调度计划设置应答数据,应答数据包含设置的数据内容.
  Input:         pData -设置内容数据指针
                 Len -    设置内容数据长度
  Out:        BytesArrayLen -    生成数据字节长度
  Return:         信号机调度计划设置应答数据字节数组指针
  ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectPlanPatternSetReply(int & BytesArrayLen,Byte*pData,int Len)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_PLAN_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen = Len;
    Gb20099FrameDataSource.pTableData =new Byte[Len];

    for(int index =0x0; index< Len; index++)
        (Gb20099FrameDataSource.pTableData)[index] = pData[14+index];

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}


/***************************************************************************************************************
 Function:   CTscKernelGb20999::ConstruectWorkTypeSetReply
 Description:   构造信号机工作方式设置应答数据.
 Input:         无
 Out:        BytesArrayLen -    生成数据字节长度.
 Return:         信号机工作方式设置应答数据字节数组指针.
 ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectWorkTypeSetReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_WORKTYPE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen =0x0;
    Gb20099FrameDataSource.pTableData = NULL;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }

}

/***************************************************************************************************************
Function:    CTscKernelGb20999::ConstruectWorkTypeSetReply
Description:   构造信号机工作方式查询应答数据.
Input:       无
Out:         BytesArrayLen -    生成数据字节长度.
Return:      信号机工作方式查询应答数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectWorkTypeQueryReply(int & BytesArrayLen)
{
    CManaKernel * pManakernel = CManaKernel::CreateInstance();
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_WORKTYPE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen =0x1;
    Gb20099FrameDataSource.pTableData =new Byte[1];
    Gb20099FrameDataSource.pTableData[0]= CGbtMsgQueue::CreateInstance()->GetCtrlStatus(pManakernel->m_pRunData->uiWorkStatus,pManakernel->m_pRunData->uiCtrl);

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:    CTscKernelGb20999::ConstruectErrorQueryReply
Description:   构造信号机故障查询应答数据.
Input:       无
Out:         BytesArrayLen -    生成数据字节长度.
Return:      信号机故障查询应答数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectErrorQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCERROR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    GBT_DB::TblEventLog tblEventLog;
    GBT_DB::EventLog*   pEventLog;
    Byte    iindex,iErrorCount;
    Uint ucCountEventLog;
    iErrorCount = 0x0;
    if((CDbInstance::m_cGbtTscDb).QueryEventLog(tblEventLog)==false)
        return NULL;
    pEventLog  = tblEventLog.GetData(ucCountEventLog);
    for(iindex = 0x0 ; iindex<ucCountEventLog; iindex++)
    {
        if(pEventLog[iindex].ucEvtType     ==   LOG_TYPE_GREEN_CONFIG||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DOOR_WARN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_OUTLAMP_ERR||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_LAMP||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_CAN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_FLASHBOARD||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DETECTOR
          )
            iErrorCount++;
    }
    Gb20099FrameDataSource.uiTableDataLen = iErrorCount*12+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = iErrorCount;
    int iindex2 = 0x0;
    for(iindex = 0x0 ; iindex<ucCountEventLog; iindex++)
    {
        if(pEventLog[iindex].ucEvtType     ==   LOG_TYPE_GREEN_CONFIG||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DOOR_WARN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_OUTLAMP_ERR||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_LAMP||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_CAN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_FLASHBOARD||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DETECTOR )
        {
            Gb20099FrameDataSource.pTableData[++iindex2]=  pEventLog[iindex].ucEvtType;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulEvtValue &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulEvtValue>>8 &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]=pEventLog[iindex].ulEvtValue >>16&0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulEvtValue>>24 &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulHappenTime &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]=pEventLog[iindex].ulHappenTime>>8 &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulHappenTime>>16 &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]= pEventLog[iindex].ulHappenTime>>24 &0xff;
            Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
            Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
            Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        }
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:    CTscKernelGb20999::ConstruectErrorSelfReply
Description:   构造信号机故障主动上传数据.
Input:       无
Out:         BytesArrayLen -    生成数据字节长度.
Return:      信号机故障主动上传数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectErrorSelfReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SELF_REPORT ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCERROR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.pTableData =NULL;
    GBT_DB::TblEventLog tblEventLog;
    GBT_DB::EventLog*   pEventLog;
    Byte  iindex,iErrorCount;
    Uint ucCountEventLog;
    iErrorCount = 0x0;
    if((CDbInstance::m_cGbtTscDb).QueryEventLog(tblEventLog)==false)
        return NULL;
    pEventLog  = tblEventLog.GetData(ucCountEventLog);
    for(iindex = 0x0 ; iindex<ucCountEventLog; iindex++)
    {
        if(pEventLog[iindex].ucEvtType  ==LOG_TYPE_GREEN_CONFIG||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DOOR_WARN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_OUTLAMP_ERR||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_LAMP||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_CAN||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_FLASHBOARD||
           pEventLog[iindex].ucEvtType ==LOG_TYPE_DETECTOR )
            iErrorCount++;
    }

    Gb20099FrameDataSource.uiTableDataLen = 0xd;//iErrorCount*12+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = 0x1;
    int iindex2 = 0x0;
    uint inittime =0x0 ;
    for(iindex = 0x0 ; iindex<ucCountEventLog; iindex++)
    {
        if( pEventLog[iindex].ucEvtType     ==   LOG_TYPE_GREEN_CONFIG||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_DOOR_WARN||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_OUTLAMP_ERR||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_LAMP||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_FLASHBOARD||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_CAN||
            pEventLog[iindex].ucEvtType ==LOG_TYPE_DETECTOR)
        {
            if(pEventLog[iindex].ulHappenTime >= inittime)
            {
                inittime = pEventLog[iindex].ulHappenTime ;
                Gb20099FrameDataSource.pTableData[1]=  pEventLog[iindex].ucEvtType;
                Gb20099FrameDataSource.pTableData[2]= pEventLog[iindex].ulEvtValue &0xff;
                Gb20099FrameDataSource.pTableData[3]= pEventLog[iindex].ulEvtValue>>8 &0xff;
                Gb20099FrameDataSource.pTableData[4]=pEventLog[iindex].ulEvtValue >>16&0xff;
                Gb20099FrameDataSource.pTableData[5]= pEventLog[iindex].ulEvtValue>>24 &0xff;
                Gb20099FrameDataSource.pTableData[6]= pEventLog[iindex].ulHappenTime &0xff;
                Gb20099FrameDataSource.pTableData[7]=pEventLog[iindex].ulHappenTime>>8 &0xff;
                Gb20099FrameDataSource.pTableData[8]= pEventLog[iindex].ulHappenTime>>16 &0xff;
                Gb20099FrameDataSource.pTableData[9]= pEventLog[iindex].ulHappenTime>>24 &0xff;

                Gb20099FrameDataSource.pTableData[10]= 0x0;
                Gb20099FrameDataSource.pTableData[11] = 0x0;
                Gb20099FrameDataSource.pTableData[12] = 0x0;
            }

        }
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:   CTscKernelGb20999::ConstruectTscVersionQueryReply
Description:   构造信号机版本查询应答数据.
Input:      无
Out:            BytesArrayLen -    生成数据字节长度.
Return:         信号机版本查询应答数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectTscVersionQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCVERSION_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen =0x14;
    Gb20099FrameDataSource.pTableData =new Byte[0x14];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    ACE_TString  Verstr;
    Configure::CreateInstance()->GetString("APPDESCRIP","version",Verstr);
    const char * pString= Verstr.c_str();
    if(ACE_OS::strlen(pString) <=  0x14)
        ACE_OS::memcpy(Gb20099FrameDataSource.pTableData,pString,ACE_OS::strlen(pString));
    //ACE_OS::printf("\r\n%s:%d Verstr =%s \r\n ",__FILE__,__LINE__,pString);
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:    CTscKernelGb20999::ConstruectscVersionSelfReply
Description:   构造信号机版本主动上传数据.
Input:       无
Out:         BytesArrayLen -    生成数据字节长度.
Return:      信号机版本主动上传数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectscVersionSelfReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SELF_REPORT ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCVERSION_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen =0x14;
    Gb20099FrameDataSource.pTableData =new Byte[0x14];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    ACE_TString  Verstr;
    Configure::CreateInstance()->GetString("APPDESCRIP","version",Verstr);
    const char * pString= Verstr.c_str();
    if(ACE_OS::strlen(pString) <=  0x14)
        ACE_OS::memcpy(Gb20099FrameDataSource.pTableData,pString,ACE_OS::strlen(pString));
    // ACE_OS::printf("\r\n%s:%d Verstr =%s \r\n ",__FILE__,__LINE__,pString);
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectscVersionSelfReply
  Description:    构造信号机特征参数查询应答数据.
  Input:   无
  Out:     BytesArrayLen -    生成数据字节长度.
  Return:      信号机特征参数查询应答数据字节数组指针.
  ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectParameterVerQueryReply(int & BytesArrayLen)
{
    char ParaVerStr[5] = {0x0};
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion  =  GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag  =  GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCPARAVER_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen =0x1;

    (CDbInstance::m_cGbtTscDb).GetEypSerial(ParaVerStr);
    Gb20099FrameDataSource.pTableData =new Byte[0x1];
    Gb20099FrameDataSource.pTableData[0x0]=(Byte)( ACE_OS::atoi(ParaVerStr));
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:   CTscKernelGb20999::ConstruectscVersionSelfReply
Description:   构造信号机特征参数设置应答数据.
Input:      无
Out:    BytesArrayLen -    生成数据字节长度.
Return:     信号机特征参数设置应答数据字节数组指针.
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectParameterVerSetReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCPARAVER_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen =0x0;
    Gb20099FrameDataSource.pTableData= NULL;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}


/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectTscIDQueryReply
  Description:    构造信号机识别码查询应答数据.
  Input:   无
  Out:     BytesArrayLen -    生成数据字节长度.
  Return:      信号机识别码查询数据字节数组指针.
  ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectTscIDQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_TSCIC_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen =0xE;
    Gb20099FrameDataSource.pTableData =new Byte[0xE];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    ACE_TString  strIdcode;
    Configure::CreateInstance()->GetString("APPDESCRIP","IDCode",strIdcode);
    const char * pString= strIdcode.c_str();
    if(ACE_OS::strlen(pString) == 0xE)
        ACE_OS::memcpy(Gb20099FrameDataSource.pTableData,pString,Gb20099FrameDataSource.uiTableDataLen);
    //ACE_OS::printf("\r\n%s:%d IDCodeStr =%s \r\n ",__FILE__,__LINE__,pString);
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
      Function:   CTscKernelGb20999::ConstruectTscRemoteCtrlReply
      Description:   构造信号机远程控制设置应答数据.
      Input:      无
      Out:    BytesArrayLen -    生成数据字节长度.
      Return:     信号机远程控制设置应答数据字节数组指针.
      ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectTscRemoteCtrlReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_BASICINFOLINK_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_REMOTECTRL_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen =0x0;
    Gb20099FrameDataSource.pTableData= NULL;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}
/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectTscRemoteCtrlReply
  Description:   构造信号机检测器查询数据.
  Input:      无
  Out:    BytesArrayLen -    生成数据字节长度.
  Return:     信号机检测器查询数据字节数组指针.
  ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectDetectorQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_DETECTOR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    GBT_DB::TblDetector tblDetector;
    GBT_DB::Detector*   pDetector;
    Byte  iindex;
    unsigned char  ucCountDetector;
    if((CDbInstance::m_cGbtTscDb).QueryDetector(tblDetector)==false)
        return NULL;
    pDetector  = tblDetector.GetData(ucCountDetector);

    Gb20099FrameDataSource.uiTableDataLen = ucCountDetector*15+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = ucCountDetector;
    int iindex2 = 0x0;
    for(iindex = 0x0 ; iindex<ucCountDetector; iindex++)
    {
        Gb20099FrameDataSource.pTableData[++iindex2]=  pDetector[iindex].ucDetectorId;
        Gb20099FrameDataSource.pTableData[++iindex2]=  pDetector[iindex].ucPhaseId;
        Gb20099FrameDataSource.pTableData[++iindex2]= pDetector[iindex].ucDetFlag;
        Gb20099FrameDataSource.pTableData[++iindex2]=pDetector[iindex].ucDirect;
        Gb20099FrameDataSource.pTableData[++iindex2]= pDetector[iindex].ucValidTime;
        Gb20099FrameDataSource.pTableData[++iindex2]= pDetector[iindex].ucOptionFlag;
        Gb20099FrameDataSource.pTableData[++iindex2]= pDetector[iindex].usSaturationFlow>>8 &0xff;
        Gb20099FrameDataSource.pTableData[++iindex2]= pDetector[iindex].usSaturationFlow&0xff;
        Gb20099FrameDataSource.pTableData[++iindex2]=pDetector[iindex].ucSaturationOccupy;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
    }

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
 Function:  CTscKernelGb20999::ConstruectDetectorSetReply
 Description:  构造信号检测器设置应答数据,应答数据包含设置的数据内容.
 Input:        pData -设置内容数据指针
               Len -    设置内容数据长度
 Out:       BytesArrayLen -    生成数据字节长度
 Return:        信号机检测器设置应答数据字节数组指针
 ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectDetectorSetReply(int & BytesArrayLen,Byte*pData,int Len)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_INTERVATIONDIRECTION_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_DETECTOR_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;

    Gb20099FrameDataSource.uiTableDataLen = Len;
    Gb20099FrameDataSource.pTableData =new Byte[Len];
    for(int index =0x0; index< Len; index++)
        (Gb20099FrameDataSource.pTableData)[index] = pData[14+index];
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }

}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectStagePatternQueryReply
  Description:   构造信号机配时方案查询数据.
  Input:      无
  Out:    BytesArrayLen -    生成数据字节长度.
  Return:     信号机配时方案查询数据字节数组指针.
   ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectStagePatternQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_SIGNALPATTERN_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    GBT_DB::TblPattern tblPattern;
    GBT_DB::Pattern*   pPattern;
    Byte  ucCountPattern, iindexi,iindexy;
    iindexy = 0x0;
    if((CDbInstance::m_cGbtTscDb).QueryPattern(tblPattern)==false)
        return NULL;
    pPattern     = tblPattern.GetData(ucCountPattern);
    Gb20099FrameDataSource.uiTableDataLen =(ucCountPattern)*24+1;

    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] =(Byte)(ucCountPattern);
    for(iindexi=0x0; iindexi<ucCountPattern; iindexi++)
    {
        Gb20099FrameDataSource.pTableData[++iindexy]  =0x1 ;                                          // 类型方案表
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPattern[iindexi].ucPatternId; //方案号
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPattern[iindexi].ucCycleTime;//周期
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPattern[iindexi].ucOffset;       //相位差
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPattern[iindexi].ucCoorPhase; //协调相位
        Gb20099FrameDataSource.pTableData[++iindexy]  =pPattern[iindexi].ucStagePatternId; //阶段配时号
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
        Gb20099FrameDataSource.pTableData[++iindexy]       =0x0;
    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
   Function:   CTscKernelGb20999::ConstruectStagePatternSetReply
   Description:   构造信号配时方案设置应答数据,应答数据包含设置的数据内容.
   Input:          pData -设置内容数据指针
           Len -    设置内容数据长度
   Out:            BytesArrayLen -     生成数据字节长度
   Return:          信号配时方案设置应答数据字节数组指针
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectStagePatternSetReply(int & BytesArrayLen,Byte*pData,int Len)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_SIGNALPATTERN_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;
    Gb20099FrameDataSource.uiTableDataLen = Len;
    Gb20099FrameDataSource.pTableData =new Byte[Len];
    for(int index =0x0; index< Len; index++)
        (Gb20099FrameDataSource.pTableData)[index] = pData[14+index];
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectPhaseQueryReply
  Description:   构造信号机相位查询数据.
  Input:      无
  Out:    BytesArrayLen -    生成数据字节长度.
  Return:     信号机相位查询数据字节数组指针.
   ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectPhaseQueryReply(int & BytesArrayLen)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion  = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag  = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode   = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode  = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SEEK_ACK ;
    Gb20099FrameDataSource.ucObjCode     = OBJECT_PHASE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;

    GBT_DB::TblPhase  tblPhase;
    GBT_DB::Phase*   pPhase;
    Byte  iindex;
    unsigned char   ucCountPhase;
    if((CDbInstance::m_cGbtTscDb).QueryPhase(tblPhase)==false)
        return NULL;
    pPhase  = tblPhase.GetData(ucCountPhase);

    Gb20099FrameDataSource.uiTableDataLen = ucCountPhase*24+1;
    Gb20099FrameDataSource.pTableData = new Byte[ Gb20099FrameDataSource.uiTableDataLen];
    ACE_OS::memset(Gb20099FrameDataSource.pTableData,0x0,Gb20099FrameDataSource.uiTableDataLen);
    Gb20099FrameDataSource.pTableData[0] = ucCountPhase;
    int iindex2 = 0x0;
    for(iindex = 0x0 ; iindex<ucCountPhase; iindex++)
    {
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucPhaseId;
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucPedestrianGreen;
        Gb20099FrameDataSource.pTableData[++iindex2]= pPhase[iindex].ucPedestrianClear;
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucMinGreen;
        Gb20099FrameDataSource.pTableData[++iindex2]= pPhase[iindex].ucGreenDelayUnit;
        Gb20099FrameDataSource.pTableData[++iindex2]= pPhase[iindex].ucMaxGreen1;
        Gb20099FrameDataSource.pTableData[++iindex2]= pPhase[iindex].ucMaxGreen2;
        Gb20099FrameDataSource.pTableData[++iindex2]= pPhase[iindex].ucFixGreen;
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucGreenFlash;
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucPhaseTypeFlag;
        Gb20099FrameDataSource.pTableData[++iindex2]=pPhase[iindex].ucPhaseOption;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;
        Gb20099FrameDataSource.pTableData[++iindex2]=0x0;

    }
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::ConstruectPhaseQueryReply
  Description:  构造信号机错误应答数据.
  Input:     ErrorCode -错误码
                  ErrorObject-错误对象
  Out:   BytesArrayLen -    生成数据字节长度.
  Return:    信号机错误应答数据字节数组指针.
 ****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectErrorReport(int & BytesArrayLen,Byte ErrorCode,Byte ErrorObject)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode   = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode  = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType =    GB25280_ERR_ACK ;
    Gb20099FrameDataSource.ucObjCode     =        ErrorObject ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x30;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x30;
    Gb20099FrameDataSource.uiTableDataLen = 0x1;
    Gb20099FrameDataSource.pTableData =new Byte[0x1];
    Gb20099FrameDataSource.pTableData[0x0]= ErrorCode ;
    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
 Function:   CTscKernelGb20999::ConstruectPhaseSetReply
 Description:   构造信号相位设置应答数据,应答数据包含设置的数据内容.
 Input:         pData -设置内容数据指针
        Len -    设置内容数据长度
 Out:               BytesArrayLen -     生成数据字节长度
 Return:         信号机相位设置应答数据字节数组指针
****************************************************************************************************************/
Byte* CTscKernelGb20999::ConstruectPhaseSetReply(int & BytesArrayLen,Byte*pData,int Len)
{
    SGB25280FrameTableData Gb20099FrameDataSource ;
    Gb20099FrameDataSource.ucVsersion = GB25280TABLE_VERSION;
    Gb20099FrameDataSource.ucSendFlag = GB25280TABLE_SENDFLAG;
    Gb20099FrameDataSource.ucRecvFlag = GB25280TABLE_RECVFLAG;
    Gb20099FrameDataSource.ucDataLinkCode = DATALINK_TSCPARA_GB25280;
    Gb20099FrameDataSource.ucAreaCode = (Byte)(TscGB25280TscConfig.TscAreaCode &0xFF) ;
    Gb20099FrameDataSource.ucRoadCode = (Ushort)(TscGB25280TscConfig.TscCrossCode &0xFFFF);
    Gb20099FrameDataSource.ucOperaType = GB25280_SET_ACK ;
    Gb20099FrameDataSource.ucObjCode = OBJECT_PHASE_GB25280 ;
    Gb20099FrameDataSource.ucReserveDara[0x0] = 0x41;
    Gb20099FrameDataSource.ucReserveDara[0x1] = 0x42;
    Gb20099FrameDataSource.ucReserveDara[0x2] = 0x43;
    Gb20099FrameDataSource.ucReserveDara[0x3] = 0x44;
    Gb20099FrameDataSource.ucReserveDara[0x4] = 0x45;

    Gb20099FrameDataSource.uiTableDataLen = Len;
    Gb20099FrameDataSource.pTableData =new Byte[Len];
    for(int index =0x0; index< Len; index++)
        (Gb20099FrameDataSource.pTableData)[index] = pData[14+index];

    CLinklistGb209992017Data* pLinklistGbData = BytesSendConstructedGB25580FrameData(Gb20099FrameDataSource);
    if(pLinklistGbData !=NULL)
    {
        int iListCoount = pLinklistGbData->LIST_GetCount();
        Byte * pListToBytes = new Byte[iListCoount] ;
        if(ListToBytesArray(pLinklistGbData,pListToBytes,iListCoount) ==false)
            return NULL ;
        BytesArrayLen = iListCoount;
        delete pLinklistGbData;
        return pListToBytes ;
    }
    else
    {
        return NULL ;
    }
}

/***************************************************************************************************************
Function:   CTscKernelGb20999::ListToBytesArray
Description:   将数据链表转换为字节数组
Input:        pLinkListGbData -被转换的数据链表指针
     BytesArray -    存储转换后数据的字节数组地址指针
                  BytesArrayLen -   数据链表长度
Out:              无
Return:       转换是否成
****************************************************************************************************************/
bool CTscKernelGb20999::ListToBytesArray(CLinklistGb209992017Data * pLinkListGbData,Byte  BytesArray[],int BytesArrayLen)
{
    if(pLinkListGbData ==NULL )
        return false ;
    for(int index =0x1 ; index<=BytesArrayLen ; index++)
    {
        if(pLinkListGbData->Getelem(index,&BytesArray[index-0x1]) ==-1)
            return false ;
    }
    return true ;
}

/***************************************************************************************************************
 Function:   CTscKernelGb20999::SendBytesGbDataToClient
 Description:   发送协议数据到上位端
 Input:         pBytes -发送数据字节数组指针
        BytesLen -  发送数据字节长度
 Out:               BytesArrayLen -    数据链表长度
 Return:        空
****************************************************************************************************************/
void CTscKernelGb20999::SendBytesGbDataToClient(Byte *pBytes,int BytesLen)
{
    if(pBytes !=NULL)
    {
        CGbtMsgQueue::CreateInstance()->m_sockLocal.send(pBytes, BytesLen, ACE_INET_Addr(TscGB25280TscConfig.iPort,TscGB25280TscConfig.TscClientIp.c_str()));
    }
    return ;
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::CheckGB25280Msg
  Description:   检测是否是合法的GB25280-2016附录协议数据
  Input:         ucDealDataIndex -上位端下标
             iBufLen -   接收数据长度
             pBuf  -接收数据地址指针
  Out:       无
  Return:        是否合法 false-非法 true-合法
****************************************************************************************************************/
bool CTscKernelGb20999::CheckGB25280Msg(Byte ucDealDataIndex,Uint iBufLen,Byte* pBuf)
{
    CGbtMsgQueue *pCGbtMsgQueue = CGbtMsgQueue::CreateInstance();
    if ( pBuf == NULL )
    {
        return false;
    }
    int  len= 0x0;
    Byte* pErrorData = NULL ;
    char   CheckCode[5] = {0x0};
    if (pBuf[0] != 0xC0 || pBuf[iBufLen - 1] != 0xC0 )
    {
        pErrorData = ConstruectErrorReport(len,GB25280_ERROR_FRAEME,OBJECT_DETECTOR_OTHER);
    }
    else if ( (int)iBufLen < MIX_GB25280FRAME_LEN ) //协议的消息长度短// 数据帧最少17鲎纸?
    {
        pErrorData = ConstruectErrorReport(len,GB25280_ERROR_SHORT,OBJECT_DETECTOR_OTHER);
    }
    else
    {
        CLinklistGb209992017Data*  pCLinklistGb209992017Data= CTscKernelGb20999::BytesRecvDecodeGB25580FrameData(pBuf,iBufLen);
        if(pCLinklistGb209992017Data ==NULL)
        {
            /*
             Byte* pErrorData = ConstruectErrorReport(len ,GB25280_ERROR_CHECKCODE);
            if(pErrorData !=NULL)
                  {
                           ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pErrorData,len);
                         delete []pErrorData;
                }*/
        }
        else
        {
            int iListCoount = pCLinklistGb209992017Data->LIST_GetCount();
            Byte * pListToBytes = new Byte[iListCoount] ;
            ListToBytesArray(pCLinklistGb209992017Data,pListToBytes,iListCoount) ;
            CheckCode[0] = pListToBytes[9];
            CheckCode[1] = pListToBytes[10];
            CheckCode[2] = pListToBytes[11];
            CheckCode[3] = pListToBytes[12];
            CheckCode[4] = pListToBytes[13];

            if(pListToBytes[8]<OBJECT_ONLINE_GB25280 ||pListToBytes[8] > OBJECT_DETECTOR_GB25280)   //判断是否是合法协议对象
            {
                pErrorData = ConstruectErrorReport(len,GB25280_ERROR_TYPE,OBJECT_DETECTOR_OTHER);

            }
            else if(pListToBytes[0]!= 0x10||pListToBytes[1]!= 0x20 || pListToBytes[2]!=0x10 ||  pListToBytes[4] !=0x1 || pListToBytes[5]!=0x1 ||  pListToBytes[6]!= 0x0)
            {
                pErrorData = ConstruectErrorReport(len,GB25280_ERROR_FRAEME,pListToBytes[8]);    //判断帧数据特定部分内容是否合法
            }
            else    if ( (pListToBytes[7]&0xff)<GB25280_SEEK_REQ || (pListToBytes[7]&0xff)>GB25280_ERR_ACK || ((pListToBytes[7]>>7)&1)!=1 ) //协议其它问题
            {
                pErrorData = ConstruectErrorReport(len,GB25280_ERROR_OTHER,pListToBytes[8]);
            }
            else if(pListToBytes[8] != OBJECT_ONLINE_GB25280 &&  TscGB25280TscConfig.TscCommunicationStatus  == TSC_COMMUICATION_OFFLINE )
            {
                pErrorData = ConstruectErrorReport(len,GB25280_ERROR_COMMERROR,pListToBytes[8]);     //判断是否联机状态

            }
            else if( (pListToBytes[7]&0xff) == GB25280_SET_REQ && (ACE_OS::memcmp(CheckCode,TscGB25280TscConfig.IdentifyCode.c_str(),0x5)!=0x0 ))
            {
                //判断设置指令 远程控制保留数据的指令验证码是否和配置文件指令验证码一致
                // ACE_OS::printf("\r\n %s:%d CheckCode Error ClientCode=%s TscCode=%s \r\n" ,__FILE__,__LINE__,CheckCode,TscGB25280TscConfig.IdentifyCode.c_str());
                pErrorData = ConstruectErrorReport(len,GB25280_ERROR_CHECKCODE,pListToBytes[8]);
            }
            else
            {
                delete []pListToBytes;
                return true ;
            }
            delete []pListToBytes;
        }
    }
    if(pErrorData !=NULL)
    {
        ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pErrorData,len);
        delete []pErrorData;
    }
    pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = len;
    SThreadMsg sGbtMsg;
    sGbtMsg.ulType       = GBT_MSG_SEND_HOST;
    sGbtMsg.ucMsgOpt     = ucDealDataIndex;
    sGbtMsg.uiMsgDataLen = 0;
    sGbtMsg.pDataBuf     = NULL;
    pCGbtMsgQueue->SendGbtMsg(&sGbtMsg,sizeof(sGbtMsg));
    return false;
}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::DealGb25280RecvBuf
  Description:   处理从上位端接收到的GB25280-2016附录协议数据
  Input:         ucDealDataIndex -上位端下标
  Out:       无
  Return:        无
****************************************************************************************************************/
void  CTscKernelGb20999::DealGb25280RecvBuf(Byte ucDealDataIndex)
{
    //  ACE_OS::printf("\r\n%s:%d Begin deal Gb25280 gbdata!\r\n",__FILE__,__LINE__);
    CGbtMsgQueue *pCGbtMsgQueue = CGbtMsgQueue::CreateInstance();
    CLinklistGb209992017Data*  pCLinklistGb209992017Data= CTscKernelGb20999::BytesRecvDecodeGB25580FrameData(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sRecvFrame.ucBuf,pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sRecvFrame.iBufLen);

    int iListCoount = pCLinklistGb209992017Data->LIST_GetCount();
    Byte * pListToBytes = new Byte[iListCoount] ;
    ListToBytesArray(pCLinklistGb209992017Data,pListToBytes,iListCoount) ;
    Byte Gb25280Object =pListToBytes[8]  ;
    Byte ucOperType        = pListToBytes[7];
    switch(Gb25280Object)
    {
        case OBJECT_ONLINE_GB25280 :                 //联机对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x84)                              //收到收到上位端发送的联机请求应答处理
            {
                Byte *  pBytesArraySetReply = NULL ;
                if(iListCoount > 0x14  )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_ONLINE_GB25280);
                    //ACE_OS::printf("\r\n%s:%d Request onle data too long  \r\n",__FILE__,__LINE__);
                }
                else if(iListCoount <0x14  )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_ONLINE_GB25280);
                    //ACE_OS::printf("\r\n%s:%d Request online data too short  \r\n",__FILE__,__LINE__);
                }
                else
                {
                    Byte ReqCode[0x6] = {0x0};
                    ACE_TString  strReqCode;
                    Configure::CreateInstance()->GetString("COMMUNICATION","OnLineReqCode",strReqCode);
                    const char * pString= strReqCode.c_str();
                    if(ACE_OS::strlen(pString) == 0x6)
                        ACE_OS::memcpy(ReqCode,pString,0x6);
                    if(ReqCode[0] ==pListToBytes[14] && ReqCode[1] ==pListToBytes[15]&&ReqCode[2]==pListToBytes[16] &&ReqCode[3]==pListToBytes[17] && ReqCode[4] ==pListToBytes[18]&&ReqCode[5] ==pListToBytes[19] )
                    {
                        TscGB25280TscConfig.TscCommunicationStatus= TSC_COMMUICATION_ONLINE ;
                        //ACE_OS::printf("\r\n%s:%d Gb25280 TSC online!\r\n",__FILE__,__LINE__);
                    }
                    else
                    {
                        pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_CHECKCODE,OBJECT_ONLINE_GB25280);
                    }
                }
                delete  []pListToBytes ;
                if(pBytesArraySetReply != NULL)
                {
                    ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                    delete []pBytesArraySetReply ;
                    pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                    pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                }
            }
            else if(ucOperType == 0x83)                         //收到收到上位端发送的联机查询应答处理
            {
                TscGB25280TscConfig.bGetQueryResponse= true ;
                delete []pListToBytes ;
                //  pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                //   ACE_OS::printf("%s:%d Get 25280client QueryResponse !\r\n",__FILE__,__LINE__);
            }
            break ;
        }
        case OBJECT_TRAFFIC_FLOW_GB25280 :    //交通流信息对象
        {
            if(ucOperType == 0x82)                       //交通流信息主动上传开关设置
            {
                if(
                    pListToBytes[14] == 0x1)
                    TscGB25280TscConfig.bUpTrafficFlow = true ;
                else
                    TscGB25280TscConfig.bUpTrafficFlow = false ;
            }
            delete[]pListToBytes ;
            break ;
        }
        case OBJECT_WORKSTATUS_GB25280  : //信号机工作状态对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)       //信号机工作状态查询应答处理
            {
                Byte *pBytesArrayQueryReply = ConstruectWorkStatusReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x82)
            {
                if(
                    pListToBytes[14] == 0x1)
                    TscGB25280TscConfig.bUpTscWorkStatus = true ;
                else
                    TscGB25280TscConfig.bUpTscWorkStatus = false ;
            }
            delete  []pListToBytes ;
            break ;
        }
        case OBJECT_LAMPCOLOR_GB25280  :     //灯色状态对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)                     //收到灯色状态查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectLampColorStatusReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x82)           //设置灯色状态主动上传开关
            {
                if(
                    pListToBytes[14] == 0x1)
                    TscGB25280TscConfig.bUpLampColor = true ;
                else
                    TscGB25280TscConfig.bUpLampColor = false ;
            }
            delete  []pListToBytes ;
            break ;
        }
        case OBJECT_CURRENTTIME_GB25280  :     //当前时间对象
        {

            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)                        //收到时间查询请求应答处理
            {
                Byte * pBytesArrayQueryReply = ConstruectTimeQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)                //收到时间设置应答处理
            {
                int iIndex = 0;
                Byte *  pBytesArrayTimeSetReply = NULL ;
                ACE_Time_Value tvCurTime;
                ACE_Date_Time  tvDateTime;

                int iTotalSec = 0x0;
                iTotalSec |= pListToBytes[14];
                iTotalSec |= pListToBytes[15]<< 0x8;
                iTotalSec |= pListToBytes[16]<<0x10;
                iTotalSec |= pListToBytes[17]<< 0x18;
                iTotalSec += 16*3600;
                tvCurTime.sec(iTotalSec-8*3600);
                tvDateTime.update(tvCurTime);

                if (!IsRightDate((Uint)tvDateTime.year(),(Byte)tvDateTime.month(),(Byte)tvDateTime.day()))
                {
                    pBytesArrayTimeSetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_OBJECT_VALUE,OBJECT_CURRENTTIME_GB25280);

                }
                else
                {
                    SThreadMsg sTscMsg;
                    Byte Time[4]= {0x0};
                    Time[0] = (Byte)(iTotalSec>>0x18 & 0xFF);
                    Time[1] =(Byte)(iTotalSec>>0x10 & 0xFF);
                    Time[2] = (Byte)(iTotalSec>>0x8 & 0xFF);
                    Time[3] =(Byte)( iTotalSec&0xFF);
                    sTscMsg.ulType       = TSC_MSG_CORRECT_TIME;
                    sTscMsg.ucMsgOpt     = OBJECT_UTC_TIME;
                    sTscMsg.uiMsgDataLen = 4;
                    sTscMsg.pDataBuf     = ACE_OS::malloc(4);
                    ACE_OS::memcpy(sTscMsg.pDataBuf,Time,4);
                    CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));
                }
                if(pBytesArrayTimeSetReply ==NULL)
                    pBytesArrayTimeSetReply = ConstruectTimeSetReply( BytesArrayLen);

                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayTimeSetReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayTimeSetReply ;
                delete []pListToBytes;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_LANPGROUP_GB25280  :   //信号灯组对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)                //收到信号灯组查询请求应答处理
            {
                Byte *pBytesArrayQueryReply = ConstruectLampGrpQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)             //收到信号灯组设置应答处理
            {
                Byte *pBytesArraySetReply = NULL ;
                int CountPhae= pListToBytes[14];
                if(iListCoount >(CountPhae*12+0xF) )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_LANPGROUP_GB25280);
                    ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                    pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                    pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                }
                else if(iListCoount <(CountPhae*12+0xF))
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_LANPGROUP_GB25280);
                    ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                    pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                    pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                }
                else
                {
                    int indexx = 0x0 ;
                    Byte uErrorSts,uErrorIdx;
                    Byte *pBytesPlan = new Byte[pListToBytes[14]*0x4+1] ;//
                    pBytesPlan[0] = pListToBytes[14] ;
                    for(int index=0 ; index< pListToBytes[14]; index++)
                    {
                        pBytesPlan[1+index*4] = pListToBytes[15+index*12] ;
                        pBytesPlan[2+index*4] = pListToBytes[16+index*12] ;
                        pBytesPlan[3+index*4] = pListToBytes[17+index*12] ;
                        pBytesPlan[4+index*4] = pListToBytes[18+index*12] ;
                    }
                    pBytesArraySetReply = ConstruectLampGrpSetReply( BytesArrayLen,pListToBytes,pListToBytes[14]*12+1);
                    ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                    pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                    pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                    GBT_DB::SetChannel(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0x4+1, uErrorSts, uErrorIdx);
                    delete []pBytesPlan;
                }
                delete []pBytesArraySetReply ;
                delete []pListToBytes ;
            }

            break ;
        }
        case OBJECT_PHASE_GB25280  :          //信号相位对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)           //收到信号相位查询请求应答处理
            {
                Byte *pBytesArrayQueryReply = ConstruectPhaseQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)            //收到信号相位设置应答处理
            {
                Byte *  pBytesArraySetReply = NULL ;
                int CountPhae= pListToBytes[14];
                if(iListCoount >(CountPhae*24+0xF) )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_PHASE_GB25280);
                }
                else if(iListCoount <(CountPhae*24+0xF))
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_PHASE_GB25280);
                }
                else
                {
                    int indexx = 0x0 ;
                    Byte uErrorSts,uErrorIdx;
                    Byte *pBytesPlan = new Byte[pListToBytes[14]*0xc+1] ;//
                    pBytesPlan[0] = pListToBytes[14] ;
                    for(int index=0 ; index< pListToBytes[14]; index++)
                    {
                        pBytesPlan[1+index*12] = pListToBytes[15+index*24] ;
                        pBytesPlan[2+index*12] = pListToBytes[16+index*24] ;
                        pBytesPlan[3+index*12] = pListToBytes[17+index*24] ;
                        pBytesPlan[4+index*12] = pListToBytes[18+index*24] ;
                        pBytesPlan[5+index*12] = pListToBytes[19+index*24] ;
                        pBytesPlan[6+index*12] = pListToBytes[20+index*24] ;
                        pBytesPlan[7+index*12] = pListToBytes[21+index*24] ;
                        pBytesPlan[8+index*12] = pListToBytes[22+index*24] ;
                        pBytesPlan[9+index*12] = pListToBytes[23+index*24] ;
                        pBytesPlan[10+index*12] = pListToBytes[24+index*24] ;
                        pBytesPlan[11+index*12] = pListToBytes[25+index*24] ;
                        pBytesPlan[12+index*12] = pListToBytes[26+index*24] ;
                    }
                    GBT_DB::SetPhase(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0xc+1, uErrorSts, uErrorIdx);
                    delete []pBytesPlan;
                }
                if(pBytesArraySetReply ==NULL)
                    pBytesArraySetReply = ConstruectPhaseSetReply( BytesArrayLen,pListToBytes,pListToBytes[14]*24+1);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArraySetReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_SIGNALPATTERN_GB25280  :    //信号配时方案对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)          //收到信号配时方案查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectStagePatternQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)    //收到信号配时方案设置应答处理
            {
                Byte *  pBytesArraySetReply = NULL ;
                int CountPhae= pListToBytes[14];
                if(iListCoount >(CountPhae*24+0xF) )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_SIGNALPATTERN_GB25280);
                }
                else if(iListCoount <(CountPhae*24+0xF))
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_SIGNALPATTERN_GB25280);
                }
                else
                {
                    int indexx = 0x0 ;
                    if(pListToBytes[15] ==0x1)
                    {
                        Byte uErrorSts,uErrorIdx;
                        Byte *pBytesPlan = new Byte[pListToBytes[14]*0x5+1] ;//
                        pBytesPlan[0] = pListToBytes[14] ;
                        for(int index=0 ; index< pListToBytes[14]; index++)
                        {
                            pBytesPlan[1+index*5] = pListToBytes[16+index*24] ;
                            pBytesPlan[2+index*5] = pListToBytes[17+index*24] ;
                            pBytesPlan[3+index*5] = pListToBytes[18+index*24] ;
                            pBytesPlan[4+index*5] = pListToBytes[19+index*24] ;
                            pBytesPlan[5+index*5] = pListToBytes[20+index*24] ;

                        }
                        GBT_DB::SetPattern(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0x5+1, uErrorSts, uErrorIdx);
                        delete []pBytesPlan;
                    }
                    else
                    {
                        pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_OBJECT_VALUE,OBJECT_SIGNALPATTERN_GB25280);
                    }

                }
                if(pBytesArraySetReply ==NULL)
                    pBytesArraySetReply = ConstruectStagePatternSetReply( BytesArrayLen,pListToBytes,pListToBytes[14]*24+1);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArraySetReply ;
                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);

            }
            break ;
        }
        case OBJECT_PLAN_GB25280  :       //方案调度计划对象
        {

            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)           //收到方案调度计划查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectPlanPatternQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;

                delete  []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)     //收到方案调度计划设置应答处理
            {
                int iPos = 0x0 ;
                Byte *  pBytesArraySetReply = NULL ;
                int CountPlanSchedule = pListToBytes[14];
                if(iListCoount >(CountPlanSchedule*12+0xF) )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_PLAN_GB25280);
                }
                else if(iListCoount <(CountPlanSchedule*12+0xF))
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_PLAN_GB25280);
                }
                else
                {
                    int indexx = 0x0 ;
                    if(pListToBytes[15] ==0x2)
                    {
                        Byte uErrorSts,uErrorIdx;
                        Byte *pBytesPlan = new Byte[pListToBytes[14]*0x8+2] ;//
                        pBytesPlan[0] = 0x0; ;
                        pBytesPlan[1] = pListToBytes[14] ;
                        for(int index=0 ; index< pListToBytes[14]; index++)
                        {
                            pBytesPlan[2+index*8] = pListToBytes[16+index*12] ;
                            pBytesPlan[3+index*8] = pListToBytes[17+index*12] ;
                            pBytesPlan[4+index*8] = pListToBytes[18+index*12] ;
                            pBytesPlan[5+index*8] = pListToBytes[19+index*12] ;
                            pBytesPlan[6+index*8] = pListToBytes[20+index*12] ;
                            pBytesPlan[7+index*8] = pListToBytes[21+index*12] ;
                            pBytesPlan[8+index*8] = pListToBytes[22+index*12] ;
                            pBytesPlan[9+index*8] = pListToBytes[23+index*12] ;
                        }
                        GBT_DB::SetSchedule(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0x8+2, uErrorSts, uErrorIdx);
                        delete []pBytesPlan;
                    }
                    else if(pListToBytes[15] ==0x1)
                    {
                        Byte uErrorSts,uErrorIdx;
                        Byte *pBytesPlan = new Byte[pListToBytes[14]*0x9+1] ;//
                        pBytesPlan[0] = pListToBytes[14] ;
                        for(int index=0 ; index< pListToBytes[14]; index++)
                        {
                            pBytesPlan[1+index*9] = pListToBytes[16+index*12] ;
                            pBytesPlan[2+index*9] = pListToBytes[18+index*12] ;
                            pBytesPlan[3+index*9] = pListToBytes[17+index*12] ;
                            pBytesPlan[4+index*9] = pListToBytes[19+index*12] ;
                            pBytesPlan[5+index*9] = pListToBytes[23+index*12] ;
                            pBytesPlan[6+index*9] = pListToBytes[22+index*12] ;
                            pBytesPlan[7+index*9] = pListToBytes[21+index*12] ;
                            pBytesPlan[8+index*9] = pListToBytes[20+index*12] ;
                            pBytesPlan[9+index*9] = pListToBytes[24+index*12] ;
                        }
                        GBT_DB::SetPlan(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0x9+1, uErrorSts, uErrorIdx);
                        delete []pBytesPlan;
                    }
                }
                if(pBytesArraySetReply == NULL)
                    pBytesArraySetReply = ConstruectPlanPatternSetReply( BytesArrayLen,pListToBytes,pListToBytes[14]*12+1);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArraySetReply ;
                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_WORKTYPE_GB25280  :     //工作方式对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)     //收到工作方式查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectWorkTypeQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;

                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)   //收到工作方式设置应答处理
            {
                Byte *   pBytesWorkTypeReply  = NULL ;
                CLinklistGb209992017Data*  pCLinklistGb209992017Data= CTscKernelGb20999::BytesRecvDecodeGB25580FrameData(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sRecvFrame.ucBuf,pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sRecvFrame.iBufLen);
                int iListCoount = pCLinklistGb209992017Data->LIST_GetCount();
                Byte * pListToBytes = new Byte[iListCoount] ;
                ListToBytesArray(pCLinklistGb209992017Data,pListToBytes,iListCoount) ;
                if(pListToBytes[14] ==0x1|| pListToBytes[14] ==0x2 ||pListToBytes[14] ==0x3 ||
                   pListToBytes[14] ==0x5 ||pListToBytes[14] ==0x6 ||pListToBytes[14]==0x8 ||
                   pListToBytes[14] ==0xa ||pListToBytes[14] ==0xc ||pListToBytes[14]==0xd )
                {
                    switch(pListToBytes[14])
                    {
                        case 0x1:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x1);
                            break;
                        case 0x2:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x2);
                            break;
                        case 0x3:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x3);
                            break;
                        case 0x5:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x5);
                            break;
                        case 0x6:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x6);
                            break;
                        case 0x8:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x8);
                            break;
                        case 0xa:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x8);
                            break;
                        case 0xc:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x8);
                            break;
                        case 0xd:
                            CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_CONTROL,0x8);
                            break;
                        default:
                            break;
                    }
                    pBytesWorkTypeReply = ConstruectWorkTypeSetReply( BytesArrayLen);
                    //  ACE_OS::printf("\r\n\r\n%s:%d wortypeset\r\n",__FILE__,__LINE__);
                }
                else
                {
                    pBytesWorkTypeReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_OBJECT_VALUE,OBJECT_WORKTYPE_GB25280);
                }
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesWorkTypeReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesWorkTypeReply ;
                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_TSCERROR_GB25280  :     //信号机故障对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)               //信号机故障查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectErrorQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x82)
            {
                if(
                    pListToBytes[14] == 0x1)
                    TscGB25280TscConfig.bUpTscError = true ;
                else
                    TscGB25280TscConfig.bUpTscError = false ;
            }
            delete  []pListToBytes ;
            break ;
        }
        case OBJECT_TSCVERSION_GB25280  :            //信号机版本对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)                //信号机版本查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectTscVersionQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x82)
            {
                if(
                    pListToBytes[14] == 0x1)
                    TscGB25280TscConfig.bUpTscVerUp = true ;
                else
                    TscGB25280TscConfig.bUpTscVerUp = false ;
            }
            delete []pListToBytes ;
            break ;
        }
        case OBJECT_TSCPARAVER_GB25280  :      //特征参数版本对象
        {

            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)                   //收到特征参数版本查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectParameterVerQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;

                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)           //收到特征参数版本设置应答处理
            {
                char ParaVerStr[5] = {0x0};
                ACE_OS::sprintf(ParaVerStr,"%d",pListToBytes[14]);
                (CDbInstance::m_cGbtTscDb).SetEypSerial(ParaVerStr);
                // ACE_OS::printf("\r\n%s:%d ParameterVer=%s \r\n",__FILE__,__LINE__,ParaVerStr);
                Byte *  pBytesArrayQueryReply = ConstruectParameterVerSetReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_TSCIC_GB25280  :     //信号机识别码对象
        {

            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)        //信号机识别码读取处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectTscIDQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;
                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
        case OBJECT_REMOTECTRL_GB25280  :    //信号机远程控制对象
        {
            int BytesArrayLen = 0x0;
            Byte *pBytesArrayQueryReply = NULL ;
            bool bOk = true ;
            if(ucOperType == 0x81)                   //信号机远程控制设置应答对象
            {
                switch(pListToBytes[14])
                {
                    case 0x1:
                        //ACE_OS::system("reboot") ;
                        break;
                    case 0x2:
                        ACE_OS::system("mv -f GbAitonTsc.db GbAitonTsc.db.bak");
                        break;
                    case 0x3:
                        ACE_OS::system("cp -f GbAitonTsc.db GbAitonTsc.db_`date \"+%Y%m%d%H%M%S\"`_bak");
                        break ;
                    case 0x4:
                        (CDbInstance::m_cGbtTscDb).DelEventLog();
                        break;
                    default:
                        bOk = false  ;
                        break ;
                }
                if(bOk == false)
                    pBytesArrayQueryReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_OBJECT_VALUE,OBJECT_REMOTECTRL_GB25280);
                else
                    pBytesArrayQueryReply = ConstruectTscRemoteCtrlReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
                if(pListToBytes[14] == 0x1)
                    ACE_OS::system("reboot") ;
                delete []pBytesArrayQueryReply ;
                delete []pListToBytes ;
            }
            break ;
        }
        case OBJECT_DETECTOR_GB25280  :    //检测器对象
        {
            int BytesArrayLen = 0x0;
            if(ucOperType == 0x80)     //收到检测器查询请求应答处理
            {
                Byte *  pBytesArrayQueryReply = ConstruectDetectorQueryReply( BytesArrayLen);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArrayQueryReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArrayQueryReply ;

                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            else if(ucOperType == 0x81)    //收到检测器设置应答处理
            {
                Byte *  pBytesArraySetReply = NULL ;
                int CountPhae= pListToBytes[14];
                if(iListCoount >(CountPhae*0xF+0xF) )
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_LONG,OBJECT_DETECTOR_GB25280);
                }
                else if(iListCoount <(CountPhae*0xF+0xF))
                {
                    pBytesArraySetReply =ConstruectErrorReport(BytesArrayLen,GB25280_ERROR_SHORT,OBJECT_DETECTOR_GB25280);
                }
                else
                {
                    int indexx = 0x0 ;
                    Byte uErrorSts,uErrorIdx;
                    Byte *pBytesPlan = new Byte[pListToBytes[14]*0x9+1] ;//
                    pBytesPlan[0] = pListToBytes[14] ;
                    for(int index=0 ; index< pListToBytes[14]; index++)
                    {
                        pBytesPlan[1+index*9] = pListToBytes[15+index*15] ;
                        pBytesPlan[2+index*9] = pListToBytes[16+index*15] ;
                        pBytesPlan[3+index*9] = pListToBytes[17+index*15] ;
                        pBytesPlan[4+index*9] = pListToBytes[18+index*15] ;
                        pBytesPlan[5+index*9] = pListToBytes[19+index*15] ;
                        pBytesPlan[6+index*9] = pListToBytes[20+index*15] ;
                        pBytesPlan[7+index*9] = pListToBytes[21+index*15] ;
                        pBytesPlan[8+index*9] = pListToBytes[22+index*15] ;
                        pBytesPlan[9+index*9] = pListToBytes[23+index*15] ;
                    }
                    GBT_DB::SetDetector(GBT_DB::m_gTscDb, pBytesPlan, pListToBytes[14]*0x9+1, uErrorSts, uErrorIdx);
                    delete []pBytesPlan;
                }
                if(pBytesArraySetReply ==NULL)
                    pBytesArraySetReply = ConstruectDetectorSetReply(BytesArrayLen,pListToBytes,pListToBytes[14]*0xF+1);
                ACE_OS::memcpy(pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.ucBuf,pBytesArraySetReply,BytesArrayLen);
                pCGbtMsgQueue->m_sGbtDealData[ucDealDataIndex].sSendFrame.iBufLen = BytesArrayLen ;
                delete []pBytesArraySetReply ;
                delete []pListToBytes ;
                pCGbtMsgQueue->GotoSendToHost(ucDealDataIndex);
            }
            break ;
        }
    }
}

/***************************************************************************************************************
 Function:  CTscKernelGb20999::SelfReportLampGroupColor
 Description:  灯组颜色状态主动上传
 Input:        无
 Out:      无
 Return:       无
****************************************************************************************************************/
void CTscKernelGb20999::SelfReportLampGroupColor()
{
    if(TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE &&TscGB25280TscConfig.bUpLampColor== true)
    {
        int  BytesQueryLen = 0x0;
        Byte *  pBytesLampColorUp =ConstruectLampColorStatusSelfReply( BytesQueryLen);
        if(pBytesLampColorUp != NULL)
        {
            SendBytesGbDataToClient(pBytesLampColorUp,BytesQueryLen);
            delete []pBytesLampColorUp;
        }
    }

}

/***************************************************************************************************************
  Function:   CTscKernelGb20999::SelfReportWorkStatus
  Description:   工作状态主动上传
  Input:         无
  Out:       无
  Return:        无
  ****************************************************************************************************************/
void CTscKernelGb20999::SelfReportWorkStatus()
{
    if(TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE &&TscGB25280TscConfig.bUpTscWorkStatus == true)
    {
        int  BytesQueryLen = 0x0;
        Byte *  pBytesWorkStatusUp =ConstruectWorkStatusSelfReply( BytesQueryLen);
        if(pBytesWorkStatusUp != NULL)
        {
            SendBytesGbDataToClient(pBytesWorkStatusUp,BytesQueryLen);
            delete []pBytesWorkStatusUp;
        }
    }
}


/***************************************************************************************************************
  Function:   CTscKernelGb20999::SelfReportWorkStatus
  Description:   信号机故障主动上传
  Input:         无
  Out:       无
  Return:        无
****************************************************************************************************************/
void CTscKernelGb20999::SelfReportTscError()
{
    if(TscGB25280TscConfig.TscCommunicationStatus == TSC_COMMUICATION_ONLINE &&TscGB25280TscConfig.bUpTscError== true)
    {
        int  BytesQueryLen = 0x0;
        Byte *pBytesTscErrorUp =ConstruectErrorSelfReply( BytesQueryLen);
        if(pBytesTscErrorUp != NULL)
        {
            SendBytesGbDataToClient(pBytesTscErrorUp,BytesQueryLen);
            delete []pBytesTscErrorUp;
        }
    }
}



