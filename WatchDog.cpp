
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   WatchDog.cpp
Date:       2013-1-1
Description:看门狗设备文件操作文件
Version:    V1.0
History:    
***************************************************************/
#include "WatchDog.h"

/**************************************************************
Function:       WatchDog::WatchDog
Description:    WatchDog类构造函数，初始化看门狗参数				
Input:          无              
Output:         无
Return:         无
***************************************************************/
WatchDog::WatchDog()
{
	m_watchdogFd = -1;
	ACE_DEBUG((LM_DEBUG,"\n%s:%d Init WatchDog object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       WatchDog::~WatchDog
Description:    WatchDog类析构函数	
Input:          无              
Output:         无
Return:         无
***************************************************************/
WatchDog::~WatchDog()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct WatchDog object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       WatchDog::CreateInstance
Description:    创建	WatchDog静态对象
Input:          无              
Output:         无
Return:         静态对象指针
***************************************************************/
WatchDog* WatchDog::CreateInstance()
{
	static WatchDog cWatchDog; 

	return &cWatchDog;
}


/**************************************************************
Function:       WatchDog::OpenWatchdog
Description:    打开看门狗设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
void WatchDog::OpenWatchdog()
{
#ifndef WINDOWS
	
	m_watchdogFd = open(DEV_WATCHDOG, O_WRONLY);
	timeout = 3;
	ioctl(m_watchdogFd, WDIOC_SETTIMEOUT, &timeout);
	ioctl(m_watchdogFd, WDIOC_GETTIMEOUT, &timeout);
	ACE_DEBUG((LM_DEBUG,"%s:%d The timeout was is %d seconds\n\n",__FILE__,__LINE__,timeout));
	
	//下面为forlinux核心板里的代码
	//m_watchdogFd = open(DEV_WATCHDOG, O_WRONLY, 0);
#endif
}


/**************************************************************
Function:       WatchDog::CloseWatchdog
Description:    关闭看门狗设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
void WatchDog::CloseWatchdog()
{
#ifndef WINDOWS
	if(m_watchdogFd >= 0)
	{
		close(m_watchdogFd);
	}
#endif
}


/**************************************************************
Function:       WatchDog::FillWatchdog
Description:    喂狗操作，如果输入字符‘V’，可以关闭看门狗（扩展）
Input:          无              
Output:         无
Return:         无
***************************************************************/
void WatchDog::FillWatchdog(char cData)
{
#ifndef WINDOWS
	if(m_watchdogFd > 0)
	{
		write(m_watchdogFd, &cData, sizeof(cData)); //喂狗操作，如果输入字符‘V’，可以关闭看门狗
		//ACE_DEBUG((LM_DEBUG,"%s:%d filldog\n",__FILE__,__LINE__));
	}
#endif

}




