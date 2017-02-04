#pragma once
#include "myEngine_API.h"
#include "RenderTypes.h"
#include "Entity.h"
#include "Bone.h"
#include <string>
#include <vector>


namespace engine{
	enum CollisionResult{
		CollisionVertical,
		CollisionHorizontal,
		NoCollision,
		CollisionZ
	};

	class BoneInfo;
	class Renderer;
	class IndexBuffer;
	class VertexBuffer;
	class Node;

	class MYENGINE_API Mesh : public Entity{

	public:
		Mesh(Renderer& r, bool isTextured = false);
		~Mesh();

		void SetData(ColorVertex* , size_t vertexCount, engine::Primitive, unsigned short*, size_t indexCount);
		void SetData(MeshVertex*  , size_t vertexCount, engine::Primitive, unsigned short*, size_t indexCount);
		void SetTexture(Texture);
		
		void Draw();
		void DrawBB() const;
		void Mesh::GetBox(D3DXMATRIX *, D3DXVECTOR3*);

		///
		void AnimationMeshDraw(Renderer* pRenderer);
		void AddBoneInfo(BoneInfo* m_pBoneInfo) { m_vBoneData.push_back(m_pBoneInfo); }
		bool HasBones() { return (m_vBoneData.size()); }
		///

		D3DXVECTOR3* m_pBB;


		D3DXPLANE GetPlane(D3DXMATRIX*);

	private:
//		IndexBuffer* m_pIndexBuffer;
//		VertexBuffer* m_pVertexBuffer;
		Primitive m_Primitive;
		Renderer& m_rRenderer;
		Texture m_pTexture;

		MeshVertex* m_TextureVertex;
		unsigned int m_VertexCount = 0;

		//

		friend class Node;
		D3DXVECTOR3* m_VtxHuesos;
		D3DXVECTOR3* VectorDraw;
		std::vector<BoneInfo*> m_vBoneData;
		//

	};
}