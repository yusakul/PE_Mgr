#pragma once
#include "AntiVirusScan.h"

// CAntiVirus �Ի���

class CAntiVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiVirus)

public:
	CAntiVirus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAntiVirus();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANTI_VIRUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	DWORD m_scantype;

public:
	afx_msg void OnBnClickedButtonFastantivirus();		//����
	afx_msg void OnBnClickedButtonAllantivirus();		//ȫ��
	afx_msg void OnBnClickedButtonCustomantivirus();	//�Զ���λ��
	virtual BOOL OnInitDialog();						
	afx_msg void OnBnClickedButtonCustomfile();			//�Զ����ļ�
	afx_msg void OnBnClickedButtonMd5();				

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonnote();
};
