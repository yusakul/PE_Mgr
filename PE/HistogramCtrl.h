#if !defined(AFX_HISTOGRAMCTRL_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_)
#define AFX_HISTOGRAMCTRL_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramCtrl.h : header file
//

#include "AfxTempl.h"
/////////////////////////////////////////////////////////////////////////////
// CHistogramCtrl window

class CHistogramCtrl : public CWnd
{
// Construction
public:
	CHistogramCtrl();

// Attributes
public:
/*	typedef enum {LOW_SPEED = 3000, NORMAL_SPEED = 1000, HIGH_SPEED = 500, IDLE = 0} SPEED;*/
	enum SPEED { LOW_SPEED = 3000, NORMAL_SPEED = 1000, HIGH_SPEED = 500, IDLE = 0 };


// Operations
public:
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT uID);
	void SetRange(UINT uLower, UINT uUpper);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistogramCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetGridsColor(COLORREF cr);
	BOOL SetBkColor(COLORREF cr);
	void SetPen(int nWidth, COLORREF crColor);
	CHistogramCtrl::SPEED SetSpeed(CHistogramCtrl::SPEED uSpeed);
	void SetPos(UINT uPos);
	virtual ~CHistogramCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistogramCtrl)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	enum SPEED m_uSpeed;
	UINT m_yPos;
	UINT m_yPreviousPos;
	UINT m_uOffset;

	void DrawLine();
	BOOL InvalidateCtrl();
	UINT GetAverage();

	CDC *m_pMemDC;

	UINT m_uLower;
	UINT m_uUpper;

	CRect m_rcClient;
	CPen m_colorPen;

	int m_nFirstLinePos;

	COLORREF m_crBackGround;
	COLORREF m_crGrids;

	CList<UINT, UINT&> *m_pList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMCTRL_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_)
