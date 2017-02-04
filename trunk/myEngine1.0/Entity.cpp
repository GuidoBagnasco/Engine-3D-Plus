#include "Entity.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <d3dx9.h>
using namespace engine;


Entity::Entity() :
m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_fPrevPosX(0.0f),
m_fPrevPosY(0.0f),
m_fPrevPosZ(0.0f),
_RotX(0.0f),
_RotY(0.0f),
_RotZ(0.0f),
_ScaleX(1.0f),
_ScaleY(1.0f),
_ScaleZ(1.0f),
pivot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_sColGroup(""),
m_sName(""),
p_vb(NULL),
p_ib(NULL)
{

}


Entity::~Entity(){
	if (p_vb){
		delete p_vb;
		p_vb = NULL;
	}

	if (p_ib){
		delete p_ib;
		p_ib = NULL;
	}
}

void Entity::SetPos(float fPosX, float fPosY, float fPosZ){
	m_fPrevPosX = m_position.x;
	m_fPrevPosY = m_position.y;
	m_fPrevPosZ = m_position.z;

	m_position.x = fPosX;
	m_position.y = fPosY;
	m_position.z = fPosZ;
}

// no se usa
void Entity::ReturnToPreviousPos(){
	m_position.x = m_fPrevPosX;
	m_position.y = m_fPrevPosY;
	m_position.z = m_fPrevPosZ;
}


float Entity::PosX() const{
	return m_position.x;
}


float Entity::PosY() const{
	return m_position.y;
}


float Entity::PosZ() const{
	return m_position.z;
}

float Entity::ScaleX() const{
	return _ScaleX;
}


float Entity::ScaleY() const{
	return _ScaleY;
}


float Entity::ScaleZ() const{
	return _ScaleZ;
}


float Entity::RotationX() const{
	return _RotX;
}


float Entity::RotationY() const{
	return _RotY;
}


float Entity::RotationZ() const{
	return _RotZ;
}



void Entity::SetRotation(float fRotX, float fRotY, float fRotZ){
	//x - x1
	_RotX = fRotX;
	_RotY = fRotY;
	_RotZ = fRotZ;
	/*
	_RotX -= pivot.x;
	_RotY -= pivot.y;
	_RotZ -= pivot.z;
	*/
}


void Entity::SetPivot(float x, float y, float z){
	pivot = D3DXVECTOR3(x, y, z);
}


void Entity::SetScale(float fScaleX, float fScaleY, float fScaleZ){
	_ScaleX = fScaleX;
	_ScaleY = fScaleY;
	_ScaleZ = fScaleZ;
}