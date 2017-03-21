#pragma once
#include "defines.h"
//always one instance so singleton is a good choice
class CAboutDlg
{
public:
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static CAboutDlg* Object();
	~CAboutDlg(void);
private:
	HWND hWnd;
	static CAboutDlg* theDlg;
	CAboutDlg(void);
};
