#include "pch.h"
#include "DirectXDrawing.h"
#include "ColorTab.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(ColorTab, CDialogEx)

ColorTab::ColorTab(function<void(COLOR)> onOutlineColorUpdated, function<void(COLOR)> onFillColorUpdated, CWnd* pParent) :
	CDialogEx(IDD_OLE_PROPPAGE_COLOR_TAB, pParent)
{
	OnOutlineColorUpdated = onOutlineColorUpdated;
	OnFillColorUpdated = onFillColorUpdated;
}

ColorTab::~ColorTab()
{
}

void ColorTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_OUTLINE, OutlineColorButton);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_FILL, FillColorButton);
}

BEGIN_MESSAGE_MAP(ColorTab, CDialogEx)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_OUTLINE, &ColorTab::OnBnClickedMfccolorbuttonOutline)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_FILL, &ColorTab::OnBnClickedMfccolorbuttonFill)
END_MESSAGE_MAP()

BOOL ColorTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void ColorTab::OnBnClickedMfccolorbuttonOutline()
{
	COLORREF color = OutlineColorButton.GetColor();
	
	if (OnOutlineColorUpdated)
		OnOutlineColorUpdated(ResolveColor(color));
}


void ColorTab::OnBnClickedMfccolorbuttonFill()
{
	COLORREF color = FillColorButton.GetColor();

	if (OnFillColorUpdated)
		OnFillColorUpdated(ResolveColor(color));
}

COLOR ColorTab::ResolveColor(COLORREF color)
{
	float red = GetRValue(color);
	float green = GetGValue(color);
	float blue = GetBValue(color);

	float rgb = red + green + blue;

	float normalizedRed = red / rgb;
	float normalizedGreen = green / rgb;
	float normalizedBlue = blue / rgb;

	return COLOR{ normalizedRed, normalizedGreen, normalizedBlue };
}
