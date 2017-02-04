#include "Scene.h"
#include "timer\Timer.h"
#include <iostream>

namespace MyGame{
	class MyCustomScene : public engine::Scene{
	public:
		MyCustomScene(std::string s) : Scene(s){ }
		~MyCustomScene() { }
		virtual void OnStart() { std::cout << "Initialized" << std::endl;  }
		virtual void OnUpdate(engine::Timer*) {  }
		virtual void OnShutdown() { std::cout << "Shuting down" << std::endl; }
	};
}