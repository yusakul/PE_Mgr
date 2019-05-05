#pragma once
#include "afxwin.h"


// CPE_Time 对话框

class CPE_Time : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Time)

public:
	CPE_Time(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_Time(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_Time();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_TIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_TimeDate;//时间
	CString m_TimeStamps;//时间戳
	DWORD   m_dwTimeDateStamp;	//时间戳
	afx_msg void OnBnClickedButtonTimeok();//计算
	void TimeStamp2TimeDate(DWORD   TimeDateStamp);		//转换时间

	void TimeDate2TimeStamp(DWORD TimeDate);

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;


	CButton m_radioTimeStamps;
	CButton m_radioTimeDate;
	int flag_radio;

	afx_msg void OnBnClickedRadioTimestamps();
	afx_msg void OnBnClickedRadioTimedate();
	CEdit m_edit_stams;
	CEdit m_edit_date;
};
