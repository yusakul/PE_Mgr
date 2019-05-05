#pragma once
#include "afxwin.h"


// CPE_FOA_Calc �Ի���

class CPE_FOA_Calc : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_FOA_Calc)

public:
	CPE_FOA_Calc(PIMAGE_NT_HEADERS32 pNt32,	CWnd* pParent = NULL);   // ��׼���캯��
	CPE_FOA_Calc(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPE_FOA_Calc();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOA_CALC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonCalc();			//���㰴ť



	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;


	//edit�ؼ�����
	CString m_FOA;
	CString m_Position;
	CString m_RVA;
	CString m_VA;
	CEdit m_editFoa;
	CEdit m_editRva;
	CEdit m_editVa;

	//radio�ؼ�����
	CButton m_radioFOA;
	CButton m_radioRVA;
	CButton m_radioVA;
	afx_msg void OnBnClickedRadioVa();
	afx_msg void OnBnClickedRadioRva();
	afx_msg void OnBnClickedRadioFoa();
	int flag_radio;	//radioѡ���ʶ


					
	DWORD RVAtoFOA(DWORD dwRVA);//RVAתFOA

	DWORD FOAtoRVA(DWORD dwFOA);//FOAתRVA

	CEdit m_editPosition;
};
