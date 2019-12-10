#ifndef _GBTMSGQUEUE_H_
#define _GBTMSGQUEUE_H_

#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ComDef.h"
#include "ComStruct.h"

/*****************GBT协议对象标志类型和扩充协议标志类型那个枚举********************/
enum 
{
	//公共配置相关对象
	OBJECT_DEV_PARA     = 0x81,  //公共设备识别参数
	OBJECT_MAX_MODULE         ,  //公共模块表最大行数
	OBJECT_COM_SYN_TIME       ,  //公共同步时间
	OBJECT_SYN_FLAG           ,  //公共同步标志
	OBJECT_MODULE_TABLE       ,  //模块表

	//公共时间参数
	OBJECT_UTC_TIME     = 0x86,  //公共时间
	OBJECT_STANDAR_TIMEZONE   ,  //标准时区
	OBJECT_LOCAL_TIME         ,  //本地时间
	OBJECT_MAX_TIMEGROUP      ,  //时基调度表最大行数
	OBJECT_MAX_SCHEDULE       ,  //时段表最大行数
	OBJECT_MAX_SUBSCHEDULE    ,  //时段表的最大时段数
	OBJECT_ACTIVESCHEDULE_NO  ,  //活动时段表的编号
	OBJECT_TIMEGROUP_TABLE    ,  //时基调度表
	OBJECT_SCHEDULE_TABLE     ,  //时段表

	//上报参数
	OBJECT_MAX_EVENTTYPE    = 0x8F,  //事件类型表最大行数
	OBJECT_MAX_EVENTLOG           ,  //事件日志表的最大行数
	OBJECT_EVENTTYPE_TABLE        ,  //事件类型表参数
	OBJECT_EVENTLOG_TEBLE         ,  //事件日志表参数

	//相位参数
	OBJECT_MAX_PHASE      = 0x93  ,  //相位表最大行数 
	OBJECT_MAX_PHASESTS           ,  //相位状态组最大数量
	OBJECT_PHASE_TABLE            ,  //相位参数表
	OBJECT_PHASESTATUS_TABLE      ,  //相位输出状态表
	OBJECT_PHASECONFLICT_TABLE    ,  //相位冲突表

	//检测器参数
	OBJECT_MAX_DETECTOR    = 0x98,  //车辆检测器最大数量
	OBJECT_MAX_DETECTORSTS       ,  //检测器状态组的最大数量
	OBJECT_DETECTORDATA_ID       ,  //监测数据流水号
	OBJECT_DATA_CYCLE            ,  //数据采集周期
	OBJECT_ACTIVEDETECTOR_NUM    ,  //活动检测器总数
	OBJECT_PULSEDATA_ID          ,  //脉冲数据流水号
	OBJECT_PULSEDATA_GETCYCLE    ,  //脉冲数据采集周期
	OBJECT_DETECTORDEF_TABLE     ,  //车辆检测器参数定义表
	OBJECT_DETECTORSTS_TABLE     ,  //检测器状态表
	OBJECT_DETECTORDATA_TABLE    ,  //交通检测数据表
	OBJECT_DETECTORWARN_TABLE    ,  //车辆检测器警告参数表

	//单元参数
	OBJECT_START_FLASH  = 0xA3   ,  //启动时的闪光控制时间
	OBJECT_START_ALLRED          ,  //启动时的全红控制时间
	OBJECT_CURTSC_CTRL           ,  //当前的信号机控制状态
	OBJECT_CURTSC_FLASHCTRL      ,  //当前的闪光控制模式
	OBJECT_TSC_WARN2             ,  //信号机报警2
	OBJECT_TSC_WARN1             ,  //信号机报警1
	OBJECT_TSC_WARN_SUMMARY      ,  //信号机报警摘要
	OBJECT_ALLOW_FUN             ,  //允许远程控制实体激活信号机的某些功能
	OBJECT_FLASH_FRE             ,  //闪光频率
	OBJECT_SHINE_STARTTIME       ,  //辉度控制开启时间
	OBJECT_SHINE_DOWM_TIME       ,  //辉度控制关闭时间

