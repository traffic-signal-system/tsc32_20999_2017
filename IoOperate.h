#ifndef _IOOPERATE_H_
#define _IOOPERATE_H_

#include "ComStruct.h"

/*
*IO操作类 主要保证write和read的成功执行
*/
class CIoOprate
{
public:
	CIoOprate();
	~CIoOprate();

	static bool TscWrite(int iDevFd , Byte* pWriteData , int iWriteCnt);
	static bool TscRead(int iDevFd , Byte* pReadData , int iReadCnt );

};


#endif   //_IOOPERATE_H_
