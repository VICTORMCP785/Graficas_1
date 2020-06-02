#include "Buffer.h"

CBuffer::CBuffer()
{
	//Constructor CBuffer
}

void CBuffer::Init(BufferStruct BSi)
{
	BS = BSi;
#ifdef D3D11
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = (D3D11_USAGE)BS.usage;
	BufferDesc.ByteWidth = BS.byteWidth;
	BufferDesc.BindFlags = BS.bindFlags;
	BufferDesc.CPUAccessFlags = BS.cpuAccessFlags;
#endif
}

void CBuffer::Init(SubresourceData s, BufferStruct b)
{
	Init(b);
#ifdef D3D11
	ZeroMemory(&SUBDATA, sizeof(SUBDATA));
	SUBDATA.pSysMem = s.psysMem;
#endif
}

void CBuffer::Update()
{
}

void CBuffer::Destroy()
{
}

void CBuffer::Render()
{
}
