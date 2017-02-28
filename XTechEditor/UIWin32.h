#pragma once

#include <windows.h>
#include <Shlwapi.h>
#include <CommCtrl.h>

#include "gui.h"
#include "GL/glew.h"
#include "resource.h"
#include "ToolBarBase.h"
#include "ToolBarMain.h"
#include "ToolBarCamera.h"
#include "ToolBarMesh.h"
#include "ToolBarLight.h"
#include "ToolBarMainTop.h"
#include "ToolBarParticles.h"

#include "Object.hpp"

#include <vector>


class UIWin32 :
	public GUI
{


	
	static LRESULT CALLBACK WndProcRenderS(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        UIWin32 *c = (UIWin32 *)GetWindowLongPtr(hWnd,GWLP_USERDATA);
        if (c == NULL)
            return DefWindowProc(hWnd, msg, wParam, lParam);
        return c->WndProcRender(hWnd, msg, wParam, lParam);
    }

public:
	UIWin32( HINSTANCE hInstance,  HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nShowCmd);

	virtual int create();

	LRESULT CALLBACK WndProcRender(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void redrawRender(void);
	void OnRButtonUpRender(LPARAM lParam);
    void OnRButtonDownRender(LPARAM lParam);
	virtual void OnLButtonDownRender(LPARAM lParam);
	virtual void OnLButtonUpRender(LPARAM lParam);
	virtual void OnMouseMoveRender(WPARAM wParam, LPARAM lParam);

	virtual void keyboardKeyRenderUp(int key);
	virtual void keyboardKeyRenderDown(int key);


	virtual void resize(void);

	void initData();
	void createToolBar(int idToolBar);
	void loadMesh();

	virtual bool getLoadSceneDataFromMesh();
	virtual void setLoadSceneDataFromMesh(bool value);
	virtual void addMeshInterface();
	virtual void setManipulator(ManipulatorType type);
	virtual void changeMode(ModeControlls newMode);
	virtual void moveObjectX(float value);
	virtual void selectToolBar(Object*objectPicked);
	virtual void removeSelection();
	virtual void playAnimation();
	virtual void stopAnimation() ;
	virtual void setAnimationRange(unsigned int start, unsigned int end);
	virtual XVector2<unsigned int> getAnimationRange();
	virtual void setParticleInfo(ParticleInfo info);
	virtual void addParticles();

	virtual void setCameraHome();
	virtual void setCameraLeft();
	virtual void setCameraTop();
	virtual void setTexture(std::string path);
	virtual std::string getTexture();
	virtual void saveMesh(std::string path);

	virtual void setXYZValueCurrentObject(XVector3 val);
	virtual void setXYZFromToolbar(XVector3 val);

	HWND toolbar;
	~UIWin32(void);


	std::vector< ToolBarBase*> toolbars;
	int lastTypeObject;
	int winWidth;
	int winHeight;
private:
	
	ToolBarMainTop*mainTopToolbar;

	HIMAGELIST g_hImageList;

	OPENFILENAME ofnMesh,ofnMeshAnimation,ofnSaveMesh;
    char szFile[100];

	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR _lpCmdLine;
	int _nShowCmd;

	HGLRC _mainContext;
	HGLRC _loaderContext;
	HDC   _hdcMainWindow;
};

