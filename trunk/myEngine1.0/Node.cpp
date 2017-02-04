#include "Node.h"
#include "Mesh.h"
#include "Renderer.h"

#include "timer\Timer.h"
#include "animation.h"

#include <iostream>
using namespace engine;


Node::Node() : bone(NULL), currentAnimation(NULL), animationFrame(-1), m_vBB(new D3DXVECTOR3[8])
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixIdentity(&local);
	isPlane = false;
}

D3DXPLANE Node::GetPlane() {
	return m_vMeshes[0]->GetPlane(&world);
}

Node::~Node(){
	if (m_vBB) {
		delete m_vBB;
		m_vBB = NULL;
	}

	for (int i = 0; i < m_vChilds.size(); i++) {
		delete m_vChilds[i];
		m_vChilds[i] = NULL;
	}

	for (int i = 0; i < m_vMeshes.size(); i++) {
		delete m_vMeshes[i];
		m_vMeshes[i] = NULL;
	}
}



void Node::ReceiveAssimpData(D3DXMATRIX* mx){
	defaultMatrix = *mx;
}


void Node::Draw(Renderer& r){

	D3DXMATRIX matrix;

	D3DXMatrixIdentity(&matrix);
	D3DXMatrixIdentity(&world);

	//r.SetMatrix(World, &matrix);
	r.CalculateFrustrum();

	UpdateTransformation(world);
	NodeDraw(r);
}

void Node::DrawMeshes(Renderer& r) {
	for (int i = 0; i < m_vMeshes.size(); i++) {
		r.SetMatrix(World, &world);
		m_vMeshes[i]->Draw();
	}
}

void Node::UpdateTransformation(D3DXMATRIX parentWorld){
	D3DXMATRIX translation;
	D3DXMATRIX scalation;
	D3DXMATRIX rotx;
	D3DXMATRIX roty;
	D3DXMATRIX rotz;

	D3DXMatrixIdentity(&local);

	// translation
	D3DXMatrixTranslation(&translation, m_position.x + pivot.x, m_position.y + pivot.y, m_position.z + pivot.z);
	D3DXMatrixMultiply(&local, &translation, &local);


	//	rotation
	D3DXMatrixRotationX(&rotx, _RotX);
	D3DXMatrixRotationY(&roty, _RotY);
	D3DXMatrixRotationZ(&rotz, _RotZ);

	D3DXMatrixMultiply(&local, &rotx, &local);
	D3DXMatrixMultiply(&local, &roty, &local);
	D3DXMatrixMultiply(&local, &rotz, &local);

	
	// scalation
	D3DXMatrixScaling(&scalation, _ScaleX, _ScaleY, _ScaleZ);
	D3DXMatrixMultiply(&local, &scalation, &local);

	if (!IsAnimationPlaying()) {
		D3DXMatrixMultiply(&local, &local, &defaultMatrix);
	} else {
		D3DXMATRIX frame = currentAnimation->GetMatrix(animationFrame);
		D3DXMatrixMultiply(&local, &local, &frame);
	}


	D3DXMatrixMultiply(&world, &local, &parentWorld);

	if (bone != NULL) {
		bone->setTransformation(world);
	}

	if (m_vChilds.size() > 0){
		for (int i = 0; i <m_vChilds.size(); i++){
			m_vChilds[i]->UpdateTransformation(world);
		}
	}

	SetBB();
}
//

void Node::NodeDraw(Renderer& r){
	if (r.m_frustum->CheckCube(m_vBB)){
		std::cout << "dibuja \r";
		if (m_vChilds.size() > 0){
			for (int i = 0; i < m_vChilds.size(); i++){
				m_vChilds[i]->NodeDraw(r);
			}
		}

		if (m_vMeshes.size() > 0){
			for (int i = 0; i < m_vMeshes.size(); i++){
				if (!m_vMeshes[i]->HasBones()) {
					r.SetMatrix(World, &world);
					m_vMeshes[i]->Draw();
				}
				else {
					m_vMeshes[i]->AnimationMeshDraw(&r);
				}

			}
		}
	} else {
		std::cout << "no dibuja \r";
	}
}

void Node::AddChild(Node* child){
	m_vChilds.push_back(child);
}

void Node::AddMesh(Mesh* child){
	m_vMeshes.push_back(child);
}


Node* Node::GetChild(int index){
	if (m_vChilds.empty())
		return this;

	index = index > m_vChilds.size() ? m_vChilds.size() - 1 : index;
	index = index < 0 ? 0 : index;
	return m_vChilds[index];
}



Node* Node::GetChild(std::string name){
	if (m_vChilds.empty())
		return this;

	for (short i = 0; i < m_vChilds.size(); i++){
		if (m_vChilds[i]->GetName() == name)
			return m_vChilds[i];
	}

	return this;
}



int Node::GetChildsCount(){
	if (m_vChilds.empty())
		return -1;
	return m_vChilds.size();
}

