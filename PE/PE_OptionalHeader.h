#pragma once


// CPE_OptionalHeader 对话框

class CPE_OptionalHeader : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_OptionalHeader)

public:
	CPE_OptionalHeader(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_OptionalHeader(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_OptionalHeader();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_OPTIONALHEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	CString m_BaseOfCode;
	CString m_BaseOfData;
	CString m_Checksum;
	CString m_DllCharacter;
	CString m_EntryPoint;
	CString m_FileAlignment;
	CString m_ImageBase;
	CString m_Magic;
	CString m_LoaderFlags;
	CString m_majorImageVersion;
	CString m_MajorLinkerVersion;
	CString m_MajorOsVersion;
	CString m_MajorSubSystemVersion;
	CString m_MinorImageVersion;
	CString m_MinorLinkerVersion;
	CString m_MinorOSVersion;
	CString m_MinorSubSystemVersion;
	CString m_NumberOfRvaAndSize;
	CString m_SectionAlignmemt;
	CString m_SizeOfCode;
	CString SizeOfHeaders;
	CString m_SizeOfHeapCommit;
	CString m_SizeOfHeapReserve;
	CString m_SizeOfImage;
	CString m_SizeOfInitData;
	CString m_SizeOfStackCommit;
	CString m_SizeOfStackReserve;
	CString m_SizeOfUnInitData;
	CString m_Subsystem;
	CString m_Win32Version;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	void GetOptionalHeaderInfo32();	//32位
	void GetOptionalHeaderInfo64();	//64位



public:
	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;
	
};
