#pragma once
#include "Mesh.h"

namespace engine{

	class Renderer;

	class MYENGINE_API Cube : public Mesh{
	public:
		Cube(Renderer& rkRenderer);
	protected:
		void Init();
	};
}