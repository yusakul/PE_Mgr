// Dia_Disk1.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Disk1.h"
#include "afxdialogex.h"
#include <math.h>


// CDia_Disk1 对话框

IMPLEMENT_DYNAMIC(CDia_Disk1, CDialogEx)

CDia_Disk1::CDia_Disk1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DISK, pParent)
	, m_Edit_diskc(_T(""))
	, m_Edit_diskcc(_T(""))
{

}

CDia_Disk1::~CDia_Disk1()
{
}

void CDia_Disk1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DiskC, m_Edit_diskc);
	DDX_Text(pDX, IDC_EDIT_DiskCC, m_Edit_diskcc);
	DDX_Text(pDX, IDC_EDIT_DiskD, m_Edit_diskd);
	DDX_Text(pDX, IDC_EDIT_DiskDD, m_Edit_diskdd);
	DDX_Text(pDX, IDC_EDIT_DiskE, m_Edit_diske);
	DDX_Text(pDX, IDC_EDIT_DiskEE, m_Edit_diskee);
	DDX_Text(pDX, IDC_EDIT_DiskF, m_Edit_diskf);
	DDX_Text(pDX, IDC_EDIT_DiskFF, m_Edit_diskff);
	DDX_Text(pDX, IDC_EDIT_DiskG, m_Edit_diskg);
	DDX_Text(pDX, IDC_EDIT_DiskGG, m_Edit_diskgg);
	DDX_Text(pDX, IDC_EDIT_DiskH, m_Edit_diskh);
	DDX_Text(pDX, IDC_EDIT_DiskHH, m_Edit_diskhh);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress_C);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress_D);
	DDX_Control(pDX, IDC_PROGRESS3, m_progress_E);
	DDX_Control(pDX, IDC_PROGRESS4, m_progress_F);
	DDX_Control(pDX, IDC_PROGRESS5, m_progress_G);
	DDX_Control(pDX, IDC_PROGRESS6, m_progress_H);
}


BEGIN_MESSAGE_MAP(CDia_Disk1, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDia_Disk1 消息处理程序


BOOL CDia_Disk1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(8888, 6000, NULL);//一分钟刷新一次
	GetDiskSta();

	//初始化进度条

	//设置进度条的起始值和终止值
	m_progress_C.SetRange(0, 100);
	m_progress_D.SetRange(0, 100);
	m_progress_E.SetRange(0, 100);
	m_progress_F.SetRange(0, 100);
	m_progress_G.SetRange(0, 100);
	m_progress_H.SetRange(0, 100);

	m_progress_C.SetPos(100 - Rate_C);
	m_progress_D.SetPos(100 - Rate_D);
	m_progress_E.SetPos(100 - Rate_E);
	m_progress_F.SetPos(100 - Rate_F);
	m_progress_G.SetPos(100 - Rate_G);
	m_progress_H.SetPos(100 - Rate_H);

	// 	SetWindowLong(m_progress_C.m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | PBM_SETMARQUEE| PBM_SETBKCOLOR);
	// 	m_progress_C.SetMarquee(TRUE, 30);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDia_Disk1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	GetDiskSta();

	CDialogEx::OnTimer(nIDEvent);
}

