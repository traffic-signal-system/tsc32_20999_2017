 /***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   LampBoard.cpp
Date:       2013-4-13
Description:ä¿¡å·æœºç¯æ‰‹æŽ§æ“ä½œå¤„ç†ç±»æ–‡ä»¶.åŒ…å«å¯¹æ‰‹æŽ§é¢æ¿å…¨çº¢ï¼Œé»„é—ªï¼Œæ­¥è¿›,æ‰‹åŠ¨è‡ªåŠ¨åˆ‡æ¢æŒ‰é’®æ“ä½œçš„å¤„ç†ã€‚
Version:    V1.0
History:    2013.05.29 ä¿®æ”¹æŒ‰é’®æŒ‰ä¸‹å’Œæ¾å¼€çš„å¤„ç†ã€‚
		  2013.05.31 æ·»åŠ å½“æ‰‹åŠ¨çŠ¶æ€ç»´æŠ¤5åˆ†é’Ÿä»¥ä¸Šæ—¶è‡ªåŠ¨åˆ‡æ¢ä¸ºè‡ªåŠ¨çŠ¶æ€.ã€‚
		  2013.09.10 ä¿®æ”¹æŒ‰é’®æŽ¥æ”¶å¤„ç†æ–¹å¼ï¼Œç®€åŒ–ä»£ç ã€‚
		  2014.10.06 ä¿®æ”¹æŒ‰é’®æŽ¥å—å¤„ç†æ–¹å¼ï¼Œç”±ä½ç§»å¾ªçŽ¯åˆ¤æ–­ä¿®æ”¹ä½è¯»å–é”®å€¼è¿›è¡Œåˆ¤æ–­
		  2014.10.23 ¸ÄÔì³ÉÖ§³ÖÎÞÏß°´¼üÊÖ¿Ø¿ØÖÆ
********************************************************************************************/
#include "WirelessButtons.h"
#include "TscMsgQueue.h"
#include "ace/Date_Time.h"
#include "MainBoardLed.h"
#include "TimerManager.h"
#include "GbtMsgQueue.h"
#include "Can.h"
#include "LampBoard.h"
#include "ComFunc.h"

/***************µ±Ç°ÎÞÏßÊÖ¿ØËù´¦×´Ì¬****************************************/
enum{
	 BUTTON_MANUAL_SELF 		= 0x0,	//×ÔÖ÷ÔËÐÐ
	 BUTTON_MANUAL_MANUAL 		= 0x1,	//ÊÖ¶¯ÔËÐÐ	
	 BUTTON_MANUAL_YELLOW_FLASH	= 0x2,	//»ÆÉÁ
	 BUTTON_MANUAL_ALL_RED		= 0x3,	//È«ºì
	 BUTTON_MANUAL_NEXT_STEP	= 0x4,	//ÏÂÒ»²½
	 BUTTON_MANUAL_NEXT_PHASE	= 0x5,	//ÏÂÒ»ÏàÎ»
	 BUTTON_MANUAL_NEXT_DIREC	= 0x6,	//ÏÂÒ»·½Ïò
	 BUTTON_SPECIAL_DIREC		= 0x7	//°´ÌØÊâ×éºÏÖ¸¶¨·½ÏòºÍÏàÎ»³µµÀ·½ÐÎ
	
};


/***************µ±Ç°·ÅÐÐµÆ×´Ì¬****************************************/
enum{

	LAMP_OFF = 0x0 ,         //Ï¨Ãð
	LAMP_GreenOn = 0x1 ,     //ÂÌµÆÁÁ
	LAMP_GreenFLASH = 0x2 ,  //ÂÌÉÁ
	LAMP_YELLOW = 0x3 ,      //»ÆµÆÁÁ
	LAMP_YELLOWFLASH = 0x4 , //»ÆÉÁ
	LAMP_RED = 0x5           //ºìµÆÁÁ
};

