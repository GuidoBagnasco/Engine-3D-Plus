#include "MyGame.h"
#include "animation.h"
#include "Scene.h"
#include "Importer.h"
#include "MyCustomScene.h"
#include <iostream>

using namespace MyGame;
float mSpeed = 0.1f;


engine::Node* bsp;

bool Game::Init(engine::Renderer& r){
	mainCamera = r.c;
	mainCamera->SetPosition(0, 10, -50);
	CreateScene("Scene1");
	engine::Scene* scene1 = GetScene("Scene1");

	MyCustomScene* mcs = new MyCustomScene("MyCustomSceneYAY");
	AddScene(mcs);

	engine::Importer* importer = engine::Importer::GetInstance();

	r.SetBackgroundColor(0, 0, 0);
	
	car = new engine::Node();
	if (importer->ImportScene("Porsche.obj", *car)){
		car->SetPos(-50.0f, 0.0f, -50.0f);
		car->SetScale(10.0f, 10.0f, 10.0f);
		AddToBSPTree(*car, true, false);
	}
	else{
		delete car;
		car = NULL;
	}

	pig = new engine::Node();
	if (importer->ImportScene("cerdo.obj", *pig)){
		pig->SetScale(1.0f, 1.0f, 1.0f);
		AddToBSPTree(*pig, true, false);
	}
	else{
		delete pig;
		pig = NULL;
	}

	pilot = new engine::Node();
	if (importer->ImportScene("ArmyPilot/ArmyPilot.x", *pilot)){
		pilot->SetPos(65.0f, 0.0f, -15.0f);
		pilot->SetScale(0.1f, 0.1f, 0.1f);
		pilot->SetRotation(0.0f, 0.00f, 0.00f);
	}
	else{
		delete pilot;
		pilot = NULL;
	}

	bsp = new engine::Node();
	importer->ImportScene("bspscene.3ds", *bsp);

	AddToBSPTree(*bsp, true, true);

	return true;
}


void Game::Frame(engine::Renderer& r, engine::DirectInput& dInput, engine::Timer& timer){

	engine::Game::Frame(r, dInput, timer);

	if(dInput.keyDown(engine::Input::KEY_UP) || dInput.keyDown(engine::Input::KEY_W)){
		mainCamera->MoveForward(mSpeed * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_DOWN) || dInput.keyDown(engine::Input::KEY_S)){
		mainCamera->MoveForward(-mSpeed * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_LEFT) || dInput.keyDown(engine::Input::KEY_A)){
		mainCamera->MoveRight(-mSpeed * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_RIGHT) || dInput.keyDown(engine::Input::KEY_D)){
		mainCamera->MoveRight(mSpeed * timer.timeBetweenFrames());
	}

	if (dInput.mouseDown(engine::Input::MB_1)){
		mainCamera->Yaw(dInput.mouseRelPosX() * mSpeed / 200 * timer.timeBetweenFrames());
	}	

	if(dInput.keyDown(engine::Input::KEY_SPACE)){
		mainCamera->MoveUp(mSpeed * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_LCONTROL)){
		mainCamera->MoveUp(-mSpeed * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_E)){
		mainCamera->Roll(-mSpeed / 100 * timer.timeBetweenFrames());
	}

	if(dInput.keyDown(engine::Input::KEY_Q)){
		mainCamera->Roll(mSpeed / 100 * timer.timeBetweenFrames());
	}

	if (dInput.keyDown(engine::Input::KEY_ESCAPE))
		setGame(false);

	
	if (dInput.keyDown(engine::Input::KEY_1))
		pilot->PlayAnimation("Idle");
	else if (dInput.keyDown(engine::Input::KEY_2))
		pilot->PlayAnimation("Run_Firing");
	else if (dInput.keyDown(engine::Input::KEY_3))
		pilot->PlayAnimation("Stand_Aim_Up");
	else if (dInput.keyDown(engine::Input::KEY_4))
		pilot->PlayAnimation("Run_Forwards");
	else if (dInput.keyDown(engine::Input::KEY_5))
		pilot->PauseAnimation();
	else if (dInput.keyDown(engine::Input::KEY_6))
		pilot->StopAnimation();
	
	if (pilot->IsAnimationPlaying()) {
		pilot->Update(timer);
	}

	if (pilot != NULL)
		pilot->Draw(r);

	if (dInput.keyDown(engine::Input::KEY_P)){
		pig->SetPos(pig->PosX() + timer.timeBetweenFrames() * 0.01f, pig->PosY(), pig->PosZ());
	}
	
	AddToBSPTree(*pig, true, false);
	AddToBSPTree(*car, true, false);
	AddToBSPTree(*bsp, true, true);
}


void Game::DeInit(){
	if (bsp){
		delete bsp;
		bsp = NULL;
	}

	if (car){
		delete car;
		car = NULL;
	}

	if (pig){
		delete pig;
		pig = NULL;
	}

	if (pilot){
		delete pilot;
		pilot = NULL;
	}
}
