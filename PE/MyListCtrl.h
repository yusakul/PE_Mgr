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

	void AddColumn(int nNum, ...);	//����list��


	void AddItem(int nItemNum,	//�ڼ���
		int nCount,	//�ڱ�����д���ٸ�����
		...);


};

