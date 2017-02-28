#pragma once
#include "toolbarbase.h"
class ToolBarMesh :
	public ToolBarBase
{
public:
	ToolBarMesh(HINSTANCE instance, int nCMD, ToolBarsEventInterface* toolbarInterface);
	~ToolBarMesh(void);

	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void update(Object*object);
	virtual void destroy();
	virtual void create();
};

