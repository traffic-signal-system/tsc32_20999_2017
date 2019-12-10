#ifndef __COMDEF__H__
#define __COMDEF__H__

#include "Define.h"

/*
Can报文类型 占据ID识别符 bit26-bit28
*/
enum
{
	CAN_MSG_TYPE_000 = 0, //最高实时性  保留紧急使用
	CAN_MSG_TYPE_001 = 1, //灯色和绿冲突，灯泡检测
	CAN_MSG_TYPE_010 = 2, //灯色和绿冲突，灯泡检测
	CAN_MSG_TYPE_011 = 3, //灯色和绿冲突，灯泡检测
	CAN_MSG_TYPE_100 = 4, //检测数据、手控数据和电源板数据
	CAN_MSG_TYPE_101 = 5, //配置数据的发送，如检测器故障状态，检测器测速绑定
	CAN_MSG_TYPE_110 = 6, //没有实时性要求，系统保留后期模块连接
	CAN_MSG_TYPE_111 = 7  //本系统中被禁用
};

/*
*模块地址 占据ID识别符bit20-bit25 目的地址 占据ID识别符bit12-bit17
*/
enum
{
	BOARD_ADDR_MAIN         = 0x10 ,  //主控板
	BOARD_ADDR_MAINBACK     = 0x11 ,  //主板备份单片机
	BOARD_ADDR_LAMP1        = 0x13 ,  //灯控板1
	BOARD_ADDR_LAMP2        = 0x14 ,  //灯控板2
	BOARD_ADDR_LAMP3        = 0x15 ,  //灯控板3
	BOARD_ADDR_LAMP4        = 0x16 ,  //灯控板4
	BOARD_ADDR_LAMP5        = 0x17,   //扩展灯控板1	
	BOARD_ADDR_LAMP6        = 0x18,   //扩展灯控板2 
	BOARD_ADDR_ALLLAMP      = 0x19 ,  //灯控板组播
	
	BOARD_ADDR_LAMP7        = 0x1a,   //扩展灯控板3 	
	BOARD_ADDR_LAMP8        = 0x1b,   //扩展灯控板4 
	BOARD_ADDR_LAMP9        = 0x1c,   //扩展灯控板5 
	BOARD_ADDR_LAMPa        = 0x1d,   //扩展灯控板6
	BOARD_ADDR_LAMPb        = 0x1e,   //扩展灯控板7 
	BOARD_ADDR_LAMPc        = 0x1f,   //扩展灯控板8
		
	BOARD_ADDR_POWER        = 0x20 ,  //电源模块	
	BOARD_ADDR_POWER2       = 0x22 ,  //电源模块2
	BOARD_ADDR_DETECTOR1    = 0x24 ,  //检测器1
	BOARD_ADDR_DETECTOR2    = 0x25 ,  //检测器2
	BOARD_ADDR_ALLDETECTOR  = 0x26 ,  //检测器组播
	BOARD_ADDR_INTEDET1     = 0x29 ,  //检测器接口1
	BOARD_ADDR_INTEDET2     = 0x2A ,  //检测器接口2
	BOARD_ADDR_ALLINTEDET   = 0x2B ,  //检测器接口板组播
	BOARD_ADDR_FLASH        = 0x2E ,  //硬件黄闪器
	BOARD_ADDR_HARD_CONTROL = 0x30 ,  //硬件控制模块
	BOARD_ADDR_LED          = 0x32 ,  //显示灯组 ADD:20130808 15 50
	BOARD_ADDR_WIRELESS_BTNCTRLA = 0x33 , //无线遥控器A	多键通信版（15键，315M）//ADD:20141021
	BOARD_ADDR_WIRELESS_BTNCTRLB = 0x34   //无线遥控器B	高级版，一对多遥控加设置//ADD:20141021
	
};

/*
*帧模式  占据ID识别符bit18-bit19
*/
enum
{
	FRAME_MODE_P2P        = 0 , //点对点
	FRAME_MODE_MULTICAST  = 1 , //组播
	FRAME_MODE_BROADCAST  = 2 , //广播
	FRAME_MODE_HEART_BEAT = 3   //广播心跳
};


/*
*GBT应用层协议数据类型
*/
enum
{
	DATA_HEAD_NOREPLY = 0 , //通用数据，不需要回复
	DATA_HEAD_RESEND  = 1 , //请求发送
	DATA_HEAD_CHECK   = 2 , //确认请求
	DATA_HEAD_OTHER   = 3   //未定义，保留
};

