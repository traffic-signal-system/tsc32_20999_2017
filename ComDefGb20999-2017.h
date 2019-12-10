#ifndef  __COMDEF209992017__H__
#define  __COMDEF209992017__H__
#include "Define.h"
#include "ace/OS.h"
#include "Configure.h"

#define MIX_GB25280FRAME_LEN 0x10       //定义协议最短长度16字节

typedef struct GbDataNode
{
    Byte NodeData;
    GbDataNode *pNext;
}*pList;

/*
    定义GB0999-2017协议通信超时定义
*/
enum
{
    TSC_COMMUICATION_TIMEOUT1= 0x32,         //超时5秒
    TSC_COMMUICATION_TIMEOUT2= 0x64,         //超时10秒        
    TSC_COMMUICATION_TIMEOUT3= 0xC8        //超时20秒
};


/*
    定义GB0999-2017协议通信类型
*/
enum
{
    TSC_COMMUICATION_TCP= 0x1,         //TCP方式通信
    TSC_COMMUICATION_UDP= 0x2,         //UDP方式通信        
    TSC_COMMUICATION_RS232= 0x3        //串口方式通信
};


/*****************GBT25280协议对象标志类型*************************************************/
enum
{
    OBJECT_ONLINE_GB25280                         = 0x1,     //联机
    OBJECT_TRAFFIC_FLOW_GB25280          =0x2,      //交通流信息
    OBJECT_WORKSTATUS_GB25280              =0x3,      //信号机工作状态
    OBJECT_LAMPCOLOR_GB25280                =0x4,      //灯色状态
    OBJECT_CURRENTTIME_GB25280             =0x5,      //当前时间
    OBJECT_LANPGROUP_GB25280                =0x6,      //信号灯组
    OBJECT_PHASE_GB25280                           =0x7,   //信号相位
    OBJECT_SIGNALPATTERN_GB25280         =0x8,      //信号配时方案
    OBJECT_PLAN_GB25280                            =0x9,      //方案调度计划
    OBJECT_WORKTYPE_GB25280                  =0xA,     //工作方式
    OBJECT_TSCERROR_GB25280                   =0xB,      //信号机故障
    OBJECT_TSCVERSION_GB25280               =0xC,      //信号机版本
    OBJECT_TSCPARAVER_GB25280              =0xD,       //特征参数版本
    OBJECT_TSCIC_GB25280                       =0xE,       //信号机识别码
    OBJECT_REMOTECTRL_GB25280             =0xF,       //信号机远程控制
    OBJECT_DETECTOR_GB25280            =0x10,     //检测器
    OBJECT_DETECTOR_OTHER                     =0xFF       // 未知 用于构造异常出错应答
};



/*********************************GBT20999-2017协议数据帧类型枚举**************************/
enum
{
    TYPE_QUERY                =0x10,  //查询
    TYPE_QUERY_REPLY          =0x20,  //查询应答    
    TYPE_QUERY_ERROR_REPLY    =0x21,  //查询出错回复
    TYPE_SET                  =0x30,  //设置
    TYPE_SET_REPLY            =0x40,  //设置应答
    TYPE_SET_ERROR_REPLY      =0x41,  //设置出错回复    
    TYPE_BRAODCAST            =0x50,  //广播    
    TYPE_TRAP                 =0x60,  //主动上报    
    TYPE_HEART_SEARCH         =0x70,  //心跳查询    
    TYPE_HEART_REPLY          =0x80  //心跳应答
};

/*********************************GBT25280-2016附录协议数据表头定义***********************/
enum
{
    GB25280TABLE_VERSION           =0x10,  //表数据版本
    GB25280TABLE_SENDFLAG        = 0x10,  //表数据发送标记
    GB25280TABLE_RECVFLAG        = 0x20  //表数据接收标记
};

/*********************************GB0999-2017错误值状态******************************************/
enum
{
    LIGHT_TYPE_VEHICLE     = 0x1,          //机动车灯组
    LIGHT_TYPE_NONVEHICLE  = 0x2,          //非机动车灯组
    LIGHT_TYPE_PEDESTRIAN  = 0x3,          //行人灯组
    LIGHT_TYPE_ROAD        = 0x4          //车道灯组
   
};


/*********************************GB0999-2017灯组类型******************************************/
enum
{
    STATUS_BADVALUE     = 0x10,          //值错误
    STATUS_WRONGLENGTH  = 0x11,          //值长度错误  
    STATUS_OVERFLOW     = 0x12,          //值越界    
    STATUS_READONLY     = 0x20,          //值只读
    STATUS_NULL         = 0x30,          //值不存在
    STATUS_ERROR        = 0x40,          //值一般错误
    CONTROL_FAIL        = 0x50          //控制失败
};

