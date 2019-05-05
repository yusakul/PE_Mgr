#pragma once


// CPE_FileHeader 对话框

class CPE_FileHeader : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_FileHeader)

public:
	CPE_FileHeader(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_FileHeader(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPE_FileHeader();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_FileHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Character;
	CString m_COFF;
	CString m_PointerToSym;
	CString m_SizeOfOptional;
	CString m_Time;
	CString m_Machine;
	CString m_NumberOfSections;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	void GetFileHeaderInfo32();
	void GetFileHeaderInfo64();


	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;



};
