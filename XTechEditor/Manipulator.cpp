#include "Manipulator.h"


Manipulator::Manipulator(Camera* camera, ManipulatorInterface *_interface):SceneNode() {
    nodeToManipulate = nullptr;
	manipulatorinterface = _interface;
	isChangeObject = false;
	_camera = camera;

	modAngle = 0;
	sizeManipulator = 10.0;
	axisTarget = AxisType::AXIS_NONE;
	manipulatorType = ManipulatorType::noManipulator;

	manipulatorsIDS["moveAxisX"] = ObjectToolbarTypes::MANIPULATE_AXIS_X;
	manipulatorsIDS["moveAxisY"] = ObjectToolbarTypes::MANIPULATE_AXIS_Y;
	manipulatorsIDS["moveAxisZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_Z;

	manipulatorsIDS["moveAxisXY"] = ObjectToolbarTypes::MANIPULATE_AXIS_XY;
	manipulatorsIDS["moveAxisYZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_YZ;
	manipulatorsIDS["moveAxisXZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_XZ;
	manipulatorsIDS["moveAxisXYZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_XYZ;

	manipulatorsIDS["scaleAxisX"] = ObjectToolbarTypes::MANIPULATE_AXIS_X;
	manipulatorsIDS["scaleAxisY"] = ObjectToolbarTypes::MANIPULATE_AXIS_Y;
	manipulatorsIDS["scaleAxisZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_Z;

	manipulatorsIDS["scaleAxisXY"] = ObjectToolbarTypes::MANIPULATE_AXIS_XY;
	manipulatorsIDS["scaleAxisYZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_YZ;
	manipulatorsIDS["scaleAxisXZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_XZ;
	manipulatorsIDS["scaleAxisXYZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_XYZ;

	manipulatorsIDS["rotationAxisX"] = ObjectToolbarTypes::MANIPULATE_AXIS_X;
	manipulatorsIDS["rotationAxisY"] = ObjectToolbarTypes::MANIPULATE_AXIS_Y;
	manipulatorsIDS["rotationAxisZ"] = ObjectToolbarTypes::MANIPULATE_AXIS_Z;

	axisDirs[AxisType::AXIS_X] = XVector3(1.0, 0.0, 0.0);
	axisDirs[AxisType::AXIS_Y] = XVector3(0.0, 1.0, 0.0);
	axisDirs[AxisType::AXIS_Z] = XVector3(0.0, 0.0, 1.0);

	axisDirs[AxisType::AXIS_XY] = XVector3(1.0, 1.0, 0.0);
	axisDirs[AxisType::AXIS_YZ] = XVector3(0.0, 1.0, 1.0);
	axisDirs[AxisType::AXIS_XZ] = XVector3(1.0, 0.0, 1.0);

	axisDirs[AxisType::AXIS_XYZ] = XVector3(1.0, 1.0, 1.0);
	axisDirs[AxisType::AXIS_NONE] = XVector3(0.0, 0.0, 0.0);

	colorSelect = HEXCOLOR(0xDAA520FF);


	uint genericSize = getGenericID(ManipulatorType::manipulatorsCount+1, AxisType::AxisType_axisSize+1);

	manipulatorsGenericNodes.resize(genericSize);


	for (uint i = 0; i < genericSize;i++) {
		manipulatorsGenericNodes[i] = nullptr;
	}

	for (uint i = 0; i < ManipulatorType::manipulatorsCount;i++) {
		nodesManipulators[i] = nullptr;
	}

}


Manipulator::~Manipulator(void) {

}




XVector3 Manipulator::getAxisDir(AxisType axis) {
	return axisDirs[axis];
}

void Manipulator::setAxis(AxisType axis) {
	uint genericID = getGenericID(manipulatorType, axisTarget);
	if (manipulatorsGenericNodes[genericID] != nullptr) manipulatorsGenericNodes[genericID]->getMaterial()->setFlag(MATERIAL_COLOR_FROM_MAT,false);

	axisTarget = axis;

	genericID = getGenericID(manipulatorType, axisTarget);
	if (manipulatorsGenericNodes[genericID] != nullptr) manipulatorsGenericNodes[genericID]->getMaterial()->setFlag(MATERIAL_COLOR_FROM_MAT, true);
}


float Manipulator::getTracedAxisPoint(AxisType _a_type, XVector3 _pos,XLine cameraRay, XVector3 cameraDirection, XVector3*axisOut) {
	XVector3 axis = getAxisDir(_a_type);
	XVector3 n = Xcross(Xcross(-cameraDirection, axis), axis);
	XPlane p(n, _pos);
	p.normalize();
	XTracePoint i;
	if (p.trace(cameraRay, i, true, false)) {
		axisOut->v[_a_type] = 1.0;
		return i.point.v[_a_type];
	}
	else {
		axisOut->v[_a_type] = 0.0;
		return 0.0;
	}
}


void Manipulator::mouseDown(int x, int y) {
	if (nodeToManipulate == nullptr || manipulatorType == ManipulatorType::noManipulator || axisTarget == AxisType::AXIS_NONE) return;
	isChangeObject = false;
	modAngle = 0;


	XLine ray = _camera->getRay((float)x, (float)y);
	XVector3 cameraDirection = _camera->getDirection();

	XVector3 pos = nodeToManipulate->getPosition();
	

	XVector3 axisDirs = getAxisDir(axisTarget);
	XVector3 axisCalculated = XVector3(0, 0, 0);
	XVector3 pointTraced(0, 0, 0);

	for (uint i = 0; i < 3; i++) {
		if (axisDirs.v[i]) pointTraced.v[i] = getTracedAxisPoint((AxisType)i, pos, ray, cameraDirection, &axisCalculated);
		if (axisCalculated.v[i]) moveBase.v[i] = pointTraced.v[i];
	}

	if (manipulatorType == ManipulatorType::translate) {
		savedStartValue = pos;
	} else 
	if (manipulatorType == ManipulatorType::rotation) {
		
		savedStartValue = nodeToManipulate->getRotation();

		XTracePoint i;
		AxisType axisIDInvert = axisInvertForRotation(axisTarget);
		XVector3 axisDirsInvert = getAxisDir(axisIDInvert);

		XVector3 n = Xcross(Xcross(-cameraDirection, axisDirsInvert), axisDirsInvert);

		XPlane p(n, pos);
		if (p.trace(ray, i, true, false)) {

			XVector3 v0 = Xnormalize(moveBase - pos);
			XVector3 v1 = Xnormalize(i.point - pos);
			XVector3 axis = Xnormalize(Xcross(v0, v1)); // getAxisDir(axisTarget)

			float ang0 = Xarccos(Xdot(v0, v1));
			if ((axis - n).length() > 1) ang0 = -ang0;
			//ang0 = -ang0;
			ang0 += modAngle;
			float ang = ang0;
			if (ang != 0) {
				moveBase = i.point;
			}
		}
	}
	else if (manipulatorType == ManipulatorType::scale) {
		savedStartValue = nodeToManipulate->getScale();
	}


	positionForCalculatePlane = pos;
	updateViewManipulator();
}



XVector3 Manipulator::toCameraPlane(const XVector3 &pos) {
	XVector3 prj = _camera->project(pos);
	XLine ray = _camera->getRay(prj.x, prj.y);

	XVector3 camP = _camera->getCameraPosition();
	return camP + Xnormalize(ray.dst - ray.src) * (_camera->getNearPlane() + sizeManipulator * 5.0f);
}




void Manipulator::mouseMove(int x, int y) {
	if (nodeToManipulate == nullptr || manipulatorType == ManipulatorType::noManipulator || axisTarget == AxisType::AXIS_NONE) return;



	XVector3 axisDirs = getAxisDir(axisTarget);

	XVector3 pointTraced(0, 0, 0);
	XVector3 pos = nodeToManipulate->getPosition();

	XLine ray = _camera->getRay((float)x, (float)y);
	XVector3 cameraDirection = _camera->getDirection();
	for (uint i = 0; i < 3; i++)
		if (axisDirs.v[i]) pointTraced.v[i] = getTracedAxisPoint((AxisType)i, positionForCalculatePlane, ray, cameraDirection, &axisDirs);

	XVector3 dll = axisDirs;
	XVector3 shift = (pointTraced - moveBase);

	dll.x = dll.x * shift.x;
	dll.y = dll.y * shift.y;
	dll.z = dll.z * shift.z;

	if (manipulatorType == ManipulatorType::translate)
	{
		if (dll != XVector3(0, 0, 0)) {
			XVector3 nodeSnapPos = pos + dll;
			nodeToManipulate->setX(nodeSnapPos.x);
			nodeToManipulate->setY(nodeSnapPos.y);
			nodeToManipulate->setZ(nodeSnapPos.z);
			moveBase += dll;
		}
	}
	else if (manipulatorType == ManipulatorType::rotation) {
		XTracePoint i;

		AxisType axisIDInvert = axisInvertForRotation(axisTarget);
		XVector3 axisDirsInvert = getAxisDir(axisIDInvert);

		XVector3 n = Xcross(Xcross(-cameraDirection, axisDirsInvert), axisDirsInvert);
		XPlane p(n, pos);
		if (p.trace(ray, i, true, false)) {

			XVector3 v0 = Xnormalize(moveBase - pos);
			XVector3 v1 = Xnormalize(i.point - pos);
			XVector3 axis = Xnormalize(Xcross(v0, v1)); 

			float ang0 = Xarccos(Xdot(v0, v1));
			if ((axis - n).length() > 1) ang0 = -ang0;
			//ang0 = -ang0;
			ang0 += modAngle;
			float ang = ang0;

			if (ang != 0) {
				XVector4 rotVector;
				rotVector.makeWithAxisAndRad(ang, axisDirs.x, axisDirs.y, axisDirs.z);
				rotVector.normalize();

				XVector4 currentQuat = nodeToManipulate->getQuaternion();
				XVector4 newQuat = currentQuat * rotVector;
				nodeToManipulate->setQuaternion(&newQuat);
				moveBase = i.point;
				modAngle = ang0 - ang;
			}
		}
		
	}
	else if (manipulatorType == ManipulatorType::scale) {
		XVector3 rotation;
		rotation = nodeToManipulate->getScale();

		AxisType axis = axisTarget;

		if (axisTarget == AxisType::AXIS_XY)  axis = AxisType::AXIS_Y;
		if (axisTarget == AxisType::AXIS_YZ)  axis = AxisType::AXIS_Y;
		if (axisTarget == AxisType::AXIS_XZ)  axis = AxisType::AXIS_X;
		if (axisTarget == AxisType::AXIS_XYZ) axis = AxisType::AXIS_Y;

		XVector3 affectedAxis = getAxisDir(axisTarget);
		for (uint i = 0; i < 3; i++) {
			if (affectedAxis.v[i]) {
				rotation.v[i] += dll.v[axis] > 0.0 ? 0.05 : -0.05;
			}
		}

		nodeToManipulate->setScaleX(rotation.x);
		nodeToManipulate->setScaleY(rotation.y);
		nodeToManipulate->setScaleZ(rotation.z);
		moveBase += dll;
	}

	isChangeObject = true;
	updateViewManipulator();
}


void Manipulator::mouseUp(int x, int y) {

}

AxisType Manipulator::axisInvertForRotation(AxisType axisIn) {
	AxisType axisOut = axisIn;

	switch (axisOut)
	{
	case AXIS_X:
		axisOut = AXIS_Y;
		break;
	case AXIS_Y:
		axisOut = AXIS_X;
		break;
	case AXIS_Z:
		break;
	case AXIS_XY:
		break;
	case AXIS_XZ:
		break;
	case AXIS_YZ:
		break;
	case AXIS_XYZ:
		break;
	case AXIS_NONE:
		break;
	case AxisType_axisSize:
		break;
	default:
		break;
	}

	return axisOut;
}


void Manipulator::updateViewManipulator() {
	if (nodeToManipulate == nullptr) return;
	XVector3 manipulatorPosition = toCameraPlane(nodeToManipulate->getManipulatorPosition());
	this->setPosition(&manipulatorPosition);
}

void Manipulator::setNodeToManipulate(SceneNode*node) {
    nodeToManipulate = node;
	hideManipulator(manipulatorType);
	showManipulator(manipulatorType);
}

void Manipulator::setActiveManipulator(ManipulatorType type) {
	hideManipulator(manipulatorType);
	manipulatorType = type;
	showManipulator(manipulatorType);

	updateViewManipulator();
}

void Manipulator::showManipulator(ManipulatorType type) {
	if (type == ManipulatorType::noManipulator || nodesManipulators[type] == nullptr || nodeToManipulate == nullptr) return;

	SceneNode*manipulatorNode = nodesManipulators[type];
	for (uint i = 0; i < manipulatorNode->childs.size(); i++) {
		SceneNode*child = manipulatorNode->childs[i];
		child->isVisible = true;
	}
}

void Manipulator::hideManipulator(ManipulatorType type) {
	if (type == ManipulatorType::noManipulator || nodesManipulators[type] == nullptr) return;

	SceneNode*manipulatorNode = nodesManipulators[type];
	for (uint i = 0; i < manipulatorNode->childs.size(); i++) {
		SceneNode*child = manipulatorNode->childs[i];
		child->isVisible = false;
	}
}

uint Manipulator::getGenericID(uint type, uint axisType) {
	uint genericID = type*10+axisType;
	return genericID;
}
void Manipulator::addManipulator(ManipulatorType type,std::vector<Mesh*>*meshCollection) {

	if (nodesManipulators[type] != nullptr) {
		printf("manipulator exist \n");
		return; // exist manipulator
	}
	
	SceneNode*manipulatorRootNode = new SceneNode();

    for (uint i = 0; i < meshCollection->size();i++) {
        SceneNode*node = new SceneNode();
		node->isVisible = false;
		node->zTest = false;
        Mesh*mesh = (*meshCollection)[i];
        
        Material*material = new Material();
        material->setFlag(NORMAL_MATERIAL,false);
        material->setFlag(COLOR_MATERIAL,true);
		//
		material->color = colorSelect;


        node->setScaleX(mesh->initScale.x);
        node->setScaleY(mesh->initScale.y);
        node->setScaleZ(mesh->initScale.z);

        node->setQuaternion(&mesh->initQuaternion);

        node->setX(mesh->initTranslation.x);
        node->setY(mesh->initTranslation.y);
        node->setZ(mesh->initTranslation.z);

        node->setMaterial(material);
        node->setMesh(mesh);
		
		
		uint axisID = AxisType::AXIS_NONE;

		if (manipulatorsIDS.find(mesh->nameMesh) != manipulatorsIDS.end()) {
			// control manipulator ids
			node->typeObject = manipulatorsIDS[mesh->nameMesh];
			axisID = node->typeObject - ObjectToolbarTypes::MANIPULATE_AXIS_X;
		}
		else {
			node->typeObject = ObjectToolbarTypes::NO_OBJECT; // because manipulator mesh not need pick !
		}
		

		manipulatorsGenericNodes[getGenericID(type, axisID)] = node;

		manipulatorRootNode->addChild(node);

    }

	nodesManipulators[type] = manipulatorRootNode;


	this->addChild(manipulatorRootNode);
}


void Manipulator::setPosition(XVector3*pos) {
	this->setTranslate(pos);

	for (uint i = 0; i < ManipulatorType::manipulatorsCount; i++) {
		if (nodesManipulators[i] != nullptr) {
			nodesManipulators[i]->calculateTransformMatrix();
		}
	}

    this->calculateTransformMatrix();
}