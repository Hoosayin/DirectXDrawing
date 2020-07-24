#include "pch.h"
#include "framework.h"
#include "DirectXDrawing.h"
#include "DirectXDrawingDialog.h"
#include "afxdialogex.h"

#include "ZoomTab.h"
#include "IntensityTab.h"
#include "ColorTab.h"
#include "RearrangeTab.h"
#include "NetworkTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DirectXDrawingDialog::DirectXDrawingDialog(CWnd* pParent)
	: CDialogEx(IDD_DIRECTXDRAWING_DIALOG, pParent)
	, MouseLocationX(_T(""))
	, MouseLocationY(_T(""))
	, LeftClickX(_T(""))
	, LeftClickY(_T(""))
	, RightClickX(_T(""))
	, RightClickY(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void DirectXDrawingDialog::InitializeComponent()
{
	InitializeTabs();
	InitializeBitmaps();
	InitializeDirect3D();

	_selectedShapeType = LINE;
	_outlineColor = COLOR{ 0.62f, 0.62f, 0.62f };
	_fillColor = COLOR{ 0.62f, 0.62f, 0.62f };
	_intensity = 1.0f;
	_opacity = 1.0f;
	_drawFilled = true;

	_points = new CList<POINT>();
	((CButton*)GetDlgItem(IDC_RADIO_LINE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_FILLED))->SetCheck(TRUE);
}

void DirectXDrawingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_OPTIONS, TabControlTabOptions);
	DDX_Control(pDX, IDC_STATIC_RENDER_AREA, DirectXRenderArea);
	DDX_Text(pDX, IDC_STATIC_MOUSE_LOCATION_X, MouseLocationX);
	DDX_Text(pDX, IDC_STATIC_MOUSE_LOCATION_Y, MouseLocationY);
	DDX_Text(pDX, IDC_STATIC_LEFT_CLICK_LOCATION_X, LeftClickX);
	DDX_Text(pDX, IDC_STATIC_LEFT_CLICK_LOCATION_Y, LeftClickY);
	DDX_Text(pDX, IDC_STATIC_RIGHT_CLICK_LOCATION_X, RightClickX);
	DDX_Text(pDX, IDC_STATIC_RIGHT_LOCATION_LOCATION_Y, RightClickY);
}

BEGIN_MESSAGE_MAP(DirectXDrawingDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_WM_DWMWINDOWMAXIMIZEDCHANGE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_RADIO_ELLIPSE, &DirectXDrawingDialog::OnBnClickedRadioEllipse)
	ON_BN_CLICKED(IDC_RADIO_RECTANGLE, &DirectXDrawingDialog::OnBnClickedRadioRectangle)
	ON_BN_CLICKED(IDC_RADIO_LINE, &DirectXDrawingDialog::OnBnClickedRadioLine)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE, &DirectXDrawingDialog::OnBnClickedRadioCircle)
	ON_BN_CLICKED(IDC_RADIO_OPEN_POLYGON, &DirectXDrawingDialog::OnBnClickedRadioOpenPolygon)
	ON_BN_CLICKED(IDC_RADIO_POLYGON, &DirectXDrawingDialog::OnBnClickedRadioPolygon)
	ON_BN_CLICKED(IDC_RADIO_FILLED, &DirectXDrawingDialog::OnBnClickedRadioFilled)
	ON_BN_CLICKED(IDC_RADIO_UN_FILLED, &DirectXDrawingDialog::OnBnClickedRadioUnFilled)
END_MESSAGE_MAP()

BOOL DirectXDrawingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	InitializeComponent();

	return TRUE;
}

void DirectXDrawingDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR DirectXDrawingDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void DirectXDrawingDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (TabControlTabOptions)
		TabControlTabOptions.SetRectangle();

	if (DirectXRenderArea)
		DirectXRenderArea.Resize();
}

void DirectXDrawingDialog::InitializeDirect3D()
{
	DirectXRenderArea.InitializeComponent(
		[this]() {
			return _selectedShapeType;
		},
		[this]() {
			return _outlineColor;
		},
			[this]() {
			return _fillColor;
		},
			[this]() {
			return _opacity;
		},
			[this]() {
			return _drawFilled;
		});

	DrawScene = DirectXRenderArea.GetDirectXDraw();
	ResetPoints = DirectXRenderArea.GetResetPoints();

	DirectXRenderArea.SetOnMouseMoved([this](CPoint point) {
		UpdateData(FALSE);

		MouseLocationX.Format(_T("%d"), point.x);
		MouseLocationY.Format(_T("%d"), point.y);

		UpdateData(FALSE);
		});

	DirectXRenderArea.SetOnLButtonDowned([this](CPoint point) {
		UpdateData(FALSE);

		LeftClickX.Format(_T("%d"), point.x);
		LeftClickY.Format(_T("%d"), point.y);

		UpdateData(FALSE);
		});

	DirectXRenderArea.SetOnRButtonDowned([this](CPoint point) {
		UpdateData(FALSE);

		RightClickX.Format(_T("%d"), point.x);
		RightClickY.Format(_T("%d"), point.y);

		UpdateData(FALSE);
		});
}

