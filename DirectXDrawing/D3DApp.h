#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "DirectXMath.h"
#include "D3DShape.h"
#include "Color.h"

class D3DApp
{
private:
	HWND _windowHandle;
	IDXGISwapChain* _swapChain;
	
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	D3D11_VIEWPORT _viewport;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Buffer* _indexBuffer;
	
	ID3D11Texture2D* _depthStencilBuffer;
	
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D10Blob* _vertexShaderBuffer;
	ID3D10Blob* _pixelShaderBuffer;

	ID3D11InputLayout* _vertexLayout;
	ID3D11Buffer* _constantBufferPerObjectBuffer;

	XMFLOAT4X4 _cameraView;
	XMFLOAT4X4 _cameraProjection;

	XMVECTOR _cameraPosition;
	XMVECTOR _cameraTarget;
	XMVECTOR _cameraUp;

	XMMATRIX _worldViewProjection;
	CList<D3D_SHAPE*>* _shapes;
	CList<D3D_SHAPE*>* _points;

	void GetDimensions(int& width, int& height);
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void SetRenderTargetView();
	void SetPipelineShaders();
	void SetInputLayout();
	void CreateViewPort();
	void CreateConstantBuffer();
	void InitializeCamera();
	XMVECTOR UnprojectPoint(CPoint point);
	void SetIndexBuffer(D3D_SHAPE*& shape, DWORD indices[]);
	void SetVertexBuffer(D3D_SHAPE*& shape);
	float CalculateRadius(FXMVECTOR vector1, FXMVECTOR vector2);
	CList<XMVECTOR>* MidpointCircleDraw(FXMVECTOR midpoint, float radius);

public:
	D3DApp(HWND windowHandle);
	~D3DApp();

	bool InitializeD3D();
	bool InitializeScene();

	void AddLine(CPoint point1, CPoint point2, COLOR color);
	void AddTriangle(CPoint point1, CPoint point2, CPoint point3);
	void AddPoint(CPoint point);
	void AddRectangle(CPoint point1, CPoint point2, COLOR color, float opacity);
	void AddRectangleUnfilled(CPoint point1, CPoint point2, COLOR color, float opacity);
	void AddOpenPolygon(CList<CPoint>* points, COLOR color);
	void AddPolygon(CList<CPoint>* points, COLOR color);
	void AddPolygonUnfilled(CList<CPoint>* points, COLOR color);
	void AddCircle(CPoint midpoint, CPoint point, COLOR color);

	void UpdateScene();
	void DrawScene();
	void DrawShapes();
	void DrawPoints();
	void ClearPoints();
	void ClearShapes();
	void Undo();
	void Resize();

	void Run();

	void CleanUp();
};

