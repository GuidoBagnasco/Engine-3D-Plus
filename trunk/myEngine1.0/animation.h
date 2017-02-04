#pragma once
#include "myEngine_API.h"
#include <d3dx9math.h>
#include <string>
#include <vector>
#include "Importer.h"

namespace engine {
	class MYENGINE_API Animation{
	public:
		struct Frame{
			std::string name;
			aiVectorKey* positions;
			int numPositions;
			aiVectorKey* scalings;
			int numScalings;
			aiQuatKey* rotations;
			int numRotations;
		};
		enum State{
			PLAY,
			STOP,
			PAUSE
		};

	private:

		std::string m_strName;
		std::vector<Frame*> m_vecFrames;
		State m_currentState;
		double m_dbDuration;
		double m_dbDelta;
		double m_dbTime;
		float m_fSpeed;
		D3DXMATRIX Interpolate(double&, Frame*);
		D3DXVECTOR3 InterpolateVector(double&, aiVectorKey*, int);
		D3DXQUATERNION InterpolateRotation(double&, aiQuatKey*, int);

	public:
		Animation(std::string, double, double);
		D3DXMATRIX GetMatrix(int index) { return Interpolate(m_dbTime, m_vecFrames[index]); }
		int GetFrameIndex(std::string);
		std::string GetName() { return m_strName; }
		State GetState() { return m_currentState; }
		void Update(const double&);
		void Play() { m_currentState = PLAY; }
		void Pause() { m_currentState = PAUSE; }
		void Stop() { m_currentState = STOP; m_dbTime = 0; }
		void Reset() { m_dbTime = 0; }
		void SetSpeed(float speed) { m_fSpeed = speed; }
		float GetSpeed() { return m_fSpeed; }
		void AddFrame(Frame* c) { m_vecFrames.push_back(c); }
	};
}