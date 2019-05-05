#pragma once
#include "HistogramCtrl.h"


// CSpeed 对话框

class CSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(CSpeed)

public:
	CSpeed(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpeed();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPEED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CString m_Edit_RamRate;
	CString m_Edit_UserRam;
	CString m_Edit_freeRam;
	CString m_Edit_totalRam;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//更新内存信息
	int UpdateMemroyInfo();

	//创建瀑布图对象
	CHistogramCtrl m_ctrlHistogram;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCleanram();
};
