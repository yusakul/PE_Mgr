#pragma once
#include <vector>
#include "afxcmn.h"
#include <afxwin.h>

typedef struct _MyWinProcStruct
{
	TCHAR lpWinTitle[MAX_PATH];
	TCHAR lpPaths[MAX_PATH];
	DWORD hPID;
	HICON hIcon;
	DWORD ListID;
	HANDLE hWnd;
}MyWinProcStruct, *PMyWinProcStruct;
// CDia_WinProcess1 �Ի���

class CDia_WinProcess1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_WinProcess1)

public:
	CDia_WinProcess1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_WinProcess1();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WINPROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:


	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CListCtrl m_ListCtrl_winproc;
	CImageList* pImageList = new CImageList;

	//ˢ��list
	void UpdataList();
	std::vector<MyWinProcStruct> m_WinProcVec;





	afx_msg void OnNMRClickListWinproc(NMHDR *pNMHDR, LRESULT *pResult);
};
