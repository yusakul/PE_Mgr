#pragma once
#include "AntiVirusScan.h"

// CAntiVirus 对话框

class CAntiVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiVirus)

public:
	CAntiVirus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAntiVirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANTI_VIRUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	DWORD m_scantype;

public:
	afx_msg void OnBnClickedButtonFastantivirus();		//快速
	afx_msg void OnBnClickedButtonAllantivirus();		//全盘
	afx_msg void OnBnClickedButtonCustomantivirus();	//自定义位置
	virtual BOOL OnInitDialog();						
	afx_msg void OnBnClickedButtonCustomfile();			//自定义文件
	afx_msg void OnBnClickedButtonMd5();				

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonnote();
};
