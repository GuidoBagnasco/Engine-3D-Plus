#pragma once
#include "./myEngine_API.h"
#include <d3dx9math.h>
#include <string>
#include <vector>
#include <map>

namespace engine{
	class MYENGINE_API Bone{
	public:
		Bone()										{ D3DXMatrixIdentity(&m_Matrix); }
		D3DXMATRIX getTransformation()				{ return m_Matrix; }
		void	   setTransformation(D3DXMATRIX tr) { m_Matrix = tr; }
	private:
		D3DXMATRIX m_Matrix;
	};

	// clase publica para guardar la informacion del bone
	class MYENGINE_API BoneInfo{
	public:

		D3DXMATRIX offsetMatrix;
		std::map<unsigned int, float> m_mWeight;
		BoneInfo();
		Bone* m_pBone;
		void setOffsetMatrix(
			float, float, float, float,
			float, float, float, float,
			float, float, float, float,
			float, float, float, float);

		void AddWeight(unsigned int index, float weight)  { m_mWeight[index] = weight; }
		void SetBone(Bone* pBone)						  { m_pBone = pBone; }
	};
}