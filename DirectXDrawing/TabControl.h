#pragma once

class TabControl : public CTabCtrl
{
	// Fields
private:
	CDialog** _tabPages;
	int* _tabIDs;
	int _currentTab = 0;
	int _numberOfPages;


	// Methods
public:
	TabControl();
	void InitializeComponent(CDialog** tabpages, int* tabIDs, int numberOfPages);
	void SetRectangle();

public:
	virtual ~TabControl();

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
