#pragma once
#include "D3DApp.h"

class ShapeManager
{
private:
	D3DApp* _app;

public:
	ShapeManager();
	~ShapeManager();

	void InitializeD3DApp();
};

