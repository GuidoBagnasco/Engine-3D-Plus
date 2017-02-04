#include "Scene.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Node.h"
#include "NodeBSP.h"
#include "Camera.h"
#include <algorithm>
using namespace engine;

Scene::Scene(std::string sName){
	m_sName = sName;
}

Scene::~Scene(){
	m_pClsGroups.clear();
	for (m_mIteratorMesh = m_mapMesh.begin(); m_mIteratorMesh != m_mapMesh.end(); m_mIteratorMesh++)
	{
		delete m_mIteratorMesh->second;
	}
}

void Scene::SetName(std::string sName){
	m_sName = sName;
}

std::string Scene::GetName(){
	return m_sName;
}

void Scene::Draw(Renderer* m_pRender){
	if (m_mapMesh.size() > 0){
		m_mIteratorMesh = m_mapMesh.begin();
		do{
			/* DISABLE THIS ! Para no dibujar AABBS!*/ //m_mIteratorMesh->second->drawAABB(*m_pRender);
			m_mIteratorMesh->second->Draw();
			m_mIteratorMesh++;
		} while (m_mIteratorMesh != m_mapMesh.end());
	}
}

void Scene::Update(Timer* timer){
	if (m_mapMesh.size() > 0){
		CheckColisions();
		m_mIteratorMesh = m_mapMesh.begin();
		do
		{
			m_mIteratorMesh->second->Update(*timer);
			m_mIteratorMesh++;
		} while (m_mIteratorMesh != m_mapMesh.end());
	}
}

void Scene::Add(std::string sName, Mesh* pMesh){
	m_mIteratorMesh = m_mapMesh.begin();
	m_mIteratorMesh = m_mapMesh.find(sName);
	if (m_mIteratorMesh != m_mapMesh.end())
		return;
	m_mapMesh[sName] = pMesh;
}


void Scene::RemoveMesh(std::string sName){
	m_mapMesh.erase(sName);
}

void Scene::RemoveMesh(Mesh* pMesh){
	m_mIteratorMesh = m_mapMesh.begin();
	while (m_mIteratorMesh != m_mapMesh.end())
	{
		if (m_mIteratorMesh->second == pMesh)
			m_mapMesh.erase(m_mIteratorMesh);
	}
}

Mesh*Scene::GetMesh(std::string sName){
	/*m_mIteratorMesh = m_mapMesh.begin();
	m_mIteratorMesh = m_mapMesh.find(sNombre);
	return m_mIteratorMesh->second;*/

	if (m_mapMesh.count(sName))
		return m_mapMesh[sName];
	else
		return NULL;
}

bool Scene::AddClsGroup(std::string p_Name){
	if (p_Name == "") // que no pase nombre vacio
		return false;
	CollisionGroupMapIter iter = m_pClsGroups.find(p_Name); // y q no existe ya
	if (iter != m_pClsGroups.end())
		return false;

	Mesh3DVector* NewGroup = new Mesh3DVector();
	m_pClsGroups[p_Name] = NewGroup;

	return true;
}

bool Scene::RemoveClsGroup(std::string p_Name){
	if (p_Name == "") //que no sea vacio
		return false;
	CollisionGroupMapIter iter = m_pClsGroups.find(p_Name);
	if (iter == m_pClsGroups.end())
		return false; // y que existe

	m_pClsGroups.erase(iter);//entonces borro
	return true;
}

void Scene::CheckColisions(){
	/*
	if (m_pClsGroups.size()>1)//si el mapa es mayor a 1 tiene 2 grupos
	{
		//este no llega al end, porque va a llegar el siguiente, ya que sino hacemos todo doble
		for (m_pClsGroupsIterA = m_pClsGroups.begin(); m_pClsGroupsIterA != m_pClsGroups.end(); m_pClsGroupsIterA++)
		{
			m_pEntVectorA = m_pClsGroupsIterA->second;
			//lega al end
			for (m_pClsGroupsIterB = (++m_pClsGroupsIterA); m_pClsGroupsIterB != m_pClsGroups.end(); m_pClsGroupsIterB++)
			{
				m_pEntVectorB = m_pClsGroupsIterB->second;
				//recorro el primer vector
				for (int i = 0; i<m_pEntVectorA->size(); i++)
				{
					//me guardo la Mesh
					m_pEntA = (*m_pEntVectorA)[i];
					//recorro el SEGUNO vector
					for (int j = 0; j<m_pEntVectorB->size(); j++)
					{
						//me guardo la Mesh
						m_pEntB = (*m_pEntVectorB)[j];

						if (m_pEntA->CheckCollision(*m_pEntB) != NoCollision) // me fijo si coliciono
						{
							//m_pEntA->ReturnToPreviousPos();
							m_pEntB->m_node->ReturnToPreviousPos();
						}
					}
				}
				if (m_pEntVectorA->size() < 2)
					break;
			}
			if (m_pEntVectorB->size() < 2)
				break;
		}
	}*/
}

bool Scene::AddMeshToClsGroup(Mesh* p_pMesh, std::string p_ColGroup){
	//assert(p_pMesh);

	//me fijo que tenga nombre
	if (p_ColGroup != ""){
		m_pClsGroupsIter = m_pClsGroups.find(p_ColGroup);//lo busco

		if (m_pClsGroupsIter == m_pClsGroups.end())
			return false;//si no existe no lo agrego

		m_pEntVectorA = m_pClsGroupsIter->second;
		//assert(m_pEntVectorA);

		m_pEntVectorA->push_back(p_pMesh);//lo pongo en el vector grupo
		p_pMesh->SetCollisionGroup(p_ColGroup);//le digo cual es su grupo
	}
	return true;
}

bool Scene::RemoveMeshFromClsGroup(Mesh* p_pMesh){
	if (p_pMesh->GetCollisionGroup() != "") // si tiene grupo
	{
		m_pEntVectorA = m_pClsGroups[p_pMesh->GetCollisionGroup()];
		//assert(m_pEntVectorA);

		Mesh3DVectorIter ClsIterador = find(m_pEntVectorA->begin(), m_pEntVectorA->end(), p_pMesh);

		if (ClsIterador != m_pEntVectorA->end())
			m_pEntVectorA->erase(ClsIterador);
	}
	return true;
}

bool Scene::ChangeMeshFromClsGroup(Mesh* p_pMesh, std::string NewClsGroup){
	if (RemoveMeshFromClsGroup(p_pMesh)){
		if (AddMeshToClsGroup(p_pMesh, NewClsGroup))
			return true;
	}

	return false;
}