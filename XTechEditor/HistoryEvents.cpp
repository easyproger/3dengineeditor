#include "HistoryEvents.h"
#include "SceneNode.h"


HistoryEvents::HistoryEvents()
{
	currentHistoryPosition = 0;
	actions[HistoryTypes::HISTORY_DELETE_OBJECT]   = &HistoryEvents::removeObject;

	actions[HistoryTypes::HISTORY_MOVE_OBJECT]     = &HistoryEvents::moveObject;
	actions[HistoryTypes::HISTORY_ROTATION_OBJECT] = &HistoryEvents::rotationObject;
	actions[HistoryTypes::HISTORY_SCALE_OBJECT]    = &HistoryEvents::scaleObject;
}


HistoryEvents::~HistoryEvents()
{
}


void HistoryEvents::moveObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction) {
	
	SceneNode* obj = (SceneNode*)dataHistory;
	if (obj) {
		HistoryVector3Value* val = (HistoryVector3Value*)value;
		obj->setTranslate(&val->val);
	}
}
void HistoryEvents::rotationObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction) {

	SceneNode* obj = (SceneNode*)dataHistory;
	if (obj) {
		HistoryVector3Value* val = (HistoryVector3Value*)value;
		obj->setRotationEulerinRadians(&val->val);
	}
}
void HistoryEvents::scaleObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction) {

	SceneNode* obj = (SceneNode*)dataHistory;
	if (obj) {
		HistoryVector3Value* val = (HistoryVector3Value*)value;
		obj->setScale(&val->val);
	}
}

void HistoryEvents::removeObject(void*dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction) {
	
	SceneNode* obj = (SceneNode*)dataHistory;
	if (obj) {
		HistoryBoolValue* boolval = (HistoryBoolValue*)value;
		obj->isRemoved(boolval->val);
	}
}


void HistoryEvents::forward() {
	
	if (currentHistoryPosition+1 >= actions_vec.size()) return;
	currentHistoryPosition++;
	HistoryAction a = actions_vec[currentHistoryPosition];
	(this->*a.action)(a.dataHistory,a.value2, a.value, false); // run
	
}

void HistoryEvents::back() {
	
	if (currentHistoryPosition < 0) return;
	
	if (currentHistoryPosition >= actions_vec.size()) return;

	HistoryAction a = actions_vec[currentHistoryPosition];
	(this->*a.action)(a.dataHistory,a.value, a.value2,true); // run

	currentHistoryPosition--;
}


void HistoryEvents::addAction(HistoryTypes idAction, void*data, HistoryBaseValue *valueIn, HistoryBaseValue *valueOut) {
	if (idAction == HistoryTypes::MAX_HISTORY) return;


	if (actions_vec.size()) {
		for (uint i = actions_vec.size() - 1; i >= currentHistoryPosition+1; i--) {
			free(actions_vec[i].value);
			free(actions_vec[i].value2);
		}
		actions_vec.resize(currentHistoryPosition+1);
	}


	HistoryAction aIn;
	aIn.action = actions[idAction];
	aIn.dataHistory = data;
	aIn.value = valueIn;
	aIn.value2 = valueOut;
	actions_vec.push_back(aIn);

	currentHistoryPosition = actions_vec.size() - 1;
}

