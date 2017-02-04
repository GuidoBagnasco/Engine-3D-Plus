#pragma once
#include "myEngine_API.h"
#include <Windows.h>
#include <string>

// IO Debug...
#include <io.h>         //_open_osfhandle
#include <fcntl.h>      //_O_TEXT

namespace engine{
class Window;
class Renderer;
class Game;
class Timer;
class DirectInput;
class Importer;
	class MYENGINE_API Engine{

		public:
			static const WORD MAX_CONSOLE_LINES = 1000;
			Engine(HINSTANCE hInst , std::string t, int w, int h);
			~Engine();
			bool init();
			void run();
			HWND hWnd;
			Game* G;
		private:
			void RedirectIOToConsole();

			HINSTANCE hInstance;
			Window* WndC;
			Renderer* Rendr;

			Timer* m_tTimer;
			DirectInput* m_diInput;

			Importer* m_iImporter;

			int nCmdShow;
			std::string _t;
			int _w;
			int _h;
	};
}