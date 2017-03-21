/*    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Application.h"
#include "AboutDlg.h"
#include "res/resource.h"

CApplication::CApplication(void) :
mainhWnd(NULL),
mainInstance(NULL),
menuChosen(0)
{
}

CApplication::~CApplication(void)
{
	theApp = NULL;
}

void CApplication::startPatching()
{
	char szFile[260];
	ZeroMemory(szFile, 260);
	OPENFILENAME ofn;       // common dialog box structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Object()->mainhWnd;
	ofn.hInstance = NULL;
	ofn.lpstrFile = &szFile[0];
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All supported (*.iso, *.bin)\0*.iso;*.bin\0ISO (*.iso)\0*.iso\0BIN (*.bin)\0*.bin\0\0\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = ".\\\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		switch (Object()->m_patcher.doPatch(ofn.lpstrFile))
		{
			case ESR_FILE_OK:
				MessageBox(Object()->mainhWnd, "Patching process finished successfuly!", "Notification", MB_OK | MB_ICONINFORMATION);
				break;
			case ESR_FILE_ALREADY_PATCHED:
				MessageBox(Object()->mainhWnd, "File already patched!", "Notification", MB_OK | MB_ICONINFORMATION);
				break;
			case ESR_FILE_CANNOT_OPEN:
				MessageBox(Object()->mainhWnd, "Couldn't open file!", "Error...", MB_OK | MB_ICONSTOP);
				break;
			case ESR_FILE_NO_UDF_DESCRIPTOR:
				MessageBox(Object()->mainhWnd, "No UDF descriptor!", "Error...", MB_OK | MB_ICONSTOP);
				break;
			default:
				MessageBox(Object()->mainhWnd, "Unknown error during patching procedure!", "Error...", MB_OK | MB_ICONSTOP);
				break;
		}
	}
}
void CApplication::startUnPatching()
{
	char szFile[260];
	ZeroMemory(szFile, 260);
	OPENFILENAME ofn;       // common dialog box structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Object()->mainhWnd;
	ofn.hInstance = NULL;
	ofn.lpstrFile = &szFile[0];
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All supported (*.iso, *.bin)\0*.iso;*.bin\0ISO (*.iso)\0*.iso\0BIN (*.bin)\0*.bin\0\0\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = ".\\\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		switch (Object()->m_patcher.doUnPatch(ofn.lpstrFile))
		{
			case ESR_FILE_OK:
				MessageBox(Object()->mainhWnd, "Unpatching process finished successfuly!", "Notification", MB_OK | MB_ICONINFORMATION);
				break;
			case ESR_FILE_NOT_PATCHED:
				MessageBox(Object()->mainhWnd, "File not patched!", "Notification", MB_OK | MB_ICONINFORMATION);
				break;
			case ESR_FILE_CANNOT_OPEN:
				MessageBox(Object()->mainhWnd, "Couldn't open file!", "Error...", MB_OK | MB_ICONSTOP);
				break;
			case ESR_FILE_NO_UDF_DESCRIPTOR:
				MessageBox(Object()->mainhWnd, "No UDF descriptor!", "Error...", MB_OK | MB_ICONSTOP);
				break;
			default:
				MessageBox(Object()->mainhWnd, "Unknown error during unpatching procedure!", "Error...", MB_OK | MB_ICONSTOP);
				break;
		}
	}
}

INT_PTR CALLBACK CApplication::DialogProc(HWND hWnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (uMsg)
	{
		//in modal dialog arrow keys, escape and some others will not generate WM_KEYDOWN
		//here. Cubclassing doesn't solve this, so it seems it's handled somewhere else
		//hotkeys will do for now ;).. though better way would be to use medeless dialog
		//case WM_KEYDOWN:
		case WM_HOTKEY:
			{
				int oldChos;
				oldChos = Object()->menuChosen;
				switch ((int) wParam)
				{
					case VK_LEFT:
						Object()->menuChosen--;
						if (Object()->menuChosen < 1) Object()->menuChosen = 3;
						break;
					case VK_RIGHT:
						Object()->menuChosen++;
						if (Object()->menuChosen > 3) Object()->menuChosen = 1;
						break;
					case VK_RETURN:
						switch (Object()->menuChosen)
						{
							case 1:
								Object()->startPatching();
								break;
							case 2:
								Object()->startUnPatching();
								break;
							case 3:
								DialogBox(Object()->mainInstance, (LPCTSTR) MAKEINTRESOURCE (IDD_ABOUT), Object()->mainhWnd, &CAboutDlg::DialogProc);
								break;
						}
						if (Object()->menuChosen != 0)
						{
							Object()->menuChosen = 0;
						}
						break;
					case VK_ESCAPE:
						Object()->menuChosen = 0;
						break;
				}
				if (oldChos != Object()->menuChosen)
				{
					SendMessage(hWnd, WM_ERASEBKGND, 0, 0);
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				int xPos, yPos, oldChos;
				xPos = GET_X_LPARAM(lParam); 
				yPos = GET_Y_LPARAM(lParam);
				
				oldChos = Object()->menuChosen;
				if (xPos > 0 && xPos < 140 && yPos > 109 && yPos < 131) Object()->menuChosen = 1;
				else if (xPos > 139 && xPos < 280 && yPos > 109 && yPos < 131) Object()->menuChosen = 2;
				else if (xPos > 210 && xPos < 280 && yPos > 0 && yPos < 21) Object()->menuChosen = 3;
				else Object()->menuChosen = 0;
				if (oldChos != Object()->menuChosen) SendMessage(hWnd, WM_ERASEBKGND, 0, 0);

				switch (Object()->menuChosen)
				{
					case 1:
						Object()->startPatching();
						break;
					case 2:
						Object()->startUnPatching();
						break;
					case 3:
						DialogBox(Object()->mainInstance, (LPCTSTR) MAKEINTRESOURCE (IDD_ABOUT), Object()->mainhWnd, &CAboutDlg::DialogProc);
						break;
				}
				if (Object()->menuChosen != 0)
				{
					Object()->menuChosen = 0;
					SendMessage(hWnd, WM_ERASEBKGND, 0, 0);
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				int xPos, yPos, oldChos;
				xPos = GET_X_LPARAM(lParam); 
				yPos = GET_Y_LPARAM(lParam);
				
				oldChos = Object()->menuChosen;
				if (xPos > 0 && xPos < 140 && yPos > 109 && yPos < 131) Object()->menuChosen = 1;
				else if (xPos > 139 && xPos < 280 && yPos > 109 && yPos < 131) Object()->menuChosen = 2;
				else if (xPos > 210 && xPos < 280 && yPos > 0 && yPos < 21) Object()->menuChosen = 3;
				else Object()->menuChosen = 0;
				if (oldChos != Object()->menuChosen) SendMessage(hWnd, WM_ERASEBKGND, 0, 0);

				//just some easy cheat to quickly capture mouse out of modal dialog frame
				//it's not the nice way, but works ;)
				if (xPos >= 0 && yPos >=0 && xPos < 280 && yPos < 131)
					SetCapture(hWnd);
				else ReleaseCapture();
			}
			break;
		case WM_ERASEBKGND:
			{
				HDC hDC, hDCComp;
				RECT rect;
				hDC = GetDC(hWnd);
				hDCComp = CreateCompatibleDC(hDC);
				SelectObject(hDCComp, Object()->hBmp);
				GetClientRect(hWnd, &rect);
				StretchBlt(hDC,
					rect.left, rect.top, rect.right, rect.bottom,
					hDCComp, 0, 0, 280, 131, SRCCOPY);

				DeleteDC(hDCComp);

				if (Object()->menuChosen == 1)
				{
					hDCComp = CreateCompatibleDC(hDC);
					SelectObject(hDCComp, Object()->hBmpPatch);
					StretchBlt(hDC,
						0, 110, 139, 20,
						hDCComp, 0, 0, 139, 20, SRCCOPY);
					DeleteDC(hDCComp);
				} else if (Object()->menuChosen == 2)
				{
					hDCComp = CreateCompatibleDC(hDC);
					SelectObject(hDCComp, Object()->hBmpUnpatch);
					StretchBlt(hDC,
						140, 110, 139, 20,
						hDCComp, 0, 0, 139, 20, SRCCOPY);
					DeleteDC(hDCComp);
				} else if (Object()->menuChosen == 3)
				{
					hDCComp = CreateCompatibleDC(hDC);
					SelectObject(hDCComp, Object()->hBmpAbout);
					StretchBlt(hDC,
						211, 1, 68, 20,
						hDCComp, 0, 0, 68, 20, SRCCOPY);
					DeleteDC(hDCComp);
				}
			}
			return 1;
			
			break;
		case WM_DROPFILES:
			{
				bool first = false;
				char *p = NULL;
				std::string outMsg, singleMsg;
				UINT filesDropCount = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
				char filePath[260];
				outMsg.clear();
				for (UINT i = 0; i < filesDropCount; i++)
				{
					singleMsg.clear();
					DragQueryFile((HDROP)wParam, i, filePath, 260);
					p = strrchr(filePath, '\\');
					if (p == NULL)
					{
						p = strrchr(filePath, '/');
						if (p == NULL) p = filePath;
						else p++;
					} else
					{
						p++;
					}
					switch (Object()->m_patcher.doPatch(filePath))
					{
						case ESR_FILE_OK:
							outMsg.append(p);
							outMsg.append(" - Success!\n");
							//MessageBox(Object()->mainhWnd, "Patching process finished successfuly!", "Notification", MB_OK | MB_ICONINFORMATION);
							break;
						case ESR_FILE_ALREADY_PATCHED:
							outMsg.append(p);
							
							singleMsg = "File \"";
							singleMsg += p;
							singleMsg += "\" already patched!\nDo you want to unpatch it?";
							//MessageBox(Object()->mainhWnd, "File already patched!", "Notification", MB_OK | MB_ICONINFORMATION);
							if (MessageBox(Object()->mainhWnd, singleMsg.c_str(), "Notification", MB_YESNO | MB_ICONINFORMATION) == IDYES)
							{
								if (Object()->m_patcher.doUnPatch(filePath) == ESR_FILE_OK)
								{
									outMsg.append(" - Unpatched!\n");
								} else
								{
									outMsg.append(" - Unpatching Failed!\n");
								}
							} else
							{
								outMsg.append(" - Already Patched!\n");
							}
							break;
						case ESR_FILE_CANNOT_OPEN:
							outMsg.append(p);
							outMsg.append(" - Can't Open!\n");
							//MessageBox(Object()->mainhWnd, "Couldn't open file!", "Error...", MB_OK | MB_ICONSTOP);
							break;
						case ESR_FILE_NO_UDF_DESCRIPTOR:
							outMsg.append(p);
							outMsg.append(" - No UDF Desc!\n");
							break;
						default:
							outMsg.append(p);
							outMsg.append(" - Failed!\n");
							//MessageBox(Object()->mainhWnd, "Unknown error during patching procedure!", "Error...", MB_OK | MB_ICONSTOP);
							break;
					}
				}
				MessageBox(Object()->mainhWnd, outMsg.c_str(), "Info", MB_OK | MB_ICONASTERISK);
				DragFinish((HDROP)wParam);
			}
			break;
		case WM_INITDIALOG:
			Object()->mainhWnd = hWnd;
			HICON icon;
			icon = LoadIcon(Object()->mainInstance, MAKEINTRESOURCE (IDI_MAIN_ICO));
			if (icon != NULL)
				SetClassLong (hWnd, GCL_HICON, (LONG)icon);
			Object()->hBmp = LoadBitmap(Object()->mainInstance, MAKEINTRESOURCE(IDB_MAINBG));
			Object()->hBmpAbout = LoadBitmap(Object()->mainInstance, MAKEINTRESOURCE(IDB_BMABOUT));
			Object()->hBmpPatch = LoadBitmap(Object()->mainInstance, MAKEINTRESOURCE(IDB_BMPATCH));
			Object()->hBmpUnpatch = LoadBitmap(Object()->mainInstance, MAKEINTRESOURCE(IDB_BMUNPATCH));
			return true;

		//just cheating, subclassing doesn't work, WM_GETDLGCODE won't do any good for
		//modals and by sending messages in there, it sometimes sends one keystroke
		//twice. This is global,  but should be fine. Any easier way to get
		//Arrows/Escape in modal dialogs (hooks are unacceptable)?
		case WM_CLOSE:
			UnregisterHotKey(hWnd, VK_LEFT);
			UnregisterHotKey(hWnd, VK_ESCAPE);
			UnregisterHotKey(hWnd, VK_RIGHT);
			UnregisterHotKey(hWnd, VK_RETURN);
			EndDialog (hWnd, 0);
			return true;
		case WM_SETFOCUS:
			RegisterHotKey(hWnd,VK_LEFT,0,VK_LEFT);
			RegisterHotKey(hWnd,VK_ESCAPE,0,VK_ESCAPE);
			RegisterHotKey(hWnd,VK_RIGHT,0,VK_RIGHT);
			RegisterHotKey(hWnd,VK_RETURN,0,VK_RETURN);
			break;
		case WM_KILLFOCUS:
			//we don't want to steal hotkeys from other apps
			UnregisterHotKey(hWnd, VK_LEFT);
			UnregisterHotKey(hWnd, VK_ESCAPE);
			UnregisterHotKey(hWnd, VK_RIGHT);
			UnregisterHotKey(hWnd, VK_RETURN);
			break;
	}
	return false;
}

CApplication* CApplication::Object()
{
	if (theApp == NULL)
	{
		theApp = new CApplication;
	}
	return theApp;
}
int WINAPI CApplication::AppMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	mainInstance = hInstance;
	LPINITCOMMONCONTROLSEX iccx = new INITCOMMONCONTROLSEX;
	iccx->dwSize = sizeof (iccx);
	iccx->dwICC = ICC_WIN95_CLASSES | ICC_INTERNET_CLASSES;
	InitCommonControlsEx (iccx);
	return (int)DialogBox(hInstance, (LPCTSTR) MAKEINTRESOURCE (IDD_MAINWINDOW), NULL, &DialogProc);
}

CApplication* CApplication::theApp = NULL;
