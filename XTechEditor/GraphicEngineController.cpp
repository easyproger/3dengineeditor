#include "GraphicEngineController.h"


GraphicEngineController::GraphicEngineController(GUIEventInterface*guiInterface)
{
	_guiInterface = guiInterface;
	
	canRender = false;


	savedAngle = 0;
	radius_ = 0;
	lastTime = 0;
	isAnimationPlayed = false;
	minAnimationFrame = 0;
	currentAnimationFrame = 0;
	maxAnimationFrame = 100;

	loadSceneData = false;

	windowWidth = 0;
	windowHeight = 0;
	
	mouseLPressed = false;

	camera = nullptr;
	currentSelectedObject = nullptr;
	lastSelectedObject = nullptr;
	currentMode = ModeControlls::camera_free;

	shiftKeyPressed = false;
	ctrlKeyPressed = false;
	
	sizeManipulator = 10.0;
}




GraphicEngineController::~GraphicEngineController(void)
{
}

void GraphicEngineController::init(std::string basePath) {
	_basePath = basePath;

	printf("_basePath is %s:\n", _basePath.c_str());


	meshCache = new MeshCache(&damagedMeshs);
	animationsCache = new XAnimations(&skinAnimationsCache);

	loader = new FileLoader(meshCache,animationsCache);

	std::string shadersPath;
	
	shadersPath = basePath;
	shadersPath+="\\shaders\\";
	
	ResourcesManager::me()->init((char*)shadersPath.c_str());
	ResourcesManager::me()->setBasePath(basePath);
	
	textureCache    = new TextureCache();
	soundSystem     = new SoundSystem();
	
	
	
	camera = new Camera();

	manipulator = new Manipulator(camera,this);

	camera->setPerspective(45.0, windowWidth,windowHeight, 1.0, 10300.0);
	
	
	
	xRender = new Renderer(camera);
	mainSceneGraph = new SceneGraph();
	xRender->setDefaultSceneGraph(mainSceneGraph);




	
	defaultLight = new Light();
	defaultLight->lookAt(vec3(0, 100, -100), vec3(0.0f, 0.0f, 0.0f), vec3(0,1,0));
	

	defaultLight->setTranslate(&XVector3(0, 120, 120));

	mainSceneGraph->addLightinTree(defaultLight);
	mainSceneGraph->addObjectinTree(defaultLight);

	camera->setViewport(0,0,windowWidth,windowHeight);
	camera->lookAt(vec3(10, 26, -45),vec3( 0, 0, 0),vec3(0,1,0));
	

	
	

	std::vector<Mesh*> meshs;

	loadMesh(basePath+"\\manipulatorsData\\manipulatorMove.XMESH",&meshs);
	manipulator->addManipulator(ManipulatorType::translate,&meshs);
	meshs.clear();

	loadMesh(basePath + "\\manipulatorsData\\manipulatorRotation.XMESH", &meshs);
	manipulator->addManipulator(ManipulatorType::rotation, &meshs);
	meshs.clear();

	loadMesh(basePath + "\\manipulatorsData\\manipulatorScale.XMESH", &meshs);
	manipulator->addManipulator(ManipulatorType::scale, &meshs);
	meshs.clear();

	mainSceneGraph->addObjectinTree(manipulator);


	std::vector<SceneNode*> tmpData;
	loadMesh(basePath+"\\sceneData\\plane.XMESH",&meshs);
	createNodes(&meshs,&tmpData);
	meshs.clear();
	SceneNode*planeNode = tmpData[0];
	planeNode->setWireFrameRender();
	planeNode->typeObject = NO_OBJECT;

	loadMesh(basePath + "\\sceneData\\lightView.XMESH", &meshs);
	defaultLight->setMesh(meshs[0]);
	defaultLight->setMaterial(generateMaterial(meshs[0]));



	mainSceneGraph->createListObjects();
	canRender = true;
}

