#include "pch.h"
#include "DirectXDrawing.h"
#include "DirectXScreen.h"
#include "afxdialogex.h"
#include "ShapeTypes.h"


// DirectXScreen dialog
IMPLEMENT_DYNAMIC(DirectXScreen, CDialogEx)

DirectXScreen::DirectXScreen(CWnd* pParent)
	: CDialogEx(IDD_OLE_PROPPAGE_DIRECTX, pParent)
{
	_points = new CList<CPoint>();
	_isDrawing = true;
}

DirectXScreen::~DirectXScreen()
{
}

void DirectXScreen::InitializeComponent()
{
	InitializeD3DApp();
}

void DirectXScreen::InitializeD3DApp()
{
	_d3dApp = new D3DApp(GetSafeHwnd());
	_d3dApp->InitializeD3D();
}

void DirectXScreen::ResetPoints()
{
	_points->RemoveAll();
	_d3dApp->ClearPoints();
}

void DirectXScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DirectXScreen, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL DirectXScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitializeComponent();

	return TRUE;
}

function<void()> DirectXScreen::GetDirectXDraw()
{
	return bind(&D3DApp::Run, _d3dApp);
}

void DirectXScreen::OnMouseMove(UINT nFlags, CPoint point)
{
	if (OnMouseMoved)
		OnMouseMoved(point);

	CDialogEx::OnMouseMove(nFlags, point);
}


void DirectXScreen::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (OnLButtonDowned)
		OnLButtonDowned(point);

	_isDrawing = true;
	HandleDrawing(point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void DirectXScreen::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (OnRButtonDowned)
		OnRButtonDowned(point);

	_isDrawing = false;
	ResetPoints();

	CDialogEx::OnRButtonDown(nFlags, point);
}

void DirectXScreen::HandleDrawing(CPoint point)
{
	CString selectedShapeType = GetSelectedShapeType ? 
		GetSelectedShapeType() : LINE;

	if (selectedShapeType == ELLIPSE)
	{
		// Get data for drawing ellipse.
	}
	else if (selectedShapeType == RECTANGLE)
	{
		if (_points->GetCount() < 2)
		{
			_points->AddTail(point);
			_d3dApp->AddPoint(point);

			if (_points->GetCount() == 2)
			{
				CPoint point1 = _points->GetAt(_points->FindIndex(0));
				CPoint point2 = _points->GetAt(_points->FindIndex(1));

				if (DrawFilled())
					_d3dApp->AddRectangle(point1, point2, GetFillColor(), GetOpacity());
				else
					_d3dApp->AddRectangleUnfilled(point1, point2, GetFillColor(), GetOpacity());
				
				_isDrawing = false;
				ResetPoints();
			}
		}
	}
	else if (selectedShapeType == LINE)
	{
		if (_points->GetCount() < 2)
		{
			_points->AddTail(point);
			_d3dApp->AddPoint(point);

			if (_points->GetCount() == 2)
			{
				CPoint point1 = _points->GetAt(_points->FindIndex(0));
				CPoint point2 = _points->GetAt(_points->FindIndex(1));

				_d3dApp->AddLine(point1, point2, GetOutlineColor());
				_isDrawing = false;
				ResetPoints();
			}
		}
	}
	else if (selectedShapeType == CIRCLE)
	{
		if (_points->GetCount() < 2)
		{
			_points->AddTail(point);
			_d3dApp->AddPoint(point);

			if (_points->GetCount() == 2)
			{
				CPoint point1 = _points->GetAt(_points->FindIndex(0));
				CPoint point2 = _points->GetAt(_points->FindIndex(1));

				if (DrawFilled())
					_d3dApp->AddCircle(point1, point2, GetFillColor());
				else
					_d3dApp->AddCircleUnfilled(point1, point2, GetOutlineColor());

				_isDrawing = false;
				ResetPoints();
			}
		}
	}
	else if (selectedShapeType == OPEN_POLYGON)
	{
		if (_isDrawing)
		{
			_points->AddTail(point);
			_d3dApp->AddPoint(point);
		}
		else
		{
			_d3dApp->AddOpenPolygon(_points, GetOutlineColor());
			ResetPoints();
		}
	}
	else
	{
		if (_isDrawing)
		{
			_points->AddTail(point);
			_d3dApp->AddPoint(point);
		}
		else
		{
			if (DrawFilled())
				_d3dApp->AddPolygon(_points, GetFillColor());
			else
				_d3dApp->AddPolygonUnfilled(_points, GetOutlineColor());

			ResetPoints();
		}
	}
}

function<void()> DirectXScreen::GetResetPoints()
{
	return bind(&DirectXScreen::ResetPoints, this);
}

function<void()> DirectXScreen::GetDirectXCleanUp()
{
	return bind(&D3DApp::CleanUp, _d3dApp);
}


void DirectXScreen::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (_d3dApp)
		_d3dApp->Resize();
}


void DirectXScreen::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString selectedShapeType = GetSelectedShapeType ?
		GetSelectedShapeType() : LINE;

	if (_isDrawing && (selectedShapeType == POLYGON || selectedShapeType == OPEN_POLYGON))
	{
		_isDrawing = false;
		HandleDrawing(CPoint(0, 0));
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void DirectXScreen::OnClose()
{
	_d3dApp->CleanUp();
	CDialogEx::OnClose();
}
