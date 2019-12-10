
/***************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   ComFunc.cpp
Date:       2013-9-25
Description:通用处理函数
Version:    V1.0
History:    201310081456  yiodng testmem()
***************************************************************/
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ComFunc.h"
#include "ManaKernel.h"
#include "DbInstance.h"

/**************************************************************
Function:       IsLeapYear
Description:    判断是否时润年
Input:          ucYear  年份
Output:         无
Return:         false-非润年 true-润年
***************************************************************/
bool IsLeapYear(Uint ucYear)
{
    if ( ( 0==ucYear%4 && ucYear%100!=0)||(0==ucYear%400) )
    {
        return true;
    }
    return false;
}

/**************************************************************
Function:       IsRightDate
Description:    判断日期到合法性
Input:          ucYear  年份
                ucMonth 月份
                ucDay   日期
Output:         无
Return:         false-非润年 true-润年
***************************************************************/
bool IsRightDate(Uint ucYear,Byte ucMonth,Byte ucDay)
{
    Byte ucMaxDay = 31;

    if ( ucYear < 2000 || ucYear >= 2038 )
    {
        return false;
    }
    if ( ucMonth < 1 || ucMonth > 12 )
    {
        return false;
    }

    switch ( ucMonth )
    {
        case 2:
            if ( IsLeapYear(ucYear) )  //闰年
            {
                ucDay = 29;
            }
            else
            {
                ucDay = 28;
            }
            break;
        case 4:
            ucMaxDay = 30;
            break;
        case 6:
            ucMaxDay = 30;
            break;
        case 9:
            ucMaxDay = 30;
            break;
        case 11:
            ucMaxDay = 30;
            break;
        default:
            break;
    }

    if ( ucDay > ucMaxDay )
    {
        return false;
    }
    return true;

}

/**************************************************************
Function:       getCurrTime
Description:    获取当前时间，这个时间获取函数主要用于信号机定时器，
                避免修改时间到时候造成定时器停止
Input:          无
Output:         无
Return:         ACE时间
***************************************************************/
ACE_Time_Value getCurrTime()
{
    ACE_Time_Value tv;
    struct timespec ts = {0};
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    ACE_hrtime_t hrt = static_cast<ACE_hrtime_t> (ts.tv_sec) * ACE_U_ONE_SECOND_IN_NSECS+ static_cast<ACE_hrtime_t> (ts.tv_nsec);
    ACE_High_Res_Timer::hrtime_to_tv (tv, hrt);
    return tv;
}

/**************************************************************
Function:       GetCurTime
Description:    获取当前时间，用于除定时器时间获取外其他地方
Input:          无
Output:         无
Return:         ACE时间
***************************************************************/
ACE_Time_Value GetCurTime()
{
#ifdef LINUX
    time_t ti;
    struct tm rtctm;
    int fd = open(DEV_RTC, O_WRONLY, 0);
    int ret = -1;
    if(fd>0)
    {
        ret = ioctl(fd, RTC_RD_TIME, &rtctm);
        close(fd);
    }
    ti = mktime(&rtctm);
    return ACE_Time_Value(ti);
#else
    return ACE_OS::gettimeofday();
#endif
}