void GraphicEngineController::changeMode(ModeControlls newMode) {
	currentMode = newMode;
}

void GraphicEngineController::setManipulator(ManipulatorType type) {
	manipulator->setActiveManipulator(type);
	updateXYZValue();
}


void GraphicEngineController::updateXYZValue() {
	ManipulatorType type = manipulator->getManipulatorType();
	if (type == ManipulatorType::noManipulator || currentSelectedObject == nullptr) return;

	XVector3 value;
	if (type == ManipulatorType::translate) value = currentSelectedObject->getPosition();
	if (type == ManipulatorType::scale)     value = currentSelectedObject->getScale();
	if (type == ManipulatorType::rotation)  value = currentSelectedObject->getRotationDegress();

	_guiInterface->setXYZValueCurrentObject(value);
}

bool GraphicEngineController::getLoadSceneDataFromMesh() {
	return loadSceneData;
}

void GraphicEngineController::setLoadSceneDataFromMesh(bool value) {
	loadSceneData = value;
}

void GraphicEngineController::addMesh(std::string &meshPath) {
	std::vector<Mesh*> meshs;
	std::vector<GenericObjects*> genericNodes; // particles and other generic nodes
	std::vector<CameraInfo> cameras;
	std::vector<LightInfo> lights;

	loadMesh(meshPath,&meshs,&genericNodes,&cameras,&lights,loadSceneData);
	createNodes(&meshs,&nodesScene,true);
	createGenericObjects(&genericNodes);

	mainSceneGraph->createListObjects();

	for (uint i = 0; i < genericNodes.size(); i++) {
		GenericObjects*object = genericNodes[i];
		if (object->typeGeneric == TypeGenericObjects::GenericObject_PARTICLE) {
			ParticleInfo*info = (ParticleInfo*)object;
			if (info) delete info;
		}
		else {
			delete object;
		}
	}

	genericNodes.clear();

	if (loadSceneData) {
		// hack for lights and cameras 
		if (cameras.size()) {
			camera->setInfo(&cameras[0]);
		}
		if (lights.size()) {
			defaultLight->setInfo(&lights[0]);
		}
	}

}

void GraphicEngineController::createGenericObjects(std::vector<GenericObjects*>*genericNodes) {
	for (uint i = 0; i < genericNodes->size();i++) {
		GenericObjects*object = (*genericNodes)[i];
		if (object->typeGeneric == TypeGenericObjects::GenericObject_PARTICLE) {
			ParticleInfo* info = (ParticleInfo*)object;
			if (!info) continue;
			ParticleEmiter*emimter = new ParticleEmiter(*info);
			mainSceneGraph->addObjectinTree(emimter);
			nodesUpdate.push_back(emimter);

			// history 
			HistoryBoolValue *valOut, *valIn;
			valIn = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
			valOut = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
			valIn->val = true;
			valOut->val = emimter->isRemoved();
			historyController.addAction(HistoryTypes::HISTORY_DELETE_OBJECT, (void*)emimter, valIn, valOut);
		}
	}
}

void GraphicEngineController::setAnimationRange(unsigned int start, unsigned int end) {
	if (start >= end) start = 0;
	minAnimationFrame       = start;
	maxAnimationFrame       = end;
	currentAnimationFrame   = start;
}

XVector2<unsigned int> GraphicEngineController::getAnimationRange() {
	return XVector2<unsigned int>(minAnimationFrame,maxAnimationFrame);
}

void GraphicEngineController::playAnimation() {
	isAnimationPlayed = true;
}

void GraphicEngineController::stopAnimation() {
	isAnimationPlayed = false;
}

void GraphicEngineController::removeSelection() {
	if (currentSelectedObject != nullptr) {
		HistoryBoolValue *valOut,*valIn;
		valIn = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
		valOut = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));

		valIn->val  = currentSelectedObject->isRemoved();
		valOut->val = !currentSelectedObject->isRemoved();

		historyController.addAction(HistoryTypes::HISTORY_DELETE_OBJECT, (void*)currentSelectedObject, valIn, valOut);

		currentSelectedObject->isRemoved(true);

		deselectObject();
	}
}

