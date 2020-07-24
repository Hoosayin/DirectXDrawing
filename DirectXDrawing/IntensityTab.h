#pragma once
#include "functional"

using namespace std;

class IntensityTab : public CDialogEx
{
	DECLARE_DYNAMIC(IntensityTab)

public:
	IntensityTab(function<void(float)> onIntensityUpdated, function<void(float)> onOpacityUpdated, CWnd* pParent = nullptr);
	virtual ~IntensityTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_INTENSITY_TAB };
#endif

protected:
	function<void(float)> OnIntensityUpdated;
	function<void(float)> OnOpacityUpdated;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void InitializeComponent();

public:
	CSliderCtrl SliderOpacity;
	CSliderCtrl SliderIntensity;
	CString OpacityString;
	CString IntensityString;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic StaticOpacity;
	CStatic StaticIntentsity;
	virtual BOOL OnInitDialog();
};
