// PE_Time.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Time.h"
#include "afxdialogex.h"
#include <time.h>


// CPE_Time �Ի���

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


// CPE_Time ��Ϣ�������


BOOL CPE_Time::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//Ĭ��ѡ�е�1��radio
	m_radioTimeStamps.SetCheck(TRUE);
	flag_radio = 1;	//radioѡ���ʶĬ��Ϊ��1��

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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPE_Time::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


//���㰴ť
void CPE_Time::OnBnClickedButtonTimeok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(flag_radio=1)
	{
		//���ʱ��
		m_dwTimeDateStamp = 0;

		//���»�ȡʱ��
		UpdateData(TRUE);

		//����
		m_dwTimeDateStamp = wcstoll(m_TimeStamps, NULL, 16);
		TimeStamp2TimeDate(m_dwTimeDateStamp);

		//ˢ��
		UpdateData(FALSE);
	}
	else if (flag_radio==2)
	{

	}
	
}


//ʱ���ת����
void CPE_Time::TimeStamp2TimeDate(DWORD TimeDateStamp)
{
	m_TimeStamps = "";
	m_TimeDate = "";

	//��ȡʱ��
	time_t times = TimeDateStamp;

	char buf[65] = {};
	tm* FileTime = new tm{};

	//ת��ʱ��
	localtime_s(FileTime, &times);
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", FileTime);

	//��ʾ
	m_TimeDate = buf;
	m_TimeStamps.Format(L"%08X", TimeDateStamp);
}


//����תʱ���
void CPE_Time::TimeDate2TimeStamp(DWORD TimeDate)
{
	struct tm* tmp_time = new struct tm;
	//strptime??

}


//ѡ��ʱ���ת����
void CPE_Time::OnBnClickedRadioTimestamps()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�޸Ķ�Ӧedit�ؼ��Ƿ��д
	m_edit_stams.SetReadOnly(FALSE);
	m_edit_date.SetReadOnly(TRUE);
	flag_radio = 1;

}


//ѡ������תʱ���
void CPE_Time::OnBnClickedRadioTimedate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edit_stams.SetReadOnly(TRUE);
	m_edit_date.SetReadOnly(FALSE);
	flag_radio = 2;

}
