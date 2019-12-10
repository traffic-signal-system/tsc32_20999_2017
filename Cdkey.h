#ifndef _CDKEY_H_
#define _CDKEY_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "ace/OS.h"
#include "Define.h"
#include "ace/Log_Msg.h" 



class Cdkey
{
private :
	

	Cdkey();
	~Cdkey();
public:
	static Cdkey* CreateInstance();
	static bool GetCdkey(char (&cdkey)[8]);
	void InitCdkey();
};



#endif  //_CDKEY_H_

