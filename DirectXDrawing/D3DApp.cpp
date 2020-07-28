#include "pch.h"
#include "D3DApp.h"
#include "exception"
#include "Vertex.h"
#include "Math.h"

constexpr auto Z = 0.0f;

using namespace std;

struct CONSTANT_BUFFER_PER_OBJECT
{
	XMMATRIX WorldViewProjection;
};

CONSTANT_BUFFER_PER_OBJECT constantBufferPerObject;

D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

UINT numberOfElements = ARRAYSIZE(inputElementDescriptions);

void D3DApp::GetDimensions(int& width, int& height)
{
	CRect rect;
	GetWindowRect(_windowHandle, &rect);

	width = rect.Width();
	height = rect.Height();

	TRACE("width = %d", width);
	TRACE("height = %d", height);
}

void D3DApp::CreateSwapChain()
{
	int width;
	int height;
	GetDimensions(width, height);

	DXGI_MODE_DESC swapChainBufferDescription;
	ZeroMemory(&swapChainBufferDescription, sizeof(DXGI_MODE_DESC));

	swapChainBufferDescription.Width = width;
	swapChainBufferDescription.Height = height;
	swapChainBufferDescription.RefreshRate.Numerator = 60;
	swapChainBufferDescription.RefreshRate.Denominator = 1;
	swapChainBufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainBufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainBufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDescription.BufferDesc = swapChainBufferDescription;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = _windowHandle;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDescription, &_swapChain, &_device, NULL, &_deviceContext);
}

void D3DApp::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	_device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
	backBuffer->Release();
}

void D3DApp::CreateDepthStencilView()
{
	int width;
	int height;
	GetDimensions(width, height);

	D3D11_TEXTURE2D_DESC depthStencilDescription;

	depthStencilDescription.Width = width;
	depthStencilDescription.Height = height;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	_device->CreateTexture2D(&depthStencilDescription, NULL, &_depthStencilBuffer);
	_device->CreateDepthStencilView(_depthStencilBuffer, NULL, &_depthStencilView);
}

void D3DApp::SetRenderTargetView()
{
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
}

void D3DApp::SetPipelineShaders()
{
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_vertexShaderBuffer, 0, 0);
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_pixelShaderBuffer, 0, 0);

	_device->CreateVertexShader(_vertexShaderBuffer->GetBufferPointer(), _vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	_device->CreatePixelShader(_pixelShaderBuffer->GetBufferPointer(), _pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);

	_deviceContext->VSSetShader(_vertexShader, 0, 0);
	_deviceContext->PSSetShader(_pixelShader, 0, 0);
}

void D3DApp::SetInputLayout()
{
	_device->CreateInputLayout(inputElementDescriptions, numberOfElements, _vertexShaderBuffer->GetBufferPointer(),
		_vertexShaderBuffer->GetBufferSize(), &_vertexLayout);

	_deviceContext->IASetInputLayout(_vertexLayout);
}

void D3DApp::CreateViewPort()
{
	int width;
	int height;
	GetDimensions(width, height);

	ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));

	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = width;
	_viewport.Height = height;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	_deviceContext->RSSetViewports(1, &_viewport);
}

void D3DApp::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDescription;
	ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));

	constantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDescription.ByteWidth = sizeof(CONSTANT_BUFFER_PER_OBJECT);
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = 0;
	constantBufferDescription.MiscFlags = 0;

	_device->CreateBuffer(&constantBufferDescription, NULL, &_constantBufferPerObjectBuffer);
}

void D3DApp::InitializeCamera()
{
	int width;
	int height;
	GetDimensions(width, height);

	_cameraPosition = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	_cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	_cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX cameraView = XMLoadFloat4x4(&_cameraView);
	XMMATRIX cameraProjection = XMLoadFloat4x4(&_cameraProjection);

	cameraView = XMMatrixLookAtLH(_cameraPosition, _cameraTarget, _cameraUp);
	cameraProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f * 2.0, width / height, 1.0f, 1000.0f);

	XMStoreFloat4x4(&_cameraView, cameraView);
	XMStoreFloat4x4(&_cameraProjection, cameraProjection);
}

XMVECTOR D3DApp::UnprojectPoint(CPoint point)
{
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX cameraView = XMLoadFloat4x4(&_cameraView);
	XMMATRIX cameraProjection = XMLoadFloat4x4(&_cameraProjection);

	return XMVector3Unproject(
		XMVectorSet(point.x, point.y, 0.0f, 1.0f), 
		_viewport.TopLeftX,
		_viewport.TopLeftY,
		_viewport.Width,
		_viewport.Height,
		_viewport.MinDepth,
		_viewport.MaxDepth,
		cameraProjection,
		cameraView,
		world);
}

