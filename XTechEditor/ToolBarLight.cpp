#include "ToolBarLight.h"


ToolBarLight::ToolBarLight(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface):ToolBarBase(instance,nCMD,toolbarInterface)
{
}


ToolBarLight::~ToolBarLight(void)
{
}


INT_PTR CALLBACK ToolBarLight::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

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



void ToolBarLight::update(Object*object) {

}
void ToolBarLight::destroy() {

}

void ToolBarLight::create() {
}