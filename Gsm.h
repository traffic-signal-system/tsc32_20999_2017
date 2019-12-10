#ifndef _GSM_H_
#define _GSM_H_
#include "Define.h"
#include <time.h>
#include "SerialCtrl.h"
#include <iconv.h>



#define Snd_Flag   0
#define Recv_Flag  1

#define bit7 0
#define ucs2  1
typedef struct SMS_BOX_TYPE
{	
	char acPhone[20];
	char acTime[30];
	char Message[300];
	char Stat[2];
}SMS_T;


class CGsm
{
public:
	static CGsm* CreateInstance();
    void RunGsmData();
	bool GsmInit();
	bool GetString(int fd ,char *gsmstring);
	void PhoneNumFormat(char*buff, const char *tel_num, int flag);
	void DoCmt(char* smsmsg);
	void GetSmsRecvTime(char *smstime ,char *recvstr);
	int UCStoUTF(char *unicode , char *utf, Uint codetype);
	
	int UTFtoUCS(const char *utf , char *unicode);

	void SendSms( char* sendTelNum , char *sendMsgText) ;

	bool GetBit(Byte bitnum,Byte data) ;
	Byte SetBit(Byte bitnum,Byte data,bool bit) ;
	int Encode7bitPDU(char *src, char *output) ;
	void EnCode7Bit(char* pInByteStr, char* pOutByteStr);
	int Decode7bitPDU(char *from_7bits, char *to_utf8);
	int DeCode7Bit( char* pSrc, char* pDst, int nSrcLength);
	bool IfUnicode(const char *Text);
	void EncodeSMS(char * s_number,char * s_sms_text,int& sndPduNum,char * s_out_pdu) ;
	
	void OpenTcpConnec(char * IP ,char * port) ;
	void SendTcpData(char * senddata,Byte sendtype) ;
	void OPerateGprs(Byte dotype) ;
private:

	CGsm();
	~CGsm();
	bool bgsm ;
	int    m_iGsmFd;
	CSerialCtrl* pGsmSerial ;
	static char charMsg[500] ;
	bool bInit ;
	Byte iInitCount ;
	SMS_T Msg ;
	char incomecall[20];
};


#endif