void D3DApp::SetIndexBuffer(D3D_SHAPE*& shape, DWORD indices[])
{
	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory(&indexBufferDescription, sizeof(indexBufferDescription));

	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = sizeof(DWORD) * shape->NumberOfIndices * shape->NumberOfVertices;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initialData;

	initialData.pSysMem = indices;
	_device->CreateBuffer(&indexBufferDescription, &initialData, &shape->IndexBuffer);
}

void D3DApp::SetVertexBuffer(D3D_SHAPE*& shape)
{
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory(&vertexBufferDescription, sizeof(vertexBufferDescription));

	vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescription.ByteWidth = sizeof(VERTEX) * shape->NumberOfVertices;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = shape->Vertices;
	_device->CreateBuffer(&vertexBufferDescription, &vertexBufferData, &shape->VertexBuffer);

	shape->Strides = sizeof(VERTEX);
	shape->Offsets = 0;
}

float D3DApp::CalculateRadius(FXMVECTOR vector1, FXMVECTOR vector2)
{
	return sqrt(pow((XMVectorGetX(vector1) - XMVectorGetX(vector2)), 2) 
		+ pow((XMVectorGetY(vector1) - XMVectorGetY(vector2)), 2));
}

CList<XMVECTOR>* D3DApp::MidpointCircleDraw(FXMVECTOR midpoint, float radius)
{
	CList<XMVECTOR>* vectors = new CList<XMVECTOR>();

	float x_center = XMVectorGetX(midpoint);
	float y_center = XMVectorGetY(midpoint);

	float x = radius;
	float y = 0.0f;

	vectors->AddTail(XMVectorSet(x + x_center, y + y_center, 0.0f, 0.0f));

	if (radius > 0.0f)
	{
		vectors->AddTail(XMVectorSet(x + x_center, -y + y_center, 0.0f, 0.0f));
		vectors->AddTail(XMVectorSet(y + x_center, x + y_center, 0.0f, 0.0f));
		vectors->AddTail(XMVectorSet(-y + x_center, x + y_center, 0.0f, 0.0f));
	}

	float P = 1 - radius;

	while (x > y)
	{
		y += 0.001f;
 
		if (P <= 0.0f)
		{
			P = P + 2 * y + 0.001f;
		} 
		else
		{
			x -= 0.001f;
			P = P + 2 * y - 2 * x + 0.001f;
		}

		if (x < y)
			break;

		vectors->AddTail(XMVectorSet(x + x_center, y + y_center, 0.0f, 0.0f));
		vectors->AddTail(XMVectorSet(-x + x_center, y + y_center, 0.0f, 0.0f));
		vectors->AddTail(XMVectorSet(x + x_center, -y + y_center, 0.0f, 0.0f));
		vectors->AddTail(XMVectorSet(-x + x_center, -y + y_center, 0.0f, 0.0f));

		if (x != y)
		{
			vectors->AddTail(XMVectorSet(y + x_center, x + y_center, 0.0f, 0.0f));
			vectors->AddTail(XMVectorSet(-y + x_center, x + y_center, 0.0f, 0.0f));
			vectors->AddTail(XMVectorSet(y + x_center, -x + y_center, 0.0f, 0.0f));
			vectors->AddTail(XMVectorSet(-y + x_center, -x + y_center, 0.0f, 0.0f));
		}
	}

	return vectors;
}

void D3DApp::AddTriangle(CPoint point1, CPoint point2, CPoint point3)
{
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = 3; 

	XMVECTOR vector1 = UnprojectPoint(point1);
	XMVECTOR vector2 = UnprojectPoint(point2);
	XMVECTOR vector3 = UnprojectPoint(point3);

	shape->Vertices = new VERTEX[3]
	{
		VERTEX(
			XMVectorGetX(vector1),
			XMVectorGetY(vector1),
			0.0f, 0.62f, 0.62f, 0.62f, 1.0f),

		VERTEX(
			XMVectorGetX(vector2),
			XMVectorGetY(vector2),
			0.0f, 0.62f, 0.62f, 0.62f, 1.0f),

		VERTEX(
			XMVectorGetX(vector3),
			XMVectorGetY(vector3),
			0.0f, 0.62f, 0.62f, 0.62f, 1.0f),
	};

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	_shapes->AddTail(shape);
}

