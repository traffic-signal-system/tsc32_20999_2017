
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   FlashMac.cpp
Date:       2013-1-1
Description:黄闪器处理类，处理与黄闪器部件相关操作
Version:    V1.0
History:    201306041700  添加黄闪器心跳检测函数
			201306051116  添加黄闪器配置获取函数
			201306050907  添加下发黄闪器配置函数
***************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "ComStruct.h"
#include "FlashMac.h"
#include "ManaKernel.h"
#include "IoOperate.h"
#include "TscMsgQueue.h"
#include "LampBoard.h"
#include "GbtMsgQueue.h"

#include "Can.h" //ADD: 0604 16 49

/*
黄闪器功能类型枚举
*/
enum  
{
 	 FLASH_HEAD_CFGGET = 0x02 ,   //获取黄闪器配置数据
	 FLASH_HEAD_CFGSET = 0x03 ,	  //设置黄闪器配置数据
	 FLASH_HEAD_ENTRY  = 0x04 ,   //强制黄闪
	 FLASH_HEAD_EXIT   = 0x05 ,   //退出黄闪器强制黄闪状态
	 FLASH_HEAD_VER    =0xff       //黄闪器版本

};

/**************************************************************
Function:       CFlashMac::CFlashMac
Description:    CFlashMac类构造函数，用于初始化类。				
Input:          无			      
Output:         无
Return:         无
***************************************************************/
CFlashMac::CFlashMac()
{
	m_bSetHardwareFlash = false;   
	
	m_iMaxTemperature   = MAX_DEFAULT_TEMPERATURE;        
	m_iMinTemperature   = MIN_DEFAULT_TEMPERATURE;  	

	m_ucSetColdCnt      = 0;         
	m_ucSetHotCnt       = 0;         
	m_ucSetNullFanCnt   = MAX_WORK_TIME;

	m_ucSetFanSts       = FAN_NULL;
	m_ucLastSetFanSts   = FAN_NULL;

	m_iVoltage          = 210;           
	m_iTemperature      = 40;  

	m_bRecordSts    = true;       
	m_ucErrCheckCnt = 0;    
	m_ucRightCnt    = 0; 	

	m_pTscCfg = CManaKernel::CreateInstance()->m_pTscConfig;
	
	ACE_DEBUG((LM_DEBUG,"%s:%d Init FlashYW object ok !\n",__FILE__,__LINE__));
}


/**************************************************************
Function:       CFlashMac::~CFlashMac
Description:    CFlashMac类析构函数。				
Input:          无			      
Output:         无
Return:         无
***************************************************************/
CFlashMac::~CFlashMac()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct FlashYW object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CFlashMac::CreateInstance
Description:    创建CFlashMac静态对象。				
Input:          无			      
Output:         无
Return:         静态对象指针
***************************************************************/
CFlashMac* CFlashMac::CreateInstance()
{
	static CFlashMac cFlashMac; 
	return &cFlashMac;
}