/*********************************GB0999-2017灯组状态******************************************/
enum
{
    LIGHT_STATUS_OFF            = 0x1,           //灭灯
    LIGHT_STATUS_RED            = 0x10,          //红灯
    LIGHT_STATUS_REDFLASH       = 0x11,          //红闪        
    LIGHT_STATUS_RED_FASTFLASH  = 0x12,          //红快闪
    LIGHT_STATUS_GREEN          = 0x20,          //绿灯
    LIGHT_STATUS_GREENFLASH     = 0x21,          //绿闪        
    LIGHT_STATUS_GREEN_FASTFLASH  = 0x22,        //绿快闪
    LIGHT_STATUS_YELLOW         = 0x30,          //黄灯
    LIGHT_STATUS_YELLOWFLASH    = 0x31,          //黄闪        
    LIGHT_STATUS_YELLOW_FASTFLASH  = 0x32,       //黄快闪    
    LIGHT_STATUS_REDYELLOW      = 0x40           //红黄灯
   
};

/*********************************GB0999-2017检测器类型******************************************/
enum
{
    DETECTOR_COIL            = 0x1,          //线圈
    DETECTOR_VIDEO           = 0x2,          //视频
    DETECTOR_GEOMAGNETIC     = 0x3,          //地磁        
    DETECTOR_MICROWAVE       = 0x4,          //微波检测器    
    DETECTOR_ULTRASONIC      = 0x5,          //超声波检测器    
    DETECTOR_INFRARED        = 0x6          //红外线检测器
   
};

/*********************************GB0999-2017相位阶段出现类型******************************************/
enum
{
    PHASE_STAGE_STATUS_NOTOFWAY         = 0x10,          //相位阶段未放行    
    PHASE_STAGE_STATUS_ONTHEWAY         = 0x20,         //相位阶段正在放行      
    PHASE_STAGE_STATUS_TRANSITON        = 0x30          //相位阶段过渡  
};

/*********************************GB0999-2017相位阶段状态******************************************/
enum
{
    PHASE_STAGE_TYPE_FIX            = 0x10,          //相位阶段固定出现    
    PHASE_STAGE_TYPE_DEMAND         = 0x20          //相位阶段按需求出现   
};

/*********************************GB0999-2017运行模式******************************************/
enum
{
    MODE_CENTER_CONTROL               = 0x10,          //中心控制模式
    MODE_CENTER_TIMETABLE_CONTROL     = 0x11,          //中心日计划控制    
    MODE_CENTER_OPTIMIZATION_CONTROL  = 0x12,          //中心优化控制   
    MODE_CENTER_COORDINATION_CONTROL  = 0x13,          //中心协调控制    
    MODE_CENTER_ADAPTIVE_CONTROL      = 0x14,          //中心自适应控制
    MODE_CENTER_MANUAL_CONTROL        = 0x15,          //中心手动控制
    MODE_LOCAL_CONTROL                = 0x20,          //本地控制模式
    MODE_LOCAL_FIXCYCLE_CONTROL       = 0x21,          //本地定周期控制    
    MODE_LOCAL_VA_CONTROL             = 0x22,          //本地感应控制   
    MODE_LOCAL_COORDINATION_CONTROL   = 0x23,          //本地协调控制    
    MODE_LOCAL_ADAPTIVE_CONTROL       = 0x24,          //本地自适应控制
    MODE_LOCAL_MANUAL_CONTROL         = 0x25,          //本地手动控制
    MODE_SPECIAL_CONTROL              = 0x30,          //特殊控制模式
    MODE_LOCAL_FLASH_CONTROL          = 0x31,          //黄闪控制    
    MODE_LOCAL_ALLRED_CONTROL         = 0x32,          //全红控制   
    MODE_LOCAL_ALLOFF_CONTROL         = 0x33           //关灯控制  
};



/*********************************GB0999-2017报警类型******************************************/
enum
{
    TYPE_ALARM_LIGHT        = 0x10,          //信号灯报警
    TYPE_ALARM_DETECTOR     = 0x30,          //检测器报警
    TYPE_ALARM_DEVICE       = 0x40,          //设备故障报警
    TYPE_ALARM_ENVIRONMENT  = 0x60           //工作环境异常报警
   
};

