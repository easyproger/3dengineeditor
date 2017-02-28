#pragma once

#include "GraphicDefines.h"
#include "XMathTypes.h"


class ToolBarsEventInterface
{
public:
	public:
	virtual ~ToolBarsEventInterface(void){};
	virtual void moveObjectX(float value) = 0;
	virtual void changeMode(ModeControlls newMode) = 0;
	virtual void setManipulator(ManipulatorType type) = 0;
	virtual void addParticles() = 0;

	virtual void setCameraHome() = 0;
	virtual void setCameraLeft() = 0;
	virtual void setCameraTop() = 0;

	virtual void removeSelection() = 0;
	virtual void addMeshInterface() = 0;

	virtual void playAnimation() = 0;
	virtual void stopAnimation() = 0;

	virtual bool getLoadSceneDataFromMesh() = 0;
	virtual void setLoadSceneDataFromMesh(bool value) = 0;
	virtual void setAnimationRange( unsigned int start, unsigned int end) = 0;
	virtual XVector2<unsigned int> getAnimationRange() = 0;

	virtual void setParticleInfo(ParticleInfo info) = 0;
	virtual void setTexture(std::string path) = 0;
	virtual std::string getTexture() = 0;

	virtual void saveMesh(std::string path) = 0;

	virtual void setXYZFromToolbar(XVector3 val) = 0;
};

