#pragma once
#include "Snake.h"
struct InputLayoutStruct
{

};
class CInputLayout
{
private:

public:
	CInputLayout();
	InputLayoutStruct VSS;
#ifdef D3D11
	ID3D11InputLayout * InputLayout = NULL;
#endif
	void Init(InputLayoutStruct);
	void Update();
	void Destroy();
	void Render();
};

