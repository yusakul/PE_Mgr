#pragma once
#include "PE_Import.h"
#include "PE_EXPORT.h"
#include "PE_Resource.h"
#include "PE_Relocation.h"
#include "PE_TLS.h"
#include "PE_Delay.h"


// CPE_DIRECTORY 对话框

class CPE_DIRECTORY : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_DIRECTORY)

public:
	CPE_DIRECTORY(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_DIRECTORY(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPE_DIRECTORY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_rva_architectur;
	CString m_rva_basereloc;
	CString m_rva_boundimport;
	CString m_rva_comDescrip;
	CString m_rva_config;
	CString m_rva_debug;
	CString m_rva_delayImport;
	CString m_rva_exception;
	CString m_rva_export;
	CString m_rva_globalptr;
	CString m_rva_iat;
	CString m_rva_reserved;
	CString m_rva_import;
	CString m_rva_resource;
	CString m_rva_security;
	CString m_rva_tls;
	CString m_size_architectur;
	CString m_size_baseReloc;
	CString m_size_boundImport;
	CString m_size_comDescriptor;
	CString m_size_comfig;
	CString m_size_debug;
	CString m_size_delayImport;
	CString m_size_exception;
	CString m_size_export;
	CString m_size_globalptr;
	CString m_size_iat;
	CString m_size_import;
	CString m_size_reserved;
	CString m_size_resource;
	CString m_size_security;
	CString m_size_tls;

private:
	//绑定数组变量
	CString m_ArrRva[16];
	CString m_ArrSize[16];
	
	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;

	PIMAGE_DOS_HEADER m_pDos;

	//获取数据目录表信息
	void GetDirectoryInfo32();
	void GetDirectoryInfo64();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonOpenResource();
	afx_msg void OnBnClickedButtonOpenrelocation();
	afx_msg void OnBnClickedButtonOpentsl();
	afx_msg void OnBnClickedButtonOpendelay();
};