void D3DApp::AddLine(CPoint point1, CPoint point2, COLOR color)
{
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = 2;

	XMVECTOR vector1 = UnprojectPoint(point1);
	XMVECTOR vector2 = UnprojectPoint(point2);

	shape->Vertices = new VERTEX[2]
	{
		VERTEX (
			XMVectorGetX(vector1), 
			XMVectorGetY(vector1),
			0.0f, color.Red, color.Green, color.Blue, 1.0f),

		VERTEX(
			XMVectorGetX(vector2),
			XMVectorGetY(vector2),
			0.0f, color.Red, color.Green, color.Blue, 1.0f),
	};

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	_shapes->AddTail(shape);
}

void D3DApp::AddPoint(CPoint point)
{
	D3D_SHAPE* d3DPoint = new D3D_SHAPE();
	d3DPoint->NumberOfVertices = 1;

	XMVECTOR vector = UnprojectPoint(point);

	d3DPoint->Vertices = new VERTEX[2]
	{
		VERTEX(
			XMVectorGetX(vector),
			XMVectorGetY(vector),
			0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
	};

	d3DPoint->IndexBuffer = nullptr;
	d3DPoint->IsIndexed = false;

	SetVertexBuffer(d3DPoint);

	d3DPoint->Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	_points->AddTail(d3DPoint);
}

void D3DApp::AddRectangle(CPoint point1, CPoint point2, COLOR color, float opacity)
{
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = 4;

	CPoint point3(point2.x, point1.y);
	CPoint point4(point1.x, point2.y);

	XMVECTOR vector1 = UnprojectPoint(point1);
	XMVECTOR vector2 = UnprojectPoint(point3);
	XMVECTOR vector3 = UnprojectPoint(point4);
	XMVECTOR vector4 = UnprojectPoint(point2);

	shape->Vertices = new VERTEX[4]
	{
		VERTEX(
			XMVectorGetX(vector1),
			XMVectorGetY(vector1),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector2),
			XMVectorGetY(vector2),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector3),
			XMVectorGetY(vector3),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector4),
			XMVectorGetY(vector4),
			Z, color.Red, color.Green, color.Blue, opacity),
	};

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	_shapes->AddTail(shape);
}

void D3DApp::AddRectangleUnfilled(CPoint point1, CPoint point2, COLOR color, float opacity)
{
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = 5;

	CPoint point3(point2.x, point1.y);
	CPoint point4(point1.x, point2.y);

	XMVECTOR vector1 = UnprojectPoint(point1);
	XMVECTOR vector2 = UnprojectPoint(point3);
	XMVECTOR vector3 = UnprojectPoint(point2);
	XMVECTOR vector4 = UnprojectPoint(point4);

	shape->Vertices = new VERTEX[5]
	{
		VERTEX(
			XMVectorGetX(vector1),
			XMVectorGetY(vector1),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector2),
			XMVectorGetY(vector2),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector3),
			XMVectorGetY(vector3),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector4),
			XMVectorGetY(vector4),
			Z, color.Red, color.Green, color.Blue, opacity),

		VERTEX(
			XMVectorGetX(vector1),
			XMVectorGetY(vector1),
			Z, color.Red, color.Green, color.Blue, opacity),
	};

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	_shapes->AddTail(shape);
}

void D3DApp::AddOpenPolygon(CList<CPoint>* points, COLOR color)
{
	int numberOfPoints = points->GetCount();
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = numberOfPoints;

	XMVECTOR* vectors = new XMVECTOR[numberOfPoints];
	int count = 0;

	POSITION position = points->GetHeadPosition();

	while (position)
	{
		CPoint point = points->GetNext(position);
		vectors[count++] = UnprojectPoint(point);
	}

	shape->Vertices = new VERTEX[numberOfPoints];

	for (int i = 0; i < numberOfPoints; i++)
	{
		shape->Vertices[i] = VERTEX(
			XMVectorGetX(vectors[i]),
			XMVectorGetY(vectors[i]),
			0.0f, color.Red, color.Green, color.Blue, 1.0f);
	}

	delete[] vectors;
	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	_shapes->AddTail(shape);
}