	//灯控端口参数
	OBJECT_MAX_CHANNEL     = 0xAE,  //信号机支持的最大通道数量
	OBJECT_MAX_CHANNELSTS        ,  //通道状态组数
	OBJECT_CHANNEL_TABLE         ,  //通道参数表
	OBJECT_CHANNELSTATUS_TABLE   ,  //通道输出状态表

	//控制参数
	OBJECT_TIMEPATTERN_NUM = 0xB2,  //配时方案数
	OBJECT_MAX_SCHEDULETIME      ,  //最大阶段配时表数
	OBJECT_MAX_SUBSCHEDULETIME   ,  //最大阶段数
	OBJECT_SWITCH_MANUALCONTROL  ,  //手动控制方案
	OBJECT_SWITCH_SYSTEMCONTROL  ,  //系统控制方案
	OBJECT_SWITCH_CONTROL        ,  //控制方式
	OBJECT_COMCYCLETIME          ,  //公共周期时长
	OBJECT_ADJUST_PHASEGAP       ,  //协调相位差
	OBJECT_SWITCH_STAGE          ,  //阶段状态
	OBJECT_GOSTEP                ,  //步进指令
	OBJECT_DEMOTION_MODE         ,  //降级模式
	OBJECT_DEMOTION_PATTERN      ,  //降级基准方案表
	OBJECT_CURPATTERN_SCHTIMES   ,  //当前方案各阶段时长
	OBJECT_CURPATTERN_GREENTIMES ,  //当前方案各关键相位绿灯时长
	OBJECT_TIMEPATTERN_TABLE     ,  //配时方案表
	OBJECT_SCHEDULETIME_TABLE    ,  //阶段配时表

	//下载标志参数
	OBJECT_DOWNLOAD_FLAG   = 0xC2,  //上位机下载参数的开始结束标志

	//控制主机参数
	OBJECT_CONTROLTSC_PARA = 0xC3,  //控制主机选项参数
	OBJECT_TSCADDRESS            ,  //信号机基地址
	OBJECT_CROSSNUM              ,  //路口数量

	//跟随相位参数
	OBJECT_MAX_OVERLAPPHASE = 0xC6,  //跟随相位表最大行数
	OBJECT_MAX_OVERLAPPHASESTS    ,  //跟随相位状态表最大行数
	OBJECT_OVERLAPPHASE_TABLE     ,  //跟随相位表
	OBJECT_OVERLAPPHASE_STATUS    ,  //跟随相位状态表

	//扩充对象标志
         OBJECT_WATCH_PARA      				 = 0xf5,   //监控类型参数 温度 电压 门
	OBJECT_IP              						 = 0xf6,   //信号机的ip
	OBJECT_SET_REPORTSELF  				 = 0xf7,   //定制主动上报
	OBJECT_EXT_TSC_STATUS   				 = 0xf8,   //状态类型表参数
	OBJECT_MODULE_STATUS  			          = 0xf9 ,   //模块状态
	OBJECT_CNTDOWN_DEV     				 = 0xf1,   //倒计时设备表    ADD:2013071 1034
	OBJECT_PHASETODIRECT				          = 0xfa,   //相位与方向对应表
	OBJECT_ADAPTPARA       					 = 0xfb,   //自适应参数值
	OBJECT_DET_EXTENTED     				 = 0xfc,   //检测器扩展表
	OBJECT_ADAPT_STAGE      				 = 0xfd,
	OBJECT_CONNECT_DEV     				 = 0xf0,   //外界设备数第一个字节表示倒计时设备数，取值为0-32，0表示没有倒计时设备。第二个字节表示可变标志设备数，取值为0-16，0表示没有可变标志设表示外接检测器数，取值为0-48，0表示没有外接检测器
	//OBJECT_CNTDOWN_STATS  			 = 0xf2,   	//倒计时状态表
	OBJECT_COMMAND_SIGNAL  			          = 0xf2 ,    //上位机指令控制相位阶段切换和方向
	OBJECT_CHANNEL_CHK    			          = 0xff,   //通道灯泡检测配置表 ADD?20130801 1121
	OBJECT_YWFLASH_CFG   				          = 0xe1,   //黄闪器配置
	OBJECT_DET_EXTCFG       					 = 0xe2,   //检测器扩展配置
	OBJECT_LAMPBOARD_CFG    				 = 0xe3,   //灯控板灯泡检测和红绿冲突检测配置
	OBJECT_PSCBTN_NUM 	    				 = 0xee,    //模拟8位行人按钮输入 ADD:2013 0829 1540
	OBJECT_TMPPATTERN_CFG   				 = 0xef,    //12方向临时组合方案，默认60秒
	OBJECT_SYSFUNC_CFG    			          = 0xe4,      //系统其他功能设置
	OBJECT_SENDCLIENT_CNTDOWN			 = 0xe6 ,
	OBJECT_POWERBOARD_CFG    			 = 0xe7,   //电源板配置
	OBJECT_GSM_CFG           					 = 0xe8 ,  //GSM配置
	OBJECT_BUTTONPHASE_CFG 			          = 0xe9    //模拟无线按键按钮
};
/*****************GBT协议对象标志类型和扩充协议标志类型那个枚举********************/

