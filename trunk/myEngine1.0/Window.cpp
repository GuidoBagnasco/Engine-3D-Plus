#pragma once
#include "Window.h"
#include <string>
using namespace engine;
Window::Window(HINSTANCE hInstance):WC(new WNDCLASS),m_hWnd(NULL),_hInst(hInstance),m_pszClassName("MyEngine"){
}

bool Window::CrearVentana(std::string _t, int _w, int _h) {
	
	ZeroMemory(WC, sizeof(*WC));
	WC->lpfnWndProc=WindowProc;
	WC->hInstance=_hInst;
	WC->hbrBackground=(HBRUSH)(COLOR_BACKGROUND);
	WC->lpszClassName=m_pszClassName;
	if(!RegisterClass(WC)){return false;}

	
	m_hWnd = CreateWindow( m_pszClassName,_t.c_str(), WS_VISIBLE | WS_OVERLAPPEDWINDOW, 50, 50, _w + 8, _h + 30, NULL, NULL, _hInst, NULL );
	if(m_hWnd != NULL) return true;

	return false;	
}

 LRESULT CALLBACK Window::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

	 switch(uMsg){
		case WM_CLOSE:
				PostQuitMessage(0);
				break;
		default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	 }

	 return 0;

}

Window::~Window() {
    if ( m_hWnd ) DestroyWindow( m_hWnd );
}