/**************************************************************
Function:       CFlashMac::FlashHeartBeat
Description:    给黄闪器发送心跳检测消息。
				ADD: 201306041700 				
Input:          无			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::FlashHeartBeat()  
{	
	SCanFrame sSendFrameTmp;	
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
	
	Can::BuildCanId(CAN_MSG_TYPE_011 , BOARD_ADDR_MAIN	, FRAME_MODE_HEART_BEAT   , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_NOREPLY<< 6 ) ; 
	sSendFrameTmp.ucCanDataLen = 1;	
	Can::CreateInstance()->Send(sSendFrameTmp);
}

/**************************************************************
Function:       CFlashMac::FlashHeartBeat
Description:    给黄闪器发送上传配置数据信号，用于获取黄闪器配置。
				ADD: 201306051116 				
Input:          无			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::FlashCfgGet() 
{
	SCanFrame sSendFrameTmp;	
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));	

	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN  , FRAME_MODE_P2P  , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_RESEND << 6 ) | FLASH_HEAD_CFGGET;
	sSendFrameTmp.ucCanDataLen = 1;
	Can::CreateInstance()->Send(sSendFrameTmp);	

}

/**************************************************************
Function:       CFlashMac::FlashHeartBeat
Description:    下发配置数据给黄闪器。
				ADD: 201306050907				
Input:          无			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::FlashCfgSet() 
{
		SCanFrame sSendFrameTmp;	
		ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));			
		
		Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN  , FRAME_MODE_P2P , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
		sSendFrameTmp.pCanData[0] = ( DATA_HEAD_RESEND<< 6 ) | FLASH_HEAD_CFGSET;
		sSendFrameTmp.pCanData[1] = m_ucSetFlashRate ; //黄闪器频率设置
		sSendFrameTmp.pCanData[1] |= m_ucSetDutyCycle <<4; //黄闪器占空比设置
		sSendFrameTmp.pCanData[2] = m_ucSetSyType;  //黄闪器同步方式		
		sSendFrameTmp.ucCanDataLen = 3;
	//	ACE_DEBUG((LM_DEBUG,"%s:%d pCanData[1] = %d pCanData[2] = %d m_ucSetFlashRate = %d m_ucSetDutyCycle = %d\n",__FILE__,__LINE__,sSendFrameTmp.pCanData[1],sSendFrameTmp.pCanData[2], m_ucSetFlashRate,m_ucSetDutyCycle));
		Can::CreateInstance()->Send(sSendFrameTmp);

}

/**************************************************************
Function:       CFlashMac::FlashForceStart
Description:    主控板发送强制黄闪消息给黄闪器。				
Input:          ucType  黄闪类型				      
Output:         无
Return:         0
***************************************************************/
void CFlashMac::FlashForceStart(Byte ucType)  //开始强制黄闪 ADD:0605 11 30
{
	SCanFrame sSendFrameTmp;					
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
		
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN	, FRAME_MODE_P2P , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_NOREPLY<< 6 ) | FLASH_HEAD_ENTRY;	
	sSendFrameTmp.pCanData[1] = ucType; //强制黄闪
	sSendFrameTmp.ucCanDataLen = 2;		
	Can::CreateInstance()->Send(sSendFrameTmp);
	//ACE_DEBUG((LM_DEBUG,"%s:%d Begin force flashing !\n",__FILE__,__LINE__));
	CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_MANUALCONTROL,254);
	
	CLampBoard::CreateInstance()->SetSeriousFlash(true);
	SetHardwareFlash(true);			
	CManaKernel::CreateInstance()->SndMsgLog(LOG_TYPE_FLASHBOARD,ucType,0,0,0);//ADD:201309251140  简化添加黄闪器日志消息
					
}

/**************************************************************
Function:       CFlashMac::FlashForceEnd
Description:    主控板发送退出黄闪消息给黄闪器。				
Input:          无				      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::FlashForceEnd()	//退出强制黄闪	ADD:0605 11 40
{
	SCanFrame sSendFrameTmp;
						
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));
		
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN	, FRAME_MODE_P2P , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = ( DATA_HEAD_NOREPLY<< 6 ) | FLASH_HEAD_EXIT;				
	sSendFrameTmp.ucCanDataLen = 1;			
	Can::CreateInstance()->Send(sSendFrameTmp);				//发送退出黄闪消息给黄闪器
	CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_MANUALCONTROL,0);
	CLampBoard::CreateInstance()->SetSeriousFlash(false);	
	
	SetHardwareFlash(false);

}

void CFlashMac::FlashGetVer()
{
	SCanFrame sSendFrameTmp;							
	ACE_OS::memset(&sSendFrameTmp , 0 , sizeof(SCanFrame));			
	Can::BuildCanId(CAN_MSG_TYPE_100 , BOARD_ADDR_MAIN	, FRAME_MODE_P2P , BOARD_ADDR_FLASH  , &(sSendFrameTmp.ulCanId));
	sSendFrameTmp.pCanData[0] = FLASH_HEAD_VER;				
	sSendFrameTmp.ucCanDataLen = 1; 		
	Can::CreateInstance()->Send(sSendFrameTmp); 
}

		
/**************************************************************
Function:       CFlashMac::SetHardwareFlash
Description:    设置黄闪器黄闪状态属性。				
Input:          isflash   true: 处于黄闪状态
						  false 处于非黄闪状态				      
Output:         改变CFlashMac对象的m_bSetHardwareFlash属性
Return:         无
***************************************************************/
void CFlashMac::SetHardwareFlash(bool isflash)
{
	if(isflash)
		m_bSetHardwareFlash = true;
	else
		m_bSetHardwareFlash = false;
}


