#pragma once
#include "toolbarbase.h"
#include "GraphicDefines.h"


class ToolBarMainTop :
	public ToolBarBase
{
public:
	ToolBarMainTop(HINSTANCE instance,int nCMD,ToolBarsEventInterface* toolbarInterface);
	~ToolBarMainTop(void);

	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void update(Object*object);
	virtual void destroy();
	virtual void create();
	void setXYZ(XVector3 val);

	void getAndSetXYZ();
private:
	HWND editX, editY, editZ;
};

