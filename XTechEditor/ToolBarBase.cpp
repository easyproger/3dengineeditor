#include "ToolBarBase.h"
#include "ToolBarsEventInterface.h"

ToolBarBase::ToolBarBase(HINSTANCE instance,int nCMD,ToolBarsEventInterface*toolBarInterface)
{
	_toolBarInterface = toolBarInterface;
	_instance = instance;
	_nCMD = nCMD;
}



ToolBarBase::~ToolBarBase(void)
{
}


