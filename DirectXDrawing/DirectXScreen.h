#pragma once

#include "D3DApp.h"
#include "Color.h"
#include "functional"

using namespace std;

class DirectXScreen : public CDialogEx
{
	DECLARE_DYNAMIC(DirectXScreen)

public:
	function<void(CPoint)> OnMouseMoved;
	function<void(CPoint)> OnLButtonDowned;
	function<void(CPoint)> OnRButtonDowned;
	function<CString()> GetSelectedShapeType;
	function<COLOR()> GetOutlineColor;
	function<COLOR()> GetFillColor;
	function<float()> GetOpacity;
	function<bool()> DrawFilled;

	DirectXScreen(CWnd* pParent = nullptr);
	virtual ~DirectXScreen();

	virtual BOOL OnInitDialog();
	function<void()> GetDirectXDraw();
	function<void()> GetResetPoints();
	function<void()> GetDirectXCleanUp();

private:
	D3DApp* _d3dApp;
	CList<CPoint>* _points;
	bool _isDrawing;

	void InitializeComponent();
	void InitializeD3DApp();
	void ResetPoints();
	void HandleDrawing(CPoint point);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_DIRECTX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};