void GraphicEngineController::loadMesh(std::string &meshPath,std::vector<Mesh*>*meshs, std::vector<GenericObjects*>*genericNodes, std::vector<CameraInfo>*cameras, std::vector<LightInfo>*lights,bool loadSceneData) {
	loader->loadXMESH(meshPath.c_str(),meshs, genericNodes, cameras, lights, loadSceneData);
}

Material* GraphicEngineController::generateMaterial(Mesh*mesh) {
	Material*material = new Material();
	material->setFlag(NORMAL_MATERIAL, false);

	if (mesh->color && mesh->numTextureCoordinats) {
		material->setFlag(TEXTURE_AND_COLOR, true);
	}
	else {
		if (mesh->color) {
			material->setFlag(COLOR_MATERIAL, true);
		}
		else if (mesh->numTextureCoordinats) {
			material->setFlag(NORMAL_MATERIAL, true);
		}
		else {
			material->setFlag(BASE_MATERIAL_COLOR, true);
		}
	}
	if (material->getFlagVal(NORMAL_MATERIAL)) {
		material->diffuseTexture = new Texture();
		material->diffuseTexture->createTextureSimple();
	}
	return material;
}


void GraphicEngineController::createNodes(std::vector<Mesh*>* meshs,std::vector<SceneNode*> *createdNodes,bool history) {

	for (uint i = 0; i < meshs->size();i++) {
		DynamicEntity*node = new DynamicEntity(&skinAnimationsCache);
		
		Mesh*mesh = (*meshs)[i];
		
		Material*material = generateMaterial(mesh);
		
		material->color = XVector3(1.0,0.0,1.0);
		material->setFlag(DYNAMIC_PER_VERTEX_BASE_LIGHT, true);
		
		node->setMaterial(material);
		node->setMesh(mesh);

		node->setQuaternion(&mesh->initQuaternion);

		node->setScaleX(mesh->initScale.x);
		node->setScaleY(mesh->initScale.y);
		node->setScaleZ(mesh->initScale.z);

		node->setTranslate(&mesh->initTranslation);

		createdNodes->push_back(node);
		if (mesh->assignedBaseAnimationsID != -1) {
			// need assign animation to Node !
			node->setAnimation(animationsCache->getAnimation(mesh->assignedBaseAnimationsID));

			// have animations need push to update list
			nodesUpdate.push_back(node);
		}

		if (history) {
			HistoryBoolValue *valOut, *valIn;
			valIn = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
			valOut = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));

			valIn->val = true;
			valOut->val = node->isRemoved();
			historyController.addAction(HistoryTypes::HISTORY_DELETE_OBJECT, (void*)node, valIn, valOut);
		}
		mainSceneGraph->addObjectinTree(node);
	}
}



void GraphicEngineController::setWindowSize(float width,float height) {
	windowWidth = width;
	windowHeight = height;
	
	if(camera != nullptr) {
		camera->setViewport(0,0,windowWidth,windowHeight);
		camera->setPerspective(45.0, windowWidth, windowHeight, 1.0, 10000.0);
	}
}

void GraphicEngineController::leftMouseButtonDown(int coordinatsX,int coordinatsY) {

	if (currentMode == ModeControlls::camera_free) {
		mouseRotate = true;
		myMouseYcoord = coordinatsX; 
		myMouseXcoord = coordinatsY;
	}else if (currentMode == ModeControlls::pick) {
		trySelectObject((float)coordinatsX,(float)coordinatsY);
		manipulator->mouseDown(coordinatsX, coordinatsY);
		manipulator->updateViewManipulator();
	}
}


