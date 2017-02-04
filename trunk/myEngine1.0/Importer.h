#pragma once
#include "myEngine_API.h"
#include "TinyXML\tinyxml2.h"
#include <string>
#include <map>
#include <vector>
#include "RenderTypes.h"
#include "XMLParser.h"
#pragma comment(lib, "../external/assimp/lib/assimp.lib")

// ----------------- Assimp ----------------- //
#include "../external/assimp/include/Importer.hpp"
#include "../external/assimp/include/types.h"
#include "../external/assimp/include/scene.h"
#include "../external/assimp/include/postprocess.h"
// ------------------------------------------ //

// Estructuras de ayuda de Assimp
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;
struct aiAnimation;
// - Fin Estructuras de Ayuda de Assimp


namespace engine{
	class Scene;
	class Sprite;
	class Quad;
	class Renderer;
	class Animation;
	class Importer;
	class TileMap;
	class Tile;
	class Mesh;
	class Node;
	class Bone;

	class MYENGINE_API Importer{
		public:
			Importer();
			~Importer();
			bool Init(Renderer&);

			void ImportNode(Node&, aiNode*, const aiScene*);
			void ImportMesh(const aiMesh*, const aiMaterial*, Mesh&);
			bool ImportScene(const std::string&, Node&);

			void ImportAnimations(std::vector<Animation*>&, tinyxml2::XMLElement*);		// Sprites --- Not in use

			Animation* CreateAnimation3D(aiAnimation* anim);
			std::map<std::string, Bone*> m_pBoneMap;
			void addBonesToNode(Node* pkNode);

			Animation* import3DAnimation(aiAnimation*);

			static Importer* GetInstance(){
				return m_pInstance;
			}

			Renderer* GetRenderer() const;
			Texture LoadTexture(std::string path, int KeyCode = 0);				// Sprites --- Not in use

		private:
			int ColorConverter(int);
			std::string m_sCurrentModelPath;
			static Importer* m_pInstance;
			Renderer* m_pRenderer;
			std::map<std::string, Texture> m_mTextureMap;
	};
}