#include "pch.h"
#include "resource.h"
#include "DirectXScreenPlaceholder.h"
#include "DirectXScreen.h"

DirectXScreenPlaceholder::DirectXScreenPlaceholder()
{
	_directXScreen = new DirectXScreen();
}

DirectXScreenPlaceholder::~DirectXScreenPlaceholder()
{
	delete _directXScreen;
}

void DirectXScreenPlaceholder::InitializeComponent(
	function<CString()> getSelectedShapeType, 
	function<COLOR()> getOutlineColor, 
	function<COLOR()> getFillColor,
	function<float()> getOpacity,
	function<bool()> drawFilled)
{
	_directXScreen->Create(IDD_OLE_PROPPAGE_DIRECTX, this);
	_directXScreen->ShowWindow(SW_SHOW);
	_directXScreen->GetSelectedShapeType = getSelectedShapeType;
	_directXScreen->GetOutlineColor = getOutlineColor;
	_directXScreen->GetFillColor = getFillColor;
	_directXScreen->GetOpacity = getOpacity;
	_directXScreen->DrawFilled = drawFilled;

	Resize();
}

void DirectXScreenPlaceholder::Resize()
{
	CRect rect;

	int left,
		top,
		right,
		bottom;

	GetClientRect(&rect);

	left = rect.left + 2;
	top = rect.top + 2;
	right = rect.right - 4;
	bottom = rect.bottom - 4;

	_directXScreen->SetWindowPos(&wndTop, left, top, right, bottom, SWP_SHOWWINDOW);
}

function<void()> DirectXScreenPlaceholder::GetDirectXDraw()
{
	return _directXScreen ? _directXScreen->GetDirectXDraw() : function<void()>();
}

function<void()> DirectXScreenPlaceholder::GetResetPoints()
{
	return _directXScreen ? _directXScreen->GetResetPoints() : function<void()>();
}

function<void()> DirectXScreenPlaceholder::GetDirectXCleanUp()
{
	return _directXScreen ? _directXScreen->GetDirectXCleanUp() : function<void()>();
}

void DirectXScreenPlaceholder::SetOnMouseMoved(function<void(CPoint)> onMouseMoved)
{
	_directXScreen->OnMouseMoved = onMouseMoved;
}

void DirectXScreenPlaceholder::SetOnLButtonDowned(function<void(CPoint)> onLButtonDowned)
{
	_directXScreen->OnLButtonDowned = onLButtonDowned;
}

void DirectXScreenPlaceholder::SetOnRButtonDowned(function<void(CPoint)> onRButtonDowned)
{
	_directXScreen->OnRButtonDowned = onRButtonDowned;
}
