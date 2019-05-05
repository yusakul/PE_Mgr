// PE_DIRECTORY.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_DIRECTORY.h"
#include "afxdialogex.h"


// CPE_DIRECTORY 对话框

IMPLEMENT_DYNAMIC(CPE_DIRECTORY, CDialogEx)

CPE_DIRECTORY::CPE_DIRECTORY(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIRECTORY, pParent)
	, m_rva_architectur(_T(""))
	, m_rva_basereloc(_T(""))
	, m_rva_boundimport(_T(""))
	, m_rva_comDescrip(_T(""))
	, m_rva_config(_T(""))
	, m_rva_debug(_T(""))
	, m_rva_delayImport(_T(""))
	, m_rva_exception(_T(""))
	, m_rva_export(_T(""))
	, m_rva_globalptr(_T(""))
	, m_rva_iat(_T(""))
	, m_rva_reserved(_T(""))
	, m_rva_import(_T(""))
	, m_rva_resource(_T(""))
	, m_rva_security(_T(""))
	, m_rva_tls(_T(""))
	, m_size_architectur(_T(""))
	, m_size_baseReloc(_T(""))
	, m_size_boundImport(_T(""))
	, m_size_comDescriptor(_T(""))
	, m_size_comfig(_T(""))
	, m_size_debug(_T(""))
	, m_size_delayImport(_T(""))
	, m_size_exception(_T(""))
	, m_size_export(_T(""))
	, m_size_globalptr(_T(""))
	, m_size_iat(_T(""))
	, m_size_import(_T(""))
	, m_size_reserved(_T(""))
	, m_size_resource(_T(""))
	, m_size_security(_T(""))
	, m_size_tls(_T(""))
{
	m_pDos = pDos;
	m_pNt32 = pNt32;
}

CPE_DIRECTORY::CPE_DIRECTORY(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIRECTORY, pParent)
	, m_rva_architectur(_T(""))
	, m_rva_basereloc(_T(""))
	, m_rva_boundimport(_T(""))
	, m_rva_comDescrip(_T(""))
	, m_rva_config(_T(""))
	, m_rva_debug(_T(""))
	, m_rva_delayImport(_T(""))
	, m_rva_exception(_T(""))
	, m_rva_export(_T(""))
	, m_rva_globalptr(_T(""))
	, m_rva_iat(_T(""))
	, m_rva_reserved(_T(""))
	, m_rva_import(_T(""))
	, m_rva_resource(_T(""))
	, m_rva_security(_T(""))
	, m_rva_tls(_T(""))
	, m_size_architectur(_T(""))
	, m_size_baseReloc(_T(""))
	, m_size_boundImport(_T(""))
	, m_size_comDescriptor(_T(""))
	, m_size_comfig(_T(""))
	, m_size_debug(_T(""))
	, m_size_delayImport(_T(""))
	, m_size_exception(_T(""))
	, m_size_export(_T(""))
	, m_size_globalptr(_T(""))
	, m_size_iat(_T(""))
	, m_size_import(_T(""))
	, m_size_reserved(_T(""))
	, m_size_resource(_T(""))
	, m_size_security(_T(""))
	, m_size_tls(_T(""))
{
	m_pDos = pDos;
	m_pNt64 = pNt64;
}

CPE_DIRECTORY::~CPE_DIRECTORY()
{
}

void CPE_DIRECTORY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RVA_EXPORT, m_ArrRva[0]);
	DDX_Text(pDX, IDC_EDIT_RVA_IMPORT, m_ArrRva[1]);
	DDX_Text(pDX, IDC_EDIT_RVA_RESOURCE, m_ArrRva[2]);
	DDX_Text(pDX, IDC_EDIT_RVA_EXCEPTION, m_ArrRva[3]);
	DDX_Text(pDX, IDC_EDIT_RVA_SECURITY, m_ArrRva[4]);
	DDX_Text(pDX, IDC_EDIT_RVA_BASERELOC, m_ArrRva[5]);
	DDX_Text(pDX, IDC_EDIT_RVA_DEBUG, m_ArrRva[6]);
	DDX_Text(pDX, IDC_EDIT_RVA_ARCHITECTURE, m_ArrRva[7]);
	DDX_Text(pDX, IDC_EDIT_RVA_GLOBALPTR, m_ArrRva[8]);
	DDX_Text(pDX, IDC_EDIT_RVA_TLS, m_ArrRva[9]);
	DDX_Text(pDX, IDC_EDIT_RVA_CONFIG, m_ArrRva[10]);
	DDX_Text(pDX, IDC_EDIT_RVA_BOUND_IMPORT, m_ArrRva[11]);
	DDX_Text(pDX, IDC_EDIT_RVA_IAT, m_ArrRva[12]);
	DDX_Text(pDX, IDC_EDIT_RVA_DELAY_IMPORT, m_ArrRva[13]);
	DDX_Text(pDX, IDC_EDIT_RVA_COM_DESCRIPTOR, m_ArrRva[14]);
	DDX_Text(pDX, IDC_EDIT_RVA_RESERVED, m_ArrRva[15]);

	DDX_Text(pDX, IDC_EDIT_SIZE_EXPORT, m_ArrSize[0]);
	DDX_Text(pDX, IDC_EDIT_SIZE_IMPORT, m_ArrSize[1]);
	DDX_Text(pDX, IDC_EDIT_SIZE_RESOURCE, m_ArrSize[2]);
	DDX_Text(pDX, IDC_EDIT_SIZE_EXCEPTION, m_ArrSize[3]);
	DDX_Text(pDX, IDC_EDIT_SIZE_SECURITY, m_ArrSize[4]);
	DDX_Text(pDX, IDC_EDIT_SIZE_BASERELOC, m_ArrSize[5]);
	DDX_Text(pDX, IDC_EDIT_SIZE_DEBUG, m_ArrSize[6]);
	DDX_Text(pDX, IDC_EDIT_SIZE_ARCHITECTURE, m_ArrSize[7]);
	DDX_Text(pDX, IDC_EDIT_SIZE_GLOBALPTR, m_ArrSize[8]);
	DDX_Text(pDX, IDC_EDIT_SIZE_TLS, m_ArrSize[9]);
	DDX_Text(pDX, IDC_EDIT_SIZE_CONFIG, m_ArrSize[10]);
	DDX_Text(pDX, IDC_EDIT_SIZE_BOUND_IMPORT, m_ArrSize[11]);
	DDX_Text(pDX, IDC_EDIT_SIZE_IAT, m_ArrSize[12]);
	DDX_Text(pDX, IDC_EDIT_SIZE_DELAY_IMPORT, m_ArrSize[13]);
	DDX_Text(pDX, IDC_EDIT_SIZE_COM_DESCRIPTOR, m_ArrSize[14]);
	DDX_Text(pDX, IDC_EDIT_SIZE_RESERVED, m_ArrSize[15]);
}

