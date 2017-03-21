#pragma once

#include "defines.h"
#include "Patcher.h"

class CApplication
{
public:
	static CApplication* Object();
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int WINAPI AppMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	HINSTANCE mainInstance;
	~CApplication(void);
private:
	static CApplication* theApp;
	HWND mainhWnd;
	HBITMAP hBmp;
	HBITMAP hBmpPatch, hBmpUnpatch, hBmpAbout;
	int menuChosen;
	CApplication(void);
	CPatcher m_patcher;
	void startPatching(void);
	void startUnPatching(void);
};
