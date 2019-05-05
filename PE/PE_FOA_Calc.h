#pragma once
#include "afxwin.h"


// CPE_FOA_Calc 对话框

class CPE_FOA_Calc : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_FOA_Calc)

public:
	CPE_FOA_Calc(PIMAGE_NT_HEADERS32 pNt32,	CWnd* pParent = NULL);   // 标准构造函数
	CPE_FOA_Calc(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPE_FOA_Calc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOA_CALC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonCalc();			//计算按钮



	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;


	//edit控件变量
	CString m_FOA;
	CString m_Position;
	CString m_RVA;
	CString m_VA;
	CEdit m_editFoa;
	CEdit m_editRva;
	CEdit m_editVa;

	//radio控件变量
	CButton m_radioFOA;
	CButton m_radioRVA;
	CButton m_radioVA;
	afx_msg void OnBnClickedRadioVa();
	afx_msg void OnBnClickedRadioRva();
	afx_msg void OnBnClickedRadioFoa();
	int flag_radio;	//radio选择标识


					
	DWORD RVAtoFOA(DWORD dwRVA);//RVA转FOA

	DWORD FOAtoRVA(DWORD dwFOA);//FOA转RVA

	CEdit m_editPosition;
};
