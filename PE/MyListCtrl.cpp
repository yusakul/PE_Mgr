// MyListCtrl.cpp : ʵ���ļ�
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
	//�����б������
	SetExtendedStyle(LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES );
	//��ʼ������б�
	va_list vlist;
	va_start(vlist, nNum);
	//ѭ����ȡ����
	for (int i = 0; i < nNum; i++)
	{
		TCHAR* pName = va_arg(vlist, TCHAR*);	//����
		int nWidth = va_arg(vlist, int);	//�п�
											//����һ��
		InsertColumn(i, pName, NULL, nWidth);
	}
	//����������ȡ
	va_end(vlist);
}


void CMyListCtrl::AddItem(int nItemNum,	//�ڼ���
	int nCount,	//�ڱ�����д���ٸ�����
	...)
{
	//��ʼ��
	va_list vlist;
	va_start(vlist, nCount);
	//���һ�У������õ�һ����Ԫ������
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);
	for (int i = 1; i < nCount; i++)
	{
		//���ñ���������Ԫ������
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}
	//������ȡ����
	va_end(vlist);
}
