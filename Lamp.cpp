#include "Lamp.h"
#include "TscMsgQueue.h"
#include "ManaKernel.h"
//#include "Serial1.h"
//#include "Serial2.h"
#include "SerialCtrl.h"
#include "DbInstance.h"
#include "IoOperate.h"

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

CLamp::CLamp()
{
	m_tick       = 0;   //默认黄闪频率为0.5HZ
	m_firstSend  = true;
	m_ucFlashFre = 1;
	m_ucOverCnt  = 0;

	ACE_OS::memset(m_ucLampError      , 0 , MAX_LAMP);
	ACE_OS::memset(m_ucLampBoardError , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucLastLampError  , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucLampErrTime    , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucLampStatus     , 0 , 3*MAX_LAMP_BOARD);

	ACE_OS::memset(m_ucNoCnt       , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucErrAddrCnt  , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucErrCheckCnt , 0 , MAX_LAMP_BOARD);
	ACE_OS::memset(m_ucRightCnt    , 0 , MAX_LAMP_BOARD);

	for (int iIndex=0; iIndex<MAX_LAMP; iIndex++ )
	{
		m_bLampErrFlag[iIndex]     = true;
		m_bLampGreenFlag[iIndex]   = false;
		m_bRecordSts[iIndex]       = true;
	}

	for (int iIndex = 0; iIndex<MAX_LAMP_BOARD; iIndex++ )
	{
		m_ucLampBoardError[iIndex] = DEV_IS_CONNECTED;
	}

	//m_iSerial1Fd =  CSerialCtrl::CreateInstance()->GetSerialFd(1);
	//m_iSerial2Fd =  CSerialCtrl::CreateInstance()->GetSerialFd(2);
	m_pTscCfg = CManaKernel::CreateInstance()->m_pTscConfig;

	m_iGreenConflict = 0;
	m_bSeriousFlash  = false;
#ifdef TSC_DEBUG	
	ACE_DEBUG((LM_DEBUG,"create CLamp\n"));
#endif
}

CLamp::~CLamp()
{
	ACE_DEBUG((LM_DEBUG,"free CLamp\n"));
}

CLamp* CLamp::CreateInstance()
{
	static CLamp cLamp;
	return &cLamp;
}

/*
*设置重启后的周期次数
*/
void CLamp::SetOverCycle()
{
	if ( m_ucOverCnt < 4 )
	{
		m_ucOverCnt++;
	}
}

/*
*设置闪光频率
*/
void CLamp::SetFlashFre(Byte ucFlashFre)
{
	//m_mMutex.acquire();

	m_ucFlashFre = ucFlashFre;

	//m_mMutex.release();
}

/*
*合法灯色信息校正
*input: ucType-0亮 ucType-1闪
*/
void CLamp::ReviseLampInfo(Byte ucType,Byte pLampInfo[MAX_LAMP])
{
	Byte ucCheckCnt = 0;
	Byte ucColorCnt = 0;
	SThreadMsg sTscMsg;

	ACE_OS::memset(&sTscMsg,0,sizeof(sTscMsg));

	for ( Byte ucIndex=0; ucIndex<MAX_LAMP; ucIndex++ )
	{
		if ( pLampInfo[ucIndex] != 0 )
		{
			ucColorCnt++;
		}
		ucCheckCnt++;

		if ( ucCheckCnt >= 3 )
		{
			if ( ucColorCnt >= 2 )
			{
				pLampInfo[ucIndex]   = 0;  //green
				pLampInfo[ucIndex-1] = 0;  //yellow
				if ( 0 == ucType )  //lampOn
				{
					pLampInfo[ucIndex-2] = 1;  //red
				}
				else  //lampFlash
				{
					pLampInfo[ucIndex-1] = 1;  //yellow
					pLampInfo[ucIndex-2] = 0;  //red
				}
				//ACE_DEBUG((LM_DEBUG,"%s:%d index:%d lamp info error \n",LAMP_CPP,__LINE__,ucIndex));

				sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
				sTscMsg.ucMsgOpt     = LOG_TYPE_OUTLAMP_ERR;
				sTscMsg.uiMsgDataLen = 4;
				sTscMsg.pDataBuf     = ACE_OS::malloc(4);
				((Byte*)(sTscMsg.pDataBuf))[0] = pLampInfo[ucIndex]   > 0 ?  ucIndex + 1 : 0;
				((Byte*)(sTscMsg.pDataBuf))[1] = pLampInfo[ucIndex-1] > 0 ?  ucIndex     : 0;
				((Byte*)(sTscMsg.pDataBuf))[2] = pLampInfo[ucIndex-2] > 0 ?  ucIndex - 1 : 0;  
				((Byte*)(sTscMsg.pDataBuf))[3] = ucType;
				CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));
			}
			ucColorCnt = 0;
			ucCheckCnt = 0;
		}
	}

}

