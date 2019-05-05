#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"


// CPE_Import �Ի���

class CPE_Import : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Import)

public:
	CPE_Import(CWnd* pParent = NULL);   // ��׼���캯��
	CPE_Import(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 
	CPE_Import(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 


	virtual ~CPE_Import();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;

	PIMAGE_DATA_DIRECTORY m_pDesc;		//����Ŀ¼��
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;




	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_list_API;
	CMyListCtrl m_list_DLL;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	//��ȡDLL��Ϣ
	void GetDLLInfo32();
	void GetDLLInfo64();

	//��ȡAPI��Ϣ
	void GetAPIInof32(PIMAGE_THUNK_DATA32 FirstThunk);
	void GetAPIInof64(PIMAGE_THUNK_DATA64 FirstThunk);


	

	CString m_DLL_Name;
	CString m_DLL_OriginalFirstThunk;
	CString m_DLL_TimeDateStamp;
	CString m_DLL_ForwarderChain;
	CString m_DLL_NameRVA;
	CString m_DLL_FirstThunk;

	CString m_API_Name;
	CString m_API_ThunkRVA;
	CString m_API_ThunkFOA;
	CString m_API_ThunkValue;
	CString m_API_Hint;

	
	afx_msg void OnNMClickListDllImport(NMHDR *pNMHDR, LRESULT *pResult);
};
