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

#include "AboutDlg.h"
#include "Application.h"
#include "res/resource.h"
#pragma warning(disable: 4995)
#pragma warning(disable: 4996)
CAboutDlg::CAboutDlg(void) :
hWnd(NULL)
{
}

CAboutDlg::~CAboutDlg(void)
{
	theDlg = NULL;
}
CAboutDlg* CAboutDlg::Object()
{
	if (theDlg == NULL)
	{
		theDlg = new CAboutDlg;
	}
	return theDlg;
}
INT_PTR CALLBACK CAboutDlg::DialogProc(HWND hWnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (uMsg)
    {
		case WM_INITDIALOG:
			Object()->hWnd = hWnd;
			HICON icon;
			icon = LoadIcon(CApplication::Object()->mainInstance, MAKEINTRESOURCE (IDI_MAIN_ICO));
			if (icon != NULL)
				SetClassLong (hWnd, GCL_HICON, (LONG)icon);
			return true;
		case WM_CLOSE:
			EndDialog (hWnd, ID_FILE_EXIT);
			if (theDlg)
				delete theDlg;
			return (true);
		case WM_COMMAND:
			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
					switch (LOWORD(wParam))
					{
						case IDOK:
							SendMessage(hWnd, WM_CLOSE, 0, 0);
							break;
					}
			}
		default:
			return false;
	}
}

CAboutDlg* CAboutDlg::theDlg = NULL;
