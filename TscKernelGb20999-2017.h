#ifndef _TSCKERNELGB20999_H_
#define _TSCKERNELGB20999_H_
#include "Define.h"
#include "Gb20999-2017List.h"
#include "Configure.h"

class CTscKernelGb20999
{
private:
    CTscKernelGb20999();
    ~CTscKernelGb20999();
public:
    sGB25280TscConfig  TscGB25280TscConfig ;
    static CTscKernelGb25280* CreateInstance();
    Byte  GetGbFrameCheckCode(CLinklistGbData  *pListFrameTableData); //获取校验码
    CLinklistGbData* BytesSendConstructedGB25580FrameData(SGB25280FrameTableData GbFrameDataSource);
    CLinklistGbData* BytesRecvDecodeGB25580FrameData(Byte RecvedGbFrameData[],Uint RecvDataLen);
    Byte GetTscCommunicationStatus();
    bool ListToBytesArray(CLinklistGbData * pLinkListGbData,Byte BytesArray[],int BytesArrayLen);
    Byte* ConstruectOnlineRequest(int & BytesArrayLen); //构造联机请求指令数据


    Byte* ConstruectOnlineQuery(int & BytesArrayLen); //构造联机查询指令数据

    Byte* ConstruectTrafficFlowSelfReport(int & BytesArrayLen); //构造交通流主动上报指令数据

    Byte* ConstruectWorkStatusReply(int & BytesArrayLen); //构造工作状态应答指令数据

    Byte* ConstruectWorkStatusSelfReply(int & BytesArrayLen); //构造工作状态改变主动上传指令数据

    Byte* ConstruectLampColorStatusReply(int & BytesArrayLen); //构造灯色状态应答指令数据

    Byte* ConstruectLampColorStatusSelfReply(int & BytesArrayLen); //构造灯色状态主动应答指令数据

    Byte* ConstruectTimeQueryReply(int & BytesArrayLen); //构造时间应答指令数据

    Byte* ConstruectTimeSetReply(int & BytesArrayLen); //构造时间应答指令数据


    Byte* ConstruectLampGrpQueryReply(int & BytesArrayLen); //构造信号灯组查询应答指令数据
    Byte* ConstruectLampGrpSetReply(int & BytesArrayLen,Byte*pData,int Len);       //构造信号灯组设置应答指令数据


    Byte* ConstruectPlanPatternQueryReply(int & BytesArrayLen); //构造方案调度计划查询应答指令数据

    Byte* ConstruectPlanPatternSetReply(int & BytesArrayLen,Byte*pData,int Len); //构造方案调度计划设置应答指令数据


    Byte* ConstruectWorkTypeSetReply(int & BytesArrayLen); //构造工作方式设置应答指令数据

    Byte* ConstruectWorkTypeQueryReply(int & BytesArrayLen); //构造工作方式查询应答指令数据

    Byte* ConstruectErrorQueryReply(int & BytesArrayLen); //构造信号机故障查询应答指令数据
    Byte* ConstruectErrorSelfReply(int & BytesArrayLen); //构造信号机错误主动上传指令数据

    Byte* ConstruectTscVersionQueryReply(int & BytesArrayLen); //构造信号机版本查询应答指令数据
    Byte* ConstruectscVersionSelfReply(int & BytesArrayLen); //构造信号机版本主动上传指令数据


    Byte* ConstruectParameterVerQueryReply(int & BytesArrayLen); //构造特征参数版本查询应答指令数据
    Byte* ConstruectParameterVerSetReply(int & BytesArrayLen); //构造特征参数设置应答指令数据

    Byte* ConstruectTscIDQueryReply(int & BytesArrayLen); //构造信号机识别版本查询应答指令数据

    Byte* ConstruectTscRemoteCtrlReply(int & BytesArrayLen); //构造信号机远程设置应答指令数据


    Byte* ConstruectDetectorQueryReply(int & BytesArrayLen); //构造检测器查询应答指令数据
    Byte* ConstruectDetectorSetReply(int & BytesArrayLen,Byte*pData,int Len);  //构造检测器设置应答指令数据


    Byte* ConstruectStagePatternQueryReply(int & BytesArrayLen); //构造信号配饰方案查询应答指令数据
    Byte* ConstruectStagePatternSetReply(int & BytesArrayLe,Byte*pData,int Lenn); //构造信号配时设置应答指令数据


    Byte* ConstruectPhaseQueryReply(int & BytesArrayLen); //构造相位查询应答指令数据
    Byte* ConstruectPhaseSetReply(int & BytesArrayLen,Byte*pData,int Len); //构造相位设置应答指令数据


    Byte* ConstruectErrorReport(int & BytesArrayLen,Byte ErrorCode,Byte ErrorObject);  //构造错误应答指令数据
    void SendBytesGbDataToClient(Byte *pBytes,int BytesLen);
    bool CheckGB25280Msg(Byte ucDealDataIndex,Uint iBufLen,Byte* pBuf); // 检查GB25280数据合法性
    void  DealGb25280RecvBuf(Byte ucDealDataIndex) ;   //处理接收到的GB28250协议数据();
    void SelfReportLampGroupColor();         //主动上传灯组灯色变化
    void SelfReportWorkStatus();                //主动上传工作状态
    void SelfReportTscError();                    // 主动上传信号机故障
};

#endif

