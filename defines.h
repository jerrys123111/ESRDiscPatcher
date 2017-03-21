#pragma once
#pragma warning(disable : 4995)
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <time.h>

enum eFileErr
{
	ESR_FILE_OK = 0,
	ESR_FILE_CANNOT_OPEN = -1,
	ESR_FILE_ALREADY_PATCHED = -2,
	ESR_FILE_WRONG_FORMAT = -3,
	ESR_FILE_NO_UDF_DESCRIPTOR = -4,
	ESR_FILE_NOT_PATCHED = -5
};
