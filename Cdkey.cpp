/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   Cdkey.cpp
Date:       2013-1-1
Description:对信号机的序列号进行检查.
Version:    V1.0
History:
***************************************************************/
#include "Cdkey.h"

/**************************************************************
Function:       Cdkey::Cdkey
Description:    CDKEY的构造函数
Output:         无              
Input:          无
Return:         0
***************************************************************/
Cdkey::Cdkey()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Construnt Cdkey object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       Cdkey::~Cdkey
Description:    CDKEY的析构函数
Output:         无              
Input:          无
Return:         0
***************************************************************/
Cdkey::~ Cdkey()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destroy Cdkey object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       Cdkey* Cdkey::CreateInstance()
Description:    单例模式创建一个cdkey对象
Output:         无              
Input:          无
Return:         cdkey 指针
***************************************************************/
Cdkey* Cdkey::CreateInstance()
{
	static Cdkey Ccdkey;
	return &Ccdkey;
}

/**************************************************************
Function:       Cdkey::InitCdkey()
Description:    对它进行初始化
Output:         无              
Input:          无
Return:         无
***************************************************************/
void Cdkey::InitCdkey()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d init Cdkey object ok !\r\n",__FILE__,__LINE__));
}
/**************************************************************
Function:        Cdkey::GetCdkey(char (&cdkey)[8])
Description:    对它进行初始化
Output:         char (&cdkey)[8]   8个字节的序列号            
Input:          无
Return:         1 成功返回，0 返回失败 
***************************************************************/
bool Cdkey::GetCdkey(char (&cdkey)[8])
{
	int i;	
	int ret;	
	int fd;	
	char buf[120];	
	char id[8];	
	
	fd = ACE_OS::open("/dev/sysinfo", O_RDWR);	
	if (fd < 0) 
	{		
		ACE_OS::printf("%s:%d Open /dev/sysinfo error!\n",__FILE__,__LINE__);		
		return -1;	
	}	
	ret = ACE_OS::read(fd, buf, 120);		
	for(i=0; i<8; i++) 
	{		
		id[i] = buf[112+i];
		cdkey[i] = id[i];
	}
	//cdkey = id;	
	ACE_DEBUG((LM_DEBUG,"\n%s:%d***GetCdkey*** SystemChip Id: %02x %02x %02x %02x %02x %02x %02x %02x  !\n",__FILE__,__LINE__,\
	id[0],id[1],id[2],id[3],id[4],id[5],id[6],id[7]));
	ACE_OS::close(fd);
	return true;
}
