#include "CDeviceContext.h"

CDeviceContext::CDeviceContext()
{
}

CDeviceContext * CDeviceContext::getDeviceContext()
{
	if (DC == nullptr)
	{
		DC = new CDeviceContext();
	}
	return DC;
}

void CDeviceContext::Init()
{
}

void CDeviceContext::Update()
{
}

void CDeviceContext::Destroy()
{
}

void CDeviceContext::Render()
{
}