void DirectXDrawingDialog::HandleDrawing()
{
}


void DirectXDrawingDialog::InitializeTabs()
{
	TabControlTabOptions.InsertItem(0, L"Zoom");
	TabControlTabOptions.InsertItem(1, L"Intensity");
	TabControlTabOptions.InsertItem(2, L"Colors");
	TabControlTabOptions.InsertItem(3, L"Layout");
	TabControlTabOptions.InsertItem(4, L"Network");

	CDialog** tabPages = new CDialog * [5]
	{
		new ZoomTab(),

		new IntensityTab(
			[this](float intentsity) {
			_intensity = intentsity;
			},
			[this](float opacity) {
			_opacity = opacity;
			}),

		new ColorTab(
			[this](COLOR color) {
			_outlineColor = color;
			}, 
			[this](COLOR color) {
			_fillColor = color;
			}),

		new RearrangeTab(),
		new NetworkTab()
	};

	int* tabIDs = new int[5]
	{
		IDD_OLE_PROPPAGE_ZOOM_TAB,
		IDD_OLE_PROPPAGE_INTENSITY_TAB,
		IDD_OLE_PROPPAGE_COLOR_TAB,
		IDD_OLE_PROPPAGE_REARRANGE_TAB,
		IDD_OLE_PROPPAGE_NETWORK_TAB
	};

	TabControlTabOptions.InitializeComponent(tabPages, tabIDs, 5);
}

void DirectXDrawingDialog::InitializeBitmaps()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_ELLIPSE);
	((CButton*)GetDlgItem(IDC_RADIO_ELLIPSE))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_RECTANGLE);
	((CButton*)GetDlgItem(IDC_RADIO_RECTANGLE))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_LINE);
	((CButton*)GetDlgItem(IDC_RADIO_LINE))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_CIRCLE);
	((CButton*)GetDlgItem(IDC_RADIO_CIRCLE))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_OPEN_POLYGON);
	((CButton*)GetDlgItem(IDC_RADIO_OPEN_POLYGON))->SetBitmap(bitmap);
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_POLYGON);
	((CButton*)GetDlgItem(IDC_RADIO_POLYGON))->SetBitmap(bitmap);
	bitmap.Detach();
}

void DirectXDrawingDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
}


void DirectXDrawingDialog::OnWindowMaximizedChange(BOOL bIsMaximized)
{
	CDialogEx::OnWindowMaximizedChange(bIsMaximized);
}

LRESULT DirectXDrawingDialog::OnKickIdle(WPARAM wParam, LPARAM lParam) 
{		
	DrawScene();
	CWnd::UpdateDialogControls(this, TRUE);
	return TRUE;
}

BOOL DirectXDrawingDialog::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}

void DirectXDrawingDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	/*UpdateData(FALSE);

	MouseLocationX.Format(_T("%d"), 0);
	MouseLocationY.Format(_T("%d"), 0);

	UpdateData(FALSE);*/

	CDialogEx::OnMouseMove(nFlags, point);
}

void DirectXDrawingDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	UpdateData(FALSE);

	RightClickX.Format(_T("%d"), 0);
	RightClickY.Format(_T("%d"), 0);

	UpdateData(FALSE);

	CDialogEx::OnRButtonDown(nFlags, point);
}


void DirectXDrawingDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateData(FALSE);

	LeftClickX.Format(_T("%d"), 0);
	LeftClickY.Format(_T("%d"), 0);

	UpdateData(FALSE);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void DirectXDrawingDialog::OnBnClickedButtonStart()
{
	isStarted = true;
}


void DirectXDrawingDialog::OnBnClickedRadioEllipse()
{
	_selectedShapeType = ELLIPSE;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioRectangle()
{
	_selectedShapeType = RECTANGLE;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioLine()
{
	_selectedShapeType = LINE;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioCircle()
{
	_selectedShapeType = CIRCLE;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioOpenPolygon()
{
	_selectedShapeType = OPEN_POLYGON;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioPolygon()
{
	_selectedShapeType = POLYGON;
	ResetPoints();
}


void DirectXDrawingDialog::OnBnClickedRadioFilled()
{
	_drawFilled = true;
}


void DirectXDrawingDialog::OnBnClickedRadioUnFilled()
{
	_drawFilled = false;
}