void D3DApp::AddPolygon(CList<CPoint>* points, COLOR color)
{
	int numberOfPoints = points->GetCount();
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = numberOfPoints + 1;

	XMVECTOR* vectors = new XMVECTOR[numberOfPoints];
	int count = 0;

	POSITION position = points->GetHeadPosition();

	while (position)
	{
		CPoint point = points->GetNext(position);
		vectors[count++] = UnprojectPoint(point);
	}

	shape->Vertices = new VERTEX[numberOfPoints + 1];

	for (int i = 0; i < numberOfPoints; i++)
	{
		shape->Vertices[i] = VERTEX(
			XMVectorGetX(vectors[i]),
			XMVectorGetY(vectors[i]),
			0.0f, color.Red, color.Green, color.Blue, 0.5f);
	}

	shape->Vertices[numberOfPoints] = VERTEX(
		XMVectorGetX(vectors[0]),
		XMVectorGetY(vectors[0]),
		0.0f, color.Red, color.Green, color.Blue, 0.5f);

	delete[] vectors;

	DWORD* indices = new DWORD[numberOfPoints + 2];

	for (int i = 0; i < (numberOfPoints + 1); i++)
		indices[i] = i;

	indices[numberOfPoints + 1] = 0;

	shape->NumberOfIndices = numberOfPoints + 2;
	SetIndexBuffer(shape, indices);
	shape->IsIndexed = true;

	delete[] indices;
	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	_shapes->AddTail(shape);
}

void D3DApp::AddPolygonUnfilled(CList<CPoint>* points, COLOR color)
{
	int numberOfPoints = points->GetCount();
	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = numberOfPoints + 1;

	XMVECTOR* vectors = new XMVECTOR[numberOfPoints + 1];
	int count = 0;

	POSITION position = points->GetHeadPosition();

	while (position)
	{
		CPoint point = points->GetNext(position);
		vectors[count++] = UnprojectPoint(point);
	}

	shape->Vertices = new VERTEX[numberOfPoints];

	for (int i = 0; i < numberOfPoints; i++)
	{
		shape->Vertices[i] = VERTEX(
			XMVectorGetX(vectors[i]),
			XMVectorGetY(vectors[i]),
			0.0f, color.Red, color.Green, color.Blue, 1.0f);
	}

	shape->Vertices[numberOfPoints] = VERTEX(
		XMVectorGetX(vectors[0]),
		XMVectorGetY(vectors[0]),
		0.0f, color.Red, color.Green, color.Blue, 1.0f);

	delete[] vectors;
	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	_shapes->AddTail(shape);
}

void D3DApp::AddCircle(CPoint midpoint, CPoint point, COLOR color)
{
	XMVECTOR midpointVector = UnprojectPoint(midpoint);
	XMVECTOR vector = UnprojectPoint(point);

	float radius = CalculateRadius(midpointVector, vector);
	CList<XMVECTOR>* vectors = MidpointCircleDraw(midpointVector, radius);

	int length = vectors->GetCount();

	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = length * 2;

	shape->Vertices = new VERTEX[length * 2];
	int count = 0;

	POSITION position = vectors->GetHeadPosition();

	while (position)
	{
		XMVECTOR vector = vectors->GetNext(position);
		
		shape->Vertices[count++] = VERTEX(
			XMVectorGetX(vector),
			XMVectorGetY(vector),
			0.0f, color.Red, color.Green, color.Blue, 1.0f);

		shape->Vertices[count++] = VERTEX(
			XMVectorGetX(midpointVector),
			XMVectorGetY(midpointVector),
			0.0f, color.Red, color.Green, color.Blue, 1.0f);
	}

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	_shapes->AddTail(shape);
}

void D3DApp::AddCircleUnfilled(CPoint midpoint, CPoint point, COLOR color)
{
	XMVECTOR midpointVector = UnprojectPoint(midpoint);
	XMVECTOR vector = UnprojectPoint(point);

	float radius = CalculateRadius(midpointVector, vector);
	CList<XMVECTOR>* vectors = MidpointCircleDraw(midpointVector, radius);

	int length = vectors->GetCount();

	D3D_SHAPE* shape = new D3D_SHAPE();
	shape->NumberOfVertices = length;

	shape->Vertices = new VERTEX[length];
	int count = 0;

	POSITION position = vectors->GetHeadPosition();

	while (position)
	{
		XMVECTOR vector = vectors->GetNext(position);

		shape->Vertices[count++] = VERTEX(
			XMVectorGetX(vector),
			XMVectorGetY(vector),
			0.0f, color.Red, color.Green, color.Blue, 1.0f);
	}

	shape->IndexBuffer = nullptr;
	shape->IsIndexed = false;

	SetVertexBuffer(shape);

	shape->Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	_shapes->AddTail(shape);
}

D3DApp::D3DApp(HWND windowHandle)
{
	_windowHandle = windowHandle;

	_shapes = new CList<D3D_SHAPE*>();
	_points = new CList<D3D_SHAPE*>();

	_cameraView = XMFLOAT4X4();
	_cameraProjection = XMFLOAT4X4();
}

D3DApp::~D3DApp()
{
}

