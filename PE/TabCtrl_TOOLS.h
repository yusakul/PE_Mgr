#pragma once


// CTabCtrl_TOOLS

class CTabCtrl_TOOLS : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrl_TOOLS)

public:
	CTabCtrl_TOOLS();
	virtual ~CTabCtrl_TOOLS();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CDialogEx* m_Dia[3];
	afx_msg void OnTcnSelchangeTabTools(NMHDR *pNMHDR, LRESULT *pResult);

};


