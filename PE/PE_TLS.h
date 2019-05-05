#pragma once
#include "PE_FOA_Calc.h"

// CPE_TLS 对话框

class CPE_TLS : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_TLS)

public:
	CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_TLS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	PIMAGE_NT_HEADERS32 m_pNt32;			//NT头指针
	PIMAGE_NT_HEADERS64 m_pNt64;			//NT头指针

	PIMAGE_DATA_DIRECTORY m_pDesc;		//数据目录表
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//文件起始

public:

	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

	CString m_callback;
	CString m_chara;
	CString m_end_raw;
	CString m_index;
	CString m_start_raw;
	CString m_zero;

	CString m_funAddress;
	CString m_TLSList;

	void GetTLSInfo32();				   //获取TLS信息
	void GetTLSInfo64();				   //获取TLS信息

};