/**************************************************************
Function:       CWirelessBtn::CWirelessBtn
Description:    CWirelessBtnç±»æž„é€ å‡½æ•°ï¼Œç”¨äºŽç±»åˆå§‹åŒ–å¤„ç†				
Input:          æ—               
Output:         æ— 
Return:         æ— 
***************************************************************/
CWirelessBtn::CWirelessBtn() 
{	
	m_ucLastManualSts = BUTTON_MANUAL_SELF; //³õÊ¼»¯´¦ÓÚ×Ô¶¯ÔËÐÐ×´Ì¬	
	m_ucdirectype = 0x0 ; 					//·½ÏòÀàÐÍ 0-±±·½ 1- ¶«·½ 2- ÄÏ·½ 3-Î÷·½	
	bFirstOperate = true ;
	bTransitSetp  = false ;
	RecvoldDirec = 0 ;
	bHandleWirelessBtbMsg = false ;
	pManaKernel = CManaKernel::CreateInstance();
	m_pWirelessBtnTimer = CWirelessBtnTimer::CreateInstance();
	m_tActiveWirelessBtnTimer.timer_queue()->gettimeofday(getCurrTime);
	m_tActiveWirelessBtnTimer.activate();
}

/**************************************************************
Function:       CWirelessBtn::~CWirelessBtn
Description:    CWirelessBtnç±»	æžæž„å‡½æ•°	
Input:          æ—               
Output:         æ— 
Return:         æ— 
***************************************************************/
CWirelessBtn::~CWirelessBtn() 
{
	;
}

/**************************************************************
Function:       Manual::CreateInstance
Description:    åˆ›å»º	Manualé™æ€å¯¹è±¡
Input:          æ—               
Output:         æ— 
Return:         é™æ€å¯¹è±¡æŒ‡é’ˆ
***************************************************************/
CWirelessBtn* CWirelessBtn::CreateInstance()
{
	static CWirelessBtn cWirelessBtn;
	return &cWirelessBtn;
}

/****************************************************************
Function:       CWirelessBtn::EntryWirelessManul
Description:    ÍË³öÎÞÏßÒ£¿Ø°´¼ü£¬»Ö¸´×ÔÖ÷¿ØÖÆ
Input:           ÎÞ          
Output:         ÎÞ
Return:         ÎÞ
Date:           20141023
*****************************************************************/

void CWirelessBtn::BackToAuto()
{
	if(m_ucLastManualSts==BUTTON_MANUAL_NEXT_DIREC || m_ucLastManualSts==BUTTON_SPECIAL_DIREC) //ÌØÊâ·½Ïò×Ô¶¯·µ»ØµÄÊ±ºòÓÐ¹ý¶É²½
	{		
			bTransitSetp = true ;	
			pManaKernel->SetWirelessBtnDirecCfg(RecvoldDirec,0x2);						
			ACE_OS::sleep(2);
			pManaKernel->SetWirelessBtnDirecCfg(RecvoldDirec,0x1);
			ACE_OS::sleep(3);		
			bTransitSetp = false ;
		
	}	
	else if(m_ucLastManualSts==BUTTON_MANUAL_NEXT_STEP || m_ucLastManualSts==BUTTON_MANUAL_MANUAL) //µ±´ÓÊÖ¶¯ÏÂÒ»²½ÍË³ö Ôò»ÆµÆ¹ý¶É3Ãë
	{
		
		bTransitSetp = true ;
		SetCurrenStepFlash(0x1);		
		ACE_OS::sleep(3);
		bTransitSetp = false ;
	}
	CMainBoardLed::CreateInstance()->DoAutoLed(true);
	CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_SWITCH_SYSTEMCONTROL,0);	
	pManaKernel->SetCycleStepInfo(0); //µ¥µ¥¹¹ÔìÕû¸öÖÜÆÚµÄ²½·¥ÐÅÏ¢¼´¿É
	
	CLampBoard::CreateInstance()->SetLamp(pManaKernel->m_pRunData->sStageStepInfo[pManaKernel->m_pRunData->ucStepNo].ucLampOn,pManaKernel->m_pRunData->sStageStepInfo[pManaKernel->m_pRunData->ucStepNo].ucLampFlash); //ÍË³öÀ´µÄÊ±ºòÖØÐÂ¸øµÆ¿Ø°åµÆÉ«ÐÅÏ¢
	bFirstOperate = true ;
	m_ucLastManualSts = BUTTON_MANUAL_SELF;
	pManaKernel->m_pRunData->ucManualType = Manual_CTRL_NO;
	CMainBoardLed::CreateInstance()->SetSignalLed(LED_WIRELESSBUTTON,LED_STATUS_ON);
	
	ACE_DEBUG((LM_DEBUG,"%s:%d Back to Auto-Ctrl from WirelessButtons! \n",__FILE__,__LINE__));
}



