#pragma once
#include "Snake.h"
struct BufferStruct
{
	unsigned int byteWidth;
	USAGE usage;
	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
	unsigned int structureByteStride;
};

class CBuffer
{	
public:
	CBuffer();
	//static CBuffer * B;
	BufferStruct BS;
#ifdef D3D11
	ID3D11Buffer * BufferD11 = NULL;
	ID3D11Buffer * VertexBufferD11 = NULL;
	ID3D11Buffer * IndexBufferD11 = NULL;
	D3D11_SUBRESOURCE_DATA SUBDATA;
	D3D11_BUFFER_DESC BufferDesc;
#endif
	void Init(BufferStruct FSi);
	void Init(SubresourceData s, BufferStruct b);
	void Update();
	void Destroy();
	void Render();
};

