#pragma once
#include "EMath.h"
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include "RenderTypes.h"
#include "myEngine_API.h"



namespace engine{
	class Renderer;
	class Timer;
	class VertexBuffer;
	class IndexBuffer;
	class MYENGINE_API Entity{
	public:
		Entity();
		~Entity();
		virtual void Update(Timer&){ }

		const std::string& GetCollisionGroup() const { return m_sColGroup; }
		void SetCollisionGroup(std::string ColGroup) { m_sColGroup = ColGroup; }

		void SetName(std::string n) { m_sName = n; }
		std::string GetName() { return m_sName;  }

		float PosX() const;
		float PosY() const;
		float PosZ() const;
		void ReturnToPreviousPos();

		float RotationX() const;
		float RotationY() const;
		float RotationZ() const;
		float ScaleX() const;
		float ScaleY() const;
		float ScaleZ() const;

		void SetPos(float fPosX, float fPosY, float fPosZ);
		void SetRotation(float, float, float);
		void SetScale(float fScale, float fScaleY, float fScaleZ = 1.0f);
		void SetPivot(float, float, float);

	private:
		std::string m_sColGroup;
		std::string m_sName;

		float m_fPrevPosX, m_fPrevPosY, m_fPrevPosZ;

	protected:
		D3DXVECTOR3 m_position;
		float _RotX, _RotY, _RotZ;
		float _ScaleX, _ScaleY, _ScaleZ;

		D3DXVECTOR3 pivot;

		VertexBuffer * p_vb;
		IndexBuffer * p_ib;
	};
}