/****************************************************************
*
Function:       CWirelessBtn::EntryWirelessManul
Description:    ³õ´Î½øÈëÎÞÏß°´¼üÊÖ¶¯×´Ì¬
Input:           ÎÞ          
Output:         ÎÞ
Return:         ÎÞ
Date:           20141023
*****************************************************************/

void CWirelessBtn::EntryWirelessManul()
{
	CMainBoardLed::CreateInstance()->DoAutoLed(false);					
	CGbtMsgQueue::CreateInstance()->SendTscCommand(OBJECT_CURTSC_CTRL,1);
	ACE_DEBUG((LM_DEBUG,"%s:%d First Entry  WirelessBtnManual TscMsg! \n",__FILE__,__LINE__));
	bTransitSetp = true ;
	if(!SetCurrenStepFlash(0x2)) //µ±Ç°·½ÏòÏàÎ»Èç¹ûÊÇÂÌµÆÔòÂÌÉÁ
	//Èç¹ûµ±Ç°ÊÇ·ÇÂÌµÆ×´Ì¬£¬ÂÌÉÁ²Ù×÷ÎÞÐ§£¬Ôò½øÈëÂÌÉÁ»ÆµÆ½øÈëÂÌÉÁ²Ù×÷ÅÐ¶Ï
	{
		SetCurrenStepFlash(0x3); //µ±Ç°·½ÏòÏàÎ»»ÆµÆÔò»ÆÉÁ
	}
	bTransitSetp = false ;
	RecvoldDirec = 0 ;	
	m_ucLastManualSts = BUTTON_MANUAL_MANUAL;
	pManaKernel->m_pRunData->ucManualType = Manual_CTRL_WIRELESSBUTTONS;	
	CMainBoardLed::CreateInstance()->SetSignalLed(LED_WIRELESSBUTTON,LED_STATUS_FLASH);

}

/****************************************************************
*
Function:       CWirelessBtn::RecvMacCan
Description:    ¶Ô´ÓÒ£¿ØÆ÷·µ»ØÖ÷°åµÄ°´¼üCANÊý¾Ý½øÐÐ´¦Àí
Input:            Can×ÜÏß½ÓÊÕµ½µÄÎÞÏß°´¼üÊý¾ÝÖ¡            
Output:         ÎÞ
Return:         ÎÞ
Date:           20141023
*****************************************************************/
void CWirelessBtn::RecvMacCan(SCanFrame sRecvCanTmp)
{
	if(pManaKernel->m_pRunData->ucManualType != Manual_CTRL_NO && pManaKernel->m_pRunData->ucManualType != Manual_CTRL_WIRELESSBUTTONS)
		return ; 
	Byte RecvType = sRecvCanTmp.pCanData[0] & 0x3F ;
	if(RecvType == 0x2 && bTransitSetp == false) //RecvType == 0x2 ±íÊ¾½ÓÊÕ°´¼üÐÅÏ¢,ÊÖ¿Ø·Ç¹ý¶É²½
	{
		sWirelessBtnCanMsg = sRecvCanTmp ;
		bHandleWirelessBtbMsg = true ;    //ÎÞÏßÊÖ¿ØÏß³Ì¿ÉÒÔ´¦Àí°´¼üÐÅÏ¢
	}
	else
		return ;
}


