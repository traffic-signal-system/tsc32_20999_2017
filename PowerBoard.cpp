#include "Can.h"
#include "PowerBoard.h"

#ifndef WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#endif

/*
电源模块功能类型枚举
*/
enum
{
	POWER_HEAD_ENV      = 0x02 ,  //电压数据，电压状态
	POWER_HEAD_CFG_GET  = 0x03 ,  //请求电源模块配置数据，包括电压高低压预警电压值以及强弱电控制预案
    POWER_HEAD_CFG_SET  = 0x04 ,  //发送电源模块配置数据
	POWER_HEAD_HEARBEAT = 0x05 , //心跳包
	POWER_HEAD_VER      = 0xff  //程序版本
};

/*
电源模块预案功能类型枚举
*/
enum
{
	PLAN_KEEP  = 0 ,  //预案方式不变
	PLAN_ON    = 1 ,  //保护开启
	PLAN_OFF   = 2 ,  //保护关闭
	PLAN_OTHER = 3    //备用
};

/**************************************************************
Function:       CPowerBoard::CPowerBoard
Description:    CPowerBoard类构造函数		
Input:          无              
Output:         无
Return:         无
***************************************************************/
CPowerBoard::CPowerBoard()
{
	m_iStongVoltage     = 220;
	m_iWeakVoltage = 5;
	m_iBusVoltage  = 0;
	ACE_DEBUG((LM_DEBUG,"%s:%d Init PowerBoard object ok !\n",__FILE__,__LINE__));
}

Byte CPowerBoard::iHeartBeat = 0;

/**************************************************************
Function:       CPowerBoard::~CPowerBoard
Description:    CPowerBoard类析构函数		
Input:          无              
Output:         无
Return:         无
***************************************************************/
CPowerBoard::~CPowerBoard()
{	
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct PowerBoard object ok !\n",__FILE__,__LINE__));
}


/**************************************************************
Function:       CPowerBoard::CreateInstance
Description:    创建电源板类静态对象		
Input:          无              
Output:         无
Return:         CPowerBoard静态对象指针
***************************************************************/
CPowerBoard* CPowerBoard::CreateInstance()
{
	static CPowerBoard cPowerBoard; 
	return &cPowerBoard;
}


/**************************************************************
Function:       CPowerBoard::GetPowerBoardCfg
Description:    主控板请求电源模块发送电压数据，电压状态		
Input:          无              
Output:         无
Return:         0
***************************************************************/
void CPowerBoard::CheckVoltage()
{
	SCanFrame sSendFrameTmp;
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN , FRAME_MODE_P2P  , BOARD_ADDR_POWER , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_RESEND << 6 ) | POWER_HEAD_ENV;
	sSendFrameTmp.ucCanDataLen = 1;

	Can::CreateInstance()->Send(sSendFrameTmp);

}


/**************************************************************
Function:       CPowerBoard::GetPowerBoardCfg
Description:    主控板请求电源模块发送配置数据，包括电压高低压预
				警电压值以及强弱点控制预案			
Input:          无              
Output:         无
Return:         0
***************************************************************/
void CPowerBoard::GetPowerBoardCfg()
{
	SCanFrame sSendFrameTmp;
	//SCanFrame sRecvFrameTmp;

	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
	//ACE_OS::memset(&sRecvFrameTmp , 0 , sizeof(SCanFrame));

	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN
				  , FRAME_MODE_P2P   , BOARD_ADDR_POWER
				  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_RESEND << 6 ) | POWER_HEAD_CFG_GET;
	sSendFrameTmp.ucCanDataLen = 1;

	Can::CreateInstance()->Send(sSendFrameTmp);

	
}


