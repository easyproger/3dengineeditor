#pragma once
#include "toolbarbase.h"
class ToolBarCamera :
	public ToolBarBase
{
public:
	ToolBarCamera(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface);
	~ToolBarCamera(void);


	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void update(Object*object);
	virtual void destroy();
	virtual void create();
};