/**************************************************************
Function:       CopyFile
Description:    复制文件
Input:          无
Output:         无
Return:         无
***************************************************************/
void CopyFile(char src[], char dest[])
{
#ifdef LINUX
    int sd, dd;
    char buf[100];
    int n;

    if (strcmp(dest, src) == 0)
        return;

    sd = open(src, O_RDONLY);
    dd = open(dest, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (sd == -1 || dd == -1)
        goto out;

    while ((n = read(sd, buf, sizeof(buf))) > 0)
    {
        write(dd, buf, n);
    }

out:
    if (sd != -1)
        close(sd);
    if (dd != -1)
        close(dd);
#endif
}


/**************************************************************
Function:       CopyFile
Description:    调整文件的大小，超过iMaxFileLine时，删除掉一半，
                防止文件过大
Input:          无
Output:         无
Return:         无
***************************************************************/
void AdjustFileSize(char* file,int iMaxFileLine)
{
#ifdef LINUX
    FILE *fp;
    FILE *fp2;
    int line = 0;
    int ch;
    if ((fp = fopen(file, "r")) == NULL)
    {
        return;
    }
    while ( !feof(fp) )
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            line++;
        }
    }
    if ( line < iMaxFileLine )
    {
        fclose(fp);
        return;
    }
    fseek(fp, 0, SEEK_SET);
    while (!feof(fp) && ( line > iMaxFileLine / 2 ) )
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            line--;
        }
    }
    if ( (fp2 = fopen("tmp.txt", "w")) != NULL)
    {
        while ((ch = fgetc(fp)) != EOF)
        {
            fputc(ch, fp2);
        }
        fclose(fp2);
        fclose(fp);
        CopyFile((char*)"tmp.txt", file);
        remove((char*)"tmp.txt");
    }
    else
    {
        fclose(fp);
    }
#endif
}


/**************************************************************
Function:       RecordTscStartTime
Description:    记录系统开始运行时间，并写入日志
Input:          无
Output:         无
Return:         无
***************************************************************/
void RecordTscStartTime()
{
#ifdef LINUX
    ACE_OS::system("echo At $(date) tsc restart !>> TscRun.log");
#endif
    //unsigned long mRestart = 0 ;
    //(CDbInstance::m_cGbtTscDb).GetSystemData("ucDownloadFlag",mRestart);
    CManaKernel::CreateInstance()->SndMsgLog(LOG_TYPE_REBOOT,0,0,0,0);
}

/**************************************************************
Function:       TestMem
Description:    测试系统内存稳定性
Input:          无
Output:         无
Return:         无
***************************************************************/
void TestMem(char* funName,int iLine)
{
#ifdef LINUX
    static int iMemCnt = 0;
    int iCurMemCnt     = 0;
    //FILE* fp = NULL;
    system("ps | grep Gb.aiton > mem.txt");

    int sd;
    char buf[100] = {0};
    char cTmp[36] = {0};
    int n;

    sd = open("mem.txt", O_RDONLY);
    n = read(sd, buf, sizeof(buf));
    close(sd);

    ACE_OS::memcpy(cTmp,buf+15,5);

    iCurMemCnt = atoi(cTmp);


    if ( iCurMemCnt != iMemCnt )
    {
        iMemCnt = iCurMemCnt;

        FILE* file = NULL;
        char tmp[64] = {0};
        struct tm *now;
        time_t ti;

        file = fopen("moreMem.txt","a+");
        if ( NULL == file )
        {
            return;
        }

        ti = time(NULL);
        now = localtime(&ti);
        sprintf(tmp,"%d-%d-%d %d %d:%02d:%02d %s:%d memcnt:%d.\n", now->tm_year + 1900,
                now->tm_mon + 1, now->tm_mday, now->tm_wday, now->tm_hour,
                now->tm_min, now->tm_sec,funName,iLine,iCurMemCnt);
        fputs(tmp,file);
        fclose(file);
    }

    //ACE_DEBUG((LM_DEBUG,"buf:%s cTmp:%s iMemCnt:%d \n",buf,cTmp,iMemCnt));
#endif
}


/**************************************************************
Function:       CompareTime
Description:    时间大小比较
Input:          ucHour1 ：比较时间1的小时值
                ucMin1  ：比较时间1的分钟值
                ucHour1 ：比较时间2的小时值
                ucMin1  ：比较时间2的分钟值
Output:         无
Return:         0:time2比time1小   1：time2比time1大,或相等
***************************************************************/
bool CompareTime(Byte ucHour1,Byte ucMin1,Byte ucHour2,Byte ucMin2)
{
    if ( ucHour2 > ucHour1 )
    {
        return 1;
    }
    else if ( ucHour2 == ucHour1 )
    {
        if ( ucMin2 >= ucMin1 )
        {
            return 1;
        }
    }
    return 0;
}


