#include "Animation.h"
using namespace engine;

Animation::Animation(std::string animName, double duration, double delta)
	: m_strName(animName), m_dbDuration(duration), m_dbDelta(delta), m_currentState(PLAY), m_fSpeed(1.0f) {

	if (this->m_dbDelta <= 0) {
		this->m_dbDelta = 1500;
	}
}

int Animation::GetFrameIndex(std::string node) {
	for (int i = 0; i < m_vecFrames.size(); i++) {
		if (m_vecFrames[i]->name == node)
			return i;
	}

	return -1;
}

D3DXMATRIX Animation::Interpolate(double& time, Frame* actualFrame) {
	D3DXMATRIX returnMatrix;
	D3DXMatrixIdentity(&returnMatrix);

	D3DXVECTOR3 position = InterpolateVector(time, actualFrame->positions, actualFrame->numPositions);
	D3DXQUATERNION rotation = InterpolateRotation(time, actualFrame->rotations, actualFrame->numRotations);
	D3DXVECTOR3 scale = InterpolateVector(time, actualFrame->scalings, actualFrame->numScalings);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);

	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling, scale.x, scale.y, scale.z);

	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &rotation);

	D3DXMatrixMultiply(&returnMatrix, &trans, &returnMatrix);
	D3DXMatrixMultiply(&returnMatrix, &rot, &returnMatrix);
	D3DXMatrixMultiply(&returnMatrix, &scaling, &returnMatrix);

	return returnMatrix;
}


D3DXVECTOR3 Animation::InterpolateVector(double& time, aiVectorKey* keys, int indexs) {
	D3DXVECTOR3 final;

	int aux1 = -1;
	int aux2 = -1;

	for (int i = 0; i < indexs; i++) {
		if (keys[i].mTime == time)
			return D3DXVECTOR3(keys[i].mValue.x, keys[i].mValue.y, keys[i].mValue.z);

		if (time < keys[i].mTime) {
			aux1 = i - 1;
			aux2 = i;
			break;
		}
	}

	float delta = (time - keys[aux1].mTime) / (keys[aux2].mTime - keys[aux1].mTime);
	D3DXVECTOR3 vector1(keys[aux1].mValue.x, keys[aux1].mValue.y, keys[aux1].mValue.z);
	D3DXVECTOR3 vector2(keys[aux2].mValue.x, keys[aux2].mValue.y, keys[aux2].mValue.z);

	final = vector2 * delta + vector1 * (1.0f - delta);
	return final;
}

D3DXQUATERNION Animation::InterpolateRotation(double& time, aiQuatKey* keys, int indexs) {
	int aux1 = -1;
	int aux2 = -1;

	for (int i = 0; i < indexs; i++) {
		if (keys[i].mTime == time)
			return D3DXQUATERNION(keys[i].mValue.x, keys[i].mValue.y, keys[i].mValue.z, keys[i].mValue.w);

		if (time < keys[i].mTime) {
			aux1 = i - 1;
			aux2 = i;
			break;
		}
	}

	float delta = (time - keys[aux1].mTime) / (keys[aux2].mTime - keys[aux1].mTime);
	const aiQuaternion& q1 = keys[aux1].mValue;
	const aiQuaternion& q2 = keys[aux2].mValue;

	aiQuaternion rotation;
	aiQuaternion::Interpolate(rotation, q1, q2, delta);
	return D3DXQUATERNION(rotation.x, rotation.y, rotation.z, rotation.w);
}

void Animation::Update(const double& deltaTime) {
	if (m_currentState == PLAY) {
		m_dbTime += (deltaTime / 1000) * m_dbDelta * m_fSpeed;
		while (m_dbTime >= m_dbDuration)
			m_dbTime -= m_dbDuration;

		if (m_dbTime <= 0)
			m_dbTime = m_dbDuration;
	}
}

