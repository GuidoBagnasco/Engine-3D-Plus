#include "Importer.h"
#include "Scene.h"
#include "Mesh.h"
#include "animation.h"
#include "Renderer.h"
#include "Node.h"
#include "Bone.h"
#include "animation.h"
#include <iostream>

using namespace engine;
Importer* Importer::m_pInstance = NULL;

Importer::Importer() : m_pRenderer(NULL){
}

bool Importer::Init(Renderer& r){
	m_pRenderer = &r;
	m_pInstance = this;	// Seteo mi singleton a this!
	return true;
}

Renderer* Importer::GetRenderer() const {
	return m_pRenderer;
}

int Importer::ColorConverter(int hexValue) {
  int r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  int g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  int b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
  return engine_COLOR_RGB(r,g,b);
}


bool Importer::ImportScene(const std::string& fileName, Node& SceneRoot){
	Assimp::Importer kImporter;

	const aiScene* AiScene = kImporter.ReadFile(fileName,
												aiPrimitiveType_LINE | aiPrimitiveType_POINT |
												aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);

	if (AiScene != NULL){
		SceneRoot.SetName(AiScene->mRootNode->mName.C_Str());
		for (int i = 0; i < AiScene->mNumAnimations; i++) {
			SceneRoot.AddAnimation(import3DAnimation(AiScene->mAnimations[i]));
		}
		m_sCurrentModelPath = fileName;
		ImportNode(SceneRoot, AiScene->mRootNode, AiScene);
		// Linkear huesos al nodo que le corresponda.
		addBonesToNode(&SceneRoot);

		return true;
	}

	return false;
}

void Importer::addBonesToNode(Node* fillNode){
	if (m_pBoneMap.count(fillNode->GetName())){
		fillNode->bone = m_pBoneMap[fillNode->GetName()];
	}
	for (int i = 0; i< fillNode->GetChildsCount(); i++){
		addBonesToNode(fillNode->m_vChilds[i]);
	}
}


void Importer::ImportNode(Node& rNode, aiNode* aiNode, const aiScene* aiScene){
	rNode.SetName(aiNode->mName.C_Str());

	if (rNode.GetName().find("BSP") != std::string::npos)
		rNode.isPlane = true;

	aiMatrix4x4 mx = aiNode->mTransformation.Transpose();
	D3DXMATRIX mxD3DX = D3DXMATRIX(mx.a1, mx.a2, mx.a3, mx.a4,
									mx.b1, mx.b2, mx.b3, mx.b4,
									mx.c1, mx.c2, mx.c3, mx.c4,
									mx.d1, mx.d2, mx.d3, mx.d4);


	rNode.ReceiveAssimpData(&mxD3DX);

	for (unsigned int i = 0; i < aiNode->mNumChildren; i++){
		Node* pkNode = new Node();
		
		pkNode->SetName(aiNode->mChildren[i]->mName.C_Str());

		ImportNode(*pkNode, aiNode->mChildren[i], aiScene);
		rNode.AddChild(pkNode);
	}

	for (unsigned int i = 0; i < aiNode->mNumMeshes; i++){
		Mesh* pkMesh = new Mesh(*m_pRenderer, true);
		rNode.AddMesh(pkMesh);

		aiMesh* pkAiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];
		aiMaterial* pkAiMaterial = aiScene->mMaterials[pkAiMesh->mMaterialIndex];

		ImportMesh(pkAiMesh, pkAiMaterial, *pkMesh);
	}
}



