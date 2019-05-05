#pragma once
#include "MyListCtrl.h"

// CPE_Section 对话框

class CPE_Section : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Section)

public:
	CPE_Section(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_Section(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_Section();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_SECTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();


	void GetSectionsInfo32();
	void GetSectionsInfo64();


	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;


	CString m_strNum;
	CString m_strName;
	CString m_strVS;
	CString m_strRVA;
	CString m_strFS;
	CString m_strFO;
	CString m_strCha;

	CMyListCtrl m_list;
};
