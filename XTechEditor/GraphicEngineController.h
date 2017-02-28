#pragma once

#include "GUIEventInterface.h"
#include "GL/glew.h"

#include "GraphicDefines.h"
#include "MeshCache.h"
#include "FileLoader.h"
#include "ResourceManager.h"
#include "TextureCache.h"
#include "SoundSystem.h"
#include "XAnimations.h"
#include "Camera.h"
#include "Renderer.h"
#include "SceneGraph.h"
#include "DynamicEntity.h"

#include "Manipulator.h"
#include "ManipulatorInterface.h"
#include "MathDeleted.h"
#include "ParticleEmiter.hpp"

#include "HistoryEvents.h"
#include <algorithm>

using namespace Xengine;



#define X_DOWN 0
#define X_UP 1

#define X_LEFT_BUTTON 2

#define X_KEY_UP 3
#define X_KEY_DOWN 4
#define X_KEY_LEFT 5
#define X_KEY_RIGHT 6
#define X_KEY_FORWARD 7
#define X_KEY_BACK 8





#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

class GraphicEngineController : ManipulatorInterface
{
public:

	bool shiftKeyPressed;
	bool ctrlKeyPressed;


	HistoryEvents historyController;

	GraphicEngineController(GUIEventInterface*guiInterface);
	~GraphicEngineController(void);
	void trySelectObject(float x,float y);
	void moveSelectedObjectX(float value);
	void render(double currentTime);
	void init(std::string basePath);


	void setAnimationRange(unsigned int start, unsigned int end);
	XVector2<unsigned int> getAnimationRange();
	void playAnimation();
	void stopAnimation();
	void removeSelection();

	Material * generateMaterial(Mesh * mesh);

	void createNodes(std::vector<Mesh*>* meshs,std::vector<SceneNode*> *createdNodes, bool history= false);
	void loadMesh(std::string &meshPath, std::vector<Mesh*>*meshs, std::vector<GenericObjects*>*genericNodes = nullptr, std::vector<CameraInfo>*cameras = nullptr, std::vector<LightInfo>*lights = nullptr, bool loadSceneData = false);
	void setLoadSceneDataFromMesh(bool value);
	bool getLoadSceneDataFromMesh();
	void addMesh(std::string &meshPath);

	void createGenericObjects(std::vector<GenericObjects*>* genericNodes);

	void keyboardKeyDown(int keyID);
	void keyboardKeyUP(int keyID);
	void mouseMoveEvent(int coordinatsX,int coordinatsY);

	void rightMouseButtonDown(int coordinatsX,int coordinatsY);
    void rightMouseButtonUp(int coordinatsX,int coordinatsY);
	void leftMouseButtonDown(int coordinatsX,int coordinatsY);
	void leftMouseButtonUp(int coordinatsX,int coordinatsY);

	void addParticles();

	void setCameraHome();
	void setCameraLeft();
	void setCameraTop();

	void setTexture(std::string path);
	std::string getTexture();

	void changeMode(ModeControlls newMode);
	void setManipulator(ManipulatorType type);
	void setWindowSize(float width,float height);
	void setParticleInfo(ParticleInfo info);
	void saveMesh(std::string path);

	void changeObjectEnd();
	void setXYZCurrentObject(XVector3 val);


	virtual void moveObjectEnd();
private:

	
	void updateXYZValue();
	double lastTime;
	std::map<uint, Mesh*> damagedMeshs;
	void saveHistoryChangeObject();
	Manipulator* manipulator;

	std::vector<SceneNode*> nodesScene;
	std::vector<SceneNode*> nodesUpdate;

	float sizeManipulator;

	bool isAnimationPlayed;
	uint maxAnimationFrame;
	uint minAnimationFrame;
	uint currentAnimationFrame;
    


	float windowWidth,windowHeight;

	bool canRender;
	ModeControlls currentMode;
	AxisType axisTarget;

	bool mouseRotate,mouseMove;
	bool mouseLPressed;
	bool loadSceneData;


	float savedAngle;
	float radius_;

	
	int myMouseXcoordVar,myMouseYcoordVar,myMouseYcoord,myMouseXcoord,rot_cam_X;
	void mouse_Events();
        


	std::map<std::string,SkinningFrame*> skinAnimationsCache;
	void selectObject(Object*obj);
	void deselectObject();
	std::string        _basePath;
	Light*             defaultLight;
	SceneGraph*        mainSceneGraph;
	Renderer*          xRender;
	Camera*            camera;
	XAnimations*       animationsCache;
	SoundSystem*       soundSystem;
	TextureCache*      textureCache;
	FileLoader*        loader;
	MeshCache*         meshCache;
	GUIEventInterface*_guiInterface;


	XVector3 currentStartValue;

	SceneNode* currentSelectedObject;
	BaseRenderObject* lastSelectedObject;
};

