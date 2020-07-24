#pragma once


// NetworkTab dialog

class NetworkTab : public CDialogEx
{
	DECLARE_DYNAMIC(NetworkTab)

public:
	NetworkTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NetworkTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_NETWORK_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
