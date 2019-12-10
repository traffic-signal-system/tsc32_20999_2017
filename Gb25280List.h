#ifndef _SGB25280_H_
#define _SGB25280_H_
#include "Define.h"
#include "ComDefGb25280.h"

/***********************定义GB25280-2016附录协议数据帧结构体********************/
struct SGB25280FrameTableData
{
	Byte ucVsersion;                        //协议版本
	Byte ucSendFlag;                       //发送方标志
	Byte ucRecvFlag;                        //接收方标志
	Byte ucDataLinkCode;               //数据链路类型
	Byte ucAreaCode;                      //区域号
	unsigned short ucRoadCode;   //路口号
	Byte ucOperaType;                    //操作类型
	Byte ucObjCode;                        //对象类型
	Byte ucReserveDara[5];              //保留字段
	Byte *pTableData;                       //数据内容指针
	Uint32 uiTableDataLen;              //数据内容长度
};

class CLinklistGbData
{
private:
	pList pGbDataList;
public:
	CLinklistGbData();
	~CLinklistGbData();
	void LIST_Free();
	int LIST_Add(Byte Data);
	int LIST_GetCount();
	void LIST_Print();
	int LIST_Del( int pos);
	int Getelem(int pos, Byte *Item);
	pList GetpList();
};
#endif

