#pragma once


// Dia_Thread �Ի���

class Dia_Thread : public CDialogEx
{
	DECLARE_DYNAMIC(Dia_Thread)

public:
	Dia_Thread(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dia_Thread();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	//�����б�
	void UpdataList();

	//�����߳�
	BOOL ListProcessThreads(DWORD dwOwnerPID);

	CListCtrl m_List_Thread;

	//�Ҽ�list�����˵�
	afx_msg void OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult);
};
