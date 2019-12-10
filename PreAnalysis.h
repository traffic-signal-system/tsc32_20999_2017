#ifndef _PREANALYSIS_H_
#define _PREANALYSIS_H_

#include "ComDef.h"
#include "ComStruct.h"

class CPreAnalysis
{
public:
	static CPreAnalysis* CreateInstance();	
	bool b_SmoothTransition ; //ÊÇ·ñÆ½»¬¹ı¶É Ã¿´ÎÔö¼Ó¼õÉÙÊ±¼äÓĞÏŞÖÆ»¹ÊÇÖ±½Ó°´³µ³¤¶È¼ÆËãÊ±¼ä
	bool b_HandPriority   ; //ÊÇ·ñ´¦ÓÚ¹«½»ÓÅÏÈµ÷¿ØÖĞ
	Byte p_BusDelayTime   ; //Ä¬ÈÏ³µÁ¾Æô¶¯Ô¤ÁôÊ±¼ä
	Byte p_PerBusCrossTime ; //µ¥³µÍ¨¹ıÂ·¿ÚÆ½¾ùÊ±¼ä
	Byte p_AccessDeviceType ; //½ÓÈëÉè±¸ÀàĞÍ 0-ÏßÈ¦ 1-ÉäÆµ 2-À×´ï3-ÆäËû
	void QueryAccessDev(); //²éÑ¯½ÓÈëÉè±¸ĞÅÏ¢
	void HandPreAnalysis();
	void PreAnalysisExceptionHandle(); //Òì³£Çé¿ö´¦Àí£¬±ÈÈçÍ¨ĞÅÖĞ¶Ï¡
	void AcceptDevCarsInfo();
private:
	CPreAnalysis();
	~CPreAnalysis();
	SPreAnalysisParaData sPreAnalysisParaData[MAX_PHASE] ;
};

#endif 


