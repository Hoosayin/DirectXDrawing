#pragma once

#include "Vertex.h"
#include "d3d11.h"
#include "d3dx11.h"
#include "DirectXMath.h"

struct D3D_SHAPE {
	VERTEX* Vertices;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	UINT NumberOfIndices;
	UINT NumberOfVertices;

	UINT Strides;
	UINT Offsets;

	D3D11_PRIMITIVE_TOPOLOGY Topology;
	XMFLOAT4X4 World;
	bool IsIndexed;
};