bool D3DApp::InitializeD3D()
{
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	SetRenderTargetView();
	SetPipelineShaders();
	SetInputLayout();
	InitializeCamera();
	CreateViewPort();
	CreateConstantBuffer();

	return true;
}

bool D3DApp::InitializeScene()
{
	return true;
}

void D3DApp::UpdateScene()
{
	// To-Do: Transform the worlds.
}

void D3DApp::DrawScene()
{
	float bgColor[4] = { 0.18f, 0.18f, 0.18f, 1.0f };
	_deviceContext->ClearRenderTargetView(_renderTargetView, bgColor);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DrawPoints();
	DrawShapes();

	_swapChain->Present(0, 0);
}

void D3DApp::DrawShapes()
{
	POSITION position;
	position = _shapes->GetTailPosition();

	while (position)
	{
		D3D_SHAPE* shape = _shapes->GetPrev(position);
		
		_deviceContext->IASetVertexBuffers(0, 1, &shape->VertexBuffer, &shape->Strides, &shape->Offsets);
		_deviceContext->IASetPrimitiveTopology(shape->Topology);

		XMMATRIX world = XMLoadFloat4x4(&(shape->World));
		XMMATRIX cameraView = XMLoadFloat4x4(&_cameraView);
		XMMATRIX cameraProjection = XMLoadFloat4x4(&_cameraProjection);

		world = XMMatrixIdentity();

		_worldViewProjection = XMMatrixMultiply(XMMatrixMultiply(world, cameraView), cameraProjection);
		constantBufferPerObject.WorldViewProjection = XMMatrixTranspose(_worldViewProjection);

		_deviceContext->UpdateSubresource(_constantBufferPerObjectBuffer, 0, NULL, &constantBufferPerObject, 0, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, &_constantBufferPerObjectBuffer);

		if (shape->IsIndexed)
		{
			_deviceContext->IASetIndexBuffer(shape->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			_deviceContext->DrawIndexed(shape->NumberOfIndices, 0, 0);
		}
		else
		{
			_deviceContext->Draw(shape->NumberOfVertices, 0);
		}
	}
}

void D3DApp::DrawPoints()
{
	POSITION position;
	position = _points->GetHeadPosition();

	while (position)
	{
		D3D_SHAPE* point = _points->GetNext(position);

		_deviceContext->IASetVertexBuffers(0, 1, &point->VertexBuffer, &point->Strides, &point->Offsets);
		_deviceContext->IASetPrimitiveTopology(point->Topology);

		XMMATRIX world = XMLoadFloat4x4(&(point->World));
		XMMATRIX cameraView = XMLoadFloat4x4(&_cameraView);
		XMMATRIX cameraProjection = XMLoadFloat4x4(&_cameraProjection);

		world = XMMatrixIdentity();

		_worldViewProjection = XMMatrixMultiply(XMMatrixMultiply(world, cameraView), cameraProjection);
		constantBufferPerObject.WorldViewProjection = XMMatrixTranspose(_worldViewProjection);

		_deviceContext->UpdateSubresource(_constantBufferPerObjectBuffer, 0, NULL, &constantBufferPerObject, 0, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, &_constantBufferPerObjectBuffer);

		_deviceContext->Draw(point->NumberOfVertices, 0);
	}
}

void D3DApp::ClearPoints()
{
	POSITION position = _points->GetHeadPosition();

	while (position)
		_points->GetNext(position)->VertexBuffer->Release();

	_points->RemoveAll();
}

void D3DApp::ClearShapes()
{
}

void D3DApp::Undo()
{
	_shapes->GetTail()->VertexBuffer->Release();
	_shapes->RemoveTail();
}

void D3DApp::Resize()
{
	_swapChain->Release();
	_renderTargetView->Release();
	_depthStencilView->Release();
	_depthStencilBuffer->Release();

	InitializeD3D();
}

void D3DApp::Run() 
{
	UpdateScene();
	DrawScene();
}

void D3DApp::CleanUp()
{
	_swapChain->Release();
	_device->Release();
	_deviceContext->Release();
	_renderTargetView->Release();
	_indexBuffer->Release();
	_vertexShader->Release();
	_pixelShader->Release();
	_vertexShaderBuffer->Release();
	_pixelShaderBuffer->Release();
	_vertexLayout->Release();
	_depthStencilView->Release();
	_depthStencilBuffer->Release();
	_constantBufferPerObjectBuffer->Release();

	POSITION position;
	position = _shapes->GetHeadPosition();

	while (position)
		_shapes->GetNext(position)->VertexBuffer->Release();

	position = _points->GetHeadPosition();

	while (position)
		_points->GetNext(position)->VertexBuffer->Release();
}