void GraphicEngineController::leftMouseButtonUp(int coordinatsX,int coordinatsY) {

	if (currentMode == ModeControlls::camera_free) {
		mouseRotate = false;
		mouseMove = false;
	}
	else if (currentMode == ModeControlls::pick) {
		changeObjectEnd();
		manipulator->setAxis(AxisType::AXIS_NONE);
		manipulator->mouseUp(coordinatsX, coordinatsY);
	}
}


void GraphicEngineController::rightMouseButtonDown(int coordinatsX,int coordinatsY) {
	if (currentMode == ModeControlls::camera_free) {
		mouseMove = true;
		myMouseYcoord = coordinatsX; 
		myMouseXcoord = coordinatsY;
	}
}


void GraphicEngineController::rightMouseButtonUp(int coordinatsX,int coordinatsY) {
	if (currentMode == ModeControlls::camera_free) {
		mouseRotate = false;
		mouseMove = false;
	}
}



void GraphicEngineController::mouseMoveEvent(int coordinatsX,int coordinatsY) {

	if (currentMode == ModeControlls::camera_free) {
		myMouseXcoordVar = coordinatsY;
		myMouseYcoordVar = coordinatsX;
	}
	else if (currentMode == ModeControlls::pick) {
		manipulator->mouseMove(coordinatsX, coordinatsY);
	}
}



void GraphicEngineController::mouse_Events() {
	if (mouseRotate == true) 
	{
		
		camera->rotatePosition((float)(myMouseYcoordVar - myMouseYcoord)/10, 0, 1, 0);
		
		rot_cam_X = rot_cam_X + (myMouseXcoordVar - myMouseXcoord);
		camera->upDown(((float)(myMouseXcoordVar - myMouseXcoord))/10 );
		
		myMouseYcoord = myMouseYcoordVar;
		myMouseXcoord = myMouseXcoordVar;

		
	}
	else
	{
		if (mouseMove == true)
		{
			
			camera->moveCamera((float)(myMouseXcoordVar - myMouseXcoord)/10);
			camera->strafe(-((float)(myMouseYcoordVar - myMouseYcoord) )/10);
			
			myMouseYcoord = myMouseYcoordVar;
			myMouseXcoord = myMouseXcoordVar;

			
			
		}
		
	};

	

}



void GraphicEngineController::saveMesh(std::string path) {
	std::vector<CameraInfo> cameras;
	cameras.push_back(*camera->getInfo());
	std::vector<LightInfo> lights;
	lights.push_back(*defaultLight->getInfo());
	loader->safeXMESH(path.c_str(), &mainSceneGraph->childController->childsList, &cameras, &lights);
}

void GraphicEngineController::keyboardKeyDown(int keyID) {
	
	if (keyID == 17) {
		ctrlKeyPressed = true;
	}
	if (keyID == 16) {
		shiftKeyPressed = true;
	}

	if (keyID == 90 && ctrlKeyPressed && !shiftKeyPressed) {
		// ctrl z invoked 
		historyController.back();
	}
	else if (keyID == 90 && ctrlKeyPressed && shiftKeyPressed) {
		historyController.forward();
	}


	if (currentMode == ModeControlls::camera_free) {
		switch (keyID) {
			case 37: // left
				camera->moveLeft(5);
				break;
			case 39: // left
				camera->moveRight(5);
				break;
			case 38: // up
				camera->moveForward(5);
				//radius_+=5;
				break;
			case 40: // down
				camera->moveBackward(25);
				//radius_-=5;
				break;
		}
	}
	
}

void GraphicEngineController::keyboardKeyUP(int keyID) {
	if (keyID == 17) {
		ctrlKeyPressed = false;
	}
	if (keyID == 16) {
		shiftKeyPressed = false;
	}
}




