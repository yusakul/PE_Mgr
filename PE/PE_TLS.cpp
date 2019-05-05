// PE_TLS.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_TLS.h"
#include "afxdialogex.h"


// CPE_TLS 对话框

IMPLEMENT_DYNAMIC(CPE_TLS, CDialogEx)

CPE_TLS::CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_TLS, pParent)
	, m_callback(_T(""))
	, m_chara(_T(""))
	, m_end_raw(_T(""))
	, m_index(_T(""))
	, m_start_raw(_T(""))
	, m_zero(_T(""))
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;
	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_TLS::CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_TLS, pParent)
	, m_callback(_T(""))
	, m_chara(_T(""))
	, m_end_raw(_T(""))
	, m_index(_T(""))
	, m_start_raw(_T(""))
	, m_zero(_T(""))
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;
	m_pDesc = m_pNt64->OptionalHeader.DataDirectory;
}

CPE_TLS::~CPE_TLS()
{
}

void CPE_TLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TLS_CALLBACK, m_callback);
	DDX_Text(pDX, IDC_TLS_CHARA, m_chara);
	DDX_Text(pDX, IDC_TLS_END_RAW, m_end_raw);
	DDX_Text(pDX, IDC_TLS_INDEX, m_index);
	DDX_Text(pDX, IDC_TLS_START_RAW, m_start_raw);
	DDX_Text(pDX, IDC_TLS_ZERO, m_zero);
}


BEGIN_MESSAGE_MAP(CPE_TLS, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_TLS 消息处理程序


void CPE_TLS::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CPE_TLS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_TLSList.AddColumn(2, L"索引", 100, L"函数地址", 300);
	if (m_pNt32)
	{
		GetTLSInfo32();
	}
	else
	{
		GetTLSInfo64();
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_TLS::GetTLSInfo32()
{
	CPE_FOA_Calc objcalc(m_pNt32);

	QWORD dwTLS_RVA = (QWORD)(pFileImageBase + objcalc.RVAtoFOA(m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));
	PIMAGE_TLS_DIRECTORY32 pTLS = (PIMAGE_TLS_DIRECTORY32)(dwTLS_RVA);

	m_start_raw.Format(L"%08X", pTLS->StartAddressOfRawData);
	m_end_raw.Format(L"%08X", pTLS->EndAddressOfRawData);
	m_index.Format(L"%08X", pTLS->AddressOfIndex);
	m_callback.Format(L"%08X", pTLS->AddressOfCallBacks);
	m_zero.Format(L"%08X", pTLS->SizeOfZeroFill);
	m_chara.Format(L"%08X", pTLS->Characteristics);

	//找到回调函数-
	QWORD dwRVA = pTLS->AddressOfCallBacks - m_pNt32->OptionalHeader.ImageBase;
	PWORD pFunAdd = (PWORD)(pFileImageBase + objcalc.RVAtoFOA(dwRVA));
	DWORD dwIndex = 0;
	CString strIndex;
	while (*pFunAdd)
	{
		strIndex.Format(L"%d", dwIndex);
		m_funAddress.Format(L"%08X", *pFunAdd);
		//m_TLSList.AddItem(2, strIndex, m_funAddress);
		pFunAdd++;
		dwIndex++;
	}
}



void CPE_TLS::GetTLSInfo64()
{
	CPE_FOA_Calc objcalc(m_pNt64);
	QWORD dwTLS_RVA = (QWORD)(pFileImageBase + objcalc.RVAtoFOA(m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));
	PIMAGE_TLS_DIRECTORY64 pTLS =(PIMAGE_TLS_DIRECTORY64)(dwTLS_RVA);

	m_start_raw.Format(L"%08X", pTLS->StartAddressOfRawData);
	m_end_raw.Format(L"%08X", pTLS->EndAddressOfRawData);
	m_index.Format(L"%08X", pTLS->AddressOfIndex);
	m_callback.Format(L"%08X", pTLS->AddressOfCallBacks);
	m_zero.Format(L"%08X", pTLS->SizeOfZeroFill);
	m_chara.Format(L"%08X", pTLS->Characteristics);

	//找到回调函数-
	QWORD dwRVA = pTLS->AddressOfCallBacks - m_pNt64->OptionalHeader.ImageBase;
	PWORD pFunAdd = (PWORD)(pFileImageBase + objcalc.RVAtoFOA(dwRVA));
	DWORD dwIndex = 0;
	CString strIndex;
	while (*pFunAdd)
	{
		strIndex.Format(L"%d", dwIndex);
		m_funAddress.Format(L"%08X", *pFunAdd);
		//m_TLSList.AddItem(2, strIndex, m_funAddress);
		pFunAdd++;
		dwIndex++;
	}
}


