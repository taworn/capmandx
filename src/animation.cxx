/**
 * @file animation.cxx
 * @desc Animation module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "animation.hxx"

struct ANIMATION_VERTEX {
	float x, y, z;
	float u, v;
};

Animation::~Animation()
{
	if (texture != NULL)
		texture->Release();
	if (verticesBuffer != NULL)
		verticesBuffer->Release();
}

Animation::Animation()
	: verticesBuffer(NULL), texture(NULL), uData(), vData()
	, sliceWidth(0), sliceHeight(0), spriteStart(0), spriteStop(0)
	, currentIndex(0), timeFrame(0), timeStart(0)
{
}

bool Animation::init(IDirect3DDevice9 *device, const wchar_t *imageFileName, int sliceWidth, int sliceHeight, ULONGLONG timeFrame)
{
	IDirect3DVertexBuffer9 *verticesBuffer = NULL;
	device->CreateVertexBuffer(sizeof(ANIMATION_VERTEX) * 4, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &verticesBuffer, NULL);
	if (verticesBuffer == NULL)
		return false;

	IDirect3DTexture9 *texture = NULL;
	D3DXCreateTextureFromFile(device, imageFileName, &texture);
	if (texture == NULL) {
		verticesBuffer->Release();
		return false;
	}

	if (this->verticesBuffer != NULL)
		this->verticesBuffer->Release();
	if (this->texture != NULL)
		this->texture->Release();
	this->verticesBuffer = verticesBuffer;
	this->texture = texture;

	uData.clear();
	for (int i = 0; i <= sliceWidth; i++) {
		float f = (float)i / sliceWidth;
		uData.push_back(f);
	}
	vData.clear();
	for (int i = 0; i <= sliceHeight; i++) {
		float f = (float)i / sliceHeight;
		vData.push_back(f);
	}
	this->sliceWidth = sliceWidth;
	this->sliceHeight = sliceHeight;
	this->spriteStart = 0;
	this->spriteStop = sliceWidth * sliceHeight;
	this->currentIndex = 0;
	this->timeFrame = timeFrame;
	this->timeStart = GetTickCount();

	return true;
}

void Animation::setParameters(int spriteStart, int spriteStop, int timeFrame)
{
	this->spriteStart = spriteStart;
	this->spriteStop = spriteStop;
	this->currentIndex = spriteStart;
	this->timeFrame = timeFrame;
	this->timeStart = GetTickCount();
}

void Animation::draw(IDirect3DDevice9 *device)
{
	int uIndex = currentIndex % sliceWidth;
	int vIndex = currentIndex / sliceWidth;
	float u0 = uData[uIndex];
	float u1 = uData[uIndex + 1];
	float v0 = vData[vIndex];
	float v1 = vData[vIndex + 1];

	ANIMATION_VERTEX verticesData[] = {
		// vertex             // coord
		{ -1.0f, 1.0f, 0.0f,  u0, v0, },
		{ 1.0f, 1.0f, 0.0f,   u1, v0, },
		{ -1.0f, -1.0f, 0.0f, u0, v1, },
		{ 1.0f, -1.0f, 0.0f,  u1, v1, },
	};
	void *verticesMemory;
	verticesBuffer->Lock(0, sizeof(verticesData), &verticesMemory, NULL);
	memcpy(verticesMemory, verticesData, sizeof(verticesData));
	verticesBuffer->Unlock();

	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	device->SetStreamSource(0, verticesBuffer, 0, sizeof(ANIMATION_VERTEX));
	device->SetTexture(0, texture);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	ULONGLONG timeUsage = GetTickCount() - timeStart;
	if (timeUsage > timeFrame) {
		currentIndex++;
		if (currentIndex >= spriteStop)
			currentIndex = spriteStart;
		timeStart = GetTickCount();
	}
}

