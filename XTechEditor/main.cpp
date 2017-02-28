
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <io.h>
#include <Fcntl.h>

#include <iostream>
#include <Shlwapi.h>
#include "resource.h"
#include <Commdlg.h>

#pragma comment(lib, "shlwapi.lib")


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <GL/glew.h>
#include <GL/glut.h>




#include "MeshCache.h"
#include "Camera.h"
#include "Renderer.h"
#include "FileLoader.h"
#include "XMatrix3Math.h"

#include "UIWin32.h"


GUI*mainGui;


void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();
	freopen("CONOUT$", "w", stdout);
    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // give the console window a nicer title
    SetConsoleTitle("Debug Output");

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 9999;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}

int WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	SetStdOutToNewConsole();
	mainGui = new UIWin32(hInstance,hPrevInstance,lpCmdLine,nShowCmd);
	int retResult = (int) mainGui->create();
	
	return retResult;
}

int main(int argc,char** argv) {

	return 1;

}