#pragma once


// RearrangeTab dialog

class RearrangeTab : public CDialogEx
{
	DECLARE_DYNAMIC(RearrangeTab)

public:
	RearrangeTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RearrangeTab();

private:
	void InitializeComponent();
	void InitializeBitmaps();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_REARRANGE_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
