#pragma once

#include "Vertex.h"
#include "d3d11.h"
#include "d3dx11.h"

class Shape
{
private:
	VERTEX* Vertices;
	ID3D11Buffer* VertexBuffer;
	UINT NumberOfVertices;
	UINT Strides;
	UINT Offsets;
	D3D11_PRIMITIVE_TOPOLOGY Topology;
	XMFLOAT4X4 World;
};