const int DET_STAT_CYCLE = 60;   //占有率统计的周期 1分钟
enum
{
	DETECTOR_INTERFACE_BOARD1 = 0,  //检测器接口板1地址
	DETECTOR_INTERFACE_BOARD2 = 1 ,  //检测器接口板2地址
	DETECTOR_BOARD1           = 2  ,  //检测器板1
	DETECTOR_BOARD2           = 3     //检测器板2
};






/************************************************************************************************/
/*
*信号机系统常量值定义，对应GBT协议部分常量对象
*/
const int MAX_TIMEGROUP          = 40;   //最大时基调度表数
const int MAX_SCHEDULE_PER_DAY   = 48;   //每天最大时段数
const int MAX_TIMEPATTERN        = 32;   //最大的配时方案表
const int MAX_SCHEDULETIME_TYPE  = 17;   //1-16 用户定义，17用于特殊方案 阶段配时方案类型???
const int MAX_SON_SCHEDULE       = 16;   //最大阶段配时表里的子阶段数
const int MAX_PHASE              = 32;   //最大相位表数
const int MAX_OVERLAP_PHASE      = 16;    //最大跟随相位表数
const int MAX_CONFLICT_PHASE     = 32;   //最大冲突表数
const int MAX_STATUSOUT_PER      = 8;    //每组状态输出包含的个数
const int MAX_STEP               = 64;   //最大的步伐数
const int MAX_LAMP_BOARD         = 8;    //最大灯驱板数
const int MAX_LAMP_NUM_PER_BOARD = 12;   //每块板的灯具数量
const int MAX_LAMPGROUP_PER_BOARD =4 ;
const int MAX_CHANNEL            = MAX_LAMP_BOARD * 4;                        //最大通道（信号组）表数 1板4通道
const int MAX_LAMP                   = MAX_LAMP_BOARD * MAX_LAMP_NUM_PER_BOARD;   //最大灯具数  1通道3灯具 1板12灯具
const int MAX_DET_BOARD          = 4;    //最大车检板(接口板)数目
const int MAX_DETECTOR_PER_BOARD = 16;   //每块板包含的检测器数量
const int MAX_INTERFACE_PER_BOARD =32 ;  //每块接口板包含的通道数量
const int MAX_DETECTOR           = (MAX_DETECTOR_PER_BOARD+MAX_INTERFACE_PER_BOARD)*MAX_DET_BOARD/2;   //最大检测器数量

const int MAX_SPESTATUS_CYCLE    = 10;   //时段表里定义的特殊状态周期时长 
const int MIN_GREEN_TIME	         = 7;    //最小绿灯时长

const int MAX_REGET_TIME         = 10;   //100ms 1per 1s=10times
const int USLEEP_TIME            = 2000;
const int MAX_WORK_TIME          = 3;    //3次一样的才起作用
const int BOARD_REPEART_TIME     = 5;    //板状态判断的重复次数
const int MAX_DREC               = 80;  //最大方向数量   201310181705->201401031017 8个方向，每个方向10个支流
const Byte MAX_CNTDOWNDEV        = MAX_PHASE ;  //支持最大的倒计时设备数32,相位倒计时
const Byte MAX_MODULE            =50 ;  //最大设备模块数 20150112
const Byte MAX_POWERBOARD        =2 ;    //最大电源板数量
/************************************************************************************************/

/*
信号机的工作模式
*/
enum
{
	MODE_TSC  = 0 , //tsc模式
	MODE_PSC1 = 1 , //一次过街模式
	MODE_PSC2 = 2 , //二次过街模式
	MODE_OTHER =3   //其他待定模式
};



/*
信号机的工作状态
*/
enum
{
	SIGNALOFF = 0,  //关灯
	ALLRED       ,  //全红
	FLASH        ,  //闪光
	STANDARD        //标准
};

