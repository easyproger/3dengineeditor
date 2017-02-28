#pragma once
#include "Object.hpp"
#include "XMathTypes.h"
class GUIEventInterface
{
public:
	public:
        virtual ~GUIEventInterface() {}
        virtual void selectToolBar(Object*objectPicked) = 0;

		virtual void setXYZValueCurrentObject(XVector3 val) = 0;
};

