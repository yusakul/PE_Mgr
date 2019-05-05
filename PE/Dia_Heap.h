#pragma once


// Dia_Heap 对话框

class Dia_Heap : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Heap)

public:
	Dia_Heap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Dia_Heap();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_List_Heap;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//遍历堆
	BOOL ListProcessHeap(DWORD dwOwnerPID);

};
