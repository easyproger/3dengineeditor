#pragma once
#include "XMathBase.h"
#include "XMathTypes.h"
enum HistoryTypes {HISTORY_DELETE_OBJECT, HISTORY_MOVE_OBJECT, HISTORY_SCALE_OBJECT, HISTORY_ROTATION_OBJECT,MAX_HISTORY};


class HistoryBaseValue {
public:

};

class HistoryBoolValue : public HistoryBaseValue {
public:
	bool val;
};

class HistoryVector3Value : public HistoryBaseValue {
public:
	XVector3 val;
};

class HistoryVector4Value : public HistoryBaseValue {
public:
	XVector4 val;
};

class HistoryEvents
{
public:

	typedef struct {
		void (HistoryEvents::*action)(void*dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2,bool isBackAction);
		void*dataHistory;
		HistoryBaseValue *value;
		HistoryBaseValue *value2;
	}HistoryAction;

	HistoryEvents();
	~HistoryEvents();
	
	void moveObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction);
	void rotationObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction);
	void scaleObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction);
	void removeObject(void * dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction);


	void forward();
	void back();
	void addAction(HistoryTypes idAction, void * data, HistoryBaseValue *valueIn, HistoryBaseValue *valueOut);

	void (HistoryEvents::*actions[HistoryTypes::MAX_HISTORY])(void*dataHistory, HistoryBaseValue *value, HistoryBaseValue *value2, bool isBackAction);

	std::vector<HistoryAction> actions_vec;
	int currentHistoryPosition;
};