/*
信号机的控制模式
*/
enum
{
	CTRL_UNKNOWN         = 0  ,  //未知控制状态
	CTRL_SCHEDULE        = 1  ,  //多时段
	CTRL_UTCS            = 2  ,  //系统优化，即联网
	CTRL_WIRELESS        = 3  ,  //无电线缆协调
	CTRL_LINE            = 4  ,  //有线电缆协调
	CTRL_HOST            = 5  ,  //主从线控
	CTRL_MANUAL          = 6  ,  //手动控制
	CTRL_VEHACTUATED     = 7  ,  //单点全感应
	CTRL_MAIN_PRIORITY   = 8  ,  //单点主线优先半感应
    CTRL_SECOND_PRIORITY = 9  ,  //单点次线优先半感应
	CTRL_ACTIVATE        = 10 ,  //自适应
	CTRL_PANEL           = 11 ,  //面板控制	
	CTRL_SCHEDULE_OFF    = 12 ,  //时段表关灯	
	CTRL_SCHEDULE_FLASH  = 13 ,  //时段表黄闪	
	CTRL_SCHEDULE_RED    = 14 ,  //时段表全红	
	CTRL_PREANALYSIS     = 15 ,
	CTRL_LAST_CTRL       = 16    //上次的控制方式
};

/*
新增控制器LCD 黄闪原因20141106
*/
enum
{
	CTRLBOARD_FLASH_NORMAL 		 = 0x0  , //正常情况下黄闪	
	CTRLBOARD_FLASH_MANUALCTRL	 = 0x01 , //手控黄闪	
	CTRLBOARD_FLASH_NOPATTERN	 = 0x03 , //无方案黄闪
	CTRLBOARD_FLASH_LAMPCHECK    = 0x02 , //灯泡检测黄闪
	CTRLBOARD_FLASH_DOWNGRADE 	 = 0x04 , //降级黄闪
	CTRLBOARD_FLASH_FORCEFLASH	 = 0x05 , //黄闪器强制黄闪
	CTRLBOARD_FLASH_PHASECONFLIC = 0x06 , //相位冲突
	
};

/*
信号机控制队列消息枚举类型
*/
enum
{
	TSC_MSG_SWITCH_STATUS = 0,  //工作状态切换
	TSC_MSG_SWITCH_CTRL      ,  //控制方式切换
	TSC_MSG_UPDATE_PARA      ,  //数据库得到更新
	TSC_MSG_NEXT_STEP        ,  //步进命令
	TSC_MSG_OVER_CYCLE       ,  //周期结束
	TSC_MSG_LOCK_STEP        ,  //锁定步伐
	TSC_MSG_LOCK_PHASE       ,  //锁定相位
	TSC_MSG_LOCK_STAGE       ,  //锁定阶段
	TSC_MSG_NEXT_STAGE       ,  //下一个阶段
	TSC_MSG_STATUS_READ      ,  //状态获取
	TSC_MSG_EXSTATUS_READ    ,  //扩展对象获取
	TSC_MSG_EVENT_READ       ,  //事件读取
	TSC_MSG_LOG_WRITE        ,  //日志记录
	TSC_MSG_CORRECT_TIME     ,  //时间校时  SpeTimePattern
	TSC_MSG_TIMEPATTERN      ,  //特定的时间方案
	TSC_MSG_GREENCONFLICT    ,   //绿冲突
	TSC_MSG_PATTER_RECOVER   ,    //特定方案切换
	TSC_MSG_MANUALBUTTON_HANDLE   //无线手控按键处理ADD:201411051548
};

/*
通信进程gbt处理队列消息类型枚举
*/
enum
{
	GBT_MSG_FIRST_RECV = 0    ,  //首次接收
	GBT_MSG_DEAL_RECVBUF      ,  //处理接收到的BUF
	GBT_MSG_SEND_HOST         ,  //发往上位机
	GBT_MSG_TSC_STATUS        ,  //得到信号机状态
	GBT_MSG_TSC_EXSTATUS      ,  //得到信号机扩展状态
	GBT_MSG_SELF_REPORT       ,  //主动上报
	GBT_MSG_OTHER_OBJECT      ,  //gbt协议的其他类型对象
	GBT_MSG_EXTEND               //扩展对象
};




/*
设备状态类型枚举
*/
enum
{	
	DEV_IS_GOOD      = 0,  //正常

	DEV_ALWAYS_ON    = 1,  //常亮
	DEV_ALWAYS_OFF   = 2,  //常灭

	DEV_SILICON_SHOT = 3,  //可控硅击穿

	DEV_ALWAYS_ON_CLEAR,
	DEV_ALWAYS_OFF_CLEAR,

	DEV_IS_BROKEN,
	DEV_IS_SHORT_CIRCUIT,

	DEV_IS_BROKEN_CLEAR,
	DEV_IS_SHORT_CIRCUIT_CLEAR,

	DEV_IS_CONNECTED,
	DEV_IS_DISCONNECTED
};

/*
*主控板显示灯状态类型枚举
*/
enum
{
	LED_OP_START   = 0 ,
	