/**************************************************************
Function:       CWirelessBtn::HandWirelessBtnMsg
Description:    Ïß³Ì´¦ÀíÎÞÏßÊÖ¿Ø×éºÏ·½ÏòµÄ·ÅÐÐµÆÉ«
Input:           ÎÞ             
Output:         ÎÞ
Return:          ÎÞ
Date:            20141117
***************************************************************/
void CWirelessBtn::HandWirelessBtnMsg() //ADD:201411171521
{		
		bHandleWirelessBtbMsg = false ;
		CGbtMsgQueue *pGbtMsgQueue = CGbtMsgQueue::CreateInstance();
		Byte RecvButtonType = sWirelessBtnCanMsg.pCanData[1] ;
		Uint DirecButtonCfg = ((sWirelessBtnCanMsg.pCanData[2]|sWirelessBtnCanMsg.pCanData[3]<<8)|sWirelessBtnCanMsg.pCanData[4]<<16)|sWirelessBtnCanMsg.pCanData[5]<<24;
		Byte iAuto_Manul = RecvButtonType&0x1 ;	
		if(iAuto_Manul == 0x0) //×Ô¶¯×´Ì¬
		{
			if(m_ucLastManualSts == BUTTON_MANUAL_SELF) //Èç¹ûÉÏ´Î×´Ì¬ÒÑ¾­ÊÇ×ÔÖ÷ÔËÐÐ
				return ;
			else
			{
				//TscBeep();
				BackToAuto();
				CloseWirelessBtn();								
				m_pWirelessBtnTimer->Resettimeout();  //ÍË³ö¶¨Ê±Æ÷³¬Ê±¼ÆÊýÖÃÎª0
			}
		}
		else if(iAuto_Manul == 0x1) //ÊÖ¿Ø×´Ì¬
		{				
			if(m_ucLastManualSts == BUTTON_MANUAL_SELF) //Èç¹ûÉÏ´Î×´Ì¬ÊÇ×ÔÖ÷ÔËÐÐ
			{
				//TscBeep();
				EntryWirelessManul();
				StartWirelessBtnTimer();				
			}		
			else
			{									
				CWirelessBtnTimer::CreateInstance()->Resettimeout();  //ÓÐÔÚ½øÐÐÎÞÏßÊÖ¿Ø²Ù×÷Ôò£¬³¬Ê±¼ÆÊýÖÃÎª0
				if((RecvButtonType>>1)&0x1)  //½øÈë»ÆÉÁ¿ØÖÆ
				{
					if(m_ucLastManualSts == BUTTON_MANUAL_YELLOW_FLASH)
					{
						return ;
					}
					else
					{
						//TscBeep();
						pGbtMsgQueue->SendTscCommand(OBJECT_SWITCH_SYSTEMCONTROL,254);						
						pManaKernel->SndMsgLog(LOG_TYPE_MANUAL,2,0,0,0); //ADD:20141031
						ACE_DEBUG((LM_DEBUG,"%s:%d Send WirelessBtn FLASH! TscMsg!\n",__FILE__,__LINE__));
						m_ucLastManualSts = BUTTON_MANUAL_YELLOW_FLASH;
						pManaKernel->m_pRunData->flashType = CTRLBOARD_FLASH_MANUALCTRL;
						bFirstOperate = true ;
					}
				}
				else if((RecvButtonType>>2)&0x1)//½øÈëÈ«ºì¿ØÖÆ
				{
					if (m_ucLastManualSts == BUTTON_MANUAL_ALL_RED)
					{
						return ;
					}
					else
					{
						//TscBeep();
						pGbtMsgQueue->SendTscCommand(OBJECT_SWITCH_SYSTEMCONTROL,253);
						pManaKernel->SndMsgLog(LOG_TYPE_MANUAL,4,0,0,0); //ADD:20141031
						ACE_DEBUG((LM_DEBUG,"%s:%d Send WirelessBtn  ALLRED TscMsg!\n",__FILE__,__LINE__));
						m_ucLastManualSts = BUTTON_MANUAL_ALL_RED;
						bFirstOperate = true ;
					}
				}
				else if((RecvButtonType>>3)&0x1)//µã»÷ÏÂÒ»²½°´¼ü
				{
					if(m_ucLastManualSts == BUTTON_MANUAL_MANUAL || m_ucLastManualSts == BUTTON_MANUAL_NEXT_STEP)  //ÏÂÒ»²½Ö»ÄÜÓÉÊÖ¿ØºóÖ±½Ó½øÈë£¬ÆäËû²Ù×÷ºóÎÞ·¨Ö±½Ó½øÈë
					{
						//TscBeep();
						pGbtMsgQueue->SendTscCommand(OBJECT_GOSTEP,0);
						ACE_DEBUG((LM_DEBUG,"%s:%d Send WirelessBtn Next Step TscMsg ! \n",__FILE__,__LINE__));
						pManaKernel->SndMsgLog(LOG_TYPE_MANUAL,6,0,0,0);
						m_ucLastManualSts = BUTTON_MANUAL_NEXT_STEP;
						bFirstOperate = true ;						
					}
				}
				else if((RecvButtonType>>4)&0x1) //ÏÂÒ»ÏàÎ»
				{
					if(m_ucLastManualSts == BUTTON_MANUAL_MANUAL || m_ucLastManualSts == BUTTON_MANUAL_NEXT_PHASE)  //ÏÂÒ»²½Ö»ÄÜÓÉÊÖ¿ØºóÖ±½Ó½øÈë£¬ÆäËû²Ù×÷ºóÎÞ·¨Ö±½Ó½øÈë
					{
						//TscBeep();
						pGbtMsgQueue->SendTscCommand(OBJECT_SWITCH_STAGE,0);
						ACE_DEBUG((LM_DEBUG,"%s:%d Send WirelessBtn Next Phase TscMsg !\n",__FILE__,__LINE__));
						if(m_ucLastManualSts != BUTTON_MANUAL_NEXT_PHASE)
						m_ucLastManualSts = BUTTON_MANUAL_NEXT_PHASE;	
						bFirstOperate = true ;						
					}
				}
				else if((RecvButtonType>>5)&0x1)//ÏÂÒ»·½Ïò
				{
					//TscBeep();
					if(m_ucLastManualSts == BUTTON_MANUAL_MANUAL && m_ucAllowLampStatus == LAMP_GreenFLASH) 
					{
						
						SetCurrenStepFlash(0x1); //Èç¹ûÊÖ¶¯½øÈëÊÇÂÌÉÁÔò»ÆµÆ¹ý¶É
						ACE_OS::sleep(3);
					}
					else if(m_ucLastManualSts == BUTTON_MANUAL_NEXT_STEP)
					{
						
						bTransitSetp = true ;	
						SetCurrenStepFlash(0x2); //µ±Ç°·½ÏòÏàÎ»ÂÌÉÁË	
						ACE_OS::sleep(2);	
						SetCurrenStepFlash(0x1); //µ±Ç°·½ÏòÏàÎ»»ÆµÆ
						bTransitSetp = false ;
					}
					SThreadMsg sTscMsg ;
					sTscMsg.ulType       = TSC_MSG_MANUALBUTTON_HANDLE; 
					sTscMsg.ucMsgOpt     = 0;
					sTscMsg.uiMsgDataLen = 0;			
					sTscMsg.pDataBuf     = ACE_OS::malloc(4);
					ACE_OS::memset((Byte*)sTscMsg.pDataBuf,0x0,4);
					((Byte*)sTscMsg.pDataBuf)[(m_ucdirectype++)%4] = 0x7 ;
					CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));	
					ACE_DEBUG((LM_DEBUG,"%s:%d Send Next WirelessBtn DIRE=%d !\n",__FILE__,__LINE__,m_ucdirectype-1));
					if(m_ucLastManualSts != BUTTON_MANUAL_NEXT_DIREC)
						m_ucLastManualSts = BUTTON_MANUAL_NEXT_DIREC;
					pManaKernel->m_pRunData->uiWorkStatus = STANDARD ;					
				}
				else if(DirecButtonCfg != 0x0)
				{
					//TscBeep();
					if(m_ucLastManualSts == BUTTON_MANUAL_MANUAL && m_ucAllowLampStatus == LAMP_GreenFLASH) 
					{
						SetCurrenStepFlash(0x1); //Èç¹ûÊÖ¶¯½øÈëÊÇÂÌÉÁÔò»ÆµÆ¹ý¶É
						ACE_OS::sleep(3);
					}
					else if(m_ucLastManualSts == BUTTON_MANUAL_NEXT_STEP)
					{
						bTransitSetp = true ;	
						SetCurrenStepFlash(0x2); //µ±Ç°·½ÏòÏàÎ»ÂÌÉÁË	
						ACE_OS::sleep(2);	
						SetCurrenStepFlash(0x1); //µ±Ç°·½ÏòÏàÎ»»ÆµÆ
						bTransitSetp = false ;
					}
					SThreadMsg sTscMsg ;
					sTscMsg.ulType       = TSC_MSG_MANUALBUTTON_HANDLE; 
					sTscMsg.ucMsgOpt     = 0;
					sTscMsg.uiMsgDataLen = 4;			
					sTscMsg.pDataBuf     = ACE_OS::malloc(4);
					ACE_OS::memcpy((Byte*)sTscMsg.pDataBuf,&DirecButtonCfg,4);
					CTscMsgQueue::CreateInstance()->SendMessage(&sTscMsg,sizeof(sTscMsg));
					if(m_ucLastManualSts != BUTTON_SPECIAL_DIREC)
						m_ucLastManualSts = BUTTON_SPECIAL_DIREC;
					pManaKernel->m_pRunData->uiWorkStatus = STANDARD ; //±ÜÃâÎÞ·¨ÔÙ´Î½øÈë»ÆÉÁ»òÈ«ºì					
				}		
			}
		}
	
	
}

