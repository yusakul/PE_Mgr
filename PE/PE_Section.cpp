// PE_Section.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Section.h"
#include "afxdialogex.h"
#include <winnt.h>




// CPE_Section �Ի���

IMPLEMENT_DYNAMIC(CPE_Section, CDialogEx)

CPE_Section::CPE_Section(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_SECTIONS, pParent)
{
	m_pNt32 = pNt32;
}

CPE_Section::CPE_Section(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_SECTIONS, pParent)
{
	m_pNt64 = pNt64;
}

CPE_Section::~CPE_Section()
{
}

void CPE_Section::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Section, m_list);
}


BEGIN_MESSAGE_MAP(CPE_Section, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_Section ��Ϣ�������


void CPE_Section::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CPE_Section::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_list.AddColumn(7, L"#",30, L"Name", 100, L"Virtual Size", 100, L"Virtual Offset", 100, L"Raw Size", 100, L"Raw Offset", 100, L"Characteristics", 100);
	if (m_pNt32)
	{
		GetSectionsInfo32();

	}
	else
	{
		GetSectionsInfo64();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE         
}


void CPE_Section::GetSectionsInfo32()
{
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt32);
	for (int i = 0; i < m_pNt32->FileHeader.NumberOfSections; i++)
	{
		//������	
		CHAR pName[9] = {};
		memcpy_s(pName, 9, pSec[i].Name, 8);
		m_strNum.Format(L"%d", i + 1);
		m_strName = pName;
		m_strVS.Format(L"%08X", pSec[i].Misc.VirtualSize);
		m_strRVA.Format(L"%08X", pSec[i].VirtualAddress);
		m_strFS.Format(L"%08X", pSec[i].SizeOfRawData);
		m_strFO.Format(L"%08X", pSec[i].PointerToRawData);
		m_strCha.Format(L"%08X", pSec[i].Characteristics);

						//�ڼ��� ������ٸ����� ������
		m_list.AddItem(i, 7, m_strNum, m_strName, m_strRVA, m_strVS, m_strFO, m_strFS, m_strCha);

	}
}

void CPE_Section::GetSectionsInfo64()
{
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt64);
	for (int i = 0; i < m_pNt64->FileHeader.NumberOfSections; i++)
	{
		//������	
		CHAR pName[9] = {};
		memcpy_s(pName, 9, pSec[i].Name, 8);
		m_strNum.Format(L"%d", i + 1);
		m_strName = pName;
		m_strVS.Format(L"%08X", pSec[i].Misc.VirtualSize);
		m_strRVA.Format(L"%08X", pSec[i].VirtualAddress);
		m_strFS.Format(L"%08X", pSec[i].SizeOfRawData);
		m_strFO.Format(L"%08X", pSec[i].PointerToRawData);
		m_strCha.Format(L"%08X", pSec[i].Characteristics);

		//�ڼ��� ������ٸ����� ������
		m_list.AddItem(i, 7, m_strNum, m_strName, m_strRVA, m_strVS, m_strFO, m_strFS, m_strCha);

	}
}