/**************************************************************
Function:       CFlashMac::GetHardwareFlash
Description:    获取黄闪器黄闪状态属性。				
Input:        	无			      
Output:         无
Return:         返回bool值，是否处于黄闪
***************************************************************/
bool CFlashMac::GetHardwareFlash()
{
	return m_bSetHardwareFlash ;
}

/**************************************************************
Function:       CFlashMac::WriteAndRead
Description:    写and读操作 500ms 1per				
Input:        	无			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::WriteAndRead()
{
	Byte ucTx[MAX_FRAME_LEN] = {0};
	Byte ucRx[MAX_FRAME_LEN] = {0};
	Byte ucSum  = 0;
	int  iRet   = 0;
	int  iIndex = 0;
	static int iTick = 0;

#ifndef WINDOWS	
	fd_set fRfds;
	struct timeval 	tTv;
#endif

	SetWriteData(ucTx);

#ifndef WINDOWS
	if ( !CIoOprate::TscWrite(m_iDevFd , ucTx , MAX_FRAME_LEN) )
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
		return;
	}

	if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>5) & 1 )  != 0 )
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d send to flash: %x %x %x %x \n",__FILE__,__LINE__,ucTx[0],ucTx[1],ucTx[2],ucTx[3]));
	}

	FD_ZERO(&fRfds);
	FD_SET(m_iDevFd, &fRfds);
	tTv.tv_sec  = 0;
	tTv.tv_usec = 20000;
	iRet = select(m_iDevFd + 1, &fRfds, NULL, NULL, &tTv);

	if ( -1 == iRet )  //select error
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d select error\n",__FILE__,__LINE__));
	}
	else if ( 0 == iRet )  //timeout
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d timeout\n",__FILE__,__LINE__));
	}
	else
	{
		if ( !CIoOprate::TscRead(m_iDevFd , ucRx , MAX_FRAME_LEN) )
		{
			ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
			return;
		}

		if ( iTick < 60 )  //30s
		{
			iTick++;
			return;
		}

		while ( iIndex < (MAX_FRAME_LEN-1) )
		{
			ucSum += ucRx[iIndex];
			iIndex++;
		}
		if ( ucSum == ucRx[MAX_FRAME_LEN-1] ) //正确的数据
		{
			if ( ucSum != 0 || ( 0 == ucSum && (ucRx[0]!=0 || ucRx[1]!=0 || ucRx[2]!=0 ) ) )  //预防没有黄闪器的情况仍然收到全0的数据
			{
				ResolveReadData(ucRx);
				if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>6) & 1 )  != 0 )
				{
					ACE_DEBUG((LM_DEBUG,"%s:%d flash send to me right %x %x %x %x \n\n",__FILE__,__LINE__
										,ucRx[0],ucRx[1],ucRx[2],ucRx[3]));
				}
				SendRecordBoardMsg(0);
				return;
			}	
		}
		//校验码错误
		if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>6) & 1 )  != 0 )
		{
			ACE_DEBUG((LM_DEBUG,"%s:%d flash send to me error %x %x %x %x \n\n",__FILE__,__LINE__
								,ucRx[0],ucRx[1],ucRx[2],ucRx[3]));
		}
		SendRecordBoardMsg(1);
	}
	
#endif

}

/**************************************************************
Function:       CFlashMac::SendRecordBoardMsg
Description:    发送记录黄闪器通信状态 500ms 1per				
Input:        	ucType  - 0正确 1校验错误			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::SendRecordBoardMsg(Byte ucType)
{
	Byte ucCnt   = 0;
	Byte ucByte0 = 0;
	Byte ucByte1 = 0;

	if ( 0 == ucType && m_bRecordSts )  //当前状态正确且收到正确的数据
	{
		m_ucErrCheckCnt = 0;
		return;
	}

	if ( ucType != 0 && !m_bRecordSts ) //当前状态错误且收到错误的数据
	{
		m_ucRightCnt = 0;
		return;
	}

	switch ( ucType )
	{
		case 0:
			m_ucRightCnt++;
			ucCnt = m_ucRightCnt;  
			break;
		case 1:
			m_ucErrCheckCnt++;
			ucCnt =  m_ucErrCheckCnt;
			break;
		default:
			return;
	}

	if ( ucCnt > BOARD_REPEART_TIME )
	{
		switch ( ucType )
		{
			case 0:  //正确
				m_bRecordSts    = true;
				m_ucErrCheckCnt = 0;
				ucByte0 = 0;
				ucByte1 = 0;
				break;
			case 1:  //校验错误
				m_bRecordSts    = false;
				m_ucRightCnt = 0;
				ucByte0 = 1;
				ucByte1 = 2;
				break;
			default:
				return;
		}
	}
	else
	{
		return;
	}

	CManaKernel::CreateInstance()->SndMsgLog(LOG_TYPE_FLASHBOARD,0,0,ucByte1,ucByte0);//ADD:201309251140  简化添加黄闪器日志消息
	ACE_DEBUG((LM_DEBUG,"%s:%d LOG_TYPE_FLASHBOARD\n",__FILE__,__LINE__));
}


/**************************************************************
Function:       CFlashMac::SetWriteData
Description:    设置发送往控制器的数据用于控制加热散热状态				
Input:        	pTx  - 保存发送信息内容地址			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::SetWriteData(Byte* pTx)
{
	Byte ucTmp = 0;

	if ( m_iTemperature < m_iMinTemperature )  //加热
	{
		m_ucSetColdCnt       = 0;         
		m_ucSetHotCnt++;        
		m_ucSetNullFanCnt    = 0;  
	}
	else if ( m_iTemperature > m_iMaxTemperature ) //散热
	{ 
		m_ucSetColdCnt++;         
		m_ucSetHotCnt        = 0;        
		m_ucSetNullFanCnt    = 0;  
	}
	else   //不操作
	{
		if ( ( m_bGetHotFan && m_iTemperature > UP_NORMAL_TEMPERATURE ) //当前风扇处于加热且已经加到正常温度  停止加热风扇
			|| (m_bGetColdFan && m_iTemperature < DROP_NORMAL_TEMPERATURE ) )  //风扇处于降温且已经降到正常的温度 停止降温
		{
			m_ucSetColdCnt       = 0;         
			m_ucSetHotCnt        = 0;        
			m_ucSetNullFanCnt++;  
		}
	}

	if ( m_bSetHardwareFlash )  //硬件黄闪
	{
		ucTmp |= 1;
	}
	
	if (  m_pTscCfg->sSpecFun[FUN_TEMPERATURE].ucValue != 0 )  //温度计
	{
		if ( m_ucSetColdCnt > MAX_WORK_TIME ) 
		{
			m_ucSetFanSts = FAN_COLD;
		}
		else if ( m_ucSetHotCnt > MAX_WORK_TIME )  
		{
			m_ucSetFanSts = FAN_HOT;
		}
		else if ( m_ucSetNullFanCnt > MAX_WORK_TIME )
		{
			m_ucSetFanSts = FAN_NULL;
		}
	}
	else
	{
		m_ucSetFanSts = FAN_NULL;
	}

	if ( m_ucLastSetFanSts != m_ucSetFanSts )
	{
		m_ucLastSetFanSts = m_ucSetFanSts;		
		CManaKernel::CreateInstance()->SndMsgLog(LOG_TYPE_TEMPERATURE,0,0,m_iTemperature,m_ucLastSetFanSts);//ADD:201309251440  

	}

	switch ( m_ucSetFanSts )
	{
		case FAN_COLD:    //打开散风扇
			ucTmp |= 1<<1;
			break;
		case FAN_HOT:     //打开加热风扇
			ucTmp |= 1<<2;
			break;
		case FAN_NULL:  //无
			break;
		default:
			break;
	}
	pTx[0] = ucTmp;    //命令字
	pTx[1] = 0x55;     //保留
	pTx[2] = 0xff;     //保留	
	pTx[3] = pTx[0] + pTx[1] + pTx[2];    //检验码
}


/**************************************************************
Function:       CFlashMac::ResolveReadData
Description:    解析从控制器获取到的环境数据				
Input:        	pTx  - 获取到的内容缓存地址			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::ResolveReadData(Byte* pTx)
{
	if ( pTx[0] & 0x80 )  //供电类型
	{
		m_bPowerType = true;  //交流电
		m_iVoltage   = (pTx[0] & 0x7F) + 150;
	}
	else
	{
		m_bPowerType = false; //太阳能
		m_iVoltage   = (pTx[0] & 0xF) + 150;
	}

	m_iTemperature = pTx[1];  //温度
	
	/*硬件黄闪*/
	if ( pTx[2] & 0x1 )  
	{
		m_bGetHardwareFlash = true;
	}
	else 
	{
		m_bGetHardwareFlash = false;
	}
	
	/*散热风扇*/
	if ( pTx[2] & 0x2 )
	{
		m_bGetColdFan = true;
	}
	else
	{
		m_bGetColdFan = false;
	}
	
	/*加热风扇*/
	if ( pTx[2] & 0x4 )
	{
		m_bGetHotFan = true;
	}
	else
	{
		m_bGetHotFan = false;
	}

	/*前门打开*/
	if ( pTx[2] & 0x8 )
	{
		m_bGetForDoor = true;
	}
	else
	{
		m_bGetForDoor = false;
	}

	/*后门打开*/
	if ( pTx[2] & 0x10 )
	{
		m_bGetAfterDoor = true;
	}
	else
	{
		m_bGetAfterDoor = false;
	}
}