/*
*灯具颜色设置
*正常一个步伐设置一次
*/
void CLamp::SetLamp(Byte* pLampOn,Byte* pLampFlash)
{
	ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexLamp);

	ACE_OS::memcpy(m_ucLampOn,pLampOn,MAX_LAMP);
	ACE_OS::memcpy(m_ucLampFlash,pLampFlash,MAX_LAMP);

	//ReviseLampInfo(0,m_ucLampOn);
	//ReviseLampInfo(1,m_ucLampFlash);
}

/*
*发送灯具信息
*正常500ms一次
*/
void CLamp::SendLamp()
{
	m_tick++;

	if ( m_tick < m_ucFlashFre ) //未到发送频率
	{
		//m_mMutex.release();
		return;
	}
	else
	{
		m_tick = 0;
	}

	SendToCom1();   //将灯具信息数据发送到com1
}

/*
*将灯具信息按照一定的格式发送到灯驱板
*/
void CLamp::SendToCom1()
{
	Byte ucSum = 0;

	ACE_Guard<ACE_Thread_Mutex>  guard(m_mutexLamp);

	ACE_OS::memset(m_ucSendFrame , 0 , 3 + MAX_LAMP / 8);

	m_ucSendFrame[0] = 0x1f;  //ADDR
	m_ucSendFrame[1] = MAX_LAMP / 8;

	if ( MAX_LAMP % 8 )
	{
		m_ucSendFrame[1] += 1;
	}
	
	for (Byte i=0; i< m_ucSendFrame[1]; i++ )  //DATA
	{
		m_ucSendFrame[2+i] = 0;
		for (Byte j=0; j<8; j++ )
		{
			if ( !m_bLampBoardExit[(i*8+j)/MAX_LAMP_NUM_PER_BOARD] )  //该板没有配置
			{
				//ACE_DEBUG((LM_DEBUG,"%s:%d m_bLampBoardExit[%d]:%d\n",__FILE__,__LINE__,i,m_bLampBoardExit[i]))
				continue;
			}
			m_ucSendFrame[2+i] |= ( ( m_ucLampOn[i*8+j] & 0x1 ) << j );
			if ( m_ucLampFlash[i*8+j] )   //该灯闪
			{
				m_ucLampOn[i*8+j] = !m_ucLampOn[i*8+j];
			}
		}
	}

	//预定义
	m_ucSendFrame[8] = m_ucSendFrame[2];
	m_ucSendFrame[9] = m_ucSendFrame[3];
	m_ucSendFrame[10] = m_ucSendFrame[4];

	for ( Byte i=0; i<(m_ucSendFrame[1]+2); i++ )
	{
		ucSum = ucSum + m_ucSendFrame[i];
	}
	m_ucSendFrame[m_ucSendFrame[1]+2] = ( (~ucSum) & 0xff );   //SUM

	//合法数据判断
	CheckSendFrame(m_ucSendFrame);
	

#ifdef LINUX
	//usleep(USLEEP_TIME);
	ioctl(m_iSerial1Fd, com_9bit_enable, 0);
	//usleep(USLEEP_TIME);
	if ( !CIoOprate::TscWrite(m_iSerial1Fd , m_ucSendFrame , 1) )
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
		return;
	}
	
	//usleep(USLEEP_TIME);
	ioctl(m_iSerial1Fd, com_9bit_disable, 0);  //数据
	//usleep(USLEEP_TIME);
	
	if ( !CIoOprate::TscWrite(m_iSerial1Fd , m_ucSendFrame+1 , m_ucSendFrame[1]+2) )
	{
		//ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
		return;
	}
#endif

	m_firstSend = false;

	if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>3) & 1 )  != 0 )
	{
		int i = 0; 
		ACE_DEBUG((LM_DEBUG,"\nucSendFrame: %02x %02x ",m_ucSendFrame[0],m_ucSendFrame[1]));
		for ( i=0; i<m_ucSendFrame[1]; i++ )
		{
			ACE_DEBUG((LM_DEBUG,"%02x ",m_ucSendFrame[2+i]));
		}
		ACE_DEBUG((LM_DEBUG,"%02x ",m_ucSendFrame[2+i]));
		ACE_DEBUG((LM_DEBUG,"\n"));
	}

	return;
}

