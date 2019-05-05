#pragma once
#include "afxcmn.h"
#include "PE_FOA_Calc.h"


// CPE_Resource 对话框

class CPE_Resource : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Resource)

public:
	CPE_Resource(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_Resource(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPE_Resource();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOURCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;
	PIMAGE_DATA_DIRECTORY m_pDesc;		//数据目录表
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//文件起始

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_Resource_ID1;
	CString m_Resource_ID2;
	CString m_Resource_Name1;
	CString m_Resource_Name2;
	CString m_Resource_Offset;
	CString m_Resource_Rva;
	CString m_Resource_Size;

	CTreeCtrl m_Tree_Resource;
	HTREEITEM hParent;
	HTREEITEM hInsertAfter;



	void ShowResource32();

	void ShowResource64();

};
