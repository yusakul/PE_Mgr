#pragma once
#include "HistogramCtrl.h"


// CSpeed �Ի���

class CSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(CSpeed)

public:
	CSpeed(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpeed();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPEED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CString m_Edit_RamRate;
	CString m_Edit_UserRam;
	CString m_Edit_freeRam;
	CString m_Edit_totalRam;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//�����ڴ���Ϣ
	int UpdateMemroyInfo();

	//�����ٲ�ͼ����
	CHistogramCtrl m_ctrlHistogram;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCleanram();
};
