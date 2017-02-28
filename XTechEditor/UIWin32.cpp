#include "UIWin32.h"


UIWin32::UIWin32( HINSTANCE hInstance,  HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nShowCmd):GUI()
{

	_hInstance     = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine     = lpCmdLine;
	_nShowCmd      = nShowCmd;
	lastTypeObject = -1;

}

void UIWin32::resize(void)
{
    glViewport(0, 0, winWidth, winHeight);
	_graphicController->setWindowSize((float)winWidth,(float)winHeight);
}


void UIWin32::setXYZValueCurrentObject(XVector3 val) {
	mainTopToolbar->setXYZ(val);
}

void UIWin32::setXYZFromToolbar(XVector3 val) {
	_graphicController->setXYZCurrentObject(val);
}

void UIWin32::OnRButtonDownRender(LPARAM lParam)
{
	int x = LOWORD(lParam);
    int y = HIWORD(lParam);
	_graphicController->rightMouseButtonDown(x,y);
}

void UIWin32::OnRButtonUpRender(LPARAM lParam)
{
	int x = LOWORD(lParam);
    int y = HIWORD(lParam);
	_graphicController->rightMouseButtonUp(x,y);
}

void UIWin32::OnLButtonDownRender(LPARAM lParam)
{
	int x = LOWORD(lParam);
    int y = HIWORD(lParam);
	_graphicController->leftMouseButtonDown(x,y); 
}

void UIWin32::OnLButtonUpRender(LPARAM lParam)
{
	int x = LOWORD(lParam);
    int y = HIWORD(lParam);
	_graphicController->leftMouseButtonUp(x,y);
}

void UIWin32::OnMouseMoveRender(WPARAM wParam, LPARAM lParam)
{
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
	_graphicController->mouseMoveEvent(x,y);
}

void UIWin32::keyboardKeyRenderUp(int key) {
	_graphicController->keyboardKeyUP(key);
}
void UIWin32::keyboardKeyRenderDown(int key) {
	_graphicController->keyboardKeyDown(key);
}

#include <chrono>
void UIWin32::redrawRender(void)   {
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);


	

	_graphicController->render(ms.count());
	SwapBuffers(_hdcMainWindow);
}

LRESULT CALLBACK UIWin32::WndProcRender(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Devar Editor");

	switch (message)
	{
	case WM_CREATE:
		{
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),  /* size */
				1,                              /* version */
				PFD_SUPPORT_OPENGL |
				PFD_DRAW_TO_WINDOW |
				PFD_DOUBLEBUFFER,               /* support double-buffering */
				PFD_TYPE_RGBA,                  /* color type */
				16,                             /* prefered color depth */
				0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
				0,                              /* no alpha buffer */
				0,                              /* alpha bits (ignored) */
				0,                              /* no accumulation buffer */
				0, 0, 0, 0,                     /* accum bits (ignored) */
				16,                             /* depth buffer */
				0,                              /* no stencil buffer */
				0,                              /* no auxiliary buffers */
				PFD_MAIN_PLANE,                 /* main layer */
				0,                              /* reserved */
				0, 0, 0,                        /* no layer, visible, damage masks */
			};

			_hdcMainWindow = GetDC(hWnd);

			int  letWindowsChooseThisPixelFormat;
			letWindowsChooseThisPixelFormat = ChoosePixelFormat(_hdcMainWindow, &pfd);
			SetPixelFormat(_hdcMainWindow,letWindowsChooseThisPixelFormat, &pfd);

			_mainContext = wglCreateContext(_hdcMainWindow);
			_loaderContext = wglCreateContext(_hdcMainWindow);
			wglShareLists(_loaderContext, _mainContext);

			wglMakeCurrent (_hdcMainWindow, _mainContext);
			glewInit() ;

			TCHAR dest[MAX_PATH];
			DWORD length = GetModuleFileName( NULL, dest, MAX_PATH );
    		PathRemoveFileSpec(dest);

			std::string basePath = dest;
			_graphicController->init(dest);
			SetTimer(hWnd,0,1000/60,NULL);
		}
		break;
	case WM_MOUSEMOVE:
		OnMouseMoveRender(wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDownRender(lParam);
		break;
	case WM_RBUTTONUP:
		OnRButtonUpRender(lParam);
		break;
    case WM_LBUTTONDOWN:
		OnLButtonDownRender(lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUpRender(lParam);
		break;
	case WM_KEYDOWN:
        keyboardKeyRenderDown(wParam);
	break;
	case WM_KEYUP:
		keyboardKeyRenderUp(wParam);
	break;
	case WM_TIMER:
		if (_mainContext) {
			redrawRender();
		}
		break;
	case WM_SIZE:
		if (_mainContext) {
			winWidth = (int) LOWORD(lParam);
			winHeight = (int) HIWORD(lParam);
			resize();
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (_mainContext) {
			redrawRender();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		    case ID_FILE_EXIT:
			    DestroyWindow(hWnd);
			break;
			case ID_FILE_ADDMESH:
				loadMesh();
			break;
			default:
			    return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}


void UIWin32::loadMesh() {

	bool result = GetOpenFileName( &ofnMesh );
	if (!result) return;
	std::string pathnewmesh = ofnMesh.lpstrFile;
	_graphicController->addMesh(pathnewmesh);
}
std::string UIWin32::getTexture() {
	return _graphicController->getTexture();
}
void UIWin32::setTexture(std::string path) {
	_graphicController->setTexture(path);
}
void UIWin32::initData() {
	toolbars.resize(MAX_OBJECT_TYPES);

	toolbars[NO_OBJECT]        = new ToolBarMain(_hInstance,_nShowCmd,this);
	toolbars[CAMERA_OBJECT]    = new ToolBarCamera(_hInstance,_nShowCmd,this);
	toolbars[MESH_OBJECT]      = new ToolBarMesh(_hInstance,_nShowCmd,this);
	toolbars[LIGHT_OBJECT]     = new ToolBarLight(_hInstance,_nShowCmd,this);
	toolbars[LIGHT_OBJECT]     = new ToolBarLight(_hInstance, _nShowCmd, this);
	toolbars[PARTICLES_OBJECT] = new ToolBarParticles(_hInstance, _nShowCmd, this);

	mainTopToolbar = new ToolBarMainTop(_hInstance, _nShowCmd, this);
	mainTopToolbar->create();


	// init load mesh 
	ZeroMemory( &ofnMesh , sizeof( ofnMesh));
	ofnMesh.lStructSize = sizeof ( ofnMesh );
	ofnMesh.hwndOwner = NULL  ;
	ofnMesh.lpstrFile = szFile ;
	ofnMesh.lpstrFile[0] = '\0';
	ofnMesh.nMaxFile = sizeof( szFile );
	ofnMesh.lpstrFilter = TEXT("Mesh XMESH\0*.XMESH\0");
	ofnMesh.nFilterIndex =1;
	ofnMesh.lpstrFileTitle = NULL ;
	ofnMesh.nMaxFileTitle = 0 ;
	ofnMesh.lpstrInitialDir=NULL ;
	ofnMesh.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;



}

void UIWin32::saveMesh(std::string path) {
	bool result = GetOpenFileName(&ofnMesh);
	if (!result) return;
	std::string pathnewmesh = ofnMesh.lpstrFile;
	_graphicController->saveMesh(pathnewmesh);
}

int UIWin32::create() {

	g_hImageList = NULL;
	initData();


	WNDCLASSEX wcex;


	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Devar editor");


	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style          = CS_OWNDC| CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProcRenderS;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = _hInstance;
	wcex.hIcon          = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;//MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Devar editor"),
			NULL);

		return 0;
	}
	HWND  hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1024, 1024,
		NULL,
		NULL,
		_hInstance,
		NULL
		);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (long)this);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Devar editor"),
			NULL);

		return 0;
	}
	ShowWindow(hWnd,_nShowCmd);
	UpdateWindow(hWnd);

	

	WndProcRender(hWnd,WM_CREATE,0,0);

	MSG msg;

	lastTypeObject = NO_OBJECT;
	toolbars[NO_OBJECT]->create();
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;

}

