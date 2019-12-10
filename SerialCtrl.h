#ifndef _SERIALCTRL_H_
#define _SERIALCTRL_H_

#include "Define.h"




typedef int     INT32;
typedef short   INT16;
typedef char    INT8;
typedef unsigned int UNIT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;


#define SERIAL0  "/dev/ttyO0"    //串口0
#define SERIAL1  "/dev/ttyO1"    //串口1
#define SERIAL2  "/dev/ttyO2"    //串口2
#define SERIAL3  "/dev/ttyO3"    //串口3
#define SERIAL4  "/dev/ttyO4"	//485
#define SERIAL5  "/dev/ttyO5"    //485

#define SERIALNUM1  1
#define SERIALNUM2  2
#define SERIALNUM3  3
#define SERIALNUM4  4
#define SERIALNUM5  5


class CSerialCtrl
{
public:
	static CSerialCtrl* CreateInstance();

	void OpenALLSerial();
	int GetSerialFd1();
	int GetSerialFd2();
	int GetSerialFd3();
	int GetSerialFd4();
	int GetSerialFd5();

	INT32 OpenComPort (INT32 ComPort, INT32 baudrate, INT32 databit,const char *stopbit, char parity);
	void CloseComPort (void);
	INT32 ReadComPort (Byte *data, INT32 datalength);
	INT32 ReadComPortBySerial1(Byte *data, INT32 datalength);
	INT32 ReadComPortBySerial2(Byte *data, INT32 datalength);
	INT32 ReadComPortBySerial3(Byte *data, INT32 datalength);
	INT32 ReadComPortBySerial4(Byte *data, INT32 datalength);
	INT32 ReadComPortBySerial5(Byte *data, INT32 datalength);
	INT32 WriteComPort (Byte * data, INT32 datalength);
	INT32 WriteComPortBySerial1(Byte * data, INT32 datalength);
	INT32 WriteComPortBySerial2(Byte * data, INT32 datalength);
	INT32 WriteComPortBySerial3(Byte * data, INT32 datalength);
	INT32 WriteComPortBySerial4(Byte * data, INT32 datalength);
	INT32 WriteComPortBySerial5(Byte * data, INT32 datalength);
	/** 
	* export serial fd to other program to perform 
	* directly read, write to serial. 
	*  
	* @return serial's file description  */
		int getPortFd();
private:
	CSerialCtrl();
	~CSerialCtrl();

	//与GPS、GSM板通信
	INT32  m_iSerial1fd;
	INT32  m_iSerial2fd;
	//与备份PIC通信，内容包括备份灯色，手控.
	INT32  m_iSerial3fd;
	INT32  m_iSerial4fd;
	INT32  m_iSerial5fd;
};

#endif //_SERIALCTRL_H_

