#include "Bone.h"
using namespace engine;

BoneInfo::BoneInfo() : m_pBone(NULL){
	D3DXMatrixIdentity(&offsetMatrix);
}

void BoneInfo::setOffsetMatrix(float x1, float x2, float x3, float x4,
	float y1, float y2, float y3, float y4,
	float z1, float z2, float z3, float z4,
	float w1, float w2, float w3, float w4){

	offsetMatrix._11 = x1;
	offsetMatrix._12 = x2;
	offsetMatrix._13 = x3;
	offsetMatrix._14 = x4;

	offsetMatrix._21 = y1;
	offsetMatrix._22 = y2;
	offsetMatrix._23 = y3;
	offsetMatrix._24 = y4;

	offsetMatrix._31 = z1;
	offsetMatrix._32 = z2;
	offsetMatrix._33 = z3;
	offsetMatrix._34 = z4;

	offsetMatrix._41 = w1;
	offsetMatrix._42 = w2;
	offsetMatrix._43 = w3;
	offsetMatrix._44 = w4;
}