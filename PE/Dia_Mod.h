#pragma once


// Dia_Mod �Ի���

class Dia_Mod : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Mod)

public:
	Dia_Mod(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dia_Mod();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	BOOL ListProcessModules(DWORD dwPID);
	CListCtrl m_List_Mod;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