/*
*检测广播灯控板的数据
*/
bool CLamp::CheckSendFrame(Byte* ucSendFrame)
{
	static Uint uiFlashTime        = 0;   //记录黄闪的时间
	static Uint uiMoreGreenTime    = 0;   //记录过多绿灯的时间
	static Uint uiUnusualGroupTime = 0;   //记录异常灯组的时间

	bool bFunRet      = true;
	Byte ucSendCount  = ucSendFrame[1];   //发送长度
	Byte ucCheckCount = 0;
	Byte ucColorCount = 0;
	Byte ucFlashCount = 0;   
	Byte ucGreenCount = 0;
	Byte ucData       = 0;
	Uint uiCurTime    = (Uint)ACE_OS::gettimeofday().sec();   //当前时间

	SThreadMsg sTscMsg;
	ACE_OS::memset(&sTscMsg,0,sizeof(sTscMsg));

	if ( 0x1f != ucSendFrame[0] )
	{
		return false;
	}

	ucSendCount = 6;  //目前只检测4块灯控板的数据

	for ( Byte ucIndex=0; ucIndex<ucSendCount; ucIndex++ )
	{
		ucData = ucSendFrame[2+ucIndex];

		for ( Byte ucDataIndex=0; ucDataIndex<8; ucDataIndex++ )
		{
			if ( (ucData >> ucDataIndex & 1) != 0 )  //亮灯
			{
				ucColorCount++;  

				if ( 1 == ucCheckCount )  //R Y(1) G
				{
					ucFlashCount++;
				}
				else if ( 2 == ucCheckCount )  //R Y G(1)
				{
					ucGreenCount++;
				}
			}

			ucCheckCount++;

			if ( 3 == ucCheckCount )  //R Y G 检查一次  
			{
				if ( ucColorCount >= 2 )  //灯组不止亮一个灯
				{
					bFunRet = false;

					if ( uiCurTime - uiUnusualGroupTime > 30 * 60 )
					{
						sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
						sTscMsg.ucMsgOpt     = LOG_TYPE_OUTLAMP_ERR2;
						sTscMsg.uiMsgDataLen = 4;
						sTscMsg.pDataBuf     = ACE_OS::malloc(4);
						((Byte*)(sTscMsg.pDataBuf))[0] = 0;
						((Byte*)(sTscMsg.pDataBuf))[1] = 0;
						((Byte*)(sTscMsg.pDataBuf))[2] = 0;  
						((Byte*)(sTscMsg.pDataBuf))[3] = 1;
						CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

						uiUnusualGroupTime = uiCurTime;

						//printf("%s:%d err lamp group\n",__FILE__,__LINE__);
					}
				}

				ucCheckCount = 0;
				ucColorCount = 0;
			}
		}
	}

	if ( ucFlashCount > 15 )   //黄闪
	{
		bFunRet = false;

		if ( uiCurTime - uiFlashTime > 30 * 60 )
		{
			sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
			sTscMsg.ucMsgOpt     = LOG_TYPE_OUTLAMP_ERR2;
			sTscMsg.uiMsgDataLen = 4;
			sTscMsg.pDataBuf     = ACE_OS::malloc(4);
			((Byte*)(sTscMsg.pDataBuf))[0] = 0;
			((Byte*)(sTscMsg.pDataBuf))[1] = 0;
			((Byte*)(sTscMsg.pDataBuf))[2] = 0;  
			((Byte*)(sTscMsg.pDataBuf))[3] = 2;
			CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));
			uiFlashTime = uiCurTime;

			//printf("%s:%d flash\n",__FILE__,__LINE__);
		}
	}
	if ( ucGreenCount > 10 )  //绿灯过多 多于10盏
	{
		bFunRet = false;

		if ( uiCurTime - uiMoreGreenTime > 30 * 60 )
		{
			sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
			sTscMsg.ucMsgOpt     = LOG_TYPE_OUTLAMP_ERR2;
			sTscMsg.uiMsgDataLen = 4;
			sTscMsg.pDataBuf     = ACE_OS::malloc(4);
			((Byte*)(sTscMsg.pDataBuf))[0] = 0;
			((Byte*)(sTscMsg.pDataBuf))[1] = 0;
			((Byte*)(sTscMsg.pDataBuf))[2] = 0;  
			((Byte*)(sTscMsg.pDataBuf))[3] = 3;
			CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

			uiMoreGreenTime = uiCurTime;

			//printf("%s:%d more green count\n",__FILE__,__LINE__);
		}
	}

	return bFunRet;
}

/*
*打印接收到的灯泡数据信息
*input: pFileName:文件名 iFileLine:文件行数   ucBoardIndex:板的下标
*       sErrSrc:错误原因 iPrintCnt:打印的个数 ucRecvBuf：打印的字符串
*/
void CLamp::PrintLampInfo(char* pFileName,int iFileLine,Byte ucBoardIndex,
						  char* sErrSrc,int iPrintCnt,Byte* ucRecvBuf)
{
	if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>4) & 1 )  != 0 )
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d iIndex:%d %s:",pFileName,iFileLine,ucBoardIndex,sErrSrc));
		for ( int iTmp = 0; iTmp<iPrintCnt; iTmp++ )
		{
			ACE_DEBUG((LM_DEBUG,"  %02x",ucRecvBuf[iTmp]));
		}
		ACE_DEBUG((LM_DEBUG,"\n"));
	}
}

