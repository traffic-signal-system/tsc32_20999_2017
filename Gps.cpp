
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   MainBoardLed.cpp
Date:       2013-1-1
Description:信号机GPS校时处理
Version:    V1.0
History:
***************************************************************/
#include "Gps.h"
#include "SerialCtrl.h"
#include "TscMsgQueue.h"
#include "GbtMsgQueue.h"
#include "ManaKernel.h"
#include <termios.h>
#include "MainBoardLed.h"


/**************************************************************
Function:        CGps::CGps
Description:     CGps类构造函数，初始化类
Input:          无
Output:         无
Return:         无
***************************************************************/
CGps::CGps()
{
    ACE_DEBUG((LM_DEBUG,"%s:%d Init GPS object !\r\n",__FILE__,__LINE__));
    ierrorcount = 0 ;
    m_bGpsTime = false ;
    m_tLastTi = 0 ;
    m_iGpsFd  = CSerialCtrl::CreateInstance()->GetSerialFd2();
    ACE_OS::memset(m_cBuf,0,128);
}

/**************************************************************
Function:       CGps::~CGps
Description:    CGps类析构函数
Input:          无
Output:         无
Return:         无
***************************************************************/
CGps::~CGps()
{

    ACE_DEBUG((LM_DEBUG,"%s:%d Destruct CGps object!\r\n",__FILE__,__LINE__));

}

/**************************************************************
Function:       CGps::CGps
Description:    创建CGps类静态对象
Input:          无
Output:         无
Return:         CGps静态对象指针
***************************************************************/
CGps* CGps::CreateInstance()
{
    static CGps cGps;
    return &cGps;
}

/**************************************************************
Function:       CGps::RunGpsData
Description:    GPS线程入口函数
Input:          arg  默认NULL
Output:         无
Return:         0
***************************************************************/
void CGps::RunGpsData()
{
    if ( m_iGpsFd < 0 )
    {
        ACE_DEBUG((LM_DEBUG,"\n%s:%d Open Gps Serial error!\n",__FILE__,__LINE__));
        return ;
    }
    char *pBuf   = m_cBuf;
    int iFlagGps = 0;
    tcflush(m_iGpsFd, TCIFLUSH); //清空缓冲区,避免受到上次GPS接收到的信息干扰
    while ( true )
    {
        if ( ACE_OS::read(m_iGpsFd, pBuf,1) <= 0 )
        {
            ACE_OS::sleep(ACE_Time_Value(0,100*1000));
            ierrorcount ++ ;
            if(ierrorcount > 300) // 30秒还是没有读取到则休眠一分钟
            {
                ACE_DEBUG((LM_DEBUG,"\n%s:%d Cant read gps info than 300 times!\n",__FILE__,__LINE__));
                ierrorcount = 0 ;
                //return ; TODO  这里读取1000次都没有读取到GPS信息，因此记录数据库日志中。
                CMainBoardLed::CreateInstance()->SetSignalLed(LED_GPS,LED_STATUS_FLASH); //ADD:20141106 无GPS接收数据则闪烁
                ACE_OS::sleep(60);
            }
            continue;
        }
        ACE_OS::printf("%c",*pBuf);
        if ( '$' == *pBuf )
        {
            pBuf++;
            iFlagGps = 1;
        }
        else if ( 1 == iFlagGps )
        {
            if ( ('*' == *(pBuf-2) ) || ( (pBuf - m_cBuf) >= (int)sizeof(m_cBuf) - 1) )
            {
                *(pBuf+1) = '\n';
                if ( 0 == ACE_OS::strncmp(GPRMC, m_cBuf, ACE_OS::strlen(GPRMC)) )
                {
                    //$GPRMC,023543.00,A,2308.28715,N,11322.09875,E,0.195,,240213,,,A*78
                    ACE_DEBUG((LM_DEBUG,"\n%s,%d gps_read %s \n", __FILE__, __LINE__, m_cBuf));
                    if ( CheckSum(m_cBuf) )
                    {
                        Byte result = Extract();
                        if(result == 1)
                        {
                            CMainBoardLed::CreateInstance()->SetSignalLed(LED_GPS,LED_STATUS_ON); //ADD:20141106 接收正常并解析则亮
                            //day表示几天，也就是线程休眠几天。1表示每天进行校时，2表示第两天进行校时......
                            Byte day = CManaKernel::CreateInstance()->m_pTscConfig->sSpecFun[FUN_GPS_INTERVAL].ucValue;
                            m_bGpsTime = false ;
                            Uint m_iIntervalSec = day*3600*24 ;
                            m_iIntervalSec = 3*3600; //测试默认3小时校时一次
                            while(m_iIntervalSec/60 >0) //采取一分钟休眠一次，判断是否唤醒
                            {
                                if(m_tLastTi == 0)  //强制校时
                                    break ;
                                m_iIntervalSec -= 60 ;
                                ACE_OS::sleep(60);
                            }
                            m_bGpsTime = true ;
                            tcflush(m_iGpsFd, TCIFLUSH); //清空缓冲区,避免受到上次GPS接收到的信息干扰
                        }
                    }
                    else
                    {
                        ACE_DEBUG((LM_DEBUG,"\n%s,%d gps_read checksum error! \n", __FILE__, __LINE__));
                    }
                }
                pBuf     = m_cBuf;
                iFlagGps = 0;
                ACE_OS::memset(m_cBuf,0,128);

            }
            else
            {
                pBuf++;
            }
        }
        else
        {
            pBuf = m_cBuf;
        }

    }

    return ;
}

