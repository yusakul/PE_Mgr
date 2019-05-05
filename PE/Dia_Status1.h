#pragma once


#include "HistogramCtrl.h"

//�Զ�����Ϣ
#define WM_MYMSG WM_USER+8

extern "C" {
#include <powrprof.h>	//��ȡCPU��ϢCallNtPowerInformation
}
#pragma comment (lib, "powrprof.lib")
typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG  Number;
	ULONG  MaxMhz;
	ULONG  CurrentMhz;
	ULONG  MhzLimit;
	ULONG  MaxIdleState;
	ULONG  CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;


// CDia_Status1 �Ի���

class CDia_Status1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Status1)

public:
	CDia_Status1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Status1();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();


	//����CPU��Ϣ
	void UpdataCPUInfo();

	//��ȡCPU��������
	int GetNumCPUs();


	double FILETIME2DOUBLE(const _FILETIME & fileTime);

	//CPUʹ����
	int GetCPUuseRate();

	//�����ٲ�ͼ����
	CHistogramCtrl m_ctrlHistogram;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_Edit_CPUrate;
	CString m_Edit_CPUspeed;
	CString m_CPU_CurrentIdle;
	CString m_CPU_CurrentMhz;
	CString m_CPU_MaxIdleState;
	CString m_CPU_MaxMhz;
	//	CEdit m_CPU_MhzLimit;
	CString m_CPU_MhzLimit;
	CString m_CPU_Number;
protected:

	//�Զ�����Ϣ ��ʱˢ��cpu������
	afx_msg LRESULT OnUpdatacpurate(WPARAM wParam, LPARAM lParam);
	
};
