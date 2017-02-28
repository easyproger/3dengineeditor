#pragma once


#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>

#include "Object.hpp"
#include "GUIEventInterface.h"
#include "GraphicEngineController.h"
#include "ToolBarsEventInterface.h"

class GUI: public ToolBarsEventInterface ,public GUIEventInterface
{
public:
	GUI( );
	~GUI(void);

	virtual int create() =0;
	virtual void resize(void) {};
	virtual void redrawRender(void) {};


protected:

	


	GraphicEngineController*_graphicController;
	
};

