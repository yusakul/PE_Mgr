// Home.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Home.h"
#include "afxdialogex.h"
#include <VersionHelpers.h>


//CAntiVirusScan AntiVirusScan(L"C:\\Windows", 0);
// CHome 对话框

IMPLEMENT_DYNAMIC(CHome, CDialogEx)

CHome::CHome(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HOME, pParent)
{

}

CHome::~CHome()
{
}

void CHome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WinInfo, m_WinInfo);
}


BEGIN_MESSAGE_MAP(CHome, CDialogEx)
	
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CHome::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CHome 消息处理程序



//获取系统版本信息
void CHome::GetSysInfo()
{
	CString sysverion;


	if (IsWindowsXPSP3OrGreater())
	{
		sysverion = L"Windows XP SP30";
	}

	if (IsWindowsVistaOrGreater())
	{
		sysverion = L"Windows Viata";
	}

	if (IsWindows7OrGreater())
	{
		sysverion = L"Windows 7";
	}

	if (IsWindows8OrGreater())
	{
		sysverion = L"Windows 10";
	}

	if (IsWindows8Point1OrGreater())
	{
		sysverion = L"Windows 10";
	}

	m_WinInfo.SetWindowTextW(sysverion);
}



BOOL CHome::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetSysInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHome::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
	
}



void CHome::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MessageBox(L"                            666666",0,MB_OKCANCEL)==1)
		OnBnClickedButtonStart();
}
