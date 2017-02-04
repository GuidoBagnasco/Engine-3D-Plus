#pragma once
#include "myEngine_API.h"
#include <vector>
#include <string>
#include <d3dx9.h>

namespace engine {
	class Node;
	class Renderer;
	class NodeBSP {
		D3DXPLANE m_Plane;
		D3DXVECTOR3 m_PlanePoint;
		NodeBSP* m_pFrontNode;
		NodeBSP* m_pBackNode;
		std::vector<Node*> m_vBack;
		std::vector<Node*> m_vFront;
		enum Position
		{
			BACK, FRONT, OVERLAP
		};
		Position GetBoxPos(D3DXVECTOR3[]);
		Position GetPointPos(D3DXVECTOR3&);
	public:
		NodeBSP(D3DXPLANE, D3DXVECTOR3);
		~NodeBSP();

		friend class Node;
		void AddNode(NodeBSP*);
		void Draw(Renderer&, D3DXVECTOR3);
		void AddChild(Node*);
		std::string Name;
	};
}

