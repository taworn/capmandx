/**
 * @file sprite.cxx
 * @desc Sprite module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include "sprite.hxx"

Sprite::~Sprite()
{
	if (texture != NULL)
		texture->Release();
	if (verticesBuffer != NULL)
		verticesBuffer->Release();
}

Sprite::Sprite()
	: verticesBuffer(NULL), texture(NULL)
	, uData(), vData()
	, sliceHorz(0), sliceVert(0)
{
}

bool Sprite::init(IDirect3DDevice9 *device, const wchar_t *imageFileName, int sliceHorz, int sliceVert)
{
	IDirect3DVertexBuffer9 *verticesBuffer = NULL;
	device->CreateVertexBuffer(sizeof(VERTEX) * 4, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &verticesBuffer, NULL);
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
	for (int i = 0; i <= sliceHorz; i++)
		uData.push_back((float)i / sliceHorz);
	vData.clear();
	for (int i = 0; i <= sliceVert; i++)
		vData.push_back((float)i / sliceVert);
	this->sliceHorz = sliceHorz;
	this->sliceVert = sliceVert;

	return true;
}

void Sprite::draw(IDirect3DDevice9 *device, int imageIndex)
{
	int uIndex = imageIndex % sliceHorz;
	int vIndex = imageIndex / sliceVert;
	float u0 = uData[uIndex];
	float u1 = uData[uIndex + 1];
	float v0 = vData[vIndex];
	float v1 = vData[vIndex + 1];

	VERTEX verticesData[] = {
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
	device->SetStreamSource(0, verticesBuffer, 0, sizeof(VERTEX));
	device->SetTexture(0, texture);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

