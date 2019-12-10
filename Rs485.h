#ifndef _CRS485_H_
#define _CRS485_H_

#include "ComStruct.h"
#define RS485_CPP "CRs485.cpp"

typedef int     INT32;
typedef short   INT16;
typedef char    INT8;
typedef unsigned int UNIT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

/*
*´®¿Ú1²Ù×÷Àà
*/
class CRs485
{
public:
	static CRs485* CreateInstance();
	void OpenRs485();
	bool Send(Byte* pBuffer, int iSize);
	bool Recvice(Byte* pBuffer , int iSize);
	void Reopen();
	INT32 OpenComPort(INT32 ComPort, INT32 baudrate, INT32 databit,const char *stopbit, char parity);
	void CloseComPort();
	int getPortFd();
	INT32 ReadComPort(Byte *data, INT32 datalength);
	INT32 WriteComPort(Byte * data, INT32 datalength);
	int SetOpt(int fd,int nSpeed,int nBits, char nEvent, int nStop);
	INT32 SetBaudrateType() ;
private:
	CRs485();
	~CRs485();

	INT32  m_iRs485Ctrlfd;
	int  m_iRs485Led;
};

#endif //_CRS485_H_
