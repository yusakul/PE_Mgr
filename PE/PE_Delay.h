#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"

// CPE_Delay �Ի���

class CPE_Delay : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Delay)

public:
	CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPE_Delay();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DELAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();


	PIMAGE_NT_HEADERS32 m_pNt32;			//NTͷָ��
	PIMAGE_NT_HEADERS64 m_pNt64;			//NTͷָ��

	PIMAGE_DATA_DIRECTORY m_pDesc;		//����Ŀ¼��
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//�ļ���ʼ
	PIMAGE_THUNK_DATA32  m_pDelayThunk32;		//INT��
	PIMAGE_THUNK_DATA64  m_pDelayThunk64;		//INT��


	//list===================================================
	CMyListCtrl m_list_API;
	CMyListCtrl m_list_DLL;

	CString m_DelayDllName;					//DLL��
	CString m_DelayNameRVA;					//RVA
	CString m_DelayINT;						//INT
	CString m_DelayIAT;						//IAT

	CString m_DelayFunOrdinal;				//�������
	CString m_DelayFunName;					//��������

	void GetDelayDllInfo32();					//��ȡDLL��Ϣ
	void GetDelayFunInfo32();					//��ȡ������Ϣ
	void GetDelayDllInfo64();					//��ȡDLL��Ϣ
	void GetDelayFunInfo64();					//��ȡ������Ϣ
	afx_msg void OnNMClickListDelayDll(NMHDR *pNMHDR, LRESULT *pResult);
};
