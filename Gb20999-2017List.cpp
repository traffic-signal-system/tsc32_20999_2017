/********************************************************************************************************************
Copyright(c) 2013  AITON. All rights reserved.
Author:     AITON
FileName:   Gb25280List.cpp
Date:       2019-5-10
Description: 数据链表类,用于GB20999-2017附录协议数据处理
Version:    V1.0
History:
**********************************************************************************************************************/

#include "Gb20999-2017List.h"
#include "ace/OS.h"
CLinklistGb209992017Data::CLinklistGb209992017Data()
{
    pList p =  (pList)ACE_OS::malloc(sizeof(struct GbDataNode));
    if (p == NULL)
    {
        pGbDataList = NULL;
    }
    else
    {
        ACE_OS::memset(p, 0, sizeof(struct GbDataNode));
        p->pNext = NULL;
        pGbDataList = p;
    }
}
CLinklistGb209992017Data::~CLinklistGb209992017Data()
{
    if (pGbDataList != NULL)
        LIST_Free();
}
int CLinklistGb209992017Data::LIST_Add(Byte Data)
{

    pList pNew = NULL;
    pList pTmp = NULL;

    if (pGbDataList == NULL)
    {
        ACE_OS::printf("参数有误！\n");
        return -1;
    }
    //初始化一个新节点
    pNew = (pList)malloc(sizeof(struct GbDataNode));
    memset(pNew, 0, sizeof(struct GbDataNode));
    if (pNew != NULL)
    {

        //找到尾节点
        pNew->NodeData = Data;
        pNew->pNext = NULL;
        pTmp = pGbDataList;
        while (pTmp->pNext != NULL)
        {
            pTmp = pTmp->pNext;
        }
        //将新增节点串起来
        pTmp->pNext = pNew;

        return 1;
    }
    else
        return -1;

}

void CLinklistGb209992017Data::LIST_Free()
{
    if (pGbDataList == NULL)
    {
        printf("\nList head is NULL!\n");
        return;
    }
    else
    {

        pList pTmp = pGbDataList;
        pList  pNext = NULL;

        while (pTmp != NULL)
        {

            pNext = pTmp->pNext;
            ACE_OS::free(pTmp);
            pTmp = NULL;
            pTmp = pNext;

        }
    }
    pGbDataList =NULL;
    return ;
}

int CLinklistGb209992017Data::LIST_GetCount()
{
    if (pGbDataList == NULL)
        return -1;
    int iCount = 0;
    pList pTmp = NULL;

    pTmp = pGbDataList->pNext;
    while (pTmp != NULL)
    {
        iCount++;
        pTmp = pTmp->pNext;
    }

    return iCount;
}
int CLinklistGb209992017Data::Getelem(int pos,Byte *Item)
{

    pList p = pGbDataList;
    int i = 0x0;
    while (p != NULL && i<pos)         //将指针p移动到要返回的元素位置
    {
        p = p->pNext;
        i++;                           //i记录p指向的是第几个位置
    }
    if ((p == NULL) || (i>pos))
    {
        return -1;
    }
    else
    {
        *Item = p->NodeData;
        return 0;
    }
}

void CLinklistGb209992017Data::LIST_Print()
{
    pList pTmp = NULL;
    pTmp = pGbDataList->pNext;

    ACE_OS::printf(" \r\n ");
    while (pTmp != NULL)
    {
        ACE_OS::printf(" 0x%02x + ", pTmp->NodeData);
        pTmp = pTmp->pNext;
    }
    ACE_OS::printf(" \r\n ");
}

int CLinklistGb209992017Data::LIST_Del(int pos)
{
    pList pTmp = NULL;
    pList pPrev = NULL;
    pPrev = pGbDataList;
    pTmp = pGbDataList->pNext;

    int i = 0x0;
    while (pPrev != NULL && i<pos)//将指针p移动到要返回的元素位置
    {
        pPrev = pPrev->pNext;
        i++;                                      //i记录p指向的是第几个位置
    }
    if ((pPrev == NULL) || (i>pos))
    {
        return -1;
    }
    else
    {
        pPrev->pNext = pTmp->pNext;
        ACE_OS::free(pTmp);
        pPrev = pTmp;
        pTmp = pTmp->pNext;
        return 0;
    }
}

pList CLinklistGb209992017Data::GetpList()
{
    if (pGbDataList == NULL)
        return NULL;
    else
        return pGbDataList;
}

