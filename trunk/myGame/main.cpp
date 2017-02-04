//----------------
#include "Engine.h"
#include "MyGame.h"
#include <iostream>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow){
	engine::Engine myEngine(hInst, "Engine-3D", 640, 480);
	myEngine.init();
	myEngine.G = new MyGame::Game();
	myEngine.run();
}