#include "ToolBarCamera.h"


ToolBarCamera::ToolBarCamera(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface):ToolBarBase(instance,nCMD,toolbarInterface)
{
}


ToolBarCamera::~ToolBarCamera(void)
{
}

INT_PTR CALLBACK ToolBarCamera::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch(message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_DESTROY:
			return FALSE;
		case WM_COMMAND:
			

			switch(LOWORD(wParam)) {
			}
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_CONTEXTHELP) {
				return FALSE;
			}
		}
	return FALSE;
}


void ToolBarCamera::update(Object*object) {
}

void ToolBarCamera::destroy() {
	DestroyWindow(toolbarHWND);
}

void ToolBarCamera::create() {
	toolbarHWND = CreateDialog(_instance,MAKEINTRESOURCE(IDD_BASE_TOOLBAR),NULL,toolbarCallbackR);


	SetWindowLongPtr(toolbarHWND, GWLP_USERDATA, (long)this);

	ShowWindow(toolbarHWND,_nCMD);
	UpdateWindow(toolbarHWND);
}