//获取磁盘信息并更新
void CDia_Disk1::GetDiskSta()
{
	//硬盘使用率 调用windows API  
	//已使用容量 总容量 剩余容量
	ULARGE_INTEGER FreeBytesAvailableC, TotalNumberOfBytesC, TotalNumberOfFreeBytesC;
	ULARGE_INTEGER FreeBytesAvailableD, TotalNumberOfBytesD, TotalNumberOfFreeBytesD;
	ULARGE_INTEGER FreeBytesAvailableE, TotalNumberOfBytesE, TotalNumberOfFreeBytesE;
	ULARGE_INTEGER FreeBytesAvailableF, TotalNumberOfBytesF, TotalNumberOfFreeBytesF;
	ULARGE_INTEGER FreeBytesAvailableG, TotalNumberOfBytesG, TotalNumberOfFreeBytesG;
	ULARGE_INTEGER FreeBytesAvailableH, TotalNumberOfBytesH, TotalNumberOfFreeBytesH;

	GetDiskFreeSpaceEx(_T("C:"), &FreeBytesAvailableC, &TotalNumberOfBytesC, &TotalNumberOfFreeBytesC);
	GetDiskFreeSpaceEx(_T("D:"), &FreeBytesAvailableD, &TotalNumberOfBytesD, &TotalNumberOfFreeBytesD);
	GetDiskFreeSpaceEx(_T("E:"), &FreeBytesAvailableE, &TotalNumberOfBytesE, &TotalNumberOfFreeBytesE);
	GetDiskFreeSpaceEx(_T("F:"), &FreeBytesAvailableF, &TotalNumberOfBytesF, &TotalNumberOfFreeBytesF);
	GetDiskFreeSpaceEx(_T("G:"), &FreeBytesAvailableG, &TotalNumberOfBytesG, &TotalNumberOfFreeBytesG);
	GetDiskFreeSpaceEx(_T("H:"), &FreeBytesAvailableH, &TotalNumberOfBytesH, &TotalNumberOfFreeBytesH);

	/* uLargeInt.LowPart /num1;
	然后对高32位进行运算： uLargeInt.HighPart *（(2^32)/num1）；*/
	DOUBLE  AC = (FreeBytesAvailableC.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableC.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  ACC = (TotalNumberOfBytesC.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesC.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_C = (int)(100.0 * AC / ACC);

	DOUBLE  AD = (FreeBytesAvailableD.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableD.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  ADD = (TotalNumberOfBytesD.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesD.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_D = (int)(100.0 * AD / ADD);

	DOUBLE  AE = (FreeBytesAvailableE.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableE.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  AEE = (TotalNumberOfBytesE.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesE.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_E = (int)(100.0 * AE / AEE);

	DOUBLE  AF = (FreeBytesAvailableF.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableF.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  AFF = (TotalNumberOfBytesF.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesF.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_F = (int)(100.0 * AF / AFF);

	DOUBLE  AG = (FreeBytesAvailableG.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableG.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  AGG = (TotalNumberOfBytesG.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesG.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_G = (int)(100.0 * AG / AGG);

	DOUBLE  AH = (FreeBytesAvailableH.LowPart / 1024.0 / 1024 / 1024) + (FreeBytesAvailableH.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	DOUBLE  AHH = (TotalNumberOfBytesH.LowPart / 1024.0 / 1024 / 1024) + (TotalNumberOfBytesH.HighPart*(pow(2, 32) / 1024.0 / 1024 / 1024));
	Rate_H = (int)(100.0 * AH / AHH);

	UpdateData(TRUE);
	m_Edit_diskc.Format(_T("%0.1lfGB /"), AC);
	m_Edit_diskcc.Format(_T("%0.1lfGB"), ACC);

	m_Edit_diskd.Format(_T("%0.1lfGB /"), AD);
	m_Edit_diskdd.Format(_T("%0.1lfGB"), ADD);

	m_Edit_diske.Format(_T("%0.1lfGB /"), AE);
	m_Edit_diskee.Format(_T("%0.1lfGB"), AEE);

	m_Edit_diskf.Format(_T("%0.1lfGB /"), AF);
	m_Edit_diskff.Format(_T("%0.1lfGB"), AFF);

	m_Edit_diskg.Format(_T("%0.1lfGB /"), AG);
	m_Edit_diskgg.Format(_T("%0.1lfGB"), AGG);

	m_Edit_diskh.Format(_T("%0.1lfGB /"), AH);
	m_Edit_diskhh.Format(_T("%0.1lfGB"), AHH);

	UpdateData(FALSE);

}

