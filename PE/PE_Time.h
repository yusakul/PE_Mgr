#pragma once
#include "afxwin.h"


// CPE_Time �Ի���

class CPE_Time : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Time)

public:
	CPE_Time(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_Time(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPE_Time();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_TIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_TimeDate;//ʱ��
	CString m_TimeStamps;//ʱ���
	DWORD   m_dwTimeDateStamp;	//ʱ���
	afx_msg void OnBnClickedButtonTimeok();//����
	void TimeStamp2TimeDate(DWORD   TimeDateStamp);		//ת��ʱ��

	void TimeDate2TimeStamp(DWORD TimeDate);

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;


	CButton m_radioTimeStamps;
	CButton m_radioTimeDate;
	int flag_radio;

	afx_msg void OnBnClickedRadioTimestamps();
	afx_msg void OnBnClickedRadioTimedate();
	CEdit m_edit_stams;
	CEdit m_edit_date;
};
