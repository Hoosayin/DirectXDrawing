#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "TabControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TabControl::TabControl()
{
	_tabPages = nullptr;
	_tabIDs = nullptr;
	_numberOfPages = 0;
}

TabControl::~TabControl()
{
	for (int i = 0; i < _numberOfPages; i++)
		delete _tabPages[i];
}

void TabControl::InitializeComponent(CDialog** tabpages, int* tabIDs, int numberOfPages)
{
	_tabPages = tabpages;
	_tabIDs = tabIDs;
	_numberOfPages = numberOfPages;
	_currentTab = 0;

	for (int i = 0; i < _numberOfPages; i++)
		_tabPages[i]->Create(_tabIDs[i], this);

	_tabPages[0]->ShowWindow(SW_SHOW);

	for (int i = 1; i < _numberOfPages; i++)
		_tabPages[i]->ShowWindow(SW_HIDE);

	SetRectangle();
}

void TabControl::SetRectangle()
{
	CRect tabRect, itemRect;
	
	int left, 
		top, 
		right, 
		bottom;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	left = itemRect.left;
	top = itemRect.bottom + 2;
	right = tabRect.right - itemRect.left - 3;
	bottom = tabRect.bottom - top - 2;

	

	for (int i = 0; i < _numberOfPages; i++)
		if (i != _currentTab)
			_tabPages[i]->SetWindowPos(&wndTop, left, top, right, bottom, SWP_HIDEWINDOW);
		else
			_tabPages[i]->SetWindowPos(&wndTop, left, top, right, bottom, SWP_SHOWWINDOW);
}

BEGIN_MESSAGE_MAP(TabControl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void TabControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (_currentTab != GetCurFocus()) {
		_tabPages[_currentTab]->ShowWindow(SW_HIDE);
		
		_currentTab = GetCurFocus();
		_tabPages[_currentTab]->ShowWindow(SW_SHOW);
		_tabPages[_currentTab]->SetFocus();
	}
}
