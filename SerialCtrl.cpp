/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   SerialCtrl.cpp
Date:       2013-9-13
Description:串口处理操作类
Version:    V1.0
History:    
***************************************************************/
#include "SerialCtrl.h"
#include "ace/Synch.h"
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <termios.h>            /* tcgetattr, tcsetattr */
#include <stdio.h>              /* perror, printf, puts, fprintf, fputs */
#include <unistd.h>             /* read, write, close */
#include <fcntl.h>              /* open */
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */
#include "Define.h"
#include <ace/OS.h>




/*  
* Decription for TIMEOUT_SEC(buflen,baud); 
* baud bits per second, buflen bytes to send. 
* buflen*20 (20 means sending an octect-bit data by use of the maxim bits 20)
* eg. 9600bps baudrate, buflen=1024B, then TIMEOUT_SEC = 1024*20/9600+1 = 3 
* don't change the two lines below unless you do know what you are doing.
*/
#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)
#define TIMEOUT_USEC 0
#define CH_TO_WAIT 5
#define CH_BITS 11
#define BUFFER_LEN  1024    /* sendfile() */
static INT32    fd;             //File descriptor for the port
static struct termios termios_old, termios_new;
static fd_set   fs_read, fs_write;
static struct timeval tv_timeout;
static void     set_baudrate (INT32);
static INT32    get_baudrate ();
static void     set_data_bit (INT32 databit);
static INT32    baudrate2Bxx (INT32 baudrate);
static INT32    Bxx2baudrate (INT32 _baudrate);
static INT32    set_port_attr ( 
	INT32 baudrate, 		//                             
	INT32 databit,                             
	const char *stopbit,                              
	char parity);
static void     set_stopbit (const char *stopbit);
static void     set_parity (char parity);


