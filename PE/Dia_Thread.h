#pragma once


// Dia_Thread 对话框

class Dia_Thread : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Thread)

public:
	Dia_Thread(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dia_Thread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	//更新列表
	void UpdataList();

	//遍历线程
	BOOL ListProcessThreads(DWORD dwOwnerPID);

	CListCtrl m_List_Thread;

	//右键list呼出菜单
	afx_msg void OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult);
};
