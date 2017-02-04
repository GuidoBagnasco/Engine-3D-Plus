#pragma once
#include "Game.h"
#include "Entity.h"
#include "RenderTypes.h"
#include "Renderer.h"
#include "input\DirectInput.h"
#include "timer\Timer.h"
#include <iostream>
#include "Mesh.h"
#include "Node.h"
#include "Cube.h"
#include "Camera.h"

namespace MyGame{
	class Game : public engine::Game{
	public:
		bool Init(engine::Renderer&);
		void Frame(engine::Renderer&, engine::DirectInput&, engine::Timer&);
		void DeInit();
	private:
		engine::Camera* mainCamera;
		engine::Mesh* plane;
		engine::Node* car;
		engine::Node* pig;
		engine::Node* pilot;
	};
}