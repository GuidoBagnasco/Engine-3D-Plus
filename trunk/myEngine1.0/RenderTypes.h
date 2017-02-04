#pragma once
#include <Windows.h>
#include "myEngine_API.h"

struct IDirect3DTexture9;

namespace engine{
	struct MYENGINE_API ColorVertex{
		float x,y,z;
		DWORD color;
	};

	extern const DWORD ColorVertexType;

	struct MYENGINE_API TextureVertex{
		float x, y, z;
		float u, v;
	};

	extern const DWORD TextureVertexType;

	struct MYENGINE_API	MeshVertex{
		float x,y,z;
		float nx,ny,nz;
		float u,v;
	};

	extern const DWORD MeshVertexType;

	typedef IDirect3DTexture9* Texture;
	static const Texture NoTexture = NULL;

	enum Primitive{
		TriangleList = 0,
        TriangleStrip,
        PointList,
        LineList,
        LineStrip,
        TriangleFan,
        PrimitiveCount
	};

	enum MatrixType{
		View = 0,
		Projection,
		World,
		MatrixTypeCount
	};
#define engine_COLOR_ARGB(a,r,g,b)  ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define engine_COLOR_RGB(r,g,b) engine_COLOR_ARGB(0xff,r,g,b)
#define engine_COLOR_WHITE engine_COLOR_RGB(255,255,255)
#define engine_COLOR_RED engine_COLOR_RGB(255,0,0)
}