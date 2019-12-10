/*================================================================================================== 
 * 项目名称: 读取配置文件库 
 *     功能: 提供类和方法,实现从配置文件读取参数值 
 *     作者: 鲁仁华 
 *     联系: renhualu@live.com 
 * 最近修改: 2013-4-15 
 *     版本: v1.0.2 
  ==================================================================================================*/  
  
#include <sys/timeb.h>  
#include <string>  
#include <iostream>  
#include <map>  
#include <ace/OS.h>  
#include "Configure.h"
using namespace std;

/**************************************************************
Function:       CreateInstance
Description:    单例模式创建对象
Input:          无           
Output:         无
Return:         Configure对象
***************************************************************/
Configure* Configure::CreateInstance()
{
	static Configure con;
	return &con;
}
/**************************************************************
Function:       Configure
Description:    构造函数，创建配置文件对象及配置文件
Input:          无           
Output:         无
Return:         无
***************************************************************/
Configure::Configure():impExp_(NULL)
{
    bool binitfile = InitConfig() ;
 	if(!binitfile)
	{
		ACE_DEBUG((LM_DEBUG,"%s:%d Init configure file fail !\n",__FILE__,__LINE__));
			 
	}
    if(open("tsc.ini") == -1)
 	{
		ACE_OS::printf("\r\n%s:%d ***Configure***Open tsc.ini configure file error!\r\n",__FILE__,__LINE__);
 	}  
}
 /**************************************************************
Function:       ~Configure
Description:    析构函数，删除配置文件对象。
Input:          无           
Output:         无
Return:         无
***************************************************************/
Configure::~Configure()
{
       if (impExp_)
              delete impExp_;
       impExp_ = NULL;
}
/**************************************************************
Function:       open
Description:    打开配置文件
Input:           filename 配置文件名称          
Output:         无
Return:         IO
***************************************************************/
int Configure::open(const ACE_TCHAR * filename)
{
       if (this->config.open() == -1)
              return -1;
      
       this->impExp_=new ACE_Ini_ImpExp(config);
 
       return this->impExp_->import_config(filename);
}
/**************************************************************
Function:       GetString
Description:    从配置文件读取字符串类型的值
Input:          szSection 	区域 [xxxx]
				szKey		key字符串
Output:         strValue	相对key 值的    内容 
Return:         1 成功	0 失败
***************************************************************/
int Configure::GetString(const char *szSection, const char *szKey, ACE_TString &strValue)
{
       if (config.open_section(config.root_section(),ACE_TEXT(szSection),0,this->root_key_)==-1)
              return -1;
       return config.get_string_value(this->root_key_,szKey,strValue);
}
/**************************************************************
Function:       GetInteger
Description:    从配置文件读取数字类型的值
Input:          szSection 	区域 [xxxx]
				szKey		key字符串
Output:         nValue		相对key 值的    内容 
Return:         1 成功	0 失败
***************************************************************/
int Configure::GetInteger(const char* szSection,const char* szKey,int& nValue)
{
       ACE_TString strValue;
       if (config.open_section(config.root_section(),ACE_TEXT(szSection),0,this->root_key_)==-1)
              return -1;
       if (config.get_string_value(this->root_key_,szKey,strValue) == -1)
              return -1;
       nValue = ACE_OS::atoi(strValue.c_str());
       if (nValue == 0 && strValue != "0")

              return -1;
       return nValue;
}
/**************************************************************
Function:       InitConfig
Description:    对配置文件进行初始化
Input:          无
Output:         无
Return:         1 成功	0 失败
***************************************************************/
bool Configure::InitConfig()
{
	FILE* fConfig = NULL ;
	if ( (fConfig = ACE_OS::fopen(ACE_TEXT("tsc.ini"), "r")) == NULL )
	{	
		if((fConfig = ACE_OS::fopen(ACE_TEXT("tsc.ini"), "w+")) == NULL)
			return false ;
		ACE_OS::fputs("\n#Traffic Signal Control Configure",fConfig);
		ACE_OS::fputs("\n[APPDESCRIP]",fConfig);
		ACE_OS::fputs("\napplication   =Gb.aiton",fConfig);
		ACE_OS::fputs("\ndatebase      =GbAitonTsc.db",fConfig);
		ACE_OS::fputs("\nversion       = 3.0.1",fConfig);
		ACE_OS::fputs("\nIDCode   = YAJT8417060001",fConfig);
		ACE_OS::fputs("\nIdentifyCode   = ABCDE",fConfig);
		ACE_OS::fputs("\ndescription   = 32 Phase Traffic Singal Controner",fConfig);
		
		ACE_OS::fputs("\n[COMMUNICATION]",fConfig);
		ACE_OS::fputs("\nstandard      =GBT25280",fConfig);
		ACE_OS::fputs("\nprotocol      =udp",fConfig);
		ACE_OS::fputs("\nport          =5435 ",fConfig);
                ACE_OS::fputs("\nGB25280ServerIp    = 192.168.1.2",fConfig);

		ACE_OS::fputs("\n[Location]",fConfig);
		ACE_OS::fputs("\nAreaCode       =1",fConfig);
		ACE_OS::fputs("\nCrossCode       =1",fConfig);
		
		ACE_OS::fputs("\n[CONTACT]",fConfig);
		ACE_OS::fputs("\ncompany       =AiTon",fConfig);
		ACE_OS::fputs("\nlinkman       =Mr Lu",fConfig);
		ACE_OS::fputs("\ntelephone     = 00000000000",fConfig);
		ACE_OS::fputs("\naddress       = China",fConfig);
		ACE_OS::fputs("\nWebSite       = http:	",fConfig);
		
		
		ACE_OS::fputs("\n[FUNCTION]",fConfig);
		ACE_OS::fputs("\nBACKUP       = 1	",fConfig);		
		ACE_OS::fclose(fConfig);
		return true ;
	}
	ACE_OS::fclose(fConfig);
	return true ;
}
 /**************************************************************
Function:       ShowConfig
Description:    显示配置文件中的内容
Input:          无
Output:         无
Return:         无
***************************************************************/
void Configure::ShowConfig()
{
  ACE_TString vstring ;
 cout<<endl<<"#..................Show Tsc Configure....................# "<<endl ;
 GetString("APPDESCRIP","application",vstring);
	cout<<"AppName :" <<vstring.c_str()<<endl ;
 GetString("APPDESCRIP","datebase",vstring);
	cout<<"DataBaseName :" <<vstring.c_str()<<endl ;	
 GetString("APPDESCRIP","version",vstring);
	cout<<"Version :" <<vstring.c_str()<<endl ;
 GetString("APPDESCRIP","IDCode",vstring);
	cout<<"IDCode :" <<vstring.c_str()<<endl ;	
GetString("APPDESCRIP","IdentifyCode",vstring);
	cout<<"IdentifyCode :" <<vstring.c_str()<<endl ;	
 GetString("APPDESCRIP","description",vstring);
	cout<<"Description :" <<vstring.c_str()<<endl ;

 GetString("COMMUNICATION","standard",vstring);
	cout<<"Standard :" <<vstring.c_str()<<endl ;
 GetString("COMMUNICATION","protocol",vstring);
	cout<<"Protocol :" <<vstring.c_str()<<endl ;	
 GetString("COMMUNICATION","port",vstring);
	cout<<"Portnumber :" <<vstring.c_str()<<endl ;
GetString("COMMUNICATION","ClientIp",vstring);
		cout<<"TscClientIp :" <<vstring.c_str()<<endl ;

 GetString("Location","AreaCode",vstring);
	cout<<"AreaCode :" <<vstring.c_str()<<endl ;
GetString("Location","CrossCode",vstring);
	cout<<"CrossCode :" <<vstring.c_str()<<endl ;	

 GetString("CONTACT","company",vstring);
	cout<<"Company :" <<vstring.c_str()<<endl ;
GetString("CONTACT","linkman",vstring);
	cout<<"LinkMan :" <<vstring.c_str()<<endl ;
GetString("CONTACT","telephone",vstring);
	cout<<"Telephone :" <<vstring.c_str()<<endl ;
GetString("CONTACT","address",vstring);
	cout<<"Address :" <<vstring.c_str()<<endl ;
GetString("CONTACT","WebSite",vstring);
	cout<<"WebSite :" <<vstring.c_str()<<endl ;
	
GetString("FUNCTION","BACKUP",vstring);
	cout<<"FUNCTION :" <<vstring.c_str()<<endl ;
cout<<"#................End Show Tsc Configure..................# "<<endl<<endl;
}

 /**************************************************************
Function:       close
Description:    关闭文件
Input:          无
Output:         无
Return:         1 成功	0 失败
***************************************************************/
int Configure::close()
{
       if (impExp_)
       {
              delete impExp_;
              impExp_ = NULL;
       }
       return 0;
} 
