// PE_Time.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Time.h"
#include "afxdialogex.h"
#include <time.h>


// CPE_Time 对话框

IMPLEMENT_DYNAMIC(CPE_Time, CDialogEx)

CPE_Time::CPE_Time(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_TIME, pParent)
	, m_TimeDate(_T(""))
	, m_TimeStamps(_T(""))
{
	m_pNt32 = pNt32;
}

CPE_Time::CPE_Time(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_TIME, pParent)
	, m_TimeDate(_T(""))
	, m_TimeStamps(_T(""))
{
	m_pNt64 = pNt64;
}

CPE_Time::~CPE_Time()
{
}

void CPE_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIMEDATE, m_TimeDate);
	DDX_Text(pDX, IDC_TIMESTAMPS, m_TimeStamps);
	DDX_Control(pDX, IDC_RADIO_TIMESTAMPS, m_radioTimeStamps);
	DDX_Control(pDX, IDC_RADIO_TIMEDATE, m_radioTimeDate);
	DDX_Control(pDX, IDC_TIMESTAMPS, m_edit_stams);
	DDX_Control(pDX, IDC_TIMEDATE, m_edit_date);
}


BEGIN_MESSAGE_MAP(CPE_Time, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_TimeOK, &CPE_Time::OnBnClickedButtonTimeok)
	ON_BN_CLICKED(IDC_RADIO_TIMESTAMPS, &CPE_Time::OnBnClickedRadioTimestamps)
	ON_BN_CLICKED(IDC_RADIO_TIMEDATE, &CPE_Time::OnBnClickedRadioTimedate)
END_MESSAGE_MAP()


// CPE_Time 消息处理程序


BOOL CPE_Time::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//默认选中第1项radio
	m_radioTimeStamps.SetCheck(TRUE);
	flag_radio = 1;	//radio选择标识默认为第1项

	if (m_pNt32)
	{
		m_dwTimeDateStamp = m_pNt32->FileHeader.TimeDateStamp;
	}
	else
	{
		m_dwTimeDateStamp = m_pNt64->FileHeader.TimeDateStamp;
	}
	
	TimeStamp2TimeDate(m_dwTimeDateStamp);
	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_Time::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


//计算按钮
void CPE_Time::OnBnClickedButtonTimeok()
{
	// TODO: 在此添加控件通知处理程序代码
	if(flag_radio=1)
	{
		//清空时间
		m_dwTimeDateStamp = 0;

		//重新获取时间
		UpdateData(TRUE);

		//计算
		m_dwTimeDateStamp = wcstoll(m_TimeStamps, NULL, 16);
		TimeStamp2TimeDate(m_dwTimeDateStamp);

		//刷新
		UpdateData(FALSE);
	}
	else if (flag_radio==2)
	{

	}
	
}


//时间戳转日期
void CPE_Time::TimeStamp2TimeDate(DWORD TimeDateStamp)
{
	m_TimeStamps = "";
	m_TimeDate = "";

	//获取时间
	time_t times = TimeDateStamp;

	char buf[65] = {};
	tm* FileTime = new tm{};

	//转换时间
	localtime_s(FileTime, &times);
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", FileTime);

	//显示
	m_TimeDate = buf;
	m_TimeStamps.Format(L"%08X", TimeDateStamp);
}


//日期转时间戳
void CPE_Time::TimeDate2TimeStamp(DWORD TimeDate)
{
	struct tm* tmp_time = new struct tm;
	//strptime??

}


//选择时间戳转日期
void CPE_Time::OnBnClickedRadioTimestamps()
{
	// TODO: 在此添加控件通知处理程序代码

	//修改对应edit控件是否可写
	m_edit_stams.SetReadOnly(FALSE);
	m_edit_date.SetReadOnly(TRUE);
	flag_radio = 1;

}


//选择日期转时间戳
void CPE_Time::OnBnClickedRadioTimedate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edit_stams.SetReadOnly(TRUE);
	m_edit_date.SetReadOnly(FALSE);
	flag_radio = 2;

}
