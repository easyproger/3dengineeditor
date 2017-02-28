#pragma once
#include "scenenode.h"
#include "GraphicDefines.h"
#include "Camera.h"
#include <map>
#include "MathDeleted.h"
#include "XMatrix4Math.h"
#include "ManipulatorInterface.h"
#define HEXCOLOR(c) XVector4(float((c>>24)&0xFF)/255.0, float((c>>16)&0xFF)/255.0, float((c>>8)&0xFF)/255.0, float((c)&0xFF)/255.0)




class Manipulator :
	public SceneNode
{
public:
	Manipulator(Camera* camera,ManipulatorInterface *_interface);
	~Manipulator(void);



	void setAxis(AxisType axis);

	float getTracedAxisPoint(AxisType _a_type, XVector3 _pos, XLine cameraRay, XVector3 cameraDirection, XVector3*axisOut);

	void mouseDown(int x, int y);
	void mouseMove(int x, int y);
	void mouseUp(int x, int y);
	AxisType axisInvertForRotation(AxisType axisIn);


	void updateViewManipulator();

	virtual void setPosition(XVector3*pos);

	void setNodeToManipulate(SceneNode*node);
	void showManipulator(ManipulatorType type);
	void hideManipulator(ManipulatorType type);
	uint getGenericID(uint type, uint axisType);
	void addManipulator(ManipulatorType type,std::vector<Mesh*>*meshCollection);
	void setActiveManipulator(ManipulatorType type);

	ManipulatorType getManipulatorType() { return manipulatorType; };

	XVector3 savedStartValue;
	bool isChangeObject;
private:
	ManipulatorInterface *manipulatorinterface;
	XVector3 axisDirs[AxisType::AxisType_axisSize];
	

	XVector3 positionForCalculatePlane;

	Camera* _camera;


	XVector3 colorSelect;

	XVector3 Manipulator::toCameraPlane(const XVector3 &pos);
	XVector3 moveBase;

	float modAngle;

	float sizeManipulator;
	AxisType axisTarget;

	XVector3 getAxisDir(AxisType axis);
	ManipulatorType manipulatorType;


	std::vector<SceneNode*> manipulatorsGenericNodes;
	std::map<std::string, ObjectToolbarTypes> manipulatorsIDS;
	SceneNode* nodesManipulators[ManipulatorType::manipulatorsCount];
	SceneNode* nodeToManipulate;

};