void GraphicEngineController::render(double currentTime) {


	double delta = currentTime - lastTime;
	delta = abs(delta)/ 1000.0;
	lastTime = currentTime;
	if (delta > 0.1) delta = 0.1;


	if (!canRender) return;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
	
	mouse_Events();
	camera->look();

	XVector3 cameraDir = camera->getDirection();

	

	if (isAnimationPlayed) {
		if (currentAnimationFrame >= maxAnimationFrame) currentAnimationFrame = minAnimationFrame;
		for (uint i = 0; i < nodesUpdate.size(); i++) {
			BaseRenderObject*base = nodesUpdate[i];
			if (!base->isVisible) continue;
			base->cameraDir = cameraDir;
			base->update(currentAnimationFrame, delta);
		}
		currentAnimationFrame++;
	}


	xRender->render();



	if (isAnimationPlayed) {
		for (uint i = 0; i < nodesUpdate.size(); i++) {
			BaseRenderObject*base = nodesUpdate[i];
			if (!base->waitEndUpdate) continue;
			base->endUpdate();
		}
	}
}


void GraphicEngineController::changeObjectEnd() {
	ManipulatorType type = manipulator->getManipulatorType();
	if (currentSelectedObject == nullptr || type == ManipulatorType::noManipulator || !manipulator->isChangeObject) return;
	manipulator->isChangeObject = false;
	saveHistoryChangeObject();

}

void GraphicEngineController::saveHistoryChangeObject() {
	ManipulatorType type = manipulator->getManipulatorType();
	if (currentSelectedObject == nullptr || type == ManipulatorType::noManipulator) return;

	XVector3 changedValue;
	HistoryTypes historyType = HistoryTypes::MAX_HISTORY;

	if (type == ManipulatorType::translate) {
		changedValue = currentSelectedObject->getPosition();
		historyType = HistoryTypes::HISTORY_MOVE_OBJECT;
	}
	else if (type == ManipulatorType::rotation) {
		changedValue = currentSelectedObject->getRotation();
		historyType = HistoryTypes::HISTORY_ROTATION_OBJECT;
		
	}
	else if (type == ManipulatorType::scale) {
		changedValue = currentSelectedObject->getScale();
		historyType = HistoryTypes::HISTORY_SCALE_OBJECT;
	}

	if (changedValue == manipulator->savedStartValue) return;

	HistoryVector3Value *valOut, *valIn;
	valIn = (HistoryVector3Value*)malloc(sizeof(HistoryVector3Value));
	valOut = (HistoryVector3Value*)malloc(sizeof(HistoryVector3Value));

	valIn->val = manipulator->savedStartValue;
	valOut->val = changedValue;
	historyController.addAction(historyType, (void*)currentSelectedObject, valIn, valOut);
}


void GraphicEngineController::setXYZCurrentObject(XVector3 val) {
	ManipulatorType type = manipulator->getManipulatorType();
	if (currentSelectedObject == nullptr || type == ManipulatorType::noManipulator) return;

	if (type == ManipulatorType::rotation) {
		manipulator->savedStartValue = currentSelectedObject->getRotation();
		currentSelectedObject->setRotationEulerinDegress(&val);
	}
	if (type == ManipulatorType::translate) {
		manipulator->savedStartValue = currentSelectedObject->getPosition();
		currentSelectedObject->setTranslate(&val);
	}
	if (type == ManipulatorType::scale) {
		manipulator->savedStartValue = currentSelectedObject->getScale();
		currentSelectedObject->setScale(&val);
	}

	// history 
	saveHistoryChangeObject();
}

void GraphicEngineController::moveSelectedObjectX(float value) {
	//move object 
	
}

void GraphicEngineController::moveObjectEnd() {
	if (currentSelectedObject == nullptr) return;
}

void GraphicEngineController::addParticles() {
	ParticleEmiter* emiter = new ParticleEmiter();
	nodesUpdate.push_back(emiter);
	mainSceneGraph->addObjectinTree(emiter);
	mainSceneGraph->createListObjects();

	HistoryBoolValue *valOut, *valIn;
	valIn = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
	valOut = (HistoryBoolValue*)malloc(sizeof(HistoryBoolValue));
	valIn->val = true;
	valOut->val = emiter->isRemoved();
	historyController.addAction(HistoryTypes::HISTORY_DELETE_OBJECT, (void*)emiter, valIn, valOut);

}

