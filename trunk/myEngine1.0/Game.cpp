#include "Game.h"
#include "Scene.h"
#include "Node.h"
#include "NodeBSP.h"
#include "Renderer.h"
#include "Camera.h"
using namespace engine;

Game::~Game(){
	for (m_mIterator = m_mapa.begin(); m_mIterator != m_mapa.end(); m_mIterator++) {
		delete m_mIterator->second;
	}
}


void Game::DrawScenes(Renderer* m_pRender, Timer* timer){
	m_mIterator = m_mapa.begin();
	do{
		m_mIterator->second->Update(timer);
		m_mIterator->second->Draw(m_pRender);
		m_mIterator++;
	} while (m_mIterator != m_mapa.end());
}


void Game::CreateScene(std::string str){
	Scene * escena = new Scene(str);
	//m_mapa[str] = escena;
	AddScene(escena);
}


Scene* Game::GetScene(std::string str){
	for (m_mIterator = m_mapa.begin(); m_mIterator != m_mapa.end(); m_mIterator++)
	{
		if (m_mIterator->first == str)
			return m_mIterator->second;
	}
	return NULL;
}


void Game::OnSceneInit(){
	m_mIterator = m_mapa.begin();
	do {
		m_mIterator->second->OnStart();
		m_mIterator++;
	} while (m_mIterator != m_mapa.end());
}


void Game::OnSceneUpdate(Timer* t){
	m_mIterator = m_mapa.begin();
	do {
		m_mIterator->second->OnUpdate(t);
		m_mIterator++;
	} while (m_mIterator != m_mapa.end());
}


void Game::OnSceneShutdown(){
	m_mIterator = m_mapa.begin();
	do {
		m_mIterator->second->OnShutdown();
		m_mIterator++;
	} while (m_mIterator != m_mapa.end());
}

void Game::Frame(engine::Renderer& r, engine::DirectInput&, engine::Timer&) {
	if (BSP) {
		for (int i = 0; i < m_vParentNodes.size(); i++)
			m_vParentNodes[i]->Draw(r);
		BSP->Draw(r, r.c->m_Position);
	}
}

void Game::AddScene(Scene* newScene){

	if (!newScene)	return;
	if (newScene->GetName() == "")
		newScene->SetName("NewScene-" + (m_mapa.size() + 1));
	m_mapa[newScene->GetName()] = newScene;
}

//BSP

void Game::AddToBSPTree(Node& node, bool isBSP, bool ready) {
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	node.UpdateTransformation(identity);
	if (!isBSP)
		m_vParentNodes.push_back(&node);
	else
		AddBSPPlanes(node);
	if (ready)
		SortTreeNodes();
}

void Game::AddBSPPlanes(Node& node) {
	if (node.isPlane)
	{
		D3DXPLANE plane = node.GetPlane();
		D3DXVECTOR3 point(node.world._41, node.world._42, node.world._43);
		NodeBSP* bspnode = new NodeBSP(plane, point);
		m_vBSPNodes.push_back(bspnode);
		bspnode->Name = node.GetName();
	}
	m_vNodes.push_back(&node);
	for (int i = 0; i < node.m_vChilds.size(); i++)
		AddBSPPlanes(*node.m_vChilds[i]);
}

void Game::SortTreeNodes(){
	if (m_vNodes.size() != 0){
		BSP = m_vBSPNodes[0];
		for (int i = 1; i < m_vBSPNodes.size(); i++)
			if (m_vBSPNodes[i])
				BSP->AddNode(m_vBSPNodes[i]);
		for (int i = 0; i < m_vNodes.size(); i++)
			if (m_vNodes[i]->m_vMeshes.size())
				BSP->AddChild(m_vNodes[i]);
	}

	m_vBSPNodes.clear();
	m_vNodes.clear();
}
