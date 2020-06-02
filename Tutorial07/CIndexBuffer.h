#pragma once
#include "Buffer.h"

class CIndexBuffer
{
private:
	//static CIndexBuffer * IB;
	
public:
	CIndexBuffer();
	//CBuffer Buff;
#ifdef D3D11
	//ID3D11Buffer * IndexBufferD11 = NULL;
	//D3D11_SUBRESOURCE_DATA SUBDATA;
#endif
	//void Init(BufferStruct bufstr, SubresourceData sub);
	void Update();
	void Destroy();
	void Render();
};

