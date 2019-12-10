#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"


class Configure 
{
public:
       int GetString(const char* szSection,const char* szKey,ACE_TString& strValue);
       int GetInteger(const char* szSection,const char* szKey,int& nValue);
       int open(const ACE_TCHAR * filename);
       int close();
	   static Configure* CreateInstance();
       bool InitConfig();
	   void ShowConfig();
private:
	Configure();
    ~Configure();
       ACE_Configuration_Section_Key root_key_;
       ACE_Ini_ImpExp * impExp_;
       ACE_Configuration_Heap config;
}; 

#endif  //_CONFIGURE_H_