void Importer::ImportMesh(const aiMesh* pkAiMesh, const aiMaterial* pkAiMaterial, Mesh& kMesh){
	kMesh.SetName(pkAiMesh->mName.C_Str());

	MeshVertex* pVertices = new MeshVertex[pkAiMesh->mNumVertices];
	for (unsigned int i = 0; i<pkAiMesh->mNumVertices; i++){
		pVertices[i].x = pkAiMesh->mVertices[i].x;
		pVertices[i].y = pkAiMesh->mVertices[i].y;
		pVertices[i].z = pkAiMesh->mVertices[i].z;
		if (pkAiMesh->mTextureCoords[0] != NULL) {
			pVertices[i].u = pkAiMesh->mTextureCoords[0][i].x;
			pVertices[i].v = pkAiMesh->mTextureCoords[0][i].y;
		}

		if (pkAiMesh->HasNormals()){
			pVertices[i].nx = pkAiMesh->mNormals[i].x;
			pVertices[i].ny = pkAiMesh->mNormals[i].y;
			pVertices[i].nz = pkAiMesh->mNormals[i].z;
		}
	}

	size_t uiIndexCount = pkAiMesh->mNumFaces * 3;
	unsigned short* pausIndices = new unsigned short[uiIndexCount];

	for (unsigned int i = 0; i<pkAiMesh->mNumFaces; i++){
		pausIndices[i * 3 + 0] = pkAiMesh->mFaces[i].mIndices[0];
		pausIndices[i * 3 + 1] = pkAiMesh->mFaces[i].mIndices[1];
		pausIndices[i * 3 + 2] = pkAiMesh->mFaces[i].mIndices[2];
	}

	kMesh.SetData(pVertices, pkAiMesh->mNumVertices, engine::Primitive::TriangleList, pausIndices, uiIndexCount);
	kMesh.SetName(pkAiMesh->mName.C_Str());

	if (pkAiMesh->HasBones()) {
		for (int i = 0; i < pkAiMesh->mNumBones; i++) {
			/*
				Crear el hueso.
				recorrer los pesos del hueso, y almacenarlos.

				extraer offsetMatrix del hueso
				guardar offsetMatrix del hueso

				guardar el hueso en nuestro map[]
				setearle el hueso al mesh.
			*/
			aiBone* bone = pkAiMesh->mBones[i];
			BoneInfo* bInfo = new BoneInfo();
			for (int j = 0; j < bone->mNumWeights; j++){
				bInfo->AddWeight(bone->mWeights[j].mVertexId, bone->mWeights[j].mWeight);
			}
			aiMatrix4x4 m = bone->mOffsetMatrix.Transpose();
			bInfo->setOffsetMatrix(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4,
				m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);
			std::string strName = bone->mName.C_Str();
			if (!m_pBoneMap.count(strName)){
				m_pBoneMap[strName] = new Bone();
				bInfo->SetBone(m_pBoneMap[strName]);
			}
			else
				bInfo->SetBone(m_pBoneMap[strName]);
			kMesh.AddBoneInfo(bInfo);
		}
	}
	if (pkAiMaterial){
		size_t found = m_sCurrentModelPath.find_last_of("/\\");
		std::string texPath = m_sCurrentModelPath.substr(0, found + 1);
		aiString aipath;
		pkAiMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), aipath);
		texPath.append(aipath.C_Str());

		Texture TheTexture = GetRenderer()->LoadTexture(texPath);
		kMesh.SetTexture(TheTexture);
	}
}
/*
 *  Documentation: http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
 */
Animation* Importer::import3DAnimation(aiAnimation* assimpAnimation) {
	Animation* anim = new Animation(assimpAnimation->mName.C_Str(), assimpAnimation->mDuration, assimpAnimation->mTicksPerSecond);
	for (int i = 0; i < assimpAnimation->mNumChannels; i++) {
		aiNodeAnim* channel = assimpAnimation->mChannels[i];
		
		Animation::Frame * frame = new Animation::Frame();

		frame->name = channel->mNodeName.C_Str();
		frame->numPositions = channel->mNumPositionKeys;
		frame->numRotations = channel->mNumRotationKeys;
		frame->numScalings = channel->mNumScalingKeys;

		frame->positions = new aiVectorKey[channel->mNumPositionKeys];
		for (int j = 0; j < channel->mNumPositionKeys; j++) {
			frame->positions[j].mTime = channel->mPositionKeys[j].mTime;
			frame->positions[j].mValue = channel->mPositionKeys[j].mValue;
		}

		frame->scalings = new aiVectorKey[channel->mNumScalingKeys];
		for (int j = 0; j < channel->mNumScalingKeys; j++) {
			frame->scalings[j].mTime = channel->mScalingKeys[j].mTime;
			frame->scalings[j].mValue = channel->mScalingKeys[j].mValue;
		}


		frame->rotations = new aiQuatKey[channel->mNumRotationKeys];
		for (int j = 0; j < channel->mNumRotationKeys; j++) {
			frame->rotations[j].mTime = channel->mRotationKeys[j].mTime;
			frame->rotations[j].mValue = channel->mRotationKeys[j].mValue;
		}

		anim->AddFrame(frame);
	}

	return anim;
}