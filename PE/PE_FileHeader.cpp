// PE_FileHeader.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_FileHeader.h"
#include "afxdialogex.h"


// CPE_FileHeader �Ի���

IMPLEMENT_DYNAMIC(CPE_FileHeader, CDialogEx)

CPE_FileHeader::CPE_FileHeader(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_FileHeader, pParent)
	, m_Character(_T(""))
	, m_COFF(_T(""))
	, m_PointerToSym(_T(""))
	, m_SizeOfOptional(_T(""))
	, m_Time(_T(""))
	, m_Machine(_T(""))
	, m_NumberOfSections(_T(""))
{
	m_pNt32 = pNt32;
}
CPE_FileHeader::CPE_FileHeader(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_FileHeader, pParent)
	, m_Character(_T(""))
	, m_COFF(_T(""))
	, m_PointerToSym(_T(""))
	, m_SizeOfOptional(_T(""))
	, m_Time(_T(""))
	, m_Machine(_T(""))
	, m_NumberOfSections(_T(""))
{
	m_pNt64 = pNt64;
}

CPE_FileHeader::~CPE_FileHeader()
{
}

void CPE_FileHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Character, m_Character);
	DDX_Text(pDX, IDC_EDIT_COFFTablesize, m_COFF);
	DDX_Text(pDX, IDC_EDIT_PointerToSymbol, m_PointerToSym);
	DDX_Text(pDX, IDC_EDIT_SizeOfOptional, m_SizeOfOptional);
	DDX_Text(pDX, IDC_EDIT_Time, m_Time);
	DDX_Text(pDX, IDC_Machine, m_Machine);
	DDX_Text(pDX, IDC_NumberOfSections, m_NumberOfSections);
}


BEGIN_MESSAGE_MAP(CPE_FileHeader, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_FileHeader ��Ϣ�������


BOOL CPE_FileHeader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (m_pNt32)
	{
		GetFileHeaderInfo32();
	}
	else
	{
		GetFileHeaderInfo64();
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPE_FileHeader::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}



void CPE_FileHeader::GetFileHeaderInfo32()
{
	UpdateData(TRUE);

	m_Machine.Format(L"%04X", m_pNt32->FileHeader.Machine);
	m_NumberOfSections.Format(L"%04X", m_pNt32->FileHeader.NumberOfSections);
	m_Time.Format(L"%04X", m_pNt32->FileHeader.TimeDateStamp);
	m_PointerToSym.Format(L"%04X", m_pNt32->FileHeader.PointerToSymbolTable);
	m_COFF.Format(L"%04X", m_pNt32->FileHeader.NumberOfSymbols);
	m_SizeOfOptional.Format(L"%04X", m_pNt32->FileHeader.SizeOfOptionalHeader);
	m_Character.Format(L"%04X", m_pNt32->FileHeader.Characteristics);

	UpdateData(FALSE);
}

void CPE_FileHeader::GetFileHeaderInfo64()
{
	UpdateData(TRUE);

	m_Machine.Format(L"%04X", m_pNt64->FileHeader.Machine);
	m_NumberOfSections.Format(L"%04X", m_pNt64->FileHeader.NumberOfSections);
	m_Time.Format(L"%04X", m_pNt64->FileHeader.TimeDateStamp);
	m_PointerToSym.Format(L"%04X", m_pNt64->FileHeader.PointerToSymbolTable);
	m_COFF.Format(L"%04X", m_pNt64->FileHeader.NumberOfSymbols);
	m_SizeOfOptional.Format(L"%04X", m_pNt64->FileHeader.SizeOfOptionalHeader);
	m_Character.Format(L"%04X", m_pNt64->FileHeader.Characteristics);

	UpdateData(FALSE);
}
