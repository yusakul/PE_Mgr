#pragma once
#include "PE_FileHeader.h"
#include "PE_OptionalHeader.h"
#include "PE_Section.h"
#include "PE_FOA_Calc.h"
#include "PE_Time.h"
#include "PE_DIRECTORY.h"
#include "PEDATE.h"

// PEINFO 对话框

class PEINFO : public CDialogEx
{
	DECLARE_DYNAMIC(PEINFO)

public:
	PEINFO(CPE* pPE, CString szFilePath, CWnd* pParent = NULL);
	virtual ~PEINFO();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CString m_szFilePath;

	CPE* m_pPE;

	//获取文件信息
	void GetFileInfo();
	CString m_TableOffset;
	CString m_Checksum;
	CString m_InitialCSValue;
	CString m_InitialIPValue;
	CString m_InitialSPValue;
	CString m_InitialSSValue;
	CString m_MagicNumber;
	CString m_MaximumMemory;
	CString m_MinimumMemory;
	CString m_OEM_Identifier;
	CString m_OEM_Information;
	CString m_PagesInFile;
	CString m_OverlayNumer;
	CString m_PE_Address;
	CString m_Relocations;
	CString m_SizeOfHeader;
	CString m_ByteOnLastPage;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOpenfileheader();

	afx_msg void OnBnClickedOpenoptionalheader();
	afx_msg void OnBnClickedOpensections();
	afx_msg void OnBnClickedOpenposition();
	afx_msg void OnBnClickedOpentimecalc();
	afx_msg void OnBnClickedOpendirectory();


	
};