/**************************************************************
Function:       CWirelessBtn::GetbHandleWirelessBtnMsg
Description:    ´¦ÀíÎÞÏßÊÖ¿Ø×éºÏ·½ÏòµÄ·ÅÐÐµÆÉ«
Input:           RecvDircData  -·ÅÐÐ·½ÏòÊý¾Ý             
Output:         ÎÞ
Return:         boolÖµ,true-ÐèÏß³Ì´¦ÀíÎÞÏßÊÖ¿Ø°´¼ü false-ÎÞÐè´¦Àí
Date:            20141117
***************************************************************/
bool CWirelessBtn::GetbHandleWirelessBtnMsg()
{
	return bHandleWirelessBtbMsg ;
}

/**************************************************************
Function:       CWirelessBtn::HandleSpecialDirec
Description:    ´¦ÀíÎÞÏßÊÖ¿Ø×éºÏ·½ÏòµÄ·ÅÐÐµÆÉ«
Input:           RecvDircData  -·ÅÐÐ·½ÏòÊý¾Ý             
Output:         ÎÞ
Return:         ÎÞ
Date:            20141105
***************************************************************/
void CWirelessBtn::HandleSpecialDirec(Uint * RecvtmpDircData)
{
	Uint RecvDircData = *RecvtmpDircData ;	
	if(bTransitSetp == false)  //·Ç¹ý¶É²½
	{	
		if(bFirstOperate)
		{
			pManaKernel->SetWirelessBtnDirecCfg(RecvDircData,0x0);
			bFirstOperate = false ;
			RecvoldDirec = RecvDircData ;
			//ACE_OS::printf("%s:%d First special direc ,no Transit setp !\n",__FILE__,__LINE__);
		}
		else
		{
			bTransitSetp = true ;	
			pManaKernel->SetWirelessBtnDirecCfg(RecvoldDirec,0x2);
						
			//ACE_OS::printf("%s:%d Transit setp Green Flash sleep 2 sec!\n",__FILE__,__LINE__);
			ACE_OS::sleep(2);
			pManaKernel->SetWirelessBtnDirecCfg(RecvoldDirec,0x1);
						
			//ACE_OS::printf("%s:%d Transit setp Yellow  sleep 3 sec!\n",__FILE__,__LINE__);
			ACE_OS::sleep(3);
			pManaKernel->SetWirelessBtnDirecCfg(RecvDircData,0x0);
			bTransitSetp  = false ;
			RecvoldDirec = RecvDircData ;
		}			
	
	}

}

