#pragma once


// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:

	void AddColumn(int nNum, ...);	//插入list列


	void AddItem(int nItemNum,	//第几行
		int nCount,	//在本行填写多少个数据
		...);


};

