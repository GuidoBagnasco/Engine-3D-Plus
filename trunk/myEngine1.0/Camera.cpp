#include "Camera.h"
#include "Renderer.h"
using namespace engine;

Camera::Camera()
	:
	m_Position(0.0f, 0.0f, 0.0f), //Set position to 0,0,0
	m_LookAt(0.0f, 0.0f, 1.0f), //Set look at to 0,0,1
	m_Right(1.0f, 0.0f, 0.0f), //Set right to 1,0,0
	m_Up(0.0f, 1.0f, 0.0f), //Set up to 0,1,0
	m_RotateAroundUp(0.0f),
	m_RotateAroundRight(0.0f),
	m_RotateAroundLookAt(0.0f)
{
	D3DXMatrixIdentity(&m_MatView);
}
//----------------------------------------------------------------
bool Camera::Init(Renderer* pkRenderer) {
	m_pkRenderer = pkRenderer;
	return true;
}
Camera::~Camera()
{
	// Delete Frustum Planes...
	/*for(unsigned int i=0; i < 6; i++){
	delete m_FrustumPlane[i];
	m_FrustumPlane[i] = NULL;
	}*/
}
//----------------------------------------------------------------
void Camera::SetPosition(float fX, float fY, float fZ)
{
	m_Position = D3DXVECTOR3(fX, fY, fZ);
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveForward(float Dist)
{
	m_Position += Dist*m_LookAt;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveRight(float Dist)
{
	m_Position += Dist*m_Right;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveUp(float Dist)
{
	m_Position += Dist*m_Up;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveInDirection(float Dist, float fDirectionX, float fDirectionY, float fDirectionZ)
{
	m_Position += Dist*D3DXVECTOR3(fDirectionX, fDirectionY, fDirectionZ);
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Pitch(float Angle)
{
	m_RotateAroundRight = Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Yaw(float Angle)
{
	m_RotateAroundUp = Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Roll(float Angle)
{
	m_RotateAroundLookAt += Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Update()
{
	if (m_bChanged)
	{
		//Matrices to store the transformations about our axes
		D3DXMATRIX MatTotal;
		D3DXMATRIX MatRotateAroundRight;
		D3DXMATRIX MatRotateAroundUp;
		D3DXMATRIX MatRotateAroundLookAt;
		//Get the matrix for each rotation
		D3DXMatrixRotationAxis(&MatRotateAroundRight, &m_Right, m_RotateAroundRight);
		D3DXMatrixRotationAxis(&MatRotateAroundUp, &m_Up, m_RotateAroundUp);
		D3DXMatrixRotationAxis(&MatRotateAroundLookAt, &m_LookAt, m_RotateAroundLookAt);
		//Combine the transformations into one matrix
		D3DXMatrixMultiply(&MatTotal, &MatRotateAroundUp, &MatRotateAroundRight);
		D3DXMatrixMultiply(&MatTotal, &MatRotateAroundLookAt, &MatTotal);
		//Transforms two vectors by our matrix and computes the third by
		//cross product
		D3DXVec3TransformCoord(&m_Right, &m_Right, &MatTotal);
		D3DXVec3TransformCoord(&m_Up, &m_Up, &MatTotal);
		D3DXVec3Cross(&m_LookAt, &m_Right, &m_Up);
		//Check to ensure vectors are perpendicular
		if (fabs(D3DXVec3Dot(&m_Up, &m_Right)) > 0.01)
		{
			//If theyre not
			D3DXVec3Cross(&m_Up, &m_LookAt, &m_Right);
		}
		//Normalize our vectors
		D3DXVec3Normalize(&m_Right, &m_Right);
		D3DXVec3Normalize(&m_Up, &m_Up);
		D3DXVec3Normalize(&m_LookAt, &m_LookAt);
		//Compute the bottom row of the view matrix
		float fView41, fView42, fView43;
		fView41 = -D3DXVec3Dot(&m_Right, &m_Position);
		fView42 = -D3DXVec3Dot(&m_Up, &m_Position);
		fView43 = -D3DXVec3Dot(&m_LookAt, &m_Position);
		//Fill in the view matrix
		m_MatView = D3DXMATRIX(m_Right.x, m_Up.x, m_LookAt.x, 0.0f,
			m_Right.y, m_Up.y, m_LookAt.y, 0.0f,
			m_Right.z, m_Up.z, m_LookAt.z, 0.0f,
			fView41, fView42, fView43, 1.0f);
		//BuildFrustum();
	}
	//Set view transform
	m_pkRenderer->loadIdentity();
	m_pkRenderer->setTransformMatrix(&m_MatView);
	//Reset update members
	m_RotateAroundRight = m_RotateAroundUp = m_RotateAroundLookAt = 0.0f;
	m_bChanged = false;
}
