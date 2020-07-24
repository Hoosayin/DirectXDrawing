#pragma once


// ZoomTab dialog

class ZoomTab : public CDialogEx
{
	DECLARE_DYNAMIC(ZoomTab)

public:
	ZoomTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ZoomTab();

private:
	void InitializeComponent();
	void InitializeBitmaps();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_ZOOM_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
