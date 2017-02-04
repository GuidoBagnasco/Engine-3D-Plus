//---------------------------------------------------------------------------
#include "IndexBuffer.h"
//---------------------------------------------------------------------------
#include "Renderer.h"
//---------------------------------------------------------------------------
using namespace engine;
//---------------------------------------------------------------------------
IndexBuffer::IndexBuffer (Renderer& rkRenderer, IDirect3DDevice9* pkDevice):
m_uiIndexCount(0),
m_IndexBuffer(NULL),
m_pkDevice(pkDevice),
m_rkRenderer(rkRenderer)
{
	// nothing to do
}

//---------------------------------------------------------------------------

IndexBuffer::~IndexBuffer(){
	if(m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}
}

//---------------------------------------------------------------------------

void IndexBuffer::SetIndexData(const unsigned short* pausIndices, size_t uiIndexCount){
	// release if previously created
	if(m_IndexBuffer){
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}

	// create vertex buffer
	HRESULT hr = m_pkDevice->CreateIndexBuffer(
												uiIndexCount * sizeof(unsigned short),
												D3DUSAGE_WRITEONLY,
												D3DFMT_INDEX16,
												D3DPOOL_MANAGED,
												&m_IndexBuffer,
												NULL
	);

	assert(hr == D3D_OK);

	// copy index data
	void* pvIndices;

	m_IndexBuffer->Lock(0, uiIndexCount * sizeof(unsigned short), &pvIndices, 0);
	memcpy(pvIndices, pausIndices, sizeof(unsigned short) * uiIndexCount);
	m_IndexBuffer->Unlock();

	// update internal state
	m_uiIndexCount = uiIndexCount;
}

//---------------------------------------------------------------------------

void IndexBuffer::Bind(){
	HRESULT hr = m_pkDevice->SetIndices(m_IndexBuffer);
	assert(hr == D3D_OK);

	m_rkRenderer.SetCurrentIndexBuffer(this);
}

//---------------------------------------------------------------------------