/**************************************************************
Function:       CPowerBoard::SetPowerBoardCfg
Description:    主控板设置电源板配置数据				
Input:          无              
Output:         无
Return:         0
***************************************************************/
void CPowerBoard::SetPowerBoardCfg()
{
	SCanFrame sSendFrameTmp;
	//SCanFrame sRecvFrameTmp;

	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
	//ACE_OS::memset(&sRecvFrameTmp , 0 , sizeof(SCanFrame));

	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN
				  , FRAME_MODE_P2P   , BOARD_ADDR_POWER
				  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_RESEND << 6 ) | POWER_HEAD_CFG_SET;
	sSendFrameTmp.pCanData[1] = m_iSetWarnHighVol;
	sSendFrameTmp.pCanData[2] = m_iSetWarnLowVol ;
	sSendFrameTmp.pCanData[3] |= m_ucSetStongHighVolPlan;
	sSendFrameTmp.pCanData[3] |= m_ucSetStongLowVolPlan << 2;
	sSendFrameTmp.pCanData[3] |= m_ucSetWeakHighVolPlan << 4;
	sSendFrameTmp.pCanData[3] |= m_ucSetWeakLowVolPlan << 6;
	sSendFrameTmp.pCanData[4] = m_ucSetWatchCfg ;
	sSendFrameTmp.ucCanDataLen = 5;

	Can::CreateInstance()->Send(sSendFrameTmp);

	
}
/**************************************************************
Function:       CPowerBoard::SetPowerCfgData
Description:    主控板设置电源板配置数据参数				
Input:          WarnHighVol -高电压预警值
		     WarnLowVol  -低电压预警值
		     VolPlan         -电压控制方案
		     Output:         无
Return:         0
***************************************************************/

void CPowerBoard::SetPowerCfgData(Byte m_ucWarnHighVol,Byte m_ucWarnLowVol,Byte m_ucVolPlan,Byte m_ucDogCfg)
{
	m_iSetWarnHighVol = m_ucWarnHighVol  ;
	m_iSetWarnLowVol  = m_ucWarnLowVol  ;
	m_ucSetStongHighVolPlan = m_ucVolPlan &0x03 ;
	m_ucSetStongLowVolPlan  = (m_ucVolPlan>>2) &0x03 ;
	m_ucSetWeakHighVolPlan  = (m_ucVolPlan>>4) &0x03 ;
	m_ucSetWeakLowVolPlan   = (m_ucVolPlan>>6) &0x03 ;
	m_ucSetWatchCfg         =  m_ucDogCfg ;
	return ;
}

/**************************************************************
Function:       CPowerBoard::HeartBeat
Description:    电源板发送广播心跳Can数据包，用于保持主板和部件的
				连接状态
Input:          无              
Output:         无
Return:         0
***************************************************************/
void CPowerBoard::HeartBeat()
{
	SCanFrame sSendFrameTmp;
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
	
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN , FRAME_MODE_HEART_BEAT  , BOARD_ADDR_POWER , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_NOREPLY << 6 ) | POWER_HEAD_HEARBEAT;
	sSendFrameTmp.ucCanDataLen = 1;
	
	Can::CreateInstance()->Send(sSendFrameTmp);
	
}

