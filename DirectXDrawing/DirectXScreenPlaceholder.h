#pragma once

#include "pch.h"
#include "DirectXScreen.h"
#include "functional"

using namespace std;

class DirectXScreenPlaceholder : public CStatic
{
private: 
	DirectXScreen* _directXScreen;

public:
	DirectXScreenPlaceholder();
	virtual ~DirectXScreenPlaceholder();

	void InitializeComponent(
		function<CString()> getSelectedShapeType, 
		function<COLOR()> getOutlineColor, 
		function<COLOR()> getFillColor,
		function<float()> getOpacity,
		function<bool()> drawFilled);

	void Resize();
	function<void()> GetDirectXDraw();
	function<void()> GetResetPoints();
	function<void()> GetDirectXCleanUp();

	void SetOnMouseMoved(function<void(CPoint)> onMouseMoved);
	void SetOnLButtonDowned(function<void(CPoint)> onLButtonDowned);
	void SetOnRButtonDowned(function<void(CPoint)> onRButtonDowned);
};

