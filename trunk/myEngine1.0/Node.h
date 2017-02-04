#pragma once
#include "myEngine_API.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <map>
#include "Entity.h"

namespace engine{

	class Mesh;
	class Renderer;
	class Animation;
	class Timer;
	class Bone;

	class MYENGINE_API Node : public Entity{

	private:
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_Sc;
		
		
		std::vector<Mesh*> m_vMeshes;

		D3DXVECTOR3* m_vBB;
		void SetBB();
		void GetLimits(D3DXVECTOR3*, D3DXVECTOR3*);
		void UpdateTransformation(D3DXMATRIX transformation);

		friend class NodeBSP;
		friend class Game;
	public:
		Node();
		~Node();

		void Draw(Renderer&);
		void NodeDraw(Renderer&);

		D3DXMATRIX local;
		D3DXMATRIX world;
		D3DXMATRIX defaultMatrix;

		D3DXVECTOR3 m_BoundMin;
		D3DXVECTOR3 m_BoundMax;

		std::vector<Node*> m_vChilds;

		void ReceiveAssimpData(D3DXMATRIX*);
		void AddChild(Node* child);
		void AddMesh(Mesh* child);
		Node* GetChild(int index);
		Node* GetChild(std::string name);
		int GetChildsCount();

		bool isPlane;
		D3DXPLANE GetPlane();

	private:
		std::map<std::string, Animation*> animations;
		int animationFrame;
		Animation* currentAnimation;

	public:
		Bone* bone;
		void PlayAnimation(std::string);
		void PauseAnimation();
		void StopAnimation();
		void ResetAnimation();
		void AddAnimation(Animation*);
		void SetAnimation(std::string);
		void SetAnimation(Animation*);
		Animation* GetAnimation(std::string);
		void Update(Timer&) override;
		bool IsAnimationPlaying();


		void DrawMeshes(Renderer&);
	};

}