#pragma once

#include "./myEngine_API.h"
#include "./EMath.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

namespace engine {
	class Renderer;
	class MYENGINE_API Camera {
	public:
		Camera();
		bool Init(Renderer* pkRenderer);

	protected:
		Renderer* m_pkRenderer;
		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_LookAt;
		D3DXVECTOR3 m_Right;
		D3DXVECTOR3 m_Up;
		bool m_bChanged;
		float m_RotateAroundUp;
		float m_RotateAroundRight;
		float m_RotateAroundLookAt;

		D3DXMATRIX m_MatView; // View Matrix from Camera.
							  //Plane	m_FrustumPlane[6];

	public:

		void SetPosition(float fX, float fY, float fZ);
		D3DXVECTOR3	GetPosition() { return m_Position; }
		void MoveForward(float Dist);
		void MoveRight(float Dist);
		void MoveUp(float Dist);
		void MoveInDirection(float Dist, float fDirectionX, float fDirectionY, float fDirectionZ);
		void Pitch(float Angle);
		void Yaw(float Angle);
		void Roll(float Angle);


	protected:
		void Update();
		friend class Engine;
	private:
		~Camera();
		friend class Scene;
		friend class Game;
		friend class Renderer;
	};
}