/**************************************************************
Function:       CWirelessBtn::SetCurrenStepFlash
Description:    ÉèÖÃµ±Ç°²½·¥´¦ÓÚºÎÖÖµÆÉ«×´Ì¬£¬ÓÃÓÚ½øÈëÊÖ¿ØµÄÊ±ºò¹ý¶É
Input:           LampColor--µÆÉ«×´Ì¬           
Output:         ÎÞ
Return:         ÎÞ
Date:            20141105
***************************************************************/

bool  CWirelessBtn::SetCurrenStepFlash(Byte LampColor)
{
	//ACE_OS::printf("%s:%d First WirelessManual Flash!\n",__FILE__,__LINE__);
	bool bSetLampColor = false ;
	Byte LampColorStatus = 0 ;
	Byte m_ucLampOn[MAX_LAMP]={0}; 
	Byte m_ucLampFlash[MAX_LAMP]={0}; 
	ACE_OS::memcpy(m_ucLampOn,pManaKernel->m_pRunData->sStageStepInfo[pManaKernel->m_pRunData->ucStepNo].ucLampOn,MAX_LAMP);	
	ACE_OS::memcpy(m_ucLampFlash,pManaKernel->m_pRunData->sStageStepInfo[pManaKernel->m_pRunData->ucStepNo].ucLampFlash,MAX_LAMP);
	
	for(Byte LampIndex = 0 ;LampIndex< 60 ; LampIndex++)
	{
		LampColorStatus  = pManaKernel->m_pRunData->sStageStepInfo[pManaKernel->m_pRunData->ucStepNo].ucLampOn[LampIndex];
		if(LampColorStatus ==1)
		{
			 if(LampIndex%3 ==2)
			{
				if(LampColor ==0x2)//ÂÌÉÁ
					{
						m_ucLampFlash[LampIndex]=0x1;
						if(!bSetLampColor)
							bSetLampColor = true ;
						m_ucAllowLampStatus = LAMP_GreenFLASH;
					}
				if(LampColor == 0x1)//»ÆµÆ
				{		
					m_ucLampOn[LampIndex]=0;
					m_ucLampFlash[LampIndex]=0;
					
					if((LampIndex+1)%12==0) //ÈËÐÐ
					{
						m_ucLampOn[LampIndex-2] = 0x1 ;
						
					}
					else
					{
						m_ucLampOn[LampIndex-1]=0x1;
						
					}
					if(!bSetLampColor)
							bSetLampColor = true ;
					m_ucAllowLampStatus = LAMP_YELLOW ;
				}
				if(LampColor == 0x3)//»ÆÉÁ
				{	
					if(m_ucLampFlash[LampIndex]==0x1)
					{
						m_ucLampOn[LampIndex]=0x0;
						m_ucLampFlash[LampIndex]=0x0;
						if((LampIndex+1)%12==0) //ÈËÐÐ²»»ÆµÆºÍÉÁË¸
						{
							
							m_ucLampOn[LampIndex-2] = 0x1 ; 	 //ÈËÐÐÁÁºìµÆ						
						}
						else
						{
							m_ucLampOn[LampIndex-1]=0x1;    //»ÆµÆÉÁ
							m_ucLampFlash[LampIndex-1]=0x1;
						}
						
						if(!bSetLampColor)
							bSetLampColor = true ;
						m_ucAllowLampStatus = LAMP_YELLOWFLASH ;
					}
				}
			
			}
			if(LampIndex%3 ==1)
			{
				if(LampColor == 0x3)//»ÆÉÁ
				{
						if((LampIndex+2)%12==0) //ÈËÐÐ²»»ÆµÆºÍÉÁË¸
						{
							m_ucLampOn[LampIndex]=0x0;
							m_ucLampFlash[LampIndex]=0x0;
							m_ucLampOn[LampIndex-1] =0x1;
						}
						else
							m_ucLampFlash[LampIndex]=0x1;
					if(!bSetLampColor)
							bSetLampColor = true ;
					m_ucAllowLampStatus = LAMP_YELLOWFLASH;
				}

			}
		}
	}
	
	if(bSetLampColor)
		CLampBoard::CreateInstance()->SetLamp(m_ucLampOn,m_ucLampFlash);
}


