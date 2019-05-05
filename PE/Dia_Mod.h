#pragma once


// Dia_Mod 对话框

class Dia_Mod : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Mod)

public:
	Dia_Mod(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dia_Mod();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	BOOL ListProcessModules(DWORD dwPID);
	CListCtrl m_List_Mod;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
