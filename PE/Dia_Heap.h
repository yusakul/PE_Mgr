#pragma once


// Dia_Heap �Ի���

class Dia_Heap : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Heap)

public:
	Dia_Heap(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dia_Heap();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_List_Heap;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//������
	BOOL ListProcessHeap(DWORD dwOwnerPID);

};