//
void Node::SetBB(){
	if (!m_vMeshes.empty()){
		D3DXVECTOR3 v_vertices[8];

		m_vMeshes[0]->GetBox(&world, v_vertices);

		m_BoundMax = v_vertices[0];
		m_BoundMin = v_vertices[0];

		for (int i = 1; i < 8; i++){
			// X
			if (v_vertices[i].x > m_BoundMax.x)
				m_BoundMax.x = v_vertices[i].x;
			else if (v_vertices[i].x < m_BoundMin.x)
				m_BoundMin.x = v_vertices[i].x;

			// Y
			if (v_vertices[i].y > m_BoundMax.y)
				m_BoundMax.y = v_vertices[i].y;
			else if (v_vertices[i].y < m_BoundMin.y)
				m_BoundMin.y = v_vertices[i].y;

			// Z
			if (v_vertices[i].z > m_BoundMax.z)
				m_BoundMax.z = v_vertices[i].z;
			else if (v_vertices[i].z < m_BoundMin.z)
				m_BoundMin.z = v_vertices[i].z;
		}
	}

	/// aca 
	else if (!m_vChilds.empty()){
		m_vChilds[0]->GetLimits(&m_BoundMin, &m_BoundMax);
	}

	for (int i = 0; i < m_vChilds.size(); i++){
			D3DXVECTOR3 vChildMax = m_BoundMax;
			D3DXVECTOR3 vChildMin = m_BoundMin;

			m_vChilds[0]->GetLimits(&vChildMin, &vChildMax);

			if (vChildMax.x > m_BoundMax.x)
				m_BoundMax.x = vChildMax.x;

			if (vChildMax.y > m_BoundMax.y)
				m_BoundMax.y = vChildMax.y;

			if (vChildMax.z > m_BoundMax.z)
				m_BoundMax.z = vChildMax.z;

			if (vChildMin.x < m_BoundMin.x)
				m_BoundMin.x = vChildMin.x;

			if (vChildMin.y < m_BoundMin.y)
				m_BoundMin.y = vChildMin.y;

			if (vChildMin.z < m_BoundMin.z)
				m_BoundMin.z = vChildMin.z;
	}

	m_vBB[0] = m_BoundMax;
	m_vBB[1] = D3DXVECTOR3(m_BoundMax.x, m_BoundMin.y, m_BoundMax.z);
	m_vBB[2] = D3DXVECTOR3(m_BoundMin.x, m_BoundMin.y, m_BoundMax.z);
	m_vBB[3] = D3DXVECTOR3(m_BoundMin.x, m_BoundMax.y, m_BoundMax.z);
	m_vBB[4] = D3DXVECTOR3(m_BoundMax.x, m_BoundMax.y, m_BoundMin.z);
	m_vBB[5] = D3DXVECTOR3(m_BoundMax.x, m_BoundMin.y, m_BoundMin.z);
	m_vBB[6] = m_BoundMin;
	m_vBB[7] = D3DXVECTOR3(m_BoundMin.x, m_BoundMax.y, m_BoundMin.z);
}

void Node::GetLimits(D3DXVECTOR3* pMin, D3DXVECTOR3* pMax){
	*pMax = m_BoundMax;
	*pMin = m_BoundMin;
}

void Node::Update(Timer& t) {
	if (currentAnimation != NULL) {
		currentAnimation->Update(t.timeBetweenFrames());
	}
}

void Node::AddAnimation(Animation* animation) {
	if (!animations.count(animation->GetName())) {
		animations[animation->GetName()] = animation;
	}
}

void Node::SetAnimation(std::string anim) {
	if (animations.count(anim)) {
		Animation* animation = animations[anim];
		SetAnimation(animation);
	}
}

void Node::SetAnimation(Animation* anim) {
	if (currentAnimation != NULL)
		currentAnimation->Stop();
	currentAnimation = anim;
	animationFrame = currentAnimation->GetFrameIndex(this->GetName());
	for (int i = 0; i < GetChildsCount(); i++) {
		m_vChilds[i]->SetAnimation(anim);
	}
}

bool Node::IsAnimationPlaying() {
	if (currentAnimation != NULL)
		if (currentAnimation->GetState() == Animation::State::PLAY)
			if (animationFrame != -1)
				return true;

	return false;
}

Animation* Node::GetAnimation(std::string anim) {
	if (animations.count(anim)) {
		return animations[anim];
	}

	return NULL;
}

void Node::PlayAnimation(std::string name) {
	if (animations.count(name)) {
		if (currentAnimation != animations[name])
			SetAnimation(name);
		else currentAnimation->Play();
	}
}

void Node::PauseAnimation(){
	if (currentAnimation != NULL) {
		currentAnimation->Pause();
	}
}

void Node::StopAnimation(){
	if (currentAnimation != NULL) {
		currentAnimation->Stop();
	}
}
void Node::ResetAnimation(){
	if (currentAnimation != NULL) {
		currentAnimation->Reset();
	}
}