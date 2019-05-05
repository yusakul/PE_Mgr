// PEINFO.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PEINFO.h"
#include "afxdialogex.h"
#include "PEDATE.h"

// PEINFO 对话框

IMPLEMENT_DYNAMIC(PEINFO, CDialogEx)

PEINFO::PEINFO(CPE* pPE,  CString szFilePath, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEINFO, pParent)
	, m_ByteOnLastPage(_T(""))
	, m_TableOffset(_T(""))
	, m_Checksum(_T(""))
	, m_InitialCSValue(_T(""))
	, m_InitialIPValue(_T(""))
	, m_InitialSPValue(_T(""))
	, m_InitialSSValue(_T(""))
	, m_MagicNumber(_T(""))
	, m_MaximumMemory(_T(""))
	, m_MinimumMemory(_T(""))
	, m_OEM_Identifier(_T(""))
	, m_OEM_Information(_T(""))
	, m_PagesInFile(_T(""))
	, m_OverlayNumer(_T(""))
	, m_PE_Address(_T(""))
	, m_Relocations(_T(""))
	, m_SizeOfHeader(_T(""))
{
	m_szFilePath = szFilePath;
	m_pPE = pPE;
}



PEINFO::~PEINFO()
{
}

void PEINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Table_Offset, m_TableOffset);
	DDX_Text(pDX, IDC_EDIT_Checksum, m_Checksum);
	DDX_Text(pDX, IDC_EDIT_Initial_CS_value, m_InitialCSValue);
	DDX_Text(pDX, IDC_EDIT_Initial_IP_value, m_InitialIPValue);
	DDX_Text(pDX, IDC_EDIT_Initial_SP_value, m_InitialSPValue);
	DDX_Text(pDX, IDC_EDIT_Initial_SS_value, m_InitialSSValue);
	DDX_Text(pDX, IDC_EDIT_Magic_number, m_MagicNumber);
	DDX_Text(pDX, IDC_EDIT_Maximum_Memory, m_MaximumMemory);
	DDX_Text(pDX, IDC_EDIT_MinimumMemory, m_MinimumMemory);
	DDX_Text(pDX, IDC_EDIT_OEM_Identifier, m_OEM_Identifier);
	DDX_Text(pDX, IDC_EDIT_OEM_Information, m_OEM_Information);
	DDX_Text(pDX, IDC_EDIT_Pages_in_FIle, m_PagesInFile);
	DDX_Text(pDX, IDC_EDIT_Overlay_number, m_OverlayNumer);
	DDX_Text(pDX, IDC_EDIT_PE_Address, m_PE_Address);
	DDX_Text(pDX, IDC_EDIT_Relocations, m_Relocations);
	DDX_Text(pDX, IDC_EDIT_SizeOfHeader, m_SizeOfHeader);
	DDX_Text(pDX, IDC_EDIT_ByteOnLastPage, m_ByteOnLastPage);
}




