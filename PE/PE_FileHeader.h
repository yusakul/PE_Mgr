#pragma once


// CPE_FileHeader �Ի���

class CPE_FileHeader : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_FileHeader)

public:
	CPE_FileHeader(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_FileHeader(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPE_FileHeader();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_FileHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
