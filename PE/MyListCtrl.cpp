// MyListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "MyListCtrl.h"



// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)

END_MESSAGE_MAP()


void CMyListCtrl::AddColumn(int nNum, ...)
{
	//设置列表框属性
	SetExtendedStyle(LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES );
	//初始化变参列表
	va_list vlist;
	va_start(vlist, nNum);
	//循环获取参数
	for (int i = 0; i < nNum; i++)
	{
		TCHAR* pName = va_arg(vlist, TCHAR*);	//列名
		int nWidth = va_arg(vlist, int);	//列宽
											//插入一列
		InsertColumn(i, pName, NULL, nWidth);
	}
	//结束参数获取
	va_end(vlist);
}


void CMyListCtrl::AddItem(int nItemNum,	//第几行
	int nCount,	//在本行填写多少个数据
	...)
{
	//初始化
	va_list vlist;
	va_start(vlist, nCount);
	//添加一行，并设置第一个单元格内容
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);
	for (int i = 1; i < nCount; i++)
	{
		//设置本行其他单元格内容
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}
	//结束获取参数
	va_end(vlist);
}