	LED_RUN_ON     = 1 ,
	LED_RUN_OFF    = 2 ,

	LED_GPS_ON     = 3 ,
	LED_GPS_OFF    = 4 ,

	LED_RS485_ON   = 5 , 
	LED_RS485_OFF  = 6 ,
	LED_AUTO_SLEF, 
	LED_AUTO_MANUAL,
	LED_TSCPSC_TSC,
	LED_TSCPSC_PSC,
	LED_MODE_GREEN,
	LED_MODE_YELLOW,
	LED_MODE_RED
};

/*
*串口9位通信功能枚举
*/
enum
{
	com_9bit_save_old 	    = 0x11,
	com_9bit_enable         = 0x12,
	com_9bit_disable        = 0x13,
	com_9bit_restore_old    = 0x14
};

/*
*通道类型枚举
*/
enum
{
	CHANNEL_OTHER   = 1 ,  //其他类型通道
	CHANNEL_VEHICLE = 2 ,  //机动车通道
	CHANNEL_FOOT    = 3 ,  //行人通道
	CHANNEL_OVERLAP = 4    //跟随相位通道
};

/*倒计时类型设置*/
enum
{
	CNTDOWN_15 = 1 ,      //黑屏15秒倒计时
	CNTDOWN_8  = 2,       //8秒倒计时
	CNTDOWN_NORMAL       //普通倒计时
};

/*
*信号机手动控制类型
*Date:201410211730
*/
enum
{
	Manual_CTRL_NO               = 0 ,	  //不处于手控状态
	Manual_CTRL_PANEL            = 1 ,	  //面板手控
	Manual_CTRL_WIRELESSBUTTONS  = 2 , 	  //无线遥控器
	Manual_CTRL_SYSTEM 	         = 3      //上位机命令
};


/*
*信号机特定功能类型枚举
*/
enum
{
	FUN_SERIOUS_FLASH  = 0  , //严重故障黄闪
	FUN_COUNT_DOWN     = 1  , //主动倒计时
	FUN_GPS            = 2  , //gps启用
	FUN_MSG_ALARM      = 3  , //短信报警
	FUN_CROSS_TYPE     = 4  , //过街方式 0-tsc 1-one psc 2-twice psc
	FUN_STAND_STAGEID  = 5  , //待机阶段号
	FUN_CORSS1_STAGEID = 6  , //行人通行阶段号1
	FUN_CROSS2_STAGEID = 7  , //行人通行阶段号2
	FUN_TEMPERATURE    = 8  , //温度计用户
	FUN_VOLTAGE        = 9  , //电压检测开关
	FUN_DOOR           = 10 , //门开关
	FUN_COMMU_PARA     = 11 , //通信接口
	FUN_PORT_LOW       = 12 , //端口号低字节
	FUN_PORT_HIGH      = 13 , //端口号高字节
	FUN_PRINT_FLAG     = 14 , //打印标志  0检测器 1发电源板   2收电源板 3发灯控板 4收灯控板 5发黄闪器 6收黄闪器 7步伐信息
	FUN_PRINT_FLAGII   = 15 , //打印标志  0倒计时 1StartTime 2CostTime
	FUN_CAM			   = 16 , // 摄像头
	FUN_3G             = 17 , //3G通信
	FUN_WIRELESSBTN	   = 18 , //无线手控
	FUN_CNTTYPE	       = 19 ,  //倒计时类型
	FUN_LIGHTCHECK	   = 20 ,	//灯泡检测开关	
	FUN_GPS_INTERVAL   = 21	,	//GPS定时更新时间 1 表示每天，2表示每2天
	FUN_WIRELESSBTN_TIMEOUT = 22, //无线手控按键手动控制超时时间单位分钟 ADD:201410231639	
	FUN_CROSSSTREET_TIMEOUT = 23, //行人按钮超时间 ADD:201501091738
	FUN_RS485_BITRATE       =24 , //485倒计时比特率 0-9600 1-2400-2-4800 3-38400   4-15200
	
	FUN_FLASHCNTDOWN_TIME   =25,  //闪断式倒计时闪断时间 0-0ms 1-50ms 2-100ms,以此类推. //ADD 20150605
	FUN_COUNT          = 26      // 总到特定功能数量值
};