void GraphicEngineController::setCameraHome() {
	camera->lookAt(vec3(-160, 100, -160), vec3(0, 0, 0), vec3(0, 1, 0));
}
void GraphicEngineController::setCameraLeft() {
	camera->lookAt(vec3(0, 0, -100), vec3(0, 0, 0), vec3(0, 1, 0));
}
void GraphicEngineController::setCameraTop() {
	camera->lookAt(vec3(1,100,1), vec3(0,0,0), vec3(0,1,0));
}



void GraphicEngineController::setParticleInfo(ParticleInfo info) {
	if (currentSelectedObject == nullptr) return;
	if (currentSelectedObject->typeObject != PARTICLES_OBJECT) return;
	ParticleEmiter*emiter = (ParticleEmiter*)currentSelectedObject;
	if (emiter == nullptr) return;

	emiter->setInfo(&info);

}

void GraphicEngineController::deselectObject() {
	_guiInterface->selectToolBar(nullptr);
	if (currentSelectedObject != nullptr) currentSelectedObject->getMaterial()->setFlag(MATERIAL_COLOR_FROM_MAT,false);
	manipulator->setNodeToManipulate(nullptr); // clear selection
	currentSelectedObject = nullptr;
	manipulator->setAxis(AxisType::AXIS_NONE);

	_guiInterface->setXYZValueCurrentObject(XVector3(0, 0, 0));
}

void GraphicEngineController::selectObject(Object*obj) {

	SceneNode*renderObject = (SceneNode*)obj;
	if (renderObject != nullptr) {
		//renderObject->setWireFrameRender();
		currentSelectedObject = renderObject;
		if (renderObject->typeObject != PARTICLES_OBJECT)
		currentSelectedObject->getMaterial()->setFlag(MATERIAL_COLOR_FROM_MAT, true);
		manipulator->setNodeToManipulate(currentSelectedObject);
		_guiInterface->selectToolBar(obj);

		updateXYZValue();
	}

}



void GraphicEngineController::setTexture(std::string path) {
	if (currentSelectedObject == nullptr) return;

	Material*currentMaterial = currentSelectedObject->getMaterial();
	Texture*matTexture = currentMaterial->diffuseTexture;
	if (matTexture != nullptr) {
		matTexture->deleteTexture();
		matTexture->createTexture(path);
	}
}
std::string GraphicEngineController::getTexture() {
	if (currentSelectedObject == nullptr) return "";

	Material*currentMaterial = currentSelectedObject->getMaterial();
	Texture*matTexture = currentMaterial->diffuseTexture;
	if (matTexture != nullptr) {
		return basename(matTexture->pathForTexture);
	}
	return "no texture";
}

void GraphicEngineController::trySelectObject(float x,float y) {
	Object*obj = xRender->pickObject((float)x/(float)windowWidth,(float)y/(float)windowHeight);

	manipulator->setAxis(AxisType::AXIS_NONE);

	if (obj != nullptr) {
		// some pick
		if (obj->typeObject >= ObjectToolbarTypes::MANIPULATE_AXIS_X && obj->typeObject<= ObjectToolbarTypes::MANIPULATE_AXIS_XYZ) {
			// manipulator pick 

			uint idIn  = obj->typeObject;
			uint idOut = ObjectToolbarTypes::MANIPULATE_AXIS_X;

			manipulator->setAxis(AxisType(idIn- idOut));
		}
		else if( obj->typeObject >= ObjectToolbarTypes::MESH_OBJECT && obj->typeObject<= ObjectToolbarTypes::LIGHT_OBJECT) {
			// pick mesh 
			deselectObject();
			selectObject(obj);
		}
		else {
			// pick NO_OBJECT need deSelect current object
			deselectObject();
		}
	}
	else {
		deselectObject();
	}
	
}