/**************************************************************
Function:       CSerialCtrl::CSerialCtrl
Description:    CSerialCtrl类构造函数，用于类初始化串口				
Input:          无              
Output:         无
Return:         无
***************************************************************/
CSerialCtrl::CSerialCtrl()
{
	m_iSerial1fd = -1;
	m_iSerial2fd = -1;
	m_iSerial3fd = -1;
	//m_iSerial4fd = -1;
	m_iSerial5fd = -1;
	OpenALLSerial();
	ACE_DEBUG((LM_DEBUG,"%s:%d Init SerialCom object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CSerialCtrl::~CSerialCtrl
Description:    CSerialCtrl类析构函数	
Input:          无              
Output:         无
Return:         无
***************************************************************/
CSerialCtrl::~CSerialCtrl()
{
	if( m_iSerial2fd > 0 )
	{
		close(m_iSerial2fd);
	}
	else if( m_iSerial1fd > 0 )
	{
		close(m_iSerial1fd);
	}
	else if( m_iSerial3fd > 0 )
	{
		close(m_iSerial3fd);
	}
	//else if( m_iSerial4fd > 0 )
	//{
	//	close(m_iSerial4fd);
	//}
	else if( m_iSerial5fd > 0 )
	{
		close(m_iSerial5fd);
	}else if(fd >0)
	{
		CloseComPort();
	}
	
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct SerialCom object ok !\n",__FILE__,__LINE__));
}
	

/**************************************************************
Function:       CSerialCtrl::CreateInstance
Description:    创建	CSerialCtrl静态对象
Input:          无              
Output:         无
Return:         静态对象指针
***************************************************************/
CSerialCtrl* CSerialCtrl::CreateInstance()
{
	static CSerialCtrl cSerialOperate;

	return &cSerialOperate;
}

/**************************************************************
Function:       CSerialCtrl::OpenComPort
Description:    打开串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::OpenComPort (INT32 ComPort, INT32 baudrate, INT32 databit,const char *stopbit, char parity)
{    
	char *pComPort;    
	INT32 retval;    
	switch (ComPort) 
	{    
		case 0:        
			pComPort = (char *)"/dev/ttyO0";        
			break;    
		case 1:        
			pComPort = (char *)"/dev/ttyO1";        
			break;    
		case 2:        
			pComPort = (char *)"/dev/ttyO2";        
			break;    
		case 3:        
			pComPort = (char *)"/dev/ttyO3";        
			break;    
		//case 4:        
		//	pComPort = "/dev/ttyO4";        
		//	break;    
		case 5:        
			pComPort = (char *)"/dev/ttyO5";        
			break;    
		default:        
			pComPort = (char *)"/dev/ttyO0";        
			break;    
	}    
	fd = open (pComPort, O_RDWR | O_NOCTTY);    
	if (-1 == fd) 
	{        
		fprintf (stderr, "cannot open port %s\n", pComPort);        
		return (-1);    
	}    
	ACE_OS::printf("\n%s:%d***OpenComPort*** SerialComPort fd = %d \n ",__FILE__,__LINE__,fd);    
	tcgetattr (fd, &termios_old);       /* save old termios value */   
	/* 0 on success, -1 on failure */    
	retval = set_port_attr (baudrate, databit, stopbit, parity);    
	if (-1 == retval) 
	{        
		fprintf (stderr, "\nport %s cannot set baudrate at %d\n", pComPort,baudrate);    
	} 
	switch (ComPort) 
	{    
		case 0:        
			//pComPort = "/dev/ttyO0";        
			break;    
		case 1:        
			m_iSerial1fd = fd;    
			break;    
		case 2:        
			m_iSerial2fd = fd;
			break;    
		case 3:        
			m_iSerial3fd = fd;
			break;    
		//case 4:        
		//	m_iSerial4fd = fd;
		//	break;    
		case 5:        
			m_iSerial5fd = fd;
			break;    
		default:        
			//m_iSerial1fd = fd;
			break;    
	}    
	return (retval);
}

/* close serial port by use of file descriptor fd */
void CSerialCtrl::CloseComPort ()
{    
/* flush output data before close and restore old attribute */    
	tcsetattr (fd, TCSADRAIN, &termios_old);    
	close (fd);
}
int CSerialCtrl::getPortFd()
{    
	return fd;
}
/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPort (Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}
/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPortBySerial1(Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (m_iSerial1fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (m_iSerial1fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (m_iSerial1fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}
/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPortBySerial2 (Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (m_iSerial2fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (m_iSerial2fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (m_iSerial2fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}
/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPortBySerial3 (Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (m_iSerial3fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (m_iSerial3fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (m_iSerial3fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}
/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPortBySerial4 (Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (m_iSerial4fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (m_iSerial4fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (m_iSerial4fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
INT32 CSerialCtrl::ReadComPortBySerial5 (Byte *data, INT32 datalength)
{    

	
	INT32           retval = 0;        
	FD_ZERO (&fs_read);    
	FD_SET (m_iSerial5fd, &fs_read);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;   
	//ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d   ,  %s , %d\n",fd,data,datalength));
	
	retval = select (m_iSerial5fd + 1, &fs_read, NULL, NULL, &tv_timeout);    
	if (retval > 0) 
	{        
		retval = read (m_iSerial5fd, data, datalength);        
		return (retval);    
	}    
	else 
	{        
		if (0 == retval ) 
		{            
			return (0);        
		}
		else
		{            
			return (-1);        
		}    
	}
}

INT32 ReadComPortA (Byte *data, INT32 datalength)
{    
	INT32           retval = 0;    
	int bytes_read;    
	int readlen;    
	/**     
	* caculate the time of 5 characters and get the maxim     
	* with 3ms and 5 ch's time    
	*/    
	tv_timeout.tv_sec = 0;    
	tv_timeout.tv_usec = ( (CH_TO_WAIT * CH_BITS) * (1000000/get_baudrate()));    
	//printf("port read timeout:%dus\n",tv_timeout.tv_usec);    
	bytes_read = 0;    
	while(bytes_read<datalength)
	{        
		tv_timeout.tv_sec = 0;        
		tv_timeout.tv_usec = ( (CH_TO_WAIT * CH_BITS) * (1000000/get_baudrate()));        
		FD_ZERO (&fs_read);        
		FD_SET (fd, &fs_read);        
		retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);        
		if ( retval >0 ) 
		{   
			//void * 进行运算产生warning 不影响程序运行
			readlen = read (fd, (data+bytes_read), datalength);            
			bytes_read += readlen;        
		} 
		else
			return (bytes_read==0?-1:bytes_read);    
	}    
	return -1;
}

/**************************************************************
Function:        CSerialCtrl::WriteComPortBySerial3    
Description:     通过串口3读取写入数据	
Input:        	data  - 数据内容
				datalength  - 消息内容长度		      
Output:         无
Return:         0 -错误  1-正确
***************************************************************/
INT32 CSerialCtrl::WriteComPortBySerial1(Byte * data, INT32 datalength)
{
	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (m_iSerial1fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (m_iSerial1fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (m_iSerial1fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (m_iSerial1fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}

/**************************************************************
Function:        CSerialCtrl::WriteComPortBySerial3    
Description:     通过串口3读取写入数据	
Input:        	data  - 数据内容
				datalength  - 消息内容长度		      
Output:         无
Return:         0 -错误  1-正确
***************************************************************/
INT32 CSerialCtrl::WriteComPortBySerial2(Byte * data, INT32 datalength)
{
	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (m_iSerial2fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (m_iSerial2fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (m_iSerial2fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (m_iSerial2fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}

/**************************************************************
Function:        CSerialCtrl::WriteComPortBySerial3    
Description:     通过串口3读取写入数据	
Input:        	data  - 数据内容
				datalength  - 消息内容长度		      
Output:         无
Return:         0 -错误  1-正确
***************************************************************/
INT32 CSerialCtrl::WriteComPortBySerial3(Byte * data, INT32 datalength)
{
	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (m_iSerial3fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (m_iSerial3fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (m_iSerial3fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (m_iSerial3fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}

/**************************************************************
Function:        CSerialCtrl::WriteComPortBySerial3    
Description:     通过串口3读取写入数据	
Input:        	data  - 数据内容
				datalength  - 消息内容长度		      
Output:         无
Return:         0 -错误  1-正确
***************************************************************/
INT32 CSerialCtrl::WriteComPortBySerial4(Byte * data, INT32 datalength)
{
	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (m_iSerial4fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (m_iSerial4fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (m_iSerial4fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (m_iSerial4fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}
/**************************************************************
Function:        CSerialCtrl::WriteComPortBySerial3    
Description:     通过串口3读取写入数据	
Input:        	data  - 数据内容
				datalength  - 消息内容长度		      
Output:         无
Return:         0 -错误  1-正确
***************************************************************/
INT32 CSerialCtrl::WriteComPortBySerial5(Byte * data, INT32 datalength)
{
	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (m_iSerial5fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (m_iSerial5fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (m_iSerial5fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (m_iSerial5fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}
/*  
* Write datalength bytes in buffer given by UINT8 *data, 
* return value: bytes written 
* Nonblock mode
*/
INT32 CSerialCtrl::WriteComPort (Byte * data, INT32 datalength)
{    

	INT32           retval, len = 0, total_len = 0;    
	FD_ZERO (&fs_write);    
	FD_SET (fd, &fs_write);    
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate());    
	tv_timeout.tv_usec = TIMEOUT_USEC;    
	for (total_len = 0, len = 0; total_len < datalength;)
	{        
		retval = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);        
		if (retval) 
		{            
			len = write (fd, &data[total_len], datalength - total_len);   
			if (len > 0) 
			{                
				total_len += len;           
			}      
		}        
		else 
		{            
			tcflush (fd, TCOFLUSH);     
			/* flush all output data */            
			break;        
		}    
	}    
	return (total_len);
}

/* get serial port baudrate */
static INT32 get_baudrate ()
{    
	return (Bxx2baudrate (cfgetospeed (&termios_new)));
}

/* set serial port baudrate by use of file descriptor fd */

static void set_baudrate (INT32 baudrate)
{    

	termios_new.c_cflag = baudrate2Bxx (baudrate);  
	/* set baudrate */
}

static void set_data_bit (INT32 databit)
{  
	termios_new.c_cflag &= ~CSIZE;   
	switch (databit) 
	{
		case 8:        
			termios_new.c_cflag |= CS8;   
			break;  
		case 7:      
			termios_new.c_cflag |= CS7;     
			break;  
		case 6:      
			termios_new.c_cflag |= CS6;     
			break;  
		case 5:     
			termios_new.c_cflag |= CS5;    
			break;   
		default:     
			termios_new.c_cflag |= CS8;    
			break;  
	}
}

static void set_stopbit (const char *stopbit)
{   
	if (0 == strcmp (stopbit, "1")) 
	{       
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */  
	}   
	else if (0 == strcmp (stopbit, "1.5")) 
	{       
		termios_new.c_cflag &= ~CSTOPB; /* 1.5 stop bits */  
	}   
	else if (0 == strcmp (stopbit, "2")) 
	{       
		termios_new.c_cflag |= CSTOPB;  /* 2 stop bits */  
	}    
	else 
	{       
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */  
	}
}

static void set_parity (char parity)
{   
	switch (parity) 
	{  
		case 'N':                  /* no parity check */ 
			termios_new.c_cflag &= ~PARENB;   
			break;  
		case 'E':     
			/* even */  
			termios_new.c_cflag |= PARENB;  
			termios_new.c_cflag &= ~PARODD;   
			break;  
		case 'O':        
			/* odd */    
			termios_new.c_cflag |= PARENB;     
			termios_new.c_cflag |= ~PARODD;   
			break;  
		default:      
			/* no parity check */  
			termios_new.c_cflag &= ~PARENB;   
			break;  
	}
}



static INT32 set_port_attr (  	INT32 baudrate,        // 1200 2400 4800 9600 .. 115200   
									INT32 databit,           // 5, 6, 7, 8         
									const char *stopbit,  //  "1", "1.5", "2"      
									char parity)              // N(o), O(dd), E(ven)
{   
	bzero(&termios_new, sizeof (termios_new));	
	cfmakeraw (&termios_new);	
	set_baudrate (baudrate);  
	termios_new.c_cflag |= CLOCAL | CREAD;  
	/* | CRTSCTS */   
	set_data_bit (databit);  
	set_parity (parity);   
	set_stopbit (stopbit);  
	termios_new.c_oflag 			= 0;  
	termios_new.c_lflag 			|= 0;  
	termios_new.c_oflag 			&= ~OPOST;  
	termios_new.c_cc[VTIME] 	= 1;        /* unit: 1/10 second. */ 
	termios_new.c_cc[VMIN] 		= 255; /* minimal characters for reading */  
	tcflush (fd, TCIFLUSH);
	return (tcsetattr (fd, TCSANOW, &termios_new));
}

/** 
* baudrate xxx to Bxxx *  
* @@param baudrate xxx *  
* @@return  */
static INT32 baudrate2Bxx (INT32 baudrate)
{   
	switch (baudrate) 
	{    
		case 0:    
			return (B0); 
		case 50:    
			return (B50);  
		case 75:   
			return (B75);  
		case 110:     
			return (B110);  

		case 134:   
			return (B134); 
		case 150:   
			return (B150);   
		case 200:     
			return (B200);   
		case 300:    
			return (B300);   
		case 600:    
			return (B600);  
		case 1200:      
			return (B1200);  
		case 2400:    
			return (B2400); 
		case 4800:    
			return (B4800);
		case 9600:    
			return (B9600);  
		case 19200:    
			return (B19200);  
		case 38400:    
			return (B38400);  
		case 57600:   
			return (B57600);  
		case 115200:   
			return (B115200);  
		default:    
			return (B9600); 
	}
}
/** 
* get 
boundrate from Bxxx *  
* @@param baudrate Bxxx refers to bound rate *  
* @@return  
*/

static INT32 Bxx2baudrate (INT32 _baudrate)
{
/* reverse baudrate */    
	switch (_baudrate) 
	{  
		case B0:    
			return (0); 
		case B50:   
			return (50);  
		case B75:   
			return (75);   
		case B110:   
			return (110);  
		case B134:     
			return (134);  
		case B150:   
			return (150);  
		case B200:   
			return (200); 
		case B300:    
			return (300);  
		case B600:    
			return (600);  
		case B1200:   
			return (1200);  
		case B2400:    
			return (2400);  
		case B4800:    
			return (4800);
		case B9600:    
			return (9600); 
		case B19200:    
			return (19200);   
		case B38400:    
			return (38400);  
		case B57600:    
			return (57600); 
		case B115200:     
			return (115200); 
		default:   
			return (9600);   
	}
}

/**************************************************************
Function:       CSerialCtrl::OpenALLSerial
Description:    打开所有串口设备文件
Input:          无              
Output:         无
Return:         无
***************************************************************/
void CSerialCtrl::OpenALLSerial()
{
	int ret = -1;
	ret = OpenComPort(SERIALNUM1, 115200, 8, "1", 'N');
	if (ret < 0) {
		ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d\n",SERIALNUM1));
		//return ;	
	}else{
		ACE_DEBUG((LM_DEBUG,"%s:%d***OpenALLSerial***Open ComPort %d Success !\n",__FILE__,__LINE__,SERIALNUM1));
	}
	ret = OpenComPort(SERIALNUM2, 9600, 8, "1", 'N');
	if (ret < 0) {
		ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d\n",SERIALNUM2));
		//return ;	
	}else{
		ACE_DEBUG((LM_DEBUG,"%s:%d***OpenALLSerial***Open Com Port %d Success!\n",__FILE__,__LINE__,SERIALNUM2));
	}
	ret = OpenComPort(SERIALNUM3, 57600, 8, "1", 'N');
	if (ret < 0) {
		ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d\n",SERIALNUM3));
		//return ;	
	}else{
		ACE_DEBUG((LM_DEBUG,"%s:%d***OpenALLSerial***Open Com Port %d Success!\n",__FILE__,__LINE__,SERIALNUM3));
	}
	/*ret = OpenComPort(SERIALNUM4, 9600, 8, "1", 'N');
	if (ret < 0) {
		ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d\n",SERIALNUM4));
		//return ;	
	}else{
		ACE_DEBUG((LM_DEBUG,"Open Com Port %d Success, Now going to read port\n",SERIALNUM4));
	}*/
	ret = OpenComPort(SERIALNUM5, 115200, 8, "1", 'N');
	if (ret < 0) {
		ACE_DEBUG((LM_DEBUG,"Error: Opening Com Port %d\n",SERIALNUM5));
		//return ;	
	}else{
		ACE_DEBUG((LM_DEBUG,"%s:%d***OpenALLSerial***Open Com Port %d Success!\n",__FILE__,__LINE__,SERIALNUM5));
	}
}

/**************************************************************
Function:       CSerialCtrl::GetSerialFd1
Description:    获取串口设备句柄
Input:          无              
Output:         无
Return:         串口设备句柄
***************************************************************/
int CSerialCtrl::GetSerialFd1()
{
	return m_iSerial1fd;

}
/**************************************************************
Function:       CSerialCtrl::GetSerialFd1
Description:    获取串口设备句柄
Input:          无              
Output:         无
Return:         串口设备句柄
***************************************************************/
int CSerialCtrl::GetSerialFd2()
{
	return m_iSerial2fd;

}

/**************************************************************
Function:       CSerialCtrl::GetSerialFd1
Description:    获取串口设备句柄
Input:          无              
Output:         无
Return:         串口设备句柄
***************************************************************/
int CSerialCtrl::GetSerialFd3()
{
	return m_iSerial3fd;

}
/**************************************************************
Function:       CSerialCtrl::GetSerialFd1
Description:    获取串口设备句柄
Input:          无              
Output:         无
Return:         串口设备句柄
***************************************************************/
int CSerialCtrl::GetSerialFd4()
{
	return m_iSerial4fd;

}
/**************************************************************
Function:       CSerialCtrl::GetSerialFd1
Description:    获取串口设备句柄
Input:          无              
Output:         无
Return:         串口设备句柄
***************************************************************/
int CSerialCtrl::GetSerialFd5()
{
	return m_iSerial5fd;

}




