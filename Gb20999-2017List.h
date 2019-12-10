#ifndef  _SGB209992017_H_
#define  _SGB209992017_H_
#include "Define.h"
#include "ComDefGb25280.h"

/***********************定义GB20999-2017附录A协议数据帧结构体********************/
struct SGB209992017FrameTableData
{
	Byte ucFrameBeginFlag;                  //协议报文开始标志 0X7E
	unsigned short ucFrameLength;                       //协议报文长度
	unsigned short ucFrameVersion            //协议版本号
	Byte ucTscClientId;                        //上位机ID
	Uint32 uiTscId;              //信号机ID
	Byte ucRoadId;               //路口ID
	Byte ucFrameFlowID;          //帧流水号
	Byte ucFrameType;            //帧类型
	Byte ucDataFrameCount;       //数据值数量
	Byte ucDataFrameIndex;       //数据值索引
	Byte ucDataFrameLength;      //数据值长度
	Byte ucDataClassId;          //数据类ID	
	Byte ucDataObjectId;         //对象ID
	Byte ucDataAttributeId;      //属性ID	
	Byte ucDataElementId;        //元素ID	
	Byte *pTableData;            //数据值
	unsigned short ucFrameCrcData         //CRC-16校验值
	Byte ucFrameEndFlag;                  //协议报文结束标志 0X7D
};

class CLinklistGb209992017Data
{
private:
	pList pGbDataList;
public:
	CLinklistGb209992017Data();
	~CLinklistGb209992017Data();
	void LIST_Free();
	int LIST_Add(Byte Data);
	int LIST_GetCount();
	void LIST_Print();
	int LIST_Del( int pos);
	int Getelem(int pos, Byte *Item);
	pList GetpList();
};
#endif