/*********************************GB0999-2017故障类型******************************************/
enum
{
    TYPE_FAULT_GREENCONFICT       = 0x10,          //绿冲突故障
    TYPE_FAULT_GREENREDCONFICT    = 0x11,          //红绿冲突故障
    TYPE_FAULT_REDLIGHT           = 0x20,          //红灯故障
    TYPE_FAULT_YELLOWLIGHT        = 0x21,          //黄灯故障
    TYPE_FAULT_GREENLIGHT         = 0x22,          //绿灯故障
    TYPE_FAULT_COMMUNICATION      = 0x30,          //通信故障
    TYPE_FAULT_SELF               = 0x40,          //自检故障
    TYPE_FAULT_DETECTOR           = 0x41,          //检测器故障
    TYPE_FAULT_REALY              = 0x42,          //继电器故障
    TYPE_FAULT_MEMORY             = 0x43,          //存储器故障
    TYPE_FAULT_CLOCK              = 0x44,          //时钟故障
    TYPE_FAULT_MOTHERBOARD        = 0x45,          //主板故障
    TYPE_FAULT_PHASEBOARD         = 0x46,          //相位板故障
    TYPE_FAULT_DETECTORBOARD      = 0x47,          //检测器板故障
    TYPE_FAULT_CONFIG             = 0x50,          //配置故障
    TYPE_FAULT_RESPONSE           = 0x70           //控制响应故障
   
};

/*********************************GB0999-2017故障动作******************************************/
enum
{
    SWTICH_NULL                   = 0x0,           //无故障动作
    SWTICH_TO_FLASH               = 0x10,          //切换到黄闪    
    SWTICH_TO_OFF                 = 0x20,          //切换到关灯
    SWTICH_TO_RED                 = 0x30,          //切换到全红
    SWTICH_TO_LOCAL_FIXCYCLE      = 0x40,          //切换到本地定周期
    SWTICH_TO_LOCAL_COORDINATION  = 0x50,          //切换到本地协调
    SWTICH_TO_LOCAL_VA            = 0x60           //切换到本地感应
}; 


/*********************************GB0999-2017协议命令值******************************************/
enum
{
    ORDER_FLASH         = 0x1,         //黄闪
    ORDER_RED           = 0x2,         //全红    
    ORDER_ON            = 0x3,         //开灯
    ORDER_OFF           = 0x4,         //关灯
    ORDER_RESET         = 0x5,         //重启
    ORDER_CANCEL        = 0x0          //取消命令
}; 


/*********************************GB0999-2017协议数据类定义******************************************/
enum
{
    DATACLASS_DEVICE_INFO        = 0x1,         //设备信息    
    DATACLASS_BASIC_INFO         = 0x2,         //基础信息
    DATACLASS_LIGHT_INFO         = 0x3,         //灯组信息
    DATACLASS_PHASE_INFO         = 0x4,         //相位信息
    DATACLASS_DETECTOR_INFO      = 0x5,         //检测器信息
    DATACLASS_PHASESTAGE_INFO    = 0x6,         //相位阶段信息
    DATACLASS_PHASESECURY_INFO   = 0x7,         //相位安全信息
    DATACLASS_EMERGENCYPRIORITY  = 0x8,         //紧急优先
    DATACLASS_PLAN_INFO          = 0x9,         //方案信息
    DATACLASS_TRANSITIONCONSTRAIN  = 0xA,       //过渡约束
    DATACLASS_DAILYPLAN          = 0xB,         //日计划
    DATACLASS_SCHEDULETABLE      = 0xC,         //调度表
    DATACLASS_RUNSTATUS          = 0xD,         //运行状态
    DATACLASS_TRAFFICDATA        = 0xE,         //交通数据
    DATACLASS_ALARMDATA          = 0xF,         //报警数据
    DATACLASS_FAULTDATA          = 0x10,        //故障数据
    DATACLASS_CENTERCONTROL      = 0x11,        //中心控制
    DATACLASS_PIPLECOMMAND       = 0x12         //管道命令
}; 

/************************************信号机GB25280-2016附录协议信号机配置结构体定义*/
typedef struct SGB25280TscConfig
{
    Byte  TscCommunicationStatus ;//信号机通信状态
    int   TscAreaCode ;                      //信号机区域号
    int TscCrossCode ;                      //信号机路口号
    int iPort ;                                     //通信端口
    bool bGetQueryResponse ;       //是否收到上位端发送的联机查询应答
    bool bUpTrafficFlow ;                //是否开启交通流流上传
    bool bUpTscVerUp ;                  //是否开启信号机版本主动上传
    bool bUpTscError ;                    //是否开启信号机故障主动上传
    bool bUpLampColor;                //是否开启信号机灯色状态主动上传
    bool bUpTscWorkStatus ;         //是否开启信号机工作状态主动上传
    ACE_TString  TscClientIp ;        //上位端IPv4地址
    ACE_TString  IdentifyCode;       //设置指令控制指令验证码
} sGB25280TscConfig;
;
#endif
