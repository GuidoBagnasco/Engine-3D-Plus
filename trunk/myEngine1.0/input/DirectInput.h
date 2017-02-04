
//---------------------------------------------------------------------------
#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H
//---------------------------------------------------------------------------
#define DIRECTINPUT_VERSION 0x0800
//---------------------------------------------------------------------------
#include "Input.h"
//---------------------------------------------------------------------------
#include <dinput.h>
//---------------------------------------------------------------------------
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//---------------------------------------------------------------------------
namespace engine
{
//---------------------------------------------------------------------------
class MYENGINE_API DirectInput : public Input
{
public:
	DirectInput ();
	~DirectInput ();

	bool init (HINSTANCE hInstance, HWND hWnd);
	void deinit ();

	// Mouse y Keyboard
	void acquire ();
	void reacquire ();
	void unacquire ();
	void reset ();

private:
	LPDIRECTINPUT8 m_lpObjectOne;
	LPDIRECTINPUT8 m_lpObjectTwo;

	LPDIRECTINPUTDEVICE8 m_lpKeyboard;
	LPDIRECTINPUTDEVICE8 m_lpMouse;

	DIMOUSESTATE m_kMouseTaken;
};
//---------------------------------------------------------------------------
} // end namespace
//---------------------------------------------------------------------------
#endif // DIRECTINPUT_H
//---------------------------------------------------------------------------