/*
*接收灯具回送过来的数据
*/
void CLamp::RecvFromCom1(int iBoardIndex)
{
	if ( m_ucOverCnt < 4 )
	{
		return;
	}

	if ( m_bSeriousFlash )
	{
		//ACE_DEBUG((LM_DEBUG,"%s:%d SeriousFlash over!!!\n",__FILE__,__LINE__));
		return;
	}

	Byte ucSum           = 0;
	Byte ucLampErrSts    = 0;
	Byte ucLampBoardAddr = 0;
	int iRetCnt          = 0;
	int iLampIndex       = 0;
	Byte ucRecvLampUnitBoard[6] = {0};

#ifndef WINDOWS

	fd_set rfds;
	struct timeval 	tv;

	if ( m_iSerial1Fd < 0 )
	{
		return;
	}

	if ( m_firstSend )
	{
		return;
	}
#endif

	//for ( int i=iBoardIndex; i<(iBoardIndex+2) && i<MAX_LAMP_BOARD; i++ )
	//{
		int i=iBoardIndex;
		if ( !m_bLampBoardExit[i] )
		{
			//continue;
			return;
		}

#ifndef WINDOWS
		//usleep(USLEEP_TIME);
		ioctl(m_iSerial1Fd, com_9bit_enable, 0);
		//usleep(USLEEP_TIME);
#endif
		ucLampBoardAddr = (Byte)(i + LAMP_BOARD_START_ADDR);  /*灯控板地址*/
#ifndef WINDOWS
		//write(m_iCom1fd, &ucLampBoardAddr, 1);
		if ( !CIoOprate::TscWrite(m_iSerial1Fd , &ucLampBoardAddr , 1) )
		{
			//ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
			return;
		}
		//usleep(USLEEP_TIME);
		ioctl(m_iSerial1Fd, com_9bit_disable, 0);
		//usleep(USLEEP_TIME);


		ucSum = ~ucLampBoardAddr;
		//write(m_iCom1fd, &ucSum, 1);
		if ( !CIoOprate::TscWrite(m_iSerial1Fd , &ucSum , 1) )
		{
			ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
			return;
		}

		FD_ZERO(&rfds);
		FD_SET(m_iSerial1Fd, &rfds);
		tv.tv_sec  = 0;
		tv.tv_usec = 20000;
		iRetCnt = select(m_iSerial1Fd + 1, &rfds, NULL, NULL, &tv);
#endif
		
		if ( -1 == iRetCnt )  /*select error*/
		{
#ifndef WINDOWS
			//usleep(USLEEP_TIME);
			ioctl(m_iSerial1Fd, com_9bit_disable, 0);
			//usleep(USLEEP_TIME);
			SendRecordBoardMsg(i,3);
			return;
		}
		else if ( 0 == iRetCnt )  /*timeout*/
		{
			if( m_ucLampBoardError[i] != DEV_IS_DISCONNECTED )
			{
				m_ucLampBoardError[i] = DEV_IS_DISCONNECTED;
			}
			//usleep(USLEEP_TIME);
			ioctl(m_iSerial1Fd, com_9bit_disable, 0);
			//usleep(USLEEP_TIME);
#endif
			SendRecordBoardMsg(i,3);
			PrintLampInfo((char*)"Lamp.cpp",__LINE__,i,(char*)"Timeout",0,NULL);
			return;
		}
		else  /*收到数据*/
		{
			m_ucLampBoardError[i] = DEV_IS_CONNECTED;

#ifndef WINDOWS
			//usleep(USLEEP_TIME);
			ioctl(m_iSerial1Fd, com_9bit_disable, 0);
			//usleep(USLEEP_TIME);

			//iRetCnt = read(m_iSerial1Fd, ucRecvLampUnitBoard, 6);
			if ( !CIoOprate::TscRead(m_iSerial1Fd , ucRecvLampUnitBoard , 6) )
			{
				//ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
				//continue;
				return;
			}
#endif
			/*
			if ( ( (m_pTscCfg->sSpecFun[FUN_PRINT_FLAG].ucValue>>4) & 1 )  != 0 )
			{
				ACE_DEBUG((LM_DEBUG,"%s:%d iBoardIndex:%d recv lampStatus:",__FILE__,__LINE__,i));
				for ( int iTmp = 0; iTmp<6; iTmp++ )
				{
					ACE_DEBUG((LM_DEBUG,"  %02x",ucRecvLampUnitBoard[iTmp]));
				}
				ACE_DEBUG((LM_DEBUG,"\n"));
			}
			*/

#ifdef WINDOWS
			/*ucRecvLampUnitBoard[0] = 0x16;
			ucRecvLampUnitBoard[1] = 0x03;
			ucRecvLampUnitBoard[2] = 0x59;
			ucRecvLampUnitBoard[3] = 0x55;
			ucRecvLampUnitBoard[4] = 0x69;
			ucRecvLampUnitBoard[5] = 0xcf;*/
#endif

			if ( ucRecvLampUnitBoard[0] != (0x10 + ucLampBoardAddr) )
			{
				SendRecordBoardMsg(i,2);
				PrintLampInfo((char*)"Lamp.cpp",__LINE__,i,(char*)"Addr Error",6,ucRecvLampUnitBoard);
				//continue;
				return;
			}
			else
			{
				//判断接收到的数据是否为正确的数据...............
				#if 0
				ACE_DEBUG((LM_DEBUG,"%s:%d recv lampStatus:",__FILE__,__LINE__));
				for ( int iTmp = 0; iTmp<6; iTmp++ )
				{
					ACE_DEBUG((LM_DEBUG,"  %02x",ucRecvLampUnitBoard[iTmp]));
				}
				ACE_DEBUG((LM_DEBUG,"\n"));
				#endif

				ucSum = 0;
				for (int j=0; j<5; j++ )
				{
					ucSum += ucRecvLampUnitBoard[j];
				}
				ucSum =  (~ucSum) & 0xff;

				//ACE_DEBUG((LM_DEBUG,"!!!!!!!!!!!!!!!! %d %d \n",ucSum,ucRecvLampUnitBoard[5]));

				if ( ucRecvLampUnitBoard[5] != ucSum )  /*校验码错误*/
				{
					SendRecordBoardMsg(i,1);
					PrintLampInfo((char*)"Lamp.cpp",__LINE__,i,(char*)"Checksum Error",6,ucRecvLampUnitBoard);
					return;
				}
				else   /*校验码正确*/
				{
					SendRecordBoardMsg(i,0);
					PrintLampInfo((char*)"Lamp.cpp",__LINE__,i,(char*)"Right",6,ucRecvLampUnitBoard);
					for ( int iDataId=0; iDataId<3; iDataId++ )
					{
						iLampIndex   = i * MAX_LAMP_NUM_PER_BOARD + iDataId * 4;
						
						for ( int j=0; j<4; j++ )
						{
							if ( !CManaKernel::CreateInstance()->IsInChannel((iLampIndex+j)/3+1) )  //该通道有对应配置
							{
								continue;
							}

							ucLampErrSts = (ucRecvLampUnitBoard[2+iDataId]>>(2*j)) & 0x03;

							if ( m_ucLastLampError[iLampIndex+j] == ucLampErrSts )  //00正常；01长亮；10长灭；11可控硅击穿	
							{
								if ( m_ucLampErrTime[iLampIndex+j] > 3 )
								{
									if ( m_bLampErrFlag[iLampIndex+j] && (DEV_IS_GOOD != ucLampErrSts ) )
									{
										m_ucLampError[iLampIndex+j]  = ucLampErrSts; 
										m_bLampErrFlag[iLampIndex+j] = false;
										//add err lamp
										//ACE_DEBUG((LM_DEBUG,"Add LampErr LampNo:%d ErrType:%d\n"
										//	,iLampIndex + j , ucLampErrSts));
										SThreadMsg sTscMsg;
										sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
										sTscMsg.ucMsgOpt     = LOG_TYPE_LAMP;
										sTscMsg.uiMsgDataLen = 4;
										sTscMsg.pDataBuf     = ACE_OS::malloc(4);
										((Byte*)(sTscMsg.pDataBuf))[0] = 0;
										((Byte*)(sTscMsg.pDataBuf))[1] = 0;
										((Byte*)(sTscMsg.pDataBuf))[2] = ucLampErrSts;
										((Byte*)(sTscMsg.pDataBuf))[3] = iLampIndex + j + 1; 
										CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

										SThreadMsg sTscMsgGreen;
										sTscMsgGreen.ulType       = TSC_MSG_GREENCONFLICT;  
										sTscMsgGreen.ucMsgOpt     = 0;
										sTscMsgGreen.uiMsgDataLen = 0;
										sTscMsgGreen.pDataBuf     = NULL;
										CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsgGreen,sizeof(sTscMsgGreen));
									}
									else if ( !m_bLampErrFlag[iLampIndex+j] && (DEV_IS_GOOD == ucLampErrSts) )
									{
										m_ucLampError[iLampIndex+j]  = ucLampErrSts; 
										m_bLampErrFlag[iLampIndex+j] = true;
										//clear err lamp
										//ACE_DEBUG((LM_DEBUG,"Clear LampErr LampNo:%d ErrType:%d\n"
															//,iLampIndex + j , ucLampErrSts));

										SThreadMsg sTscMsg;
										sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
										sTscMsg.ucMsgOpt     = LOG_TYPE_LAMP;
										sTscMsg.uiMsgDataLen = 4;
										sTscMsg.pDataBuf     = ACE_OS::malloc(4);
										((Byte*)(sTscMsg.pDataBuf))[0] = 0;
										((Byte*)(sTscMsg.pDataBuf))[1] = 0;
										((Byte*)(sTscMsg.pDataBuf))[2] = ucLampErrSts;
										((Byte*)(sTscMsg.pDataBuf))[3] = iLampIndex + j + 1;
										CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));
									}
									
								}
								else
								{
									m_ucLampErrTime[iLampIndex+j]++;
								}
							}
							else
							{
								m_ucLastLampError[iLampIndex+j] = ucLampErrSts;
								m_ucLampErrTime[iLampIndex+j]   = 0;
							}
						}

						m_ucLampStatus[i*3+iDataId] = ucRecvLampUnitBoard[2+iDataId];

					}
				}
			}
		}
	//}
}

