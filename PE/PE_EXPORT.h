#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"
#include <vector>

using namespace std;


typedef struct EXPORTDIRECTORY
{
	DWORD nID;			//�������
	DWORD dwRVA;		//����RVA
	DWORD dwOffset;
	CString strName;	//��������
}EXPORTDIRECTORY, *PEXPORTDIRECTORY;

// CPE_EXPORT �Ի���
class CPE_EXPORT : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_EXPORT)

public:
	CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPE_EXPORT();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();


	CString strName;
		//RVA
	CString strRVA;
		//FOA
	CString strOffset;
		//���
	CString strID;

	//����
	CString m_AddressOfFunctions;
	CString m_AddressofNameOrdinals;
	CString m_AddressOfNames;
	CString m_Base;
	CString m_characteristrics;
	CString m_MajorVersion;
	CString m_MinorVersion;
	CString m_Name;
	CString m_NameAddr;
	CString m_NumberOfFunctions;
	CString m_NumberOfNames;
	CString m_TimeStamp;
	CMyListCtrl m_list_export;



	//���õ�������Ϣ32λ
	void SetExportDirectoryInfo32();
	//���õ�������Ϣ64λ
	void SetExportDirectoryInfo64();



	PIMAGE_NT_HEADERS32 m_pNt32;			//NTͷָ��
	PIMAGE_NT_HEADERS64 m_pNt64;			//NTͷָ��

	PIMAGE_DATA_DIRECTORY m_pDesc;		//����Ŀ¼��
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//�ļ���ʼ


	vector<EXPORTDIRECTORY> m_exportDirectory;

};