/**************************************************************
Function:       GetEypDevID
Description:    对字符串异或加密处理
Input:          src ：待加密字符串指针
                dec  ：加密后字符串指针
Output:         无
Return:         -1 -加密字符串失败 0 - 加密字串串成功
***************************************************************/
int GetEypChar(char *src,char *dec)
{
    if(src == NULL || dec ==NULL)
        return -1 ;
    char *p=src;
    int i=0;
    while(*p!='\0')
    {
        dec[i]=0x01 ^ *p; /* 异或处理加密*/
        p++;
        i++;
    }
    dec[i]='\0';
    return 0 ;
}


/**************************************************************

Function:       GetStrHwAddr
Description:    获取网卡物理地址字符串
Input:          StrHwAddr ：网卡物理地址字符串
Output:         无
Return:         -1 获取失败  0 -获取成功
***************************************************************/
int GetStrHwAddr(char * strHwAddr)
{
    //char strHwAddr[32];
    int fdReq = -1;
    struct ifreq typIfReq;
    char *hw;
    memset(&typIfReq, 0x00, sizeof(typIfReq));
    fdReq = socket(AF_INET, SOCK_STREAM, 0);
    strcpy(typIfReq.ifr_name, "eth0");
    if(ioctl(fdReq, SIOCGIFHWADDR, &typIfReq) < 0)
    {
        printf("fail to get hwaddr %s %d\n", typIfReq.ifr_name, fdReq);
        close(fdReq);
        return -1;
    }
    hw = typIfReq.ifr_hwaddr.sa_data;
    sprintf(strHwAddr, "%02x%02x%02x%02x%02x%02x", *hw, *(hw+1), *(hw+2), *(hw+3), *(hw+4), *(hw+5));
    close(fdReq);

    return 0;
}

/**************************************************************
Function:       SaveEnyDevId
Description:    保存设置设备ID加密存储
Input:          SysEnyDevId ：系统加密ID指针
Output:         无
Return:         -1 - 产生加密设备ID失败 0 -产生加密设备ID成功
***************************************************************/
int GetSysEnyDevId(char *SysEnyDevId)
{
    char strHwAddr[32]= {0};
    char enyDevId[32] = {0};
    if(GetStrHwAddr(strHwAddr) == -1)
        return -1;
    if(GetEypChar(strHwAddr,enyDevId)==-1)
        return -1;
    ACE_OS::strcpy(SysEnyDevId,enyDevId);

    return 0;
}
/**************************************************************
Function:       GetMainBroadCdKey
Description:    保存设置设备ID加密存储
Input:          CdKey ：系统加密ID指针
Output:         无
Return:         -1 - 产生加密设备ID失败 0 -产生加密设备ID成功
***************************************************************/
int GetMainBroadCdKey(char *CdKey)
{

    char CCdkey[8] = {0};
    if(!Cdkey::GetCdkey(CCdkey))
    {
        return -1;
    }
    ACE_OS::strcpy(CdKey,CCdkey);
    ACE_DEBUG((LM_DEBUG,"%s:%d***GetMainBroadCdKey*** CdKey : %X !\n",__FILE__,__LINE__,&CdKey));
    return 0;
}
/**************************************************************
Function:      VaildSN
Description:    对核心板的序列号与系统的序列号进行对比
Input:          无
Output:         无
Return:         1 - 合法 0 -非法
***************************************************************/
bool VaildSN()
{
    char fileSN[8] = {0};
    char deviceSN[8] = {0};
    int i,bol;
    ReadTscSN(fileSN);
    GetMainBroadCdKey(deviceSN);

    bol = ACE_OS::strcmp(fileSN,deviceSN);
    if(bol == 0)
    {
        ACE_DEBUG((LM_DEBUG,"%s:%d***VaildSN*** SN VAILD !\n",__FILE__,__LINE__));
        return true;
    }
    else
    {
        ACE_DEBUG((LM_DEBUG,"%s:%d***VaildSN*** SN INVALID !\n",__FILE__,__LINE__));
        return false;
    }
}
/**************************************************************
Function:      ReadTscSN
Description:    读取系统的序列号
Input:          无
Output:         将8个字节的序列号保存到cdkey 指针 中
Return:         无
***************************************************************/
void ReadTscSN(char *cdkey)
{

    char tmp[8];
    FILE *infile;
    int rc;
    infile = fopen("sn.dat", "rb");
    if(infile == NULL)
    {
        ACE_DEBUG((LM_DEBUG,"%s:%d read File error !\n",__FILE__,__LINE__));
        return ;
    }
    rc = fread(tmp,sizeof(char), 8,infile);
    if(rc ==0)
    {
        ACE_DEBUG((LM_DEBUG,"%s:%d read File error !\n",__FILE__,__LINE__));
        return ;
    }
    ACE_OS::strcpy(cdkey,tmp);
    fclose(infile);
}