class CGbtMsgQueue
{
public:
	static CGbtMsgQueue* CreateInstance();
	static void* RunGbtRecv(void* arg);	
	int  SendGbtMsg(SThreadMsg* pMsg,int iLen);
	void DealData();	
#ifdef GBT_TCP
	SGbtDealData* GetGbtDealDataPoint();
	Byte GetDealDataIndex(bool bReportSelf , ACE_SOCK_Stream& sockStreamRemote);
#else
	Byte GetDealDataIndex(bool bReportSelf ,ACE_INET_Addr& addrRemote);
#endif
	bool SendTscCommand(Byte ucObjType,Byte ucValue);
	void SetCmuAndCtrl(Byte* pBuf,int& iRecvIndex);
	void SetCmuAndCtrl(Byte* pBuf,int& iRecvIndex , Byte ucSubId);
	void GetCmuAndCtrl(Byte* pBuf,int& iSendIndex);
	void GetCmuAndCtrl(Byte* pBuf,int& iSendIndex , Byte ucSubId);
	void ReworkNetPara(Byte* cIp ,Byte* cMask,Byte* cGateWay);
	void GetNetPara(Byte* pIp , Byte* pMask , Byte* pGateway);
	void InterceptStr(char* pBuf, char* pstr , Byte* pData , Byte ucCnt);
	void GetNetParaByAce(Byte* pip,char* phost_name);  //add 2014 03 20   lurenhua 通过ACE来取得IP地址，可以取得多个
	
	int GetCtrlStatus(unsigned int uiWorkStatus,unsigned int uiCtrl);
	bool GetSystemShellRst(const char* shellcmd , char * cshellrst ,Byte datasize); //ADD:201411041430
	
	void GotoSendToHost(Byte ucDealDataIndex);
public:
	ACE_INET_Addr m_addrLocal;
#ifdef GBT_TCP
	ACE_SOCK_Acceptor m_acceptor;   //tcp
#else
	ACE_SOCK_Dgram m_sockLocal;  //udp
#endif
	int iPort   ;               //MOD:201309250830	
	SGbtDealData  m_sGbtDealData[MAX_CLIENT_NUM];
private:
	CGbtMsgQueue();
	~CGbtMsgQueue();