/**************************************************************
Function:		CWirelessBtn::StartWirelessBtnTimer
Description:	¿ªÆôÎÞÏßÒ£¿ØÆ÷³¬Ê±¶¨Ê±Æ÷,ÆµÂÊ1Ãë
Input:		ÎÞ 			
Output: 		ÎÞ
Return: 		ÎÞ
Date:		 20141118
***************************************************************/

void CWirelessBtn::StartWirelessBtnTimer()
{
	m_lWirelessBtnTimerId = m_tActiveWirelessBtnTimer.schedule(m_pWirelessBtnTimer,NULL,getCurrTime(),ACE_Time_Value(1)); 
}

/**************************************************************
Function:		CWirelessBtn::CloseWirelessBtn
Description:	¹Ø±ÕÎÞÏßÒ£¿ØÆ÷³¬Ê±¶¨Ê±Æ÷
Input:			 ÎÞ 			
Output: 		ÎÞ
Return: 		ÎÞ
Date:	     20141118
***************************************************************/
void CWirelessBtn::CloseWirelessBtn()
{
	if ( m_lWirelessBtnTimerId > 0 )
	{
		m_tActiveWirelessBtnTimer.cancel(m_lWirelessBtnTimerId);
	}
}


/**************************************************************
Function:       CWirelessBtnTimer::CWirelessBtnTimer
Description:    CWirelessBtnTimerÀà¹¹Ôìº¯Êý
Input:           ÎÞ             
Output:         ÎÞ
Return:         ÎÞ
Date:            20141023
***************************************************************/
CWirelessBtnTimer::CWirelessBtnTimer()
{
	m_uctimeout = 0 ;
	//bAutoExit = false ;
	m_ucSystemSetTimeOut = CManaKernel::CreateInstance()->m_pTscConfig->sSpecFun[FUN_WIRELESSBTN_TIMEOUT].ucValue*60 ;
	ACE_DEBUG((LM_DEBUG,"%s:%d Init WirelessBtnTimer object ok !\n",__FILE__,__LINE__));

}

