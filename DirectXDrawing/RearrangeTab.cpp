// RearrangeTab.cpp : implementation file
//

#include "pch.h"
#include "DirectXDrawing.h"
#include "RearrangeTab.h"
#include "afxdialogex.h"


// RearrangeTab dialog

IMPLEMENT_DYNAMIC(RearrangeTab, CDialogEx)

RearrangeTab::RearrangeTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OLE_PROPPAGE_REARRANGE_TAB, pParent)
{

}

RearrangeTab::~RearrangeTab()
{
}

void RearrangeTab::InitializeComponent()
{
	InitializeBitmaps();
}

void RearrangeTab::InitializeBitmaps()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BRING_TO_FRONT);
	((CButton*)GetDlgItem(IDC_BUTTON_BRING_TO_FRONT))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_SEND_TO_BACK);
	((CButton*)GetDlgItem(IDC_BUTTON_SEND_TO_BACK))->SetBitmap(bitmap);
	bitmap.Detach();
}

void RearrangeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RearrangeTab, CDialogEx)
END_MESSAGE_MAP()


// RearrangeTab message handlers


BOOL RearrangeTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitializeComponent();

	return TRUE;
}
