#pragma once


// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();

protected:
	DECLARE_MESSAGE_MAP()


public:
	CDialogEx* m_Dia[6];
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


