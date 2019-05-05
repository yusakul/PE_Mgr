#pragma once
#include "PE_FOA_Calc.h"

// CPE_TLS �Ի���

class CPE_TLS : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_TLS)

public:
	CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_TLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPE_TLS();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	PIMAGE_NT_HEADERS32 m_pNt32;			//NTͷָ��
	PIMAGE_NT_HEADERS64 m_pNt64;			//NTͷָ��

	PIMAGE_DATA_DIRECTORY m_pDesc;		//����Ŀ¼��
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//�ļ���ʼ

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

	void GetTLSInfo32();				   //��ȡTLS��Ϣ
	void GetTLSInfo64();				   //��ȡTLS��Ϣ

};
