#include "ToolBarMainTop.h"


ToolBarMainTop::ToolBarMainTop(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface):ToolBarBase(instance,nCMD,toolbarInterface)
{
}


ToolBarMainTop::~ToolBarMainTop(void)
{
}

INT_PTR CALLBACK ToolBarMainTop::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch(message) {
	    case WM_INITDIALOG:

			{
				XVector2<unsigned int> range = _toolBarInterface->getAnimationRange();
				char buff[10];
				sprintf(buff, "%d\0", range.x);
				SetDlgItemText(hDlg, IDC_EDIT_START_FRAME, (LPCSTR)buff);

				sprintf(buff, "%d\0", range.y);
				SetDlgItemText(hDlg, IDC_EDIT_END_FRAME, (LPCSTR)buff);

				bool loadSceneDataFromMesh = _toolBarInterface->getLoadSceneDataFromMesh();
				CheckDlgButton(hDlg, IDC_CHECK_LOAD_SCENE_DATA, loadSceneDataFromMesh);

				return TRUE;
			}

			break;
		case WM_DESTROY:
			return FALSE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case IDC_EDIT_X:
				if (HIWORD(wParam) == EN_KILLFOCUS)
				getAndSetXYZ();
				break;
			case IDC_EDIT_Y:
				if (HIWORD(wParam) == EN_KILLFOCUS)
					getAndSetXYZ();
				break;
			case IDC_EDIT_Z:
				if (HIWORD(wParam) == EN_KILLFOCUS)
					getAndSetXYZ();
				break;
			case IDC_SAVE_MESH:
				_toolBarInterface->saveMesh("");
				break;
			case IDC_CAMERA_HOME:
				_toolBarInterface->setCameraHome();
				break;
			case IDC_CAMERA_LEFT:
				_toolBarInterface->setCameraLeft();
				break;
			case IDC_CAMERA_TOP:
				_toolBarInterface->setCameraTop();
				break;
			case IDC_ADD_PARTICLES:
				_toolBarInterface->addParticles();
				break;
			case IDC_PICK:
				_toolBarInterface->changeMode(ModeControlls::pick);
				_toolBarInterface->setManipulator(ManipulatorType::noManipulator);
				break;
			case IDC_CAMERA:
				_toolBarInterface->changeMode(ModeControlls::camera_free);
				_toolBarInterface->setManipulator(ManipulatorType::noManipulator);
				break;
			case IDC_TRANSLATE:
				_toolBarInterface->changeMode(ModeControlls::pick);
				_toolBarInterface->setManipulator(ManipulatorType::translate);
				break;
			case IDC_ROTATION:
				_toolBarInterface->changeMode(ModeControlls::pick);
				_toolBarInterface->setManipulator(ManipulatorType::rotation);
				break;
			case IDC_SCALE:
				_toolBarInterface->changeMode(ModeControlls::pick);
				_toolBarInterface->setManipulator(ManipulatorType::scale);
				break;
			case IDC_CHECK_LOAD_SCENE_DATA:
			{
				bool loadSceneDataFromMesh = IsDlgButtonChecked(hDlg, IDC_CHECK_LOAD_SCENE_DATA);
				_toolBarInterface->setLoadSceneDataFromMesh(loadSceneDataFromMesh);
			}
				break;
			case IDC_LOAD_MESH_T:
				_toolBarInterface->addMeshInterface();
				break;

			case IDC_REMOVE_SELECTION:
				_toolBarInterface->removeSelection();
				break;
			case IDC_PLAY_ANIMATION:
				_toolBarInterface->playAnimation();
				break;
			case IDC_STOP_ANIMATION:
				_toolBarInterface->stopAnimation();
				break;
			case IDC_SET_ANIMATION_FRAMES_RANGE:
				char szStr[5];
				GetDlgItemText(hDlg, IDC_EDIT_START_FRAME, szStr, 5);
				unsigned int start = atof(szStr);
				GetDlgItemText(hDlg, IDC_EDIT_END_FRAME, szStr, 5);
				unsigned int end = atof(szStr);
				_toolBarInterface->setAnimationRange(start,end);
				break;
			
			}
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_CONTEXTHELP) {
				return FALSE;
			}
		}
	return FALSE;
}

void ToolBarMainTop::getAndSetXYZ() {
	char szStr[9];
	XVector3 val;
	GetWindowText(editX, szStr, 9);
	val.x = atof(szStr);

	GetWindowText(editY, szStr, 9);
	val.y = atof(szStr);

	GetWindowText(editZ, szStr, 9);
	val.z = atof(szStr);

	_toolBarInterface->setXYZFromToolbar(val);
}


void ToolBarMainTop::setXYZ(XVector3 val) {
	TCHAR buff[20];

	sprintf(buff, "%.2f", val.x);
	SetWindowText(editX, buff);

	sprintf(buff, "%.2f", val.y);
	SetWindowText(editY, buff);

	sprintf(buff, "%.2f", val.z);
	SetWindowText(editZ, buff);
}

void ToolBarMainTop::update(Object*object) {

}
void ToolBarMainTop::destroy() {

}

void ToolBarMainTop::create() {
	toolbarHWND = CreateDialog(_instance,MAKEINTRESOURCE(IDD_MAIN_TOP_TOOLBAR),NULL,toolbarCallbackR);

	
	SetWindowLongPtr(toolbarHWND, GWLP_USERDATA, (long)this);

	editX = GetDlgItem(toolbarHWND, IDC_EDIT_X);
	editY = GetDlgItem(toolbarHWND, IDC_EDIT_Y);
	editZ = GetDlgItem(toolbarHWND, IDC_EDIT_Z);

	ShowWindow(toolbarHWND,_nCMD);
	UpdateWindow(toolbarHWND);

	toolbarCallback(toolbarHWND, WM_INITDIALOG, 0, 0);
}