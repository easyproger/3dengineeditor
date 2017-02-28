#pragma once
#include "toolbarbase.h"
class ToolBarLight :
	public ToolBarBase
{
public:
	ToolBarLight(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface);
	~ToolBarLight(void);

	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void update(Object*object);
	virtual void destroy();
	virtual void create();
};

