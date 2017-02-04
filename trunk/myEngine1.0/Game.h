#pragma once
#include "myEngine_API.h"
#include <string>
#include <map>
#include <vector>
namespace engine{
	class Renderer;
	class DirectInput;
	class Timer;
	class Scene;
	class NodeBSP;
	class Node;
	class MYENGINE_API Game{
	public:
		Game() : BSP(NULL) { GameOn = true;}
		~Game();
		virtual bool Init(engine::Renderer&) = 0;
		virtual void Frame(engine::Renderer&, engine::DirectInput&, engine::Timer&);
		virtual void DeInit() = 0;
		bool GameOn;
		void setGame(const bool b){ GameOn = b;}
		bool getGame(){ return GameOn;}

		void CreateScene(std::string);
		Scene* GetScene(std::string);
		void DrawScenes(Renderer*, Timer*);

		//Scenes
	protected:
		std::map<std::string, Scene*> m_mapa;
		std::map<std::string, Scene*>::iterator m_mIterator;
	public:
		void OnSceneInit();
		void OnSceneUpdate(Timer*);
		void OnSceneShutdown();
		void AddScene(Scene*);

		// BSP
	private:
		std::vector<Node*> m_vNodes;
		std::vector<Node*> m_vParentNodes;
		std::vector<NodeBSP*> m_vBSPNodes;
		NodeBSP* BSP;

	public:
		void AddBSPPlanes(Node&);
		void AddToBSPTree(Node&, bool, bool);
		void SortTreeNodes();
	};
}