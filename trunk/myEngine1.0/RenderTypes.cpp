#include "RenderTypes.h"
#include <d3dx9.h>

const DWORD engine::ColorVertexType		= (D3DFVF_XYZ | D3DFVF_DIFFUSE);
const DWORD engine::TextureVertexType	= (D3DFVF_XYZ | D3DFVF_TEX1);
const DWORD engine::MeshVertexType		=  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);