/*
*发送记录灯控板通信状态
*input: ucBoardIndex 板下标 0 1 2 3 
*       ucType  - 0正确 1校验错误 2地址错误 3没有数据
*/
void CLamp::SendRecordBoardMsg(Byte ucBoardIndex,Byte ucType)
{
	Byte ucCnt   = 0;
	Byte ucByte0 = 0;
	Byte ucByte1 = 0;

	if ( ( 0 == ucType && m_bRecordSts[ucBoardIndex] ) 
		|| ( ucType != 0 && !m_bRecordSts[ucBoardIndex] ) )  //当前为正确状态又来正确数据 当前为错误状态又来错误数据
	{
		switch ( ucType )
		{
			case 0:  //正确	
				m_ucErrCheckCnt[ucBoardIndex] = 0;
				m_ucNoCnt[ucBoardIndex]       = 0;          
				m_ucErrAddrCnt[ucBoardIndex]  = 0; 
				return;
			case 1:  //校验错误
				m_ucNoCnt[ucBoardIndex]       = 0;          
				m_ucErrAddrCnt[ucBoardIndex]  = 0;     
				m_ucRightCnt[ucBoardIndex]    = 0;
				return;
			case 2:  //地址错误
				m_ucErrCheckCnt[ucBoardIndex] = 0;
				m_ucNoCnt[ucBoardIndex]       = 0;            
				m_ucRightCnt[ucBoardIndex]    = 0;
				return;
			case 3:  //没有数据
				m_ucErrCheckCnt[ucBoardIndex] = 0;        
				m_ucErrAddrCnt[ucBoardIndex]  = 0;     
				m_ucRightCnt[ucBoardIndex]    = 0;
				return;
			default:
				return;
		}
	}

	switch ( ucType )
	{
		case 0:
			m_ucRightCnt[ucBoardIndex]++;
			ucCnt = m_ucRightCnt[ucBoardIndex];
			break;
		case 1:
			m_ucErrCheckCnt[ucBoardIndex]++;
			ucCnt =  m_ucErrCheckCnt[ucBoardIndex];
			break;
		case 2:
			m_ucErrAddrCnt[ucBoardIndex]++;
			ucCnt = m_ucErrAddrCnt[ucBoardIndex];
			break;
		case 3:
			m_ucNoCnt[ucBoardIndex]++;
			ucCnt = m_ucNoCnt[ucBoardIndex];
			break;
		default:
			return;
	}

	if ( ucCnt > BOARD_REPEART_TIME )
	{
		switch ( ucType )
		{
			case 0:  //正确
				m_bRecordSts[ucBoardIndex] = true;
				m_ucErrCheckCnt[ucBoardIndex] = 0;
				m_ucNoCnt[ucBoardIndex]       = 0;          
				m_ucErrAddrCnt[ucBoardIndex]  = 0; 
				ucByte0 = 0;
				ucByte1 = 0;
				break;
			case 1:  //校验错误
				m_bRecordSts[ucBoardIndex] = false;
				m_ucNoCnt[ucBoardIndex]       = 0;          
				m_ucErrAddrCnt[ucBoardIndex]  = 0;     
				m_ucRightCnt[ucBoardIndex]    = 0;
				ucByte0 = 1;
				ucByte1 = 2;
				break;
			case 2:  //地址错误
				m_bRecordSts[ucBoardIndex] = false;
				m_ucErrCheckCnt[ucBoardIndex] = 0;
				m_ucNoCnt[ucBoardIndex]       = 0;            
				m_ucRightCnt[ucBoardIndex]    = 0;
				ucByte0 = 1;
				ucByte1 = 1;
				break;
			case 3:  //没有数据
				m_bRecordSts[ucBoardIndex] = false;
				m_ucErrCheckCnt[ucBoardIndex] = 0;        
				m_ucErrAddrCnt[ucBoardIndex]  = 0;     
				m_ucRightCnt[ucBoardIndex]    = 0;
				ucByte0 = 1;
				ucByte1 = 0;
				break;
			default:
				return;
		}
	}
	else
	{
		return;
	}

	//ACE_DEBUG((LM_DEBUG,"ucBoardIndex:%d ucType:%d %d-%d-%d-%d\n",
	//	ucBoardIndex,ucType,m_bRecordSts[0],m_bRecordSts[1],m_bRecordSts[2],m_bRecordSts[3]));

	SThreadMsg sTscMsg;
	sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
	sTscMsg.ucMsgOpt     = LOG_TYPE_LAMPBOARD;
	sTscMsg.uiMsgDataLen = 4;
	sTscMsg.pDataBuf     = ACE_OS::malloc(4);
	((Byte*)(sTscMsg.pDataBuf))[0] = 0;
	((Byte*)(sTscMsg.pDataBuf))[1] = ucBoardIndex;
	((Byte*)(sTscMsg.pDataBuf))[2] = ucByte1;
	((Byte*)(sTscMsg.pDataBuf))[3] = ucByte0; 
	CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

	//ACE_DEBUG((LM_DEBUG,"%s:%d ucBoardIndex:%d ucByte1:%d ucByte0:%d\n"
		//,__FILE__,__LINE__,ucBoardIndex,ucByte1,ucByte0));
}

