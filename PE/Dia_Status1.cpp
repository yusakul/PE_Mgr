// Dia_Status1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Status1.h"
#include "afxdialogex.h"
#include "Data.h"


// CDia_Status1 �Ի���

IMPLEMENT_DYNAMIC(CDia_Status1, CDialogEx)

CDia_Status1::CDia_Status1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STATUS, pParent)
	, m_Edit_CPUrate(_T(""))
	, m_Edit_CPUspeed(_T(""))
	, m_CPU_CurrentIdle(_T(""))
	, m_CPU_CurrentMhz(_T(""))
	, m_CPU_MaxIdleState(_T(""))
	, m_CPU_MaxMhz(_T(""))
	, m_CPU_MhzLimit(_T(""))
	, m_CPU_Number(_T(""))
{

}

CDia_Status1::~CDia_Status1()
{
}

void CDia_Status1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CPU1, m_Edit_CPUrate);
	DDX_Text(pDX, IDC_EDIT_CPU_CurrentIdleState, m_CPU_CurrentIdle);
	DDX_Text(pDX, IDC_EDIT_CPU_CurrentMhz, m_CPU_CurrentMhz);
	DDX_Text(pDX, IDC_EDIT_CPU_MaxIdleState, m_CPU_MaxIdleState);
	DDX_Text(pDX, IDC_EDIT_CPU_MaxMhz, m_CPU_MaxMhz);
	//  DDX_Control(pDX, IDC_EDIT_CPU_MhzLimit, m_CPU_MhzLimit);
	DDX_Text(pDX, IDC_EDIT_CPU_MhzLimit, m_CPU_MhzLimit);
	DDX_Text(pDX, IDC_EDIT_CPU_Number, m_CPU_Number);
}


BEGIN_MESSAGE_MAP(CDia_Status1, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//ON_MESSAGE(UpdataCPUrate, &CDia_Status::OnUpdatacpurate)
	ON_MESSAGE(WM_MYMSG, OnUpdatacpurate)
END_MESSAGE_MAP()


// CDia_Status1 ��Ϣ�������




BOOL CDia_Status1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetTimer(IDC_STATIC_HISTOGRAM_CPU, 500, NULL);
	//SetTimer(IDC_STATIC_HISTOGRAM_CPU, 1000, NULL);

	CRect rect;

	// ��ȡ��̬�ؼ��ľ���

	GetDlgItem(IDC_STATIC_HISTOGRAM_CPU)->GetWindowRect(rect);

	// ���þ���ת��Ϊ�ͻ�����

	ScreenToClient(rect);

	// ����Create��������������ľ��δ���

	m_ctrlHistogram.Create(WS_VISIBLE | WS_CHILD

		| WS_TABSTOP, rect, this, IDC_STATIC_HISTOGRAM_CPU);





	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDia_Status1::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	DestroyWindow();
	CDialogEx::OnClose();
}


//����CPU��Ϣ
void CDia_Status1::UpdataCPUInfo()
{
	NTSTATUS Processor_information;
	PROCESSOR_POWER_INFORMATION* ppInfo = new PROCESSOR_POWER_INFORMATION[GetNumCPUs()];

	Processor_information = CallNtPowerInformation(ProcessorInformation
		, NULL, 0, ppInfo, GetNumCPUs() * sizeof(PROCESSOR_POWER_INFORMATION));



	UpdateData(TRUE);
	m_Edit_CPUrate.Format(_T("%d"), GetCPUuseRate());//ռ����
	m_CPU_MaxMhz = "4000.0";//���Ƶ��
	m_CPU_CurrentMhz.Format(_T("%d"), ppInfo->CurrentMhz*GetCPUuseRate()/100);//��ǰƵ��
	m_CPU_MhzLimit.Format(_T("%d"), ppInfo->MhzLimit);//����Ƶ��
	m_CPU_MaxIdleState.Format(_T("%d"), ppInfo->MaxIdleState);//������״̬
	m_CPU_CurrentIdle.Format(_T("%d"), ppInfo->CurrentIdleState);//��ǰ����״̬
	m_CPU_Number.Format(_T("%d"), GetNumCPUs());//������
	UpdateData(FALSE);

}

//����������
int  CDia_Status1::GetNumCPUs()
{
	SYSTEM_INFO system_info;
	memset(&system_info, 0x0, sizeof(system_info));//����
	GetSystemInfo(&system_info);
	// 	if (system_info.dwNumberOfProcessors == 0)//����������
	// 	{
	// 		return 1;
	// 	}
	// 	else
	return system_info.dwNumberOfProcessors;
}

//CPU����ʱ��
double CDia_Status1::FILETIME2DOUBLE(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}

//CPUʹ����
int CDia_Status1::GetCPUuseRate()
{
	_FILETIME idleTime, kernelTime, userTime;//����ʱ�䣬�ں�ʱ�䣬�û�ʱ��
											 //��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//�ȴ�1000����
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 50);
	/*PostMessage()*/
	::PostMessage(this->m_hWnd, WM_MYMSG, 0, 0);

	//��ȡ�µ�ʱ��
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	//������ʱ��ת��
	double dOldIdleTime = FILETIME2DOUBLE(idleTime);
	double dNewIdleTime = FILETIME2DOUBLE(newIdleTime);
	double dOldKernelTime = FILETIME2DOUBLE(kernelTime);
	double dNewKernelTime = FILETIME2DOUBLE(newKernelTime);
	double dOldUserTime = FILETIME2DOUBLE(userTime);
	double dNewUserTime = FILETIME2DOUBLE(newUserTime);

	//�����ʹ����
	int lResult = (int)((100.0 - 100.0*(dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)));

	//int lResult = 100*((dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)/(dNewIdleTime - dOldIdleTime));

	return lResult;
}


//��ʱִ�г���
void CDia_Status1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//����CPU��Ϣ
	UpdataCPUInfo();

	m_ctrlHistogram.SetPos((UINT)GetCPUuseRate());

	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CDia_Status1::OnUpdatacpurate(WPARAM wParam, LPARAM lParam)
{
	// 	UpdateData(TRUE);
	// 	m_Edit_CPUrate.Format(_T("%d"), GetCPUuseRate());
	// 	UpdateData(FALSE);

	return 0;
}



