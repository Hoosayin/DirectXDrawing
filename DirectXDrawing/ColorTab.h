#pragma once
#include "Color.h"
#include "functional"

using namespace std;


class ColorTab : public CDialogEx
{
	DECLARE_DYNAMIC(ColorTab)

public:
	CMFCColorButton OutlineColorButton;
	CMFCColorButton FillColorButton;

	ColorTab(function<void(COLOR)> onOutlineColorUpdated, function<void(COLOR)> onFillColorUpdated, CWnd* pParent = nullptr);
	virtual ~ColorTab();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfccolorbuttonOutline();
	afx_msg void OnBnClickedMfccolorbuttonFill();

private:
	COLOR ResolveColor(COLORREF color);

protected:
	function<void(COLOR)> OnOutlineColorUpdated;
	function<void(COLOR)> OnFillColorUpdated;

	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_COLOR_TAB };
#endif
};