/*
*判断是否为绿冲突
*/
void CLamp::IsGreenConfict()
{
	static bool bFirst = true;

	if ( bFirst )
	{
		bFirst = false;
		return;
	}

	if ( m_bSeriousFlash )  
	{
		return;
	}

	if ( LampGreenAlwaysOn() || LampAllRedOff() )  //绿冲突
	{
		if ( m_iGreenConflict < 2 )
		{
			m_iGreenConflict++;
			return;
		}

		SThreadMsg sTscMsg;
		sTscMsg.ulType       = TSC_MSG_GREENCONFLICT;  
		sTscMsg.ucMsgOpt     = 0;
		sTscMsg.uiMsgDataLen = 0;
		sTscMsg.pDataBuf     = NULL;
		CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

		//ACE_DEBUG((LM_DEBUG,"*******green conflict \n"));
	}
}


/*
*判断某个绿灯常亮
*
*return: true : 存在某个绿灯常亮
*        false: 绿灯都正常
*/
bool CLamp::LampGreenAlwaysOn()
{
	bool bFunRet = false;

	for ( int i=0; i<MAX_LAMP_BOARD; i++ )
	{
		if ( DEV_IS_CONNECTED == m_ucLampBoardError[i] )  /*灯控板存在*/
		{
			for ( int j=i*MAX_LAMP_NUM_PER_BOARD; j<(i+1)*MAX_LAMP_NUM_PER_BOARD; j++ )
			{ 
				if ( CManaKernel::CreateInstance()->IsInChannel(j/3+1)   //在通道参数里
					&& (LAMP_COLOR_GREEN == j % 3) )  //绿灯
				{
					if ( (DEV_ALWAYS_ON == m_ucLampError[j]) || (DEV_SILICON_SHOT == m_ucLampError[j]) )  //常亮
					{
						if ( !m_bLampGreenFlag[j] )
						{
							//(CDbInstance::m_cGbtTscDb).AddEventLog(j+1,2);  //绿冲突
							SThreadMsg sTscMsg;

							sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
							sTscMsg.ucMsgOpt     = LOG_TYPE_GREEN_CONFIG;
							sTscMsg.uiMsgDataLen = 4;
							sTscMsg.pDataBuf     = ACE_OS::malloc(4);
							((Byte*)(sTscMsg.pDataBuf))[0] = 0;
							((Byte*)(sTscMsg.pDataBuf))[1] = 0;
							((Byte*)(sTscMsg.pDataBuf))[2] = 0;
							((Byte*)(sTscMsg.pDataBuf))[3] = j + 1;
							CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

							m_bLampGreenFlag[j] = true;
							bFunRet = true;
							ACE_DEBUG((LM_DEBUG,"%s:%d LampGreenAlwaysOn\n",__FILE__,__LINE__));
						}
					}
				}
			}
		}
	}

	//ACE_DEBUG((LM_DEBUG,"%s:%d no LampGreenAlwaysOn\n",__FILE__,__LINE__));

	return false;
}

