#pragma once
#include "afxwin.h"



// CHome �Ի���

class CHome : public CDialogEx
{
	DECLARE_DYNAMIC(CHome)

public:
	CHome(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHome();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void GetSysInfo();//��ȡϵͳ�汾��Ϣ

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_WinInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	
	afx_msg void OnBnClickedButtonStart();

	
};
