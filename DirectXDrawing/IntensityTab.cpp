#include "pch.h"
#include "DirectXDrawing.h"
#include "IntensityTab.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(IntensityTab, CDialogEx)

IntensityTab::IntensityTab(function<void(float)> onIntensityUpdated, function<void(float)> onOpacityUpdated, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OLE_PROPPAGE_INTENSITY_TAB, pParent)
	, IntensityString(_T("Intentsity: 100"))
	, OpacityString(_T("Opactiy: 100"))
{
	OnIntensityUpdated = onIntensityUpdated;
	OnOpacityUpdated = onOpacityUpdated;
}

IntensityTab::~IntensityTab()
{
}

void IntensityTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_OPACITY, SliderOpacity);
	DDX_Control(pDX, IDC_SLIDER_INTENSITY, SliderIntensity);
	DDX_Control(pDX, IDC_STATIC_SELECTED_OPACITY, StaticOpacity);
	DDX_Control(pDX, IDC_STATIC_SELECTED_INTENTSITY, StaticIntentsity);
}

void IntensityTab::InitializeComponent()
{
	SliderIntensity.SetPos(100);
	SliderOpacity.SetPos(100);
}


BEGIN_MESSAGE_MAP(IntensityTab, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void IntensityTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER_OPACITY == pScrollBar->GetDlgCtrlID())
	{
		CSliderCtrl* slider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

		OpacityString.Format(L"Opacity: %d", slider->GetPos());
		StaticOpacity.SetWindowTextW(OpacityString);

		if (OnOpacityUpdated)
		{
			float opacity = slider->GetPos();
			OnOpacityUpdated(opacity / 100.0f);
		}
	}
	else if (IDC_SLIDER_INTENSITY == pScrollBar->GetDlgCtrlID())
	{
		CSliderCtrl* slider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
		float intensity = slider->GetPos();

		IntensityString.Format(L"Intensity: %d", slider->GetPos());
		StaticIntentsity.SetWindowTextW(IntensityString);

		if (OnIntensityUpdated)
		{
			float intensity = slider->GetPos();
			OnIntensityUpdated(intensity / 100.0f);
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL IntensityTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitializeComponent();
	return TRUE;
}
