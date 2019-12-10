#ifndef __DEFINE__H__
#define __DEFINE__H__

#if defined(__GNUC__) 
#define LINUX
#else 
#define WINDOWS
#endif

/*数据类型定义*/
#ifndef Byte
typedef unsigned char Byte;   
#endif

#ifndef Uint
typedef unsigned int Uint;
#endif

#ifndef Ulong
typedef unsigned long Ulong;
#endif

#ifndef Ushort
typedef unsigned short Ushort;
#endif

#ifndef Char
typedef char Char;
#endif

#ifndef Long
typedef long Long;
#endif 

#ifndef Uint32
typedef unsigned int Uint32;
#endif 

#define DB_NAME "./GbAitonTsc.db"
//#define SERIAL0  "/dev/ttySAC0"    //串口1
//#define SERIAL1  "/dev/ttySAC1"    //串口2
//#define SERIAL2  "/dev/ttySAC2"    //串口3
//#define SERIAL3  "/dev/ttySAC3"    //rs485口
//#define RS485    "/dev/rs485io"   //rs485驱动

#define DEV_RTC "/dev/rtc0"
#define RTC_RD_TIME  0x80247009  // ioctl code to do Read RTC time
#define RTC_SET_TIME 0x4024700a  // ioctl code to do Set RTC time

#define GA_COUNT_DOWN   //国标倒计时协议

//#define SPECIAL_CNTDOWN 
#endif  //__DEFINE__H__

