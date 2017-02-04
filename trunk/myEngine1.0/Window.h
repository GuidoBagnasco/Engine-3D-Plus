#pragma once
#include <iostream>
#include "myEngine_API.h"
#include <Windows.h>
#include <string>
namespace engine{
class Window{
public:
    bool CrearVentana(std::string _t, int _w, int _h);
	HWND hWnd(){
		return m_hWnd;
	}
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    Window(HINSTANCE hInstance);
    ~Window();
	
private:
	WNDCLASS * WC;
	HINSTANCE _hInst;
	HWND m_hWnd;
	char* m_pszClassName;
};
}