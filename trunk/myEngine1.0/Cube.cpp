#include "Cube.h"
using namespace engine;

Cube::Cube(Renderer& rkRenderer) : Mesh(rkRenderer){
	Init();
}

void Cube::Init(){/*
	ColorVertex g_8Vertices[] = {
		{ -0.5f, 0.5f, -0.5f, D3DCOLOR_XRGB(255, 0, 0) }, // 0 
		{ 0.5f, 0.5f, -0.5f, D3DCOLOR_XRGB(0, 255, 0) }, // 1 
		{ 0.5f, 0.5f, 0.5f, D3DCOLOR_XRGB(40, 0, 120) }, // 2 
		{ -0.5f, 0.5f, 0.5f, D3DCOLOR_XRGB(255, 0, 0) }, // 3

		{ -0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(0, 255, 0) }, // 4
		{ 0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(40, 0, 120) }, // 5
		{ 0.5f, -0.5f, -0.5f, D3DCOLOR_XRGB(255, 0, 0) }, // 6
		{ -0.5f, -0.5f, -0.5f, D3DCOLOR_XRGB(0, 255, 0) } // 7
	};

	USHORT g_indices[] = { 0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		3, 2, 5, 3, 5, 4,
		2, 1, 6, 2, 6, 5,
		1, 7, 6, 1, 0, 7,
		0, 3, 4, 0, 4, 7 };*/

	//SetData(g_8Vertices, 8, TriangleList, g_indices, 36);
}