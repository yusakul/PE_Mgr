#pragma once


// CDia_Disk1 对话框

class CDia_Disk1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Disk1)

public:
	CDia_Disk1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Disk1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DISK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetDiskSta();
	CString m_Edit_diskc;
	CString m_Edit_diskcc;
	CString m_Edit_diskd;
	CString m_Edit_diskdd;
	CString m_Edit_diske;
	CString m_Edit_diskee;
	CString m_Edit_diskf;
	CString m_Edit_diskff;
	CString m_Edit_diskg;
	CString m_Edit_diskgg;
	CString m_Edit_diskh;
	CString m_Edit_diskhh;
	CProgressCtrl m_progress_C;
	CProgressCtrl m_progress_D;
	CProgressCtrl m_progress_E;
	CProgressCtrl m_progress_F;
	CProgressCtrl m_progress_G;
	CProgressCtrl m_progress_H;

	int Rate_C;
	int Rate_D;
	int Rate_E;
	int Rate_F;
	int Rate_G;
	int Rate_H;
};
