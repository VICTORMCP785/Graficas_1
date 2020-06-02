#pragma once
#include "Snake.h"
#include "Buffer.h"

class CVertexBuffer
{
private:
	//static CVertexBuffer * VB;
public:
	CVertexBuffer();
	//CBuffer Buff;
#ifdef D3D11
	//ID3D11Buffer * VertexBufferD11 = NULL;
	//D3D11_SUBRESOURCE_DATA VBSubData;
#endif
	//void Init(SubresourceData s, BufferStruct b);
	void Update();
	void Destroy();
	void Render();
};

