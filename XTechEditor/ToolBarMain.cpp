#include "ToolBarMain.h"


ToolBarMain::ToolBarMain(HINSTANCE instance,int nCMD,ToolBarsEventInterface*toolbarInterface):ToolBarBase(instance,nCMD,toolbarInterface)
{

}


ToolBarMain::~ToolBarMain(void)
{

}


INT_PTR CALLBACK ToolBarMain::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

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



void ToolBarMain::update(Object*object) {

}
void ToolBarMain::destroy() {
	DestroyWindow(toolbarHWND);
}

void ToolBarMain::create() {
	//toolbarHWND = CreateDialogParam(_instance, MAKEINTRESOURCE(IDD_OPTIONS), 0, toolbarCallbackR, 0);
	toolbarHWND = CreateDialog(_instance,MAKEINTRESOURCE(IDD_OPTIONS),NULL,toolbarCallbackR);
	

	

	

	SetWindowLongPtr(toolbarHWND, GWLP_USERDATA, (long)this);

	ShowWindow(toolbarHWND,_nCMD);
	UpdateWindow(toolbarHWND);

}