	int  CheckMsg(Byte ucDealDataIndex,Uint iBufLen,Byte* pBuf);
	void FirstRecv(Byte ucDealDataIndex,Uint iBufLen,Byte* pBuf);
	Byte GetSendOperateType(Byte ucRecvOptType);
	void DealRecvBuf(Byte ucDealDataIndex);
 	void CleanDealData(Byte ucDealDataIndex);
 	void SendToHost(Byte ucDealDataIndex);
  	bool IsSingleObject(Byte ucObjectFlag);
  	bool IsGetTscStatusObject(Byte ucObjectFlag);
	bool IsSendTscCommand(Byte ucObjectFlag);
	bool IsOtherObject(Byte ucObjectFlag);
	bool IsExtendObject(Byte ucObjectFlag);
	//bool IsDoubleIndexObject(Byte ucObjectFlag);
 	void PackTscStatus(Byte ucDealDataIndex,void* pValue);
	void PackOtherObject(Byte ucDealDataIndex);
	void PackExtendObject(Byte ucDealDataIndex);
	Byte ToObjectCurTscCtrl(unsigned int uiCtrl);
 	Byte ToObjectControlSwitch(unsigned int uiWorkSts,unsigned int uiCtrl);
 	
 	void SelfReport(unsigned int uiDataLen,Byte* pDataBuf);
	int GbtCtrl2TscCtrl(Byte ucctrl);
	int GetSysCtrlStatus(unsigned int uiWorkStatus,unsigned int uiCtrl);
	int GetManualCtrlStatus(unsigned int uiWorkStatus,unsigned int uiCtrl);

	void TscCopyFile(char* fpSrc, char* fpDest);
	void UpdateNetPara(Byte* pIp);  //add: 2014 03 20   lurenhua zlg m3352核心板IP地址修改函数
	
	void GetWatchPara(Byte* pBuf,int *iSendIndex);	
	void GetModuleStatus(Byte* pBuf,int *iSendIndex ,Byte subId,Byte ucQueryType,Byte ucBdindex);
	void PackTscExStatus(Byte ucDealDataIndex,void* pValue);
	void GetFlashCfg(Byte* pBuf,int *iSendIndex) ;  //ADD: 2013 0808 10 30
	void SetFlashCtrl(Byte* pBuf,int& iRecvIndex); //ADD 2013 0808 1430
	void  GetPowerCfg(Byte* pBuf,int *iSendIndex ,Byte ucQueryType); //ADD:201404021520	
	void  SetPowerCfg(Byte* pBuf,int& iRecvIndex) ;//ADD:201404021520

	void GetDetCfg(Byte* pBuf,Byte ucBdIndex,Byte ucQueryType,int *iSendIndex) ; //ADD 20130821 1130
	void SetDetCfg(Byte* pBuf,int& iRecvIndex) ; //ADD 2013 0821 1624
	void SetLampBdtCfg(Byte* pBuf,int& iRecvIndex);  //ADD 2013 0822 1606
	void SetPscNum(Byte* pBuf,int& iRecvIndex);  //ADD 20130829 1600
	void SetTmpPattern(Byte* pBuf,int& iRecvIndex) ; //ADD 20131016 1700
	void SetSysFunc(Byte* pBuf,int& iRecvIndex); //ADD 20131019 1400
	void PrintIpList();

	void GotoMsgError(Byte ucDealDataIndex,Byte ucErrorSts,Byte ucErrorIdx);
	void GotoDealRecvbuf(Byte ucDealDataIndex);
	void SetSmsFunc(Byte* pBuf,int& iRecvIndex ,int iRecvBufLen); //ADD 201406041030
	void SetCommandSignal(Byte* pBuf,int& iRecvIndex) ;  //ADD 201409231002
	void SetButtonPhase(Byte* pBuf,int& iRecvIndex);        //ADD 201410181052
	
	void GetSysFuncCfg(Byte* pBuf,Byte ucQueryType,int *iSendIndex) ; //ADD 201601141650

	//Byte m_ucAddrNum;
	ACE_Message_Queue<ACE_MT_SYNCH>* m_pMsgQue;
      
	ACE_Thread_Mutex  m_sMutex;
	//ACE_INET_Addr m_AddrArr[MAX_CLIENT_NUM];

	int m_iManualCtrlNo;
	int m_iSystemCtrlNo;
};

#endif  //_GBTMSGQUEUE_H_

