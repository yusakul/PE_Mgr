// Speed.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Speed.h"
#include "afxdialogex.h"
#include <Psapi.h>


// CSpeed 对话框

IMPLEMENT_DYNAMIC(CSpeed, CDialogEx)

CSpeed::CSpeed(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SPEED, pParent)
	, m_Edit_RamRate(_T(""))
	, m_Edit_UserRam(_T(""))
	, m_Edit_freeRam(_T(""))
	, m_Edit_totalRam(_T(""))
{

}

CSpeed::~CSpeed()
{
}

void CSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RAM1, m_Edit_RamRate);
	DDX_Text(pDX, IDC_EDIT_RAM2, m_Edit_UserRam);
	DDX_Text(pDX, IDC_EDIT_RAM3, m_Edit_freeRam);
	DDX_Text(pDX, IDC_EDIT_RAM4, m_Edit_totalRam);
}


BEGIN_MESSAGE_MAP(CSpeed, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEANRAM, &CSpeed::OnBnClickedButtonCleanram)
END_MESSAGE_MAP()


// CSpeed 消息处理程序

BOOL CSpeed::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetTimer(7864, 1000, NULL);
	//SetTimer(IDC_STATIC_HISTOGRAM_CPU, 1000, NULL);



	CRect rect;

	// 获取静态控件的句柄

	GetDlgItem(IDC_STATIC_HISTOGRAM_RAM)->GetWindowRect(rect);

	// 将该矩形转化为客户窗口

	ScreenToClient(rect);

	// 调用Create函数，并将上面的矩形传入

	m_ctrlHistogram.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, IDC_STATIC_HISTOGRAM_RAM);






	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSpeed::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//更新ram信息
	UpdateMemroyInfo();

	//更新ram占用率给瀑布图
	m_ctrlHistogram.SetPos((UINT)UpdateMemroyInfo());

	CDialogEx::OnTimer(nIDEvent);
}


//更新内存信息
int CSpeed::UpdateMemroyInfo()
{
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD toMem = (DWORD)(memStatus.dwTotalPhys / 1024 / 1024);		//总内存
	DWORD freeMem = (DWORD)(memStatus.dwAvailPhys / 1024 / 1024);		//空闲内存
	DWORD usedMem = (DWORD)(toMem - freeMem);
	DWORD ramRate = (DWORD)(100 * usedMem / toMem);

	//内存占用率
	UpdateData(TRUE);
	m_Edit_RamRate.Format(_T("%d"), ramRate);
	m_Edit_UserRam.Format(_T("%d"), usedMem);
	m_Edit_freeRam.Format(_T("%d"), freeMem);
	m_Edit_totalRam.Format(_T("%d"), toMem);
	UpdateData(FALSE);

	return (int)ramRate;
}


void CSpeed::OnBnClickedButtonCleanram()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取当前内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys = stcMemStatusEx.ullAvailPhys;

	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	//获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUserdMem = stcMemStatusEx.ullTotalPhys = stcMemStatusEx.ullAvailPhys;
}
