#pragma once

#include "functional"
#include "TabControl.h"
#include "DirectXScreenPlaceholder.h"
#include "D3DApp.h"
#include "ShapeTypes.h"
#include "Color.h"

using namespace std;

class DirectXDrawingDialog : public CDialogEx
{
public:
	DirectXDrawingDialog(CWnd* pParent = nullptr);

private:
	CString _selectedShapeType;
	CList<POINT>* _points;

	void InitializeComponent();
	void InitializeTabs();
	void InitializeBitmaps();
	void InitializeDirect3D();
	void HandleDrawing();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTXDRAWING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	D3DApp* _d3dApp;
	COLOR _outlineColor;
	COLOR _fillColor;
	float _intensity;
	float _opacity;
	bool _drawFilled;

protected:
	HICON m_hIcon;
	function<void()> DrawScene;
	function<void()> ResetPoints;
	function<void()> CleanUp;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	TabControl TabControlTabOptions;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnWindowMaximizedChange(BOOL bIsMaximized);
	DirectXScreenPlaceholder DirectXRenderArea;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CString MouseLocationX;
	CString MouseLocationY;
	CString LeftClickX;
	CString LeftClickY;
	CString RightClickX;
	CString RightClickY;
	bool isStarted;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedRadioEllipse();
	afx_msg void OnBnClickedRadioRectangle();
	afx_msg void OnBnClickedRadioLine();
	afx_msg void OnBnClickedRadioCircle();
	afx_msg void OnBnClickedRadioOpenPolygon();
	afx_msg void OnBnClickedRadioPolygon();
	afx_msg void OnBnClickedRadioFilled();
	afx_msg void OnBnClickedRadioUnFilled();
	afx_msg void OnClose();
};
