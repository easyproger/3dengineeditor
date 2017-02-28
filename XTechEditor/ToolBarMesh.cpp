#include "ToolBarMesh.h"


ToolBarMesh::ToolBarMesh(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface):ToolBarBase(instance,nCMD,toolbarInterface)
{
}


ToolBarMesh::~ToolBarMesh(void)
{
}



INT_PTR CALLBACK ToolBarMesh::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

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



void ToolBarMesh::update(Object*object) {

}
void ToolBarMesh::destroy() {

}

void ToolBarMesh::create() {
}