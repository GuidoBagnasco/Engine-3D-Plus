#define NOMINMAX
#include "Mesh.h"
#include "Node.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <algorithm>
using namespace engine;


Mesh::Mesh(Renderer& r, bool isTextured) : m_rRenderer(r),
m_pTexture(NoTexture), m_pBB(new D3DXVECTOR3[8]), m_VtxHuesos(NULL), VectorDraw(NULL){
	if(!isTextured){
		p_vb = r.CreateVertexBuffer(sizeof(engine::ColorVertex), engine::ColorVertexType);
		p_ib = r.CreateIndexBuffer();
	}else{
		p_vb = r.CreateVertexBuffer(sizeof(engine::MeshVertex), engine::MeshVertexType);
		p_ib = r.CreateIndexBuffer();
	}
}


Mesh::~Mesh(){
	if (p_vb){
		delete p_vb;
		p_vb = NULL;
	}

	if (p_ib){
		delete p_ib;
		p_ib = NULL;
	}

	if (m_VtxHuesos)
		delete m_VtxHuesos;
	if (VectorDraw)
		delete VectorDraw;
}


D3DXPLANE Mesh::GetPlane(D3DXMATRIX* transform) {
	D3DXVECTOR3 planepoints[3];
	for (int i = 0; i < 3; i++) {
		D3DXVECTOR3 pos(m_TextureVertex[i].x, m_TextureVertex[i].y, m_TextureVertex[i].z);
		D3DXVec3TransformCoord(&planepoints[i], &pos, transform);
	}
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane, &planepoints[0], &planepoints[1], &planepoints[2]);
	return plane;
}

void Mesh::SetData(ColorVertex* Tex_Vertex, size_t vertexCount, Primitive Prim, unsigned short* pInt, size_t indexCount){
	m_Primitive = Prim;
	p_vb->SetVertexData((void *)Tex_Vertex, vertexCount);
	p_ib->SetIndexData(pInt, indexCount);
}



void Mesh::SetData(MeshVertex* Tex_Vertex, size_t vertexCount, Primitive Prim, unsigned short* pInt, size_t indexCount){
	m_Primitive = Prim;

	m_TextureVertex = Tex_Vertex;
	m_VertexCount = vertexCount;

	p_vb->SetVertexData((void *)Tex_Vertex, vertexCount);
	p_ib->SetIndexData(pInt, indexCount);

	m_VtxHuesos = new D3DXVECTOR3[m_VertexCount];
	VectorDraw = new D3DXVECTOR3[m_VertexCount];
	for (int i = 0; i < m_VertexCount; i++){
		m_VtxHuesos[i].x = m_TextureVertex[i].x;
		m_VtxHuesos[i].y = m_TextureVertex[i].y;
		m_VtxHuesos[i].z = m_TextureVertex[i].z;
	}

	DrawBB();
}

void Mesh::SetTexture(Texture t){
	m_pTexture = t;
}

void Mesh::Draw(){
	p_vb->Bind();
	p_ib->Bind();
	
	m_rRenderer.SetCurrentTexture(m_pTexture);
	m_rRenderer.Draw(m_Primitive);
}


//
void Mesh::GetBox(D3DXMATRIX * pMatrizMundo, D3DXVECTOR3* pOut){
	for (int i = 0; i < 8; i++){
		D3DXVec3TransformCoord(&pOut[i], &m_pBB[i], pMatrizMundo);
	}
}

void Mesh::DrawBB() const{
	D3DXVECTOR3 v_BoundMin;
	D3DXVECTOR3 v_BoundMax;

	v_BoundMin = D3DXVECTOR3(m_TextureVertex[0].x, m_TextureVertex[0].y, m_TextureVertex[0].z);
	v_BoundMax = D3DXVECTOR3(m_TextureVertex[0].x, m_TextureVertex[0].y, m_TextureVertex[0].z);


	for (int i = 1; i < m_VertexCount; i++){
		// X
		if (m_TextureVertex[i].x > v_BoundMax.x)
			v_BoundMax.x = m_TextureVertex[i].x;
		else if (m_TextureVertex[i].x < v_BoundMin.x)
			v_BoundMin.x = m_TextureVertex[i].x;

		// Y
		if (m_TextureVertex[i].y > v_BoundMax.y)
			v_BoundMax.y = m_TextureVertex[i].y;
		else if (m_TextureVertex[i].y < v_BoundMin.y)
			v_BoundMin.y = m_TextureVertex[i].y;

		// Z
		if (m_TextureVertex[i].z > v_BoundMax.z)
			v_BoundMax.z = m_TextureVertex[i].z;
		else if (m_TextureVertex[i].z < v_BoundMin.z)
			v_BoundMin.z = m_TextureVertex[i].z;
	}

	m_pBB[0] = v_BoundMax;
	m_pBB[1] = D3DXVECTOR3(v_BoundMax.x, v_BoundMin.y, v_BoundMax.z);
	m_pBB[2] = D3DXVECTOR3(v_BoundMin.x, v_BoundMin.y, v_BoundMax.z);
	m_pBB[3] = D3DXVECTOR3(v_BoundMin.x, v_BoundMax.y, v_BoundMax.z);
	m_pBB[4] = D3DXVECTOR3(v_BoundMax.x, v_BoundMax.y, v_BoundMin.z);
	m_pBB[5] = D3DXVECTOR3(v_BoundMax.x, v_BoundMin.y, v_BoundMin.z);
	m_pBB[6] = v_BoundMin;
	m_pBB[7] = D3DXVECTOR3(v_BoundMin.x, v_BoundMax.y, v_BoundMin.z);
}
//
void Mesh::AnimationMeshDraw(Renderer* pRenderer){
	ZeroMemory((void*)VectorDraw, sizeof(D3DXVECTOR3) * m_VertexCount);
	for (int i = 0; i < m_vBoneData.size(); i++){
		std::map<unsigned int, float>::iterator iterator = m_vBoneData[i]->m_mWeight.begin();
		while (iterator != m_vBoneData[i]->m_mWeight.end())
		{
			D3DXVECTOR3 vectorAux(0, 0, 0);
			D3DXMATRIX mat = m_vBoneData[i]->m_pBone->getTransformation();
			D3DXMatrixMultiply(&mat, &m_vBoneData[i]->offsetMatrix, &mat);
			D3DXVec3TransformCoord(&vectorAux, &m_VtxHuesos[iterator->first], &mat);
			VectorDraw[iterator->first] += vectorAux *iterator->second;
			iterator++;
		}
	}
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	pRenderer->SetMatrix(World, &identity);
	for (int i = 0; i < m_VertexCount; i++){
		m_TextureVertex[i].x = VectorDraw[i].x;
		m_TextureVertex[i].y = VectorDraw[i].y;
		m_TextureVertex[i].z = VectorDraw[i].z;
	}
	pRenderer->SetCurrentTexture(m_pTexture);
	p_vb->SetVertexData((void *)m_TextureVertex, m_VertexCount);
	p_vb->Bind();
	p_ib->Bind();
	pRenderer->Draw(m_Primitive);
}