// Home.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Home.h"
#include "afxdialogex.h"
#include <VersionHelpers.h>


//CAntiVirusScan AntiVirusScan(L"C:\\Windows", 0);
// CHome �Ի���

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


// CHome ��Ϣ�������



//��ȡϵͳ�汾��Ϣ
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetSysInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CHome::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
	
}



void CHome::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox(L"                            666666",0,MB_OKCANCEL)==1)
		OnBnClickedButtonStart();
}
