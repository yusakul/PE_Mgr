#pragma once
#include "afxwin.h"



// CHome 对话框

class CHome : public CDialogEx
{
	DECLARE_DYNAMIC(CHome)

public:
	CHome(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHome();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void GetSysInfo();//获取系统版本信息

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_WinInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	
	afx_msg void OnBnClickedButtonStart();

	
};
