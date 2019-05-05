// MyShellListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "MyShellListCtrl.h"


// CMyShellListCtrl

IMPLEMENT_DYNAMIC(CMyShellListCtrl, CMFCShellListCtrl)

CMyShellListCtrl::CMyShellListCtrl()
{

}

CMyShellListCtrl::~CMyShellListCtrl()
{
}

void CMyShellListCtrl::OnSetColumns()
{
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	LPWSTR sName[4] = { _T("名称"),_T("大小"),_T("类型"),_T("修时间") };
	int nWidth[4] = { 200,100,100,200 };

	for (int i = 0; i < 4; i++)
	{
		lvColumn.pszText = sName[i];
		lvColumn.cx = nWidth[i];
		lvColumn.fmt = LVCFMT_CENTER;
		SetColumn(i, &lvColumn);
	}
}

HRESULT CMyShellListCtrl::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	
	ASSERT_VALID(this);


	LPENUMIDLIST pEnum = NULL;
	HRESULT hRes = pParentFolder->EnumObjects(NULL, m_nTypes, &pEnum);

	if (SUCCEEDED(hRes) && pEnum != NULL)
	{
		LPITEMIDLIST pidlTemp;
		DWORD dwFetched = 1;
		LPAFX_SHELLITEMINFO pItem;

		//enumerate the item's PIDLs  
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			LVITEM lvItem;
			ZeroMemory(&lvItem, sizeof(lvItem));

			//fill in the TV_ITEM structure for this item  
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

			//AddRef the parent folder so it's pointer stays valid  
			pParentFolder->AddRef();

			//put the private information in the lParam  
			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));

			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlTemp);

			pItem->pParentFolder = pParentFolder;



			lvItem.lParam = (LPARAM)pItem;

			lvItem.pszText = _T("");
			lvItem.iImage = OnGetItemIcon(GetItemCount(), pItem);

			//determine if the item is shared  
			DWORD dwAttr = SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK;
			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);

			if (dwAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image  
			}

			if (dwAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}
			if (dwAttr & SFGAO_FOLDER)
			{
			}
			else
			{

				TCHAR szPath[MAX_PATH];

				if (SHGetPathFromIDList(pItem->pidlFQ, szPath))
				{
					CFileStatus fs;
					if (CFile::GetStatus(szPath, fs))
					{
						CString s = szPath;
						int n = s.ReverseFind('.');
						if (n != -1)
						{
							s = s.Mid(n + 1);
							s.MakeLower();

							if (s == _T("exe"))//改写扩展名就可以过滤  
								continue;


						}
					}
				}


			}
			int iItem = InsertItem(&lvItem);
			if (iItem >= 0)
			{
				// Set columns:  
				const int nColumns = m_wndHeader.GetItemCount();
				for (int iColumn = 0; iColumn < nColumns; iColumn++)
				{
					SetItemText(iItem, iColumn, OnGetItemText(iItem, iColumn, pItem));
				}
			}

			dwFetched = 0;
		}

		pEnum->Release();
	}

	return hRes;

}


BEGIN_MESSAGE_MAP(CMyShellListCtrl, CMFCShellListCtrl)
END_MESSAGE_MAP()



// CMyShellListCtrl 消息处理程序


