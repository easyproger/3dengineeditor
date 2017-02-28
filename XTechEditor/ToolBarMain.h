#pragma once

#include "ToolBarBase.h"

class ToolBarMain:public ToolBarBase
{
public:
	ToolBarMain(HINSTANCE instance,int nCMD,ToolBarsEventInterface*toolbarInterface);
	~ToolBarMain(void);


	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void update(Object*object);
	virtual void destroy();
	virtual void create();
private:

};