/*
*信号机倒计时类型
*Date:201503251001
*/
enum
{
	COUNTDOWN_STUDY                    = 0 ,	  //学习式倒计时
	COUNTDOWN_GAT5082004         	   = 1 ,	  //通讯式倒计时国标GAT508-2004 ,固定支持4个倒计时，每个方向一个倒计时
	COUNTDOWN_FLASHOFF 	     		   = 2 ,   	 //闪断式倒计时 发闪断指令给灯驱板，支持32个倒计时
	COUNTDOWN_GAT5082014  	 	       = 3 ,  	 //通讯式倒计时国标GAT508-2004最大支持32个倒计时
	COUNTDOWN_GAT5082014V2   		   = 4 ,   	//通讯史倒计时国标GAT508-2004兼容支持4方向
	COUNTDOWN_GAT5082004V2    		   = 5      //通讯式倒计时国标GAT508-2004 ，支持8个倒计时
};

/*
*信号及日志类型枚举
*/
enum 
{
	LOG_TYPE_LAMP         = 0 ,  //信号灯
	LOG_TYPE_GREEN_CONFIG = 1 ,  //绿冲突
	LOG_TYPE_DETECTOR     = 2 ,  //检测器
	LOG_TYPE_VOLTAGE      = 3 ,  //电压
	LOG_TYPE_TEMPERATURE  = 4 ,  //温度
	LOG_TYPE_POWERBOARD   = 5 ,  //电源板
	LOG_TYPE_LAMPBOARD    = 6 ,  //灯控板
	LOG_TYPE_DETBOARD     = 7 ,  //检测器板
	LOG_TYPE_FLASHBOARD   = 8 ,  //黄闪器板
	LOG_TYPE_REBOOT       = 9 ,  //程序重启
	LOG_TYPE_CORRECT_TIME = 10,  //系统时间修改
	LOG_TYPE_DOOR_WARN    = 11,  //门报警
	LOG_TYPE_MANUAL       = 12,  //机器手动
	LOG_TYPE_OUTLAMP_ERR  = 13,  //灯色输出异常
	LOG_TYPE_OUTLAMP_ERR2 = 14,  //灯色输出异常2
	LOG_TYPE_OTHER        = 15,
	LOG_TYPE_CAN                //CAN总线     
};

/*************************GBT协议消息处理常量定义************************/
const int MAX_GBT_MSG_LEN        = 484;   //gbt协议消息的最大长度
const int MIN_GBT_MSG_LEN        = 3;     //gbt协议消息的最小长度
const int MAX_CLIENT_NUM         = 0xFF;     //最大的客户端连接数
const int DEFAULT_GBT_PORT       = 5435;  //默认开辟的端口号
const int DEFAULT_BROADCAST_PORT = 8808;  //默认开辟的端口号
const int MAX_GBT_PORT           = 10024; //最大端口号    
const int MIN_GBT_PORT           = 1024;  //最小端口号
const int MAX_BUF_LEN            = 8192;  //最大帧的长度
/*************************GBT协议消息处理常量定义************************/

/*
GBT协议数据包类型枚举
*/
enum 
{
	GBT_SEEK_REQ      = 0,  //查询请求
	GBT_SET_REQ       = 1,  //设置请求
	GBT_SET_REQ_NOACK = 2,  //设置请求，但不需确认应答

	GBT_SELF_REPORT   = 3,  //主动上报

	GBT_SEEK_ACK      = 4,  //查询应答
	GBT_SET_ACK       = 5,  //设置应答
	GBT_ERR_ACK       = 6,  //出错应答

	GBT_OTHER_ACK     = 7   //其他类型
};



/*
GBT错误类型枚举
*/
enum
{
	GBT_ERROR_LONG     = 1,  //消息太长
	GBT_ERROR_TYPE        ,  //消息类型
	GBT_ERROR_OBJECT_VALUE,  //对象值超过范围
	GBT_ERROR_SHORT       ,  //消息太短
	GBT_ERROR_OTHER          //其他错误
};
/*
GB协议类型
*/
enum
{
	GBT20999     = 0x1,  //
	GB25280       =0x2     ,  //
	
};

/*
信号灯颜色类型枚举
*/
enum
{
	LAMP_COLOR_RED = 0,  //红色灯
	LAMP_COLOR_YELLOW ,  //黄色灯
	LAMP_COLOR_GREEN     //绿色灯
};


const int MAX_ADJUST_CYCLE = 3;
const int MAX_PLUS_SCALE   = 30;  //一个周期增加的最大比例 
const int MAX_MINU_SCALE   = 20;  //每个周期减少的最大比例
const int PLUS_LINE        = 50;  //增加的界线





#endif  //__COMDEF__H__
