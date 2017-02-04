
//---------------------------------------------------------------------------
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
//---------------------------------------------------------------------------
#include "myEngine_API.h"
#include "RenderTypes.h"
//---------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>

//---------------------------------------------------------------------------

namespace engine{

//---------------------------------------------------------------------------
class Renderer;
//---------------------------------------------------------------------------

class MYENGINE_API VertexBuffer{
public:
	VertexBuffer(Renderer& rkRenderer, IDirect3DDevice9* pkDevice, size_t uiVertexSize, unsigned int uiFVF);
	~VertexBuffer();

	void Bind();
	void SetVertexData(const void* pakVertices, size_t uiVertexCount);
	
	size_t vertexCount() const;

private:
	unsigned int m_uiFVF;
	size_t m_uiVertexSize;
	size_t m_uiVertexCount;

	LPDIRECT3DVERTEXBUFFER9 m_pkVertexBuffer;
	IDirect3DDevice9* m_pkDevice;
	Renderer& m_rkRenderer;
};
//---------------------------------------------------------------------------
#include "VertexBuffer.inl"
//---------------------------------------------------------------------------
} // end namespace
//---------------------------------------------------------------------------
#endif  // PG2_VERTEXBUFFER_H
//---------------------------------------------------------------------------