void PEINFO::GetFileInfo()
{
	UpdateData(TRUE);
	//获取文件信息


	//使用PIMAGE_DOS_HEADER(占64字节）解释前64个字节
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pPE->m_pFileImageBase;

	m_MagicNumber.Format(L"%04X", pDos->e_magic);
	m_ByteOnLastPage.Format(L"%04X", pDos->e_cblp);
	m_PagesInFile.Format(L"%04X", pDos->e_cp);
	m_Relocations.Format(L"%04X", pDos->e_crlc);
	m_SizeOfHeader.Format(L"%04X", pDos->e_cparhdr);
	m_MinimumMemory.Format(L"%04X", pDos->e_minalloc);
	m_MaximumMemory.Format(L"%04X", pDos->e_ss);
	m_InitialSSValue.Format(L"%04X", pDos->e_sp);
	m_InitialSPValue.Format(L"%04X", pDos->e_csum);
	m_Checksum.Format(L"%04X", pDos->e_ip);
	m_InitialIPValue.Format(L"%04X", pDos->e_cs);
	m_InitialCSValue.Format(L"%04X", pDos->e_lfarlc);
	m_TableOffset.Format(L"%04X", pDos->e_ovno);
	m_OverlayNumer.Format(L"%04X", pDos->e_res[4]);
	m_OEM_Identifier.Format(L"%04X", pDos->e_oemid);
	m_OEM_Information.Format(L"%04X", pDos->e_oeminfo);
	//m_MagicNumber.Format(L"%04X", pDos->e_res2[10]);
	m_PE_Address.Format(L"%04X", pDos->e_lfanew);
	
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(PEINFO, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(Button_OpenFileHeader, &PEINFO::OnBnClickedOpenfileheader)
	ON_BN_CLICKED(Button_OpenOptionalHeader, &PEINFO::OnBnClickedOpenoptionalheader)
	ON_BN_CLICKED(Button_OpenSections, &PEINFO::OnBnClickedOpensections)
	ON_BN_CLICKED(Button_OpenPosition, &PEINFO::OnBnClickedOpenposition)
	ON_BN_CLICKED(Button_OpenTimeCalc, &PEINFO::OnBnClickedOpentimecalc)
	ON_BN_CLICKED(Button_OpenDirectory, &PEINFO::OnBnClickedOpendirectory)
END_MESSAGE_MAP()


// PEINFO 消息处理程序


BOOL PEINFO::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//m_pPE->ReadFileToMem(m_szFilePath);
	//m_pPE->IsPEFile();


	GetFileInfo();

	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void PEINFO::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnClose();
}


//打开文件头信息窗口
void PEINFO::OnBnClickedOpenfileheader()
{
	// TODO: 在此添加控件通知处理程序代码

	
	if (m_pPE->m_pNt32)
	{
		CPE_FileHeader cfileheader(m_pPE->m_pNt32);
		cfileheader.DoModal();
	}
	else
	{
		CPE_FileHeader cfileheader(m_pPE->m_pNt64);
		cfileheader.DoModal();
	}
	



}

//打开扩展头信息窗口
void PEINFO::OnBnClickedOpenoptionalheader()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pPE->m_pNt32)
	{
		CPE_OptionalHeader cOpheader(m_pPE->m_pNt32);
		cOpheader.DoModal();
	}
	else
	{
		CPE_OptionalHeader cOpheader(m_pPE->m_pNt64);
		cOpheader.DoModal();
	}
}

//打开区段表信息窗口
void PEINFO::OnBnClickedOpensections()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pPE->m_pNt32)
	{

		CPE_Section cSecions(m_pPE->m_pNt32);
		cSecions.DoModal();
	}
	else
	{
		CPE_Section cSecions(m_pPE->m_pNt64);
		cSecions.DoModal();
	}
}

//打开位置计算器
void PEINFO::OnBnClickedOpenposition()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pPE->m_pNt32)
	{

		CPE_FOA_Calc cFoaCalc(m_pPE->m_pNt32);
		cFoaCalc.DoModal();
	}
	else
	{
		CPE_FOA_Calc cFoaCalc(m_pPE->m_pNt64);
		cFoaCalc.DoModal();
	}
}

//打开时间计算器
void PEINFO::OnBnClickedOpentimecalc()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (m_pPE->m_pNt32)
	{
		CPE_Time cTime(m_pPE->m_pNt32);
		cTime.DoModal();
	}
	else
	{
		CPE_Time cTime(m_pPE->m_pNt64);
		cTime.DoModal();
	}

}


//打开目录表
void PEINFO::OnBnClickedOpendirectory()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pPE->m_pNt32)
	{
		CPE_DIRECTORY cDire(m_pPE->m_pDosHead, m_pPE->m_pNt32);
		cDire.DoModal();
	}
	else
	{
		CPE_DIRECTORY cDire(m_pPE->m_pDosHead, m_pPE->m_pNt64);
		cDire.DoModal();
	}
}
