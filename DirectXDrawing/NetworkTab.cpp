// NetworkTab.cpp : implementation file
//

#include "pch.h"
#include "DirectXDrawing.h"
#include "NetworkTab.h"
#include "afxdialogex.h"


// NetworkTab dialog

IMPLEMENT_DYNAMIC(NetworkTab, CDialogEx)

NetworkTab::NetworkTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OLE_PROPPAGE_NETWORK_TAB, pParent)
{

}

NetworkTab::~NetworkTab()
{
}

void NetworkTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NetworkTab, CDialogEx)
END_MESSAGE_MAP()


// NetworkTab message handlers
