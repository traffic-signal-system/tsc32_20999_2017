
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   Rs485.cpp
Date:       2013-1-1
Description:Rs485串口处理文件
Version:    V1.0
History:
***************************************************************/
#include "Rs485.h"
#include "ManaKernel.h"
#include "SerialCtrl.h"
#include "IoOperate.h"
#include "MainBoardLed.h"

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

#define RS485_OTHER  "/dev/ttyO5"    //485其它功能输出
#define RS485        "/dev/ttyO4"   //用于倒计时输出


/**************************************************************
Function:       CRs485::CRs485
Description:    CRs485类构造函数，初始化串口
Input:          无                 
Output:         无
Return:         无
***************************************************************/
CRs485::CRs485()
{
	m_iRs485Led = LED_RS485_OFF;
	INT32 baudrate2Bxx = SetBaudrateType();	
	OpenComPort(4, baudrate2Bxx, 8, "1", 'N');
	ACE_DEBUG((LM_DEBUG,"%s:%d Init RS485 object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CRs485::~CRs485
Description:    CRs485类析构函数，释放类对象资源
Input:          无               
Output:         无
Return:         无
***************************************************************/
CRs485::~CRs485()
{
	//
	CloseComPort();
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct Rs485 object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CRs485::CreateInstance
Description:    创建	CRs485静态对象
Input:          无              
Output:         无
Return:         静态对象指针
***************************************************************/
CRs485* CRs485::CreateInstance()
{
	static CRs485 oRs485;

	return &oRs485;
}

/**************************************************************
Function:       CRs485::OpenRs485
Description:    打开R485串口
Input:          无              
Output:         无
Return:         静态对象指针
***************************************************************/
INT32 CRs485::OpenComPort(INT32 ComPort, INT32 baudrate, INT32 databit,const char *stopbit, char parity)
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
		case 4:        
			pComPort = (char *)"/dev/ttyO4";        
			break;    
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
	printf("%s:%d SerialComport fd = %d\n",__FILE__,__LINE__, fd);    
	tcgetattr (fd, &termios_old);       /* save old termios value */   
	/* 0 on success, -1 on failure */    
	retval = set_port_attr (baudrate, databit, stopbit, parity);    
	if (-1 == retval) 
	{        
		fprintf (stderr, "\nport %s cannot set baudrate at %d\n", pComPort,baudrate);    
	} 
	
	return (retval);
}


/* close serial port by use of file descriptor fd */
void CRs485::CloseComPort()
{    
/* flush output data before close and restore old attribute */    
	tcsetattr (fd, TCSADRAIN, &termios_old);    
	close (fd);
}
int CRs485::getPortFd()
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
INT32 CRs485::ReadComPort (Byte *data, INT32 datalength)
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


/*  
* Write datalength bytes in buffer given by UINT8 *data, 
* return value: bytes written 
* Nonblock mode
*/
INT32 CRs485::WriteComPort (Byte * data, INT32 datalength)
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
			ACE_DEBUG((LM_DEBUG,"%s:%d Dtcflush !\n",__FILE__,__LINE__));
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
/*
*往485发送数据
*/
bool CRs485::Send(Byte* pBuffer, int iSize)
{

	//ACE_DEBUG((LM_DEBUG,"%s:%d  CRs485::Send %x %x %x %x %x  size = %d\n",__FILE__,__LINE__,pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3],pBuffer[4],iSize));
	/*
	if ( ( (pTscCfg->sSpecFun[FUN_PRINT_FLAGII].ucValue) & 1 ) != 0 )
	{
		Byte ucIndex = 0;
		ACE_DEBUG((LM_DEBUG,"%s:%d send %d Bytes to rs485: ",RS485_CPP, __LINE__, iSize));
		while ( ucIndex < iSize )
		{
			ACE_DEBUG((LM_DEBUG,"%02x  ",pBuffer[ucIndex]));
			ucIndex++;
		}
		ACE_DEBUG((LM_DEBUG,"\n") );
	}
	*/
	
	WriteComPort(pBuffer,iSize);
	//CSerialCtrl::CreateInstance()->WriteComPortBySerial4(pBuffer, iSize);
	//INT32 fd4 = CSerialCtrl::CreateInstance()->GetSerialFd4();
	//SetOpt(fd4, 9600, 8, 'N', 1);
	/*if ( !CIoOprate::TscWrite(fd4, pBuffer, iSize) )	 
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d write error\n",__FILE__,__LINE__));
		return false;
	}*/
	//CRs485::CreateInstance()->WriteComPort(pBuffer, iSize);

	if ( LED_RS485_ON == m_iRs485Led  )
	{
		m_iRs485Led	= LED_RS485_OFF;
	}
	else
	{
		m_iRs485Led	= LED_RS485_ON;
	}
//	CMainBoardLed::CreateInstance()->OperateLed(m_iRs485Led);

	return true;

}

/*
*从485接收数据
*/
bool CRs485::Recvice(Byte* pBuffer , int iSize)
{
	

	return true;
}

void CRs485::Reopen()
{

}

// fd: device handle;
// nSpeed: baudrate
// nBits: data bit;
// nEvent: parity;
// nStop: stop bit;
/*
*设置设备参数
*
*input: fd-设备id        nSpeed-波特率 nBits-数据位
*       nEvent-奇偶校验位 nStop-停止位
*/
int CRs485::SetOpt(int fd,int nSpeed,int nBits, char nEvent, int nStop)
{
#ifndef WINDOWS
	struct termios newtio,oldtio;

	if(tcgetattr(fd,&oldtio) != 0)
	{
		printf("SetupSerial 1\n");
		return -1;
	}

	bzero(&newtio,sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch(nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;

	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch(nEvent)
	{
	case 'o':
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;

	case 'e':
	case 'E':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;

	case 'n':
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch(nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio,B2400);
		cfsetospeed(&newtio,B2400);
		break;

	case 4800:
		cfsetispeed(&newtio,B4800);
		cfsetospeed(&newtio,B4800);
		break;

	case 9600:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;

	case 115200:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;

	default:
		cfsetispeed(&newtio,B19200);
		cfsetospeed(&newtio,B19200);
		break;
	}


	if(nStop == 2)
	{
		newtio.c_cflag |= CSTOPB;
	}
	else
	{
		newtio.c_cflag &= ~CSTOPB;
	}

	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	tcflush(fd,TCIFLUSH);

	if((tcsetattr(fd,TCSANOW, &newtio)) != 0)
	{
		printf("SetupSerial 2\n");
		return -1;
	}
#endif

	return 0;
}

/**************************************************************
Function:       CRs485::SetBaudrateType
Description:    设置485总线的波特率
Input:          无              
Output:        无
Return:         波特率值
Date:           20150-01-29
***************************************************************/

INT32 CRs485::SetBaudrateType()
{
	INT32 baudrate2Bxx = 9600 ;
	Byte baudtype =  CManaKernel::CreateInstance()->m_pTscConfig->sSpecFun[FUN_RS485_BITRATE].ucValue ;
	switch(baudtype)
	{
		case 0:
			baudrate2Bxx = 9600 ;
			break ;
		case 1:
			baudrate2Bxx = 2400 ;
			break ;
		case 2:
			baudrate2Bxx = 4800 ;
			break ;
		case 3:
			baudrate2Bxx = 19200 ;
			break ;
		case 4:
			baudrate2Bxx = 38400 ;
			break ;
		default:
			baudrate2Bxx = 9600 ;
			break ;		
		
	}
	return baudrate2Bxx ;

}