void UIWin32::setLoadSceneDataFromMesh(bool value) {
	_graphicController->setLoadSceneDataFromMesh(value);
}

bool UIWin32::getLoadSceneDataFromMesh() {
	return _graphicController->getLoadSceneDataFromMesh();
}

void UIWin32::addMeshInterface() {
	loadMesh();
}

void UIWin32::changeMode(ModeControlls newMode) {
	_graphicController->changeMode(newMode);
}

void UIWin32::setManipulator(ManipulatorType type) {
	_graphicController->setManipulator(type);
}

void UIWin32::moveObjectX(float value) {
	_graphicController->moveSelectedObjectX(value);
}

void UIWin32::selectToolBar(Object*objectPicked) {

	if (objectPicked == nullptr || objectPicked->typeObject == -1 ){
		if (lastTypeObject != -1)
		toolbars[lastTypeObject]->destroy();
		lastTypeObject = -1;
		return;
	}

	
	if (lastTypeObject == objectPicked->typeObject) {
		toolbars[objectPicked->typeObject]->update(objectPicked);
	}else {
		if (lastTypeObject != -1)
		toolbars[lastTypeObject]->destroy();
		toolbars[objectPicked->typeObject]->create();
		toolbars[objectPicked->typeObject]->update(objectPicked);

		lastTypeObject = objectPicked->typeObject;
	}
}



void UIWin32::addParticles() {
	_graphicController->addParticles();
}
void UIWin32::setCameraHome() {
	_graphicController->setCameraHome();
}
void UIWin32::setCameraLeft() {
	_graphicController->setCameraLeft();
}
void UIWin32::setCameraTop() {
	_graphicController->setCameraTop();
}

void UIWin32::removeSelection() {
	_graphicController->removeSelection();
}

void UIWin32::playAnimation() {
	_graphicController->playAnimation();
}

void UIWin32::stopAnimation() {
	_graphicController->stopAnimation();
}

void UIWin32::setAnimationRange(unsigned int start, unsigned int end) {
	_graphicController->setAnimationRange(start, end);
}

XVector2<unsigned int> UIWin32::getAnimationRange() {
	return _graphicController->getAnimationRange();
}


void UIWin32::createToolBar(int idToolBar) {




}
void UIWin32::setParticleInfo(ParticleInfo info) {
	_graphicController->setParticleInfo(info);
}


UIWin32::~UIWin32(void)
{
}
