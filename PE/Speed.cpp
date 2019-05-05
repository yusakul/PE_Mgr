// Speed.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Speed.h"
#include "afxdialogex.h"
#include <Psapi.h>


// CSpeed �Ի���

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


// CSpeed ��Ϣ�������

BOOL CSpeed::OnInitDialog()
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


void CSpeed::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����ram��Ϣ
	UpdateMemroyInfo();

	//����ramռ���ʸ��ٲ�ͼ
	m_ctrlHistogram.SetPos((UINT)UpdateMemroyInfo());

	CDialogEx::OnTimer(nIDEvent);
}


//�����ڴ���Ϣ
int CSpeed::UpdateMemroyInfo()
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


void CSpeed::OnBnClickedButtonCleanram()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ��ǰ�ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys = stcMemStatusEx.ullAvailPhys;

	//�����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	//��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUserdMem = stcMemStatusEx.ullTotalPhys = stcMemStatusEx.ullAvailPhys;
}
