#pragma once


#include "HistogramCtrl.h"

//自定义消息
#define WM_MYMSG WM_USER+8

extern "C" {
#include <powrprof.h>	//获取CPU信息CallNtPowerInformation
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


// CDia_Status1 对话框

class CDia_Status1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Status1)

public:
	CDia_Status1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Status1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();


	//更新CPU信息
	void UpdataCPUInfo();

	//获取CPU核心数量
	int GetNumCPUs();


	double FILETIME2DOUBLE(const _FILETIME & fileTime);

	//CPU使用率
	int GetCPUuseRate();

	//创建瀑布图对象
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

	//自定义消息 即时刷新cpu利用率
	afx_msg LRESULT OnUpdatacpurate(WPARAM wParam, LPARAM lParam);
	
};