void CLamp::SetSeriousFlash()
{
	m_bSeriousFlash = true;
}

/*
*判断没有红灯亮
*
*return: true : 没有红灯亮
*        false: 存在亮的红灯
*/
bool CLamp::LampAllRedOff()
{
	bool bExitLampBoard = false;
	int iIndex = 0;
	unsigned int uiTscStatus = CManaKernel::CreateInstance()->m_pRunData->uiWorkStatus;

	if ( (SIGNALOFF == uiTscStatus) || (FLASH == uiTscStatus) )
	{
		return false;
	}

	while ( iIndex < MAX_LAMP_BOARD )
	{
		if ( DEV_IS_CONNECTED == m_ucLampBoardError[iIndex] )
		{
			bExitLampBoard = true;
			break;
		}
		iIndex++;
	}

	if ( !bExitLampBoard )  //不存在灯控板
	{
		return false;
	}

	for ( int i=0; i<MAX_LAMP_BOARD; i++ )
	{
		if ( DEV_IS_CONNECTED == m_ucLampBoardError[i] )  /*灯控板存在*/
		{
			for ( int j=i*MAX_LAMP_NUM_PER_BOARD; j<(i+1)*MAX_LAMP_NUM_PER_BOARD; j++ )
			{
				if ( (LAMP_COLOR_RED == j % 3 ) && (DEV_IS_GOOD == m_ucLampError[j]) )  /*正常亮的红灯*/
				{
					/*
					ACE_DEBUG((LM_DEBUG,"%s:%d j:%d m_ucLampError[%d]:%d LampAllRedOff\n"
						,__FILE__,__LINE__,j,j,m_ucLampError[j]));
					*/

					return false;
				}
			}
		}
	}
	
	//(CDbInstance::m_cGbtTscDb).AddEventLog(3,3);  //无红灯亮
	SThreadMsg sTscMsg;

	sTscMsg.ulType       = TSC_MSG_LOG_WRITE;
	sTscMsg.ucMsgOpt     = LOG_TYPE_GREEN_CONFIG;
	sTscMsg.uiMsgDataLen = 4;
	sTscMsg.pDataBuf     = ACE_OS::malloc(4);
	((Byte*)(sTscMsg.pDataBuf))[0] = 0;
	((Byte*)(sTscMsg.pDataBuf))[1] = 0;
	((Byte*)(sTscMsg.pDataBuf))[2] = 0;
	((Byte*)(sTscMsg.pDataBuf))[3] = 1;
	CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

	//ACE_DEBUG((LM_DEBUG,"%s:%d LampAllRedOff\n",__FILE__,__LINE__));

	return true;
}