/**************************************************************
Function:       CWirelessBtnTimer::~CWirelessBtnTimer
Description:    CTscTimerÀàÎö¹¹º¯Êý
Input:          ÎÞ               
Output:         ÎÞ
Return:         ÎÞ
Date:            20141023
***************************************************************/
CWirelessBtnTimer::~CWirelessBtnTimer()
{
	ACE_DEBUG((LM_DEBUG,"%s:%d Destruct CWirelessBtnTimer object ok !\n",__FILE__,__LINE__));
}

/**************************************************************
Function:       CWirelessBtnTimer::CreateInstance
Description:    ´´½¨µ¥Àý¶ÔÏó
Input:          ÎÞ          
Output:       ÎÞ
Return:       CWirelessBtnTimer¶ÔÏóÖ¸Õë
***************************************************************/

CWirelessBtnTimer* CWirelessBtnTimer::CreateInstance()
{
	static CWirelessBtnTimer cWirelessBtnTimer;
	return &cWirelessBtnTimer;
}


/**************************************************************
Function:       CWirelessBtnTimer::handle_timeout
Description:    ÐÅºÅ»úÒ£¿ØÆ÷°´Å¥¶¨Ê±Æ÷¶¨Ê±»Øµ÷º¯Êý£¬1sÖ´ÐÐÒ»´Î¡£´¦ÀíÐÅºÅ»ú
				¶àÊý¶¨Ê±¹¤×÷¡£
Input:           Ä¬ÈÏ´¦Àí£¬ÓÃ»§ÎÞÐèÊäÈë              
Output:         ÎÞ
Return:         0
Date:            20141023
***************************************************************/
int CWirelessBtnTimer::handle_timeout(const ACE_Time_Value &tCurrentTime, const void * /* = 0 */)
{
	m_uctimeout++;	
	if(m_uctimeout >=  m_ucSystemSetTimeOut )
	{			
		CWirelessBtn::CreateInstance()->BackToAuto();
		CWirelessBtn::CreateInstance()->CloseWirelessBtn();		
		m_uctimeout = 0 ;
		ACE_OS::printf("%s:%d WirelessBtn Manual Control timeout ,Back to AutoSelf Control!\r\n",__FILE__,__LINE__);
		
	}
	return 0 ;
}

/**************************************************************
Function:       CWirelessBtnTimer::Resettimeout
Description:    ÖØÖÃ³¬Ê±¼ÆÊý

Input:           ÎÞ           
Output:         ÎÞ
Return:         ÎÞ
Date:            20141023
***************************************************************/

void CWirelessBtnTimer::Resettimeout()
{
	m_uctimeout = 0 ;
}


