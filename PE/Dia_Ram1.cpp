// Dia_Ram1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Ram1.h"
#include "afxdialogex.h"


// CDia_Ram1 �Ի���

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


// CDia_Ram1 ��Ϣ�������


BOOL CDia_Ram1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetTimer(7864, 1000, NULL);
	//SetTimer(IDC_STATIC_HISTOGRAM_CPU, 1000, NULL);



	CRect rect;

	// ��ȡ��̬�ؼ��ľ��

	GetDlgItem(IDC_STATIC_HISTOGRAM_RAM)->GetWindowRect(rect);

	// ���þ���ת��Ϊ�ͻ�����

	ScreenToClient(rect);

	// ����Create��������������ľ��δ���

	m_ctrlHistogram.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, IDC_STATIC_HISTOGRAM_RAM);






	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDia_Ram1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����ram��Ϣ
	UpdateMemroyInfo();

	//����ramռ���ʸ��ٲ�ͼ
	m_ctrlHistogram.SetPos((UINT)UpdateMemroyInfo());

	CDialogEx::OnTimer(nIDEvent);
}


//�����ڴ���Ϣ
int CDia_Ram1::UpdateMemroyInfo()
{
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD toMem = (DWORD)(memStatus.dwTotalPhys / 1024 / 1024);		//���ڴ�
	DWORD freeMem = (DWORD)(memStatus.dwAvailPhys / 1024 / 1024);		//�����ڴ�
	DWORD usedMem = (DWORD)(toMem - freeMem);
	DWORD ramRate = (DWORD)(100 * usedMem / toMem);

	//�ڴ�ռ����
	UpdateData(TRUE);
	m_Edit_RamRate.Format(_T("%d"), ramRate);
	m_Edit_UserRam.Format(_T("%d"), usedMem);
	m_Edit_freeRam.Format(_T("%d"), freeMem);
	m_Edit_totalRam.Format(_T("%d"), toMem);
	UpdateData(FALSE);

	return (int)ramRate;
}