/*
*
* true: 冲突相位绿灯同时亮 false:正常
bool CLamp::InConflictPhase()
{
	bool bGreenConflict = false;
	int  iIndex = 0;
	int  jIndex = 0;
	int iCurStepNo    = CManaKernel::CreateInstance()->m_pRunData->ucStepNo;
	Uint uiAllowPhase = CManaKernel::CreateInstance()->m_pRunData->sStageStepInfo[iCurStepNo].uiAllowPhase;

	GBT_DB::Collision* pConflictPhase = CManaKernel::CreateInstance()->m_pTscConfig->sPhaseConflict;
	
	while ( iIndex < MAX_PHASE )
	{
		if ( (uiAllowPhase >> iIndex) & 1 )  //存在该相位
		{

		}
		iIndex++;
	}
	
	
	return false;
}
*/

/*
*模拟发送
*保存到文件
*/
#ifdef WINDOWS
void CLamp::SimulateSend()
{
	FILE* fp = ACE_OS::fopen("lamp.txt","a+");
	char cTmp[256] = {0};
	//ACE_OS::fwrite(m_usLampSendData,sizeof(m_usLampSendData[0]),1,fp);
	//ACE_OS::fwrite("\n", sizeof("\n"),1,fp);
	
	for (int i=0; i<MAX_LAMP_BOARD; i++ )
	{
		ACE_OS::memset(cTmp,0,256);
		ACE_OS::itoa(m_usLampSendData[i],cTmp,2);
		ACE_OS::fprintf(fp,"m_usLampSendData[%d]:%s\n",i,cTmp);
	}

	ACE_OS::fprintf(fp,"\n");
	
	ACE_OS::fclose(fp);
}
#endif 

/*
*发送灯具状态给串口2，用于交通信号智能管理器
*1s进来一次
*/
#if 0
void CLamp::SendLampStsToCom2()
{
//#ifdef CALLNOTEFAULT
	if ( 0 == CManaKernel::CreateInstance()->m_pTscConfig->sSpecFun[FUN_MSG_ALARM].ucValue )
	{
		return;
	}

	static int iTick = 3;
	int iIndex       = 0;
	int iBufIndex    = 1;
	Byte ucDataLen   = 0;
	Byte ucSum       = 0;
	Byte ucData[5+3*MAX_LAMP_BOARD] = {0XAA , 0};

	if ( iTick > 0 )
	{
		iTick--;
		return;
	}
	iTick = 3;

	/****** 数据 ******/
	iBufIndex = 4;
	while ( iIndex < MAX_LAMP_BOARD  )
	{
		if ( DEV_IS_CONNECTED == m_ucLampBoardError[iIndex] )  //该板存在
		{
			ucData[iBufIndex++] = m_ucLampStatus[iIndex*3];
			ucData[iBufIndex++] = m_ucLampStatus[iIndex*3+1];
			ucData[iBufIndex++] = m_ucLampStatus[iIndex*3+2];
			ucDataLen += 3;   //1块板三个字节
		}
		iIndex++;
	}

	ucData[1] = ucDataLen + 2;  //长度
	ucData[2] = 0x02;
	ucData[3] = ucDataLen;  

	iIndex = 0;
	while ( iIndex< (ucDataLen+4) )
	{
		ucSum += ucData[iIndex];
		iIndex++;
	}

	ucData[iBufIndex] = ~ucSum;

#ifndef WINDOWS

#if 0
	ACE_DEBUG((LM_DEBUG,"&&&&&&&&&& %s:%d &&&&&&&&:\n",__FILE__,__LINE__));
	iIndex = 0;
	while ( iIndex < (ucDataLen+5) )
	{
		ACE_DEBUG((LM_DEBUG,"%x ",ucData[iIndex]));
		iIndex++;
	}
	ACE_DEBUG((LM_DEBUG,"\n"));
#endif

	/*
	if ( 0 == ucDataLen ) 
	{
		write(m_iCom2fd, ucData, ucDataLen+5);
	}
	*/

	write(m_iSerial2Fd, ucData, ucDataLen+5);
	if ( !CIoOprate::TscWrite(m_iSerial2Fd,ucData,ucDataLen+5) )
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d write error",__FILE__,__LINE__));
	}
#endif
//#endif

}
#endif

/*
*获取灯控板是否存在，是否有配置存在
*/
void CLamp::GetLampBoardExit(bool* bLampBoardExit)
{
	for ( int iIndex=0; iIndex < MAX_LAMP_BOARD; iIndex++ )
	{
		m_bLampBoardExit[iIndex] = bLampBoardExit[iIndex];

		ACE_DEBUG((LM_DEBUG,"m_bLampBoardExit[%d]:%d ",iIndex,m_bLampBoardExit[iIndex]));
	}

	ACE_DEBUG((LM_DEBUG,"%s:%d \n ",__FILE__,__LINE__));
}