/**************************************************************
Function:        CPowerBoard::RecvPowerCan
Description:    主控板接收电源板返回Can数据包，解析并处理。				
Input:          ucBoardAddr  电源板地址
				sRecvCanTmp  Can数据包              
Output:         无
Return:         0
***************************************************************/
void CPowerBoard::RecvPowerCan(Byte ucBoardAddr,SCanFrame sRecvCanTmp)
{
		Byte PowerBdIndex = ((ucBoardAddr == 0x20)?0:1);
		Byte ucType = 0;
		if(sRecvCanTmp.pCanData[0] == 0xff)
			ucType = 0xff ;
		else		
			ucType = sRecvCanTmp.pCanData[0] & 0x3F ;
		ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexVoltage); 
		switch(ucType)
		{
		case POWER_HEAD_ENV :	
			m_iStongVoltage = sRecvCanTmp.pCanData[1] + 150;  //强电电压		
			m_iWeakVoltage  = 0;  //弱电电压
			m_iWeakVoltage  = (sRecvCanTmp.pCanData[2] & 0xf)  << 8;
			m_iWeakVoltage  = m_iWeakVoltage | sRecvCanTmp.pCanData[3];
				
			m_iWeakVoltage  = m_iWeakVoltage / 500;		
			m_iBusVoltage   = 0;  //总线电压
			m_iBusVoltage   = (sRecvCanTmp.pCanData[4] & 0xf)  << 8;
			m_iBusVoltage  = m_iBusVoltage | sRecvCanTmp.pCanData[5];
			m_iBusVoltage  = m_iBusVoltage / 500;
		
			//ACE_DEBUG((LM_DEBUG,"%s:%d StongVol:%d WeakVol:%d BusVol:%d			\n",__FILE__,__LINE__,m_iStongVoltage,m_iWeakVoltage,m_iBusVoltage)); //MOD:0604 1738
			break ;
		case POWER_HEAD_CFG_GET :
			m_iGetWarnHighVol = sRecvCanTmp.pCanData[1] ;
			m_iGetWarnLowVol  = sRecvCanTmp.pCanData[2] ;
			m_ucGetStongHighVolPlan = sRecvCanTmp.pCanData[3] & 0x3;
			m_ucGetStongLowVolPlan  = (sRecvCanTmp.pCanData[3] >> 2 )& 0x3;
			m_ucGetWeakHighVolPlan  = (sRecvCanTmp.pCanData[3] >> 4 )& 0x3; 
			m_ucGetWeakLowVolPlan   = (sRecvCanTmp.pCanData[3] >> 6 )& 0x3;
			m_ucSetWatchCfg  = sRecvCanTmp.pCanData[4];
			ACE_DEBUG((LM_DEBUG,"%s:%d VolHigh:%d VolLow:%d VolPlan:%d PowerWatchDog:%d !\n",__FILE__,__LINE__,m_iGetWarnHighVol,m_iGetWarnLowVol,sRecvCanTmp.pCanData[3],m_ucSetWatchCfg));
			break;
		case POWER_HEAD_HEARBEAT :
			 iHeartBeat = 0;
			 //ACE_DEBUG((LM_DEBUG,"%s:%d Get from PowerBoard,iHeartBeat = %d		\n",__FILE__,__LINE__,iHeartBeat)); //MOD:0604 1738
			 break ;
		case POWER_HEAD_VER:
			m_ucPowerBoardVer[PowerBdIndex][0] = sRecvCanTmp.pCanData[1];
			m_ucPowerBoardVer[PowerBdIndex][1] = sRecvCanTmp.pCanData[2];
			m_ucPowerBoardVer[PowerBdIndex][2] = sRecvCanTmp.pCanData[3];
			m_ucPowerBoardVer[PowerBdIndex][3] = sRecvCanTmp.pCanData[4];
			m_ucPowerBoardVer[PowerBdIndex][4] = sRecvCanTmp.pCanData[5];
			
		//	ACE_OS::printf("%s:%d PowerBoardver[%d]:%d %d %d %d %d \n",__FILE__,__LINE__,PowerBdIndex,sRecvCanTmp.pCanData[1],
				//	sRecvCanTmp.pCanData[2],sRecvCanTmp.pCanData[3],sRecvCanTmp.pCanData[4],sRecvCanTmp.pCanData[5]);
			break ;			
			default :				
				break ;
		}

}


/**************************************************************
Function:        CPowerBoard::GetStongVoltage
Description:    获取强电电压属性值，用于控制器LCD电压显示。				
Input:          无
Output:         无
Return:         强电电压
***************************************************************/
int  CPowerBoard::GetStongVoltage()
{
	return m_iStongVoltage ;
}

void CPowerBoard::GetPowerVer(Byte PowerBdindex)
{
	Byte PowerAddr = ((PowerBdindex ==0)?BOARD_ADDR_POWER:BOARD_ADDR_POWER2) ;
	SCanFrame sSendFrameTmp;	
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));	
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN , FRAME_MODE_P2P , PowerAddr , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = POWER_HEAD_VER;
	sSendFrameTmp.ucCanDataLen = 1;	
	Can::CreateInstance()->Send(sSendFrameTmp);
}
