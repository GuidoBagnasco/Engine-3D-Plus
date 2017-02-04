#pragma once
#include <string>
#include <map>
#include <vector>
#include "myEngine_API.h"

namespace engine{
	class Renderer;
	class Mesh;
	class TileMap;
	class Timer;
	class NodeBSP;
	class Node;

	class MYENGINE_API Scene{
		public:
			Scene(std::string name);
			~Scene();
			void Update(Timer*);
			void Draw(Renderer*);
			void Add(std::string, Mesh*);
			void RemoveMesh(Mesh*);
			void RemoveMesh(std::string);
			Mesh* GetMesh(std::string);
			void SetName(std::string);
			std::string GetName();
			//colisiones
			bool AddClsGroup(std::string);
			bool RemoveClsGroup(std::string);
			bool AddMeshToClsGroup(Mesh*, std::string);
			bool RemoveMeshFromClsGroup(Mesh*);
			bool ChangeMeshFromClsGroup(Mesh*, std::string);
			void CheckColisions();
			//-----------------------------------------
		private:
			std::string m_sName;
			std::map<std::string, Mesh*> m_mapMesh;
			std::map<std::string, Mesh*>::iterator m_mIteratorMesh;
			typedef std::vector<Mesh*> Mesh3DVector;
			typedef Mesh3DVector::iterator Mesh3DVectorIter;	
			typedef std::map< std::string, Mesh3DVector*> CollisionGroupMap;
			typedef std::map< std::string, Mesh3DVector*>::iterator CollisionGroupMapIter;
			CollisionGroupMap m_pClsGroups;
			CollisionGroupMapIter m_pClsGroupsIter;
			
			//------------------

			CollisionGroupMapIter m_pClsGroupsIterA;
			CollisionGroupMapIter m_pClsGroupsIterB;
			Mesh3DVector *m_pEntVectorA;
			Mesh3DVector *m_pEntVectorB;
			Mesh *m_pEntA;
			Mesh *m_pEntB;
			//------------------

		public:	// Virtual functions to override if the user want to make his own scenes...
			virtual void OnStart() { }
			virtual void OnUpdate(Timer*) { } 
			virtual void OnShutdown() { }
	};
}