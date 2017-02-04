#pragma once
#include "myEngine_API.h"
#include <d3dx10math.h>


namespace engine{
	class Renderer;
	class MYENGINE_API Frustum{
	public:
		Frustum(Renderer*);
		~Frustum();

		void ConstructFrustum();


		void Calculate(D3DXMATRIX*, D3DXMATRIX*);

		bool CheckCube(D3DXVECTOR3*);

	private:
		Renderer* _r;
		D3DXPLANE m_planes[6];
	};

}