/**************************************************************
Function:       RecordTscStartTime
Description:    记录系统开始运行时间，并写入日志
Input:          无
Output:         无
Return:         无
***************************************************************/
void RecordTscSN()
{
#ifdef LINUX

    char tmp[8] = {0};
    int i;
    FILE *outfile, *infile;
    infile = fopen("sn.dat","rb");
    if(infile != NULL)
    {
        ACE_DEBUG((LM_DEBUG,"\n%s:%d***RecordTscSN*** SN file is exsit !\n",__FILE__,__LINE__));
        fclose(infile);
        return ;
    }
    outfile = ACE_OS::fopen("sn.dat", "wb" );
    GetMainBroadCdKey(tmp);
    if( outfile == NULL)
    {
        ACE_DEBUG((LM_DEBUG,"%s:%d  open file error !\n",__FILE__,__LINE__));
        return;
    }
    //for(i=0; i<8; i++)
    //{
    //fwrite( tmp, sizeof(char), 8, outfile );
    //ACE_DEBUG((LM_DEBUG,"%s:%d  0x%x !\n",__FILE__,__LINE__,tmp[i]));
    //}
    ACE_OS::fwrite( tmp, sizeof(char), 8, outfile );
    ACE_OS::fclose(outfile);
#endif
}


/**************************************************************
Function:       TscBeep
Description:    信号机系统鸣叫
Input:          无
Output:         无
Return:         无
Date:           20151208
***************************************************************/

void TscBeep()
{
#ifdef LINUX
    ACE_OS::system("echo 113 >/sys/class/gpio/export");
    ACE_OS::system("echo out >/sys/class/gpio/gpio113/direction");
    ACE_OS::system("echo 0 > /sys/class/gpio/gpio113/value");
    ACE_OS::sleep(ACE_Time_Value(0, 100000)); //暂停100毫秒
    ACE_OS::system("echo 1 > /sys/class/gpio/gpio113/value");
    ACE_OS::system("echo in >/sys/class/gpio/gpio113/direction");
    ACE_OS::system("echo 113 >/sys/class/gpio/unexport");
#endif

}

/**************************************************************
Function:       crc_16
Description:    CRC16校验
Input:          *data 校验数据值指针
                short len 校验数据长度
Output:         无
Return:         CRC16校验值    高字节在前，低字节在后
Date:           2019518
***************************************************************/

unsigned short crc_16(unsigned char *data, unsigned short len)
{
    
    unsigned short CRC_POLY = 0x1005 ;//x16+x12+x2+1
    unsigned short crc16 = 0x0000;
    while (len--)
    {
        for (unsigned char i = 0x80; i != 0; i >>= 1)
        {
            if ((crc16 & 0x8000) != 0)
            {
                crc16 = crc16 << 1;
                crc16 = crc16 ^ CRC_POLY;
            }
            else
            {
                crc16 = crc16 << 1;
            }
            if ((*data & i) != 0)
            {
                crc16 = crc16 ^ CRC_POLY;
            }
        }
        data++;
    }
    return crc16;
}