/**************************************************************
Function:       CFlashMac::SetMaxMinTemperature
Description:    设置允许的最高温度和最低温度				
Input:        	iMaxTpt - 允许最高温度
				iMinTpt - 允许最低温度			      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::SetMaxMinTemperature(int iMaxTpt,int iMinTpt)
{
	m_iMaxTemperature = iMaxTpt;
	m_iMinTemperature = iMinTpt;
}


/**************************************************************
Function:       CFlashMac::RecvFlashCan
Description:    解析从黄闪器发过来的各类Can数据并处理				
Input:        	sRecvCanTmp - 接收到的Can数据包						      
Output:         无
Return:         无
***************************************************************/
void CFlashMac::RecvFlashCan(SCanFrame sRecvCanTmp)
{
	Byte ucType = 0x0;
	if(sRecvCanTmp.pCanData[0] == 0xff)
		ucType = 0xff ;
	else		
		ucType = sRecvCanTmp.pCanData[0] & 0x3F ;
	switch(ucType)
	{
		case FLASH_HEAD_CFGSET:
			if( (m_ucSetSyType != (sRecvCanTmp.pCanData[2])) || (m_ucSetFlashRate != (sRecvCanTmp.pCanData[1]&0xf)) || (m_ucSetDutyCycle != (sRecvCanTmp.pCanData[1])>>4 ))
			{
				ACE_DEBUG((LM_DEBUG,"%s:%d Set FLASH CFG ERROR!\n",__FILE__,__LINE__));			
			}
		break ;
		case FLASH_HEAD_CFGGET :
			m_ucGetFlashRate = sRecvCanTmp.pCanData[1] & 0xf ;
			m_ucGetDutyCycle = sRecvCanTmp.pCanData[1]>>4  & 0xf ;
			m_ucGetSyType      = sRecvCanTmp.pCanData[2] &0x3;
			m_ucFlashStatus  = sRecvCanTmp.pCanData[3] &0xf;
			
			break ;
		case FLASH_HEAD_VER:
			m_ucFlashVer[0]=sRecvCanTmp.pCanData[1];			
			m_ucFlashVer[1]=sRecvCanTmp.pCanData[2];
			m_ucFlashVer[2]=sRecvCanTmp.pCanData[3];
			m_ucFlashVer[3]=sRecvCanTmp.pCanData[4];
			m_ucFlashVer[4]=sRecvCanTmp.pCanData[5];			
			//ACE_OS::printf("%s:%d FlashBdver:%d %d %d %d %d \n",__FILE__,__LINE__,sRecvCanTmp.pCanData[1],
			//	sRecvCanTmp.pCanData[2],sRecvCanTmp.pCanData[3],sRecvCanTmp.pCanData[4],sRecvCanTmp.pCanData[5]);
			break ;
			
		default :
			return ;
			break ;
	}
	
}