//获取数据目录表信息
void CPE_DIRECTORY::GetDirectoryInfo32()
{
	for (int i = 0; i < 16; i++)
	{
		m_ArrRva[i].Format(L"%08X", m_pNt32->OptionalHeader.DataDirectory[i].VirtualAddress);
		m_ArrSize[i].Format(L"%08X", m_pNt32->OptionalHeader.DataDirectory[i].Size);
	}
}

//获取数据目录表信息
void CPE_DIRECTORY::GetDirectoryInfo64()
{
	for (int i = 0; i < 16; i++)
	{
		m_ArrRva[i].Format(L"%08X", m_pNt64->OptionalHeader.DataDirectory[i].VirtualAddress);
		m_ArrSize[i].Format(L"%08X", m_pNt64->OptionalHeader.DataDirectory[i].Size);
	}
}


BEGIN_MESSAGE_MAP(CPE_DIRECTORY, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CPE_DIRECTORY::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CPE_DIRECTORY::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RESOURCE, &CPE_DIRECTORY::OnBnClickedButtonOpenResource)
	ON_BN_CLICKED(IDC_BUTTON_OPENRELOCATION, &CPE_DIRECTORY::OnBnClickedButtonOpenrelocation)
	ON_BN_CLICKED(IDC_BUTTON_OPENTSL, &CPE_DIRECTORY::OnBnClickedButtonOpentsl)
	ON_BN_CLICKED(IDC_BUTTON_OPENDELAY, &CPE_DIRECTORY::OnBnClickedButtonOpendelay)
END_MESSAGE_MAP()


// CPE_DIRECTORY 消息处理程序


BOOL CPE_DIRECTORY::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_pNt32)
	{
		GetDirectoryInfo32();
	}
	else
	{
		GetDirectoryInfo64();
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_DIRECTORY::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


//打开导出表详细目录
void CPE_DIRECTORY::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_EXPORT cExport(m_pDos, m_pNt32);
		cExport.DoModal();
	}
	else
	{
		CPE_EXPORT cExport(m_pDos, m_pNt64);
		cExport.DoModal();
	}
}


//打开导入表详细目录
void CPE_DIRECTORY::OnBnClickedButtonImport()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_Import cImport(m_pDos, m_pNt32);
		cImport.DoModal();
	}
	else
	{
		CPE_Import cImport(m_pDos, m_pNt64);
		cImport.DoModal();
	}
	

}


//打开资源目录表
void CPE_DIRECTORY::OnBnClickedButtonOpenResource()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_Resource cResource(m_pDos, m_pNt32);
		cResource.DoModal();
	}
	else
	{
		CPE_Resource cResource(m_pDos, m_pNt64);
		cResource.DoModal();
	}

}


void CPE_DIRECTORY::OnBnClickedButtonOpenrelocation()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_Relocation cRelocation(m_pDos, m_pNt32);
		cRelocation.DoModal();
	}
	else
	{
		CPE_Relocation cRelocation(m_pDos, m_pNt64);
		cRelocation.DoModal();
	}
}


//打开TLS表
void CPE_DIRECTORY::OnBnClickedButtonOpentsl()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_TLS tls(m_pDos, m_pNt32);
		tls.DoModal();
	}
	else
	{
		CPE_TLS tls(m_pDos, m_pNt64);
		tls.DoModal();
	}

}


void CPE_DIRECTORY::OnBnClickedButtonOpendelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pNt32)
	{
		CPE_Delay delay(m_pDos, m_pNt32);
		delay.DoModal();
	}
	else
	{
		CPE_Delay delay(m_pDos, m_pNt64);
		delay.DoModal();
	}
}
