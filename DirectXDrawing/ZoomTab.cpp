// ZoomTab.cpp : implementation file
//

#include "pch.h"
#include "DirectXDrawing.h"
#include "ZoomTab.h"
#include "afxdialogex.h"


// ZoomTab dialog

IMPLEMENT_DYNAMIC(ZoomTab, CDialogEx)

ZoomTab::ZoomTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OLE_PROPPAGE_ZOOM_TAB, pParent)
{
}

ZoomTab::~ZoomTab()
{
}

void ZoomTab::InitializeComponent()
{
	InitializeBitmaps();
}

void ZoomTab::InitializeBitmaps()
{
	CBitmap bitmap;

	bitmap.LoadBitmap(IDB_ZOOM_IN);
	((CButton*)GetDlgItem(IDC_BUTTON_ZOOM_IN))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_ZOOM_OUT);
	((CButton*)GetDlgItem(IDC_BUTTON_ZOOM_OUT))->SetBitmap(bitmap);
	bitmap.Detach();
}

void ZoomTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZoomTab, CDialogEx)
END_MESSAGE_MAP()


// ZoomTab message handlers


BOOL ZoomTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitializeComponent();

	return TRUE;
}
