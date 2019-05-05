// Dia_Ram1.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Ram1.h"
#include "afxdialogex.h"


// CDia_Ram1 对话框

IMPLEMENT_DYNAMIC(CDia_Ram1, CDialogEx)

CDia_Ram1::CDia_Ram1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STATUS_RAM, pParent)
	, m_Edit_RamRate(_T(""))
	, m_Edit_UserRam(_T(""))
	, m_Edit_freeRam(_T(""))
	, m_Edit_totalRam(_T(""))
{

}

CDia_Ram1::~CDia_Ram1()
{
}

void CDia_Ram1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RAM1, m_Edit_RamRate);
	DDX_Text(pDX, IDC_EDIT_RAM2, m_Edit_UserRam);
	DDX_Text(pDX, IDC_EDIT_RAM3, m_Edit_freeRam);
	DDX_Text(pDX, IDC_EDIT_RAM4, m_Edit_totalRam);
}


BEGIN_MESSAGE_MAP(CDia_Ram1, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDia_Ram1 消息处理程序


BOOL CDia_Ram1::OnInitDialog()
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


void CDia_Ram1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//更新ram信息
	UpdateMemroyInfo();

	//更新ram占用率给瀑布图
	m_ctrlHistogram.SetPos((UINT)UpdateMemroyInfo());

	CDialogEx::OnTimer(nIDEvent);
}


//更新内存信息
int CDia_Ram1::UpdateMemroyInfo()
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
