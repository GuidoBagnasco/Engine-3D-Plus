#pragma once
#include "Engine.h"
#include <string>
#include "Window.h"
#include "Renderer.h"
#include "Importer.h"
#include "input\DirectInput.h"
#include "timer\Timer.h"
#include "Camera.h"
#include "Game.h"

using namespace engine;
Engine::Engine(HINSTANCE hInst, std::string t, int w, int h):
hInstance(hInst), _t(t), _w(w), _h(h), hWnd(0), WndC(new Window(hInst) ), Rendr(new Renderer), m_diInput(new DirectInput()), m_tTimer(new Timer()), m_iImporter(new Importer()), G(NULL){

}


bool Engine::init(){

#ifdef _DEBUG
	RedirectIOToConsole();
#endif

	if(WndC->CrearVentana(_t,_w,_h) == TRUE && Rendr->Init(WndC->hWnd()) == TRUE && m_iImporter->Init(*Rendr) == TRUE && m_diInput->init( hInstance, WndC->hWnd()) == TRUE)
		return true;
	return false;
}


void Engine::run(){
	//bool grs = true;
	MSG Mess;

	if(!G) return;
	if(!G->Init(*Rendr)) return;

	G->OnSceneInit();	// Starteo scenes...

	m_tTimer->firstMeasure();

	while (G->getGame()){
		m_tTimer->measure();

		m_diInput->reacquire();
		Rendr->BeginFrame();
		Rendr->c->Update();
		G->Frame(*Rendr, *m_diInput, *m_tTimer);
		G->OnSceneUpdate(m_tTimer);
		G->DrawScenes(Rendr, m_tTimer);
		Rendr->EndFrame();
		if (PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&Mess);
			DispatchMessage(&Mess);
		}
		if (Mess.message == WM_QUIT || (Mess.message == WM_KEYDOWN && (int)Mess.wParam == VK_ESCAPE)){
			G->setGame(false);
		}
	}

	G->OnSceneShutdown();
}


void Engine::RedirectIOToConsole(){
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
		&coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
		coninfo.dwSize);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	std::ios::sync_with_stdio();
}


Engine::~Engine(){
	if(G){
		delete G;
		G = NULL;
	}
	if (m_tTimer){
		delete m_tTimer;
		m_tTimer = NULL;
	}
	if (m_diInput){
		delete m_diInput;
		m_diInput = NULL;
	}
	if(Rendr){
		delete Rendr;
		Rendr = NULL;
	}
	if(WndC){
		delete WndC;
		WndC = NULL;
	}
}