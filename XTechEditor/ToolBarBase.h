#pragma once
#include <windows.h>
#include "resource.h"
#include "Object.hpp"
#include "ToolBarsEventInterface.h"

class ToolBarBase
{
protected:
	static INT_PTR CALLBACK toolbarCallbackR(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        ToolBarBase *c = (ToolBarBase *)GetWindowLongPtr(hWnd,GWLP_USERDATA);
        if (c == NULL)
            return DefWindowProc(hWnd, msg, wParam, lParam);
        return c->toolbarCallback(hWnd, msg, wParam, lParam);
    }

public:
	ToolBarBase(HINSTANCE instance,int nCMD,ToolBarsEventInterface*toolBarInterface);
	~ToolBarBase(void);

	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) = 0;

	virtual void update(Object*object) = 0;
	virtual void destroy() = 0;
	virtual void create() = 0;

protected:
	ToolBarsEventInterface*_toolBarInterface;
	int _nCMD;
	HINSTANCE _instance;
	HWND toolbarHWND;
};