/**************************************************************
Function:       CGps::Extract
Description:    解析GPS数据
Input:          无
Output:         无
Return:         0-失败 1-成功
***************************************************************/
int CGps::Extract()
{
    char  cValid = 'V';
    char* pBuf   = m_cBuf;
    int   iYear  = 0;
    int   iMon   = 0;
    int   iDay   = 0;
    int   iHour  = -1;
    int   iMin   = -1;
    int   iSec   = -1;
    int   iMsec  = -1;
    int   iIndex = 0;

#ifndef WINDOWS
    sscanf(pBuf, GPRMC "," "%02d%02d%02d.%03d,%c,"  "%*s\r\n", &iHour, &iMin, &iSec, &iMsec, &cValid);

    for ( iIndex = 0; pBuf && iIndex < 9; iIndex++ )
    {
        pBuf = index(pBuf, ',');
        if ( pBuf == NULL )
        {
            return 0;
        }
        pBuf++;
    }

    if ( pBuf )
    {
        sscanf(pBuf, "%02d%02d%02d,%*s\r\n", &iDay, &iMon, &iYear);
    }

    if (! (iYear >= 0 && iYear <= 99 && iMon >= 1  && iMon <= 12 && iDay  >= 1  && iDay <= 31 && iHour >= 0 && iHour <= 23 && iMin >= 0  &&
           iMin <= 59 && iSec  >= 0  && iSec <= 59 ))

    {
        return 0;
    }

    if ( cValid != 'A' )
    {
        return 0;
    }
    SetTime(2000 + iYear, iMon, iDay, iHour, iMin, iSec );


#endif
    return 1;
}


/**************************************************************
Function:       CGps::CheckSum
Description:    校验GPS数据
Input:          cMsg  GPS数据指针
Output:         无
Return:         false-失败 true-成功
***************************************************************/
bool CGps::CheckSum(char *cMsg)
{
    char *pBuf         = NULL;
    int iChecksum      = 0;
    Byte ucSum         = 0;

#ifndef WINDOWS
    if ( NULL == cMsg )
    {
        return false;
    }

    if ( NULL == (pBuf = index(cMsg, '*')) )
    {
        return false;
    }

    if ( sscanf(pBuf, "*%X\r\n", &iChecksum) != 1 )
    {
        return false;
    }
    if ( '$' == *cMsg )
    {
        cMsg++;
    }
    while ( cMsg && *cMsg != '*' )
    {
        ucSum ^= *cMsg;
        cMsg++;
    }
    //ACE_DEBUG((LM_DEBUG,"receive checksum is %02X and the calculation is %02X\n", iChecksum, ucSum ));
#endif

    if ( ucSum == iChecksum )
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**************************************************************
Function:       CGps::SetTime
Description:    设置系统时间
Input:          iYear-年    iMon-月      iDay-日
             iHour-时    iMin-分      iSec-秒
Output:         无
Return:         无
***************************************************************/
void CGps::SetTime(int iYear, int iMon, int iDay, int iHour, int iMin, int iSec)
{
    time_t Ttime;
    struct tm *pTheTime;
    SThreadMsg sTscMsg;

    Ttime = time(NULL);
    pTheTime = localtime(&Ttime);
    pTheTime->tm_year = iYear - 1900;
    pTheTime->tm_mon  = iMon - 1;
    pTheTime->tm_mday = iDay;
    pTheTime->tm_hour = iHour;
    pTheTime->tm_min  = iMin;
    pTheTime->tm_sec  = iSec;
    Ttime = mktime(pTheTime);
    m_tLastTi = Ttime ;
    char sgpstime[200]= {0} ;
    ACE_OS::sprintf(sgpstime,"echo LocalSysTime:$(date) GpsTimeUTC: %d-%d-%d %d:%02d:%02d >>GpsTime.info", iYear,iMon,iDay,iHour,iMin,iSec);
    ACE_OS::system(sgpstime);
    //ACE_OS::printf("%s:%d GpsTime: %d-%d-%d %d:%02d:%02d Gps2Localtime: %d-%d-%d %d:%02d:%02d\r\n",__FILE__,__LINE__,iYear,iMon,iDay,iHour,iMin,iSec,pLocalTime->tm_year+1900,pLocalTime->tm_mon+1,pLocalTime->tm_mday,pLocalTime->tm_hour,pLocalTime->tm_min,pLocalTime->tm_sec);
    sTscMsg.ulType       = TSC_MSG_CORRECT_TIME;
    sTscMsg.ucMsgOpt     = OBJECT_UTC_TIME;
    sTscMsg.uiMsgDataLen = 4;
    sTscMsg.pDataBuf     = ACE_OS::malloc(4);
    Ttime += 2*8*3600 ;  // 这里主要是配合上位机，上位机更新时间的时间取值.
    *((Byte*)sTscMsg.pDataBuf+3)  = Ttime & 0xff;
    *((Byte*)sTscMsg.pDataBuf+2)  = (Ttime>>8) & 0xff;
    *((Byte*)sTscMsg.pDataBuf+1)  = (Ttime>>16) & 0xff;
    *((Byte*)sTscMsg.pDataBuf)    = (Ttime>>24) & 0xff;
    CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));

}

/**************************************************************
Function:       CGps::ForceAdjust
Description:    设置强制校时    属性
Input:          无
Output:         无
Return:         无
***************************************************************/
void CGps::ForceAdjust()
{
    m_tLastTi = 0;
}
/**************************************************************
Function:       CGps::GetLastTi
Description:    返回GPS校时比较时间
Input:          无
Output:         无
Return:         无
***************************************************************/
time_t CGps::GetLastTi()
{
    return m_tLastTi ;
}



