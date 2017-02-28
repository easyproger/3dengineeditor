#pragma once
#include "ToolBarBase.h"
#include "XMathTypes.h" 
#include <string>
#include <map>
#include "XTypes.h"
#include <vector>

enum ControllSetterTypes {
	BUTTON_COLOR_SETTER,
	EDIT3_FLOAT_SETTER,
	EDIT_FLOAT_SETTER,
	EDIT_COLOR_SETTER,
	COMBO_UINT_SETTER,
	CHECKBOX_SETTER,
	CHECKBOX_SET_SETTER,
	COMBO_UINT_SET_SETTER,
	EDIT_HEX_VEC4_SET_SETTER,
	EDIT_FLOAT_SET_SETTER,
	MAX_ControllSetterTypes
};


#define MAX_HWNDS 3

class ToolBarParticles :
	public ToolBarBase
{
public:

	typedef struct ControlsInfo {
		HWND hwnd[MAX_HWNDS];
		uint idItem;
		LPARAM lParam;
		WPARAM wParam;
		void (ToolBarParticles::*action)(ControlsInfo*data);
		void (ToolBarParticles::*actionSet)(ControlsInfo*data);

		std::vector<UintNameStruct>* comboData;

		XVector4*vecto4val;
		XVector3*vector3val;
		float*floatval;
		uint*uintval;
		bool*boolval;
	}ControlsInfo;


	ToolBarParticles(HINSTANCE instance, int nCMD, ToolBarsEventInterface* toolbarInterface);
	~ToolBarParticles(void);


	std::vector<UintNameStruct> BlendInfoStaticData = {
		{ GL_ZERO , "GL_ZERO" }
		,{ GL_ONE ,"GL_ONE" }
		,{ GL_SRC_COLOR ,"GL_SRC_COLOR" }
		,{ GL_ONE_MINUS_SRC_COLOR ,"GL_ONE_MINUS_SRC_COLOR" }
		,{ GL_DST_COLOR ,"GL_DST_COLOR" }
		,{ GL_ONE_MINUS_DST_COLOR ,"GL_ONE_MINUS_DST_COLOR" }
		,{ GL_SRC_ALPHA ,"GL_SRC_ALPHA" }
		,{ GL_ONE_MINUS_SRC_ALPHA ,"GL_ONE_MINUS_SRC_ALPHA" }
		,{ GL_DST_ALPHA ,"GL_DST_ALPHA" }
		,{ GL_ONE_MINUS_DST_ALPHA ,"GL_ONE_MINUS_DST_ALPHA" }
		,{ GL_CONSTANT_COLOR ,"GL_CONSTANT_COLOR" }
		,{ GL_ONE_MINUS_CONSTANT_COLOR ,"GL_ONE_MINUS_CONSTANT_COLOR" }
		,{ GL_CONSTANT_ALPHA ,"GL_CONSTANT_ALPHA" }
		,{ GL_ONE_MINUS_CONSTANT_ALPHA ,"GL_ONE_MINUS_CONSTANT_ALPHA" }
	,{ GL_SRC_ALPHA_SATURATE ,"GL_SRC_ALPHA_SATURATE" } };

	std::vector<UintNameStruct> ParticleSortTypes = {
		{ PrticleSortTypes::NO_SORT_PARTICLE,"No sort" },
		{ PrticleSortTypes::BACK_TO_FRONT_SORT_PARTICLE,"Back to front" },
		{ PrticleSortTypes::FRONT_TO_BACK_SORT_PARTICLE,"Front to back" }
	};

	void buttonColorSetter(ControlsInfo*data);
	void editFloatSetter(ControlsInfo*data);
	void editColorSetter(ControlsInfo * data);
	void comboUintSetter(ControlsInfo * data);
	void checkboxSetter(ControlsInfo*data);

	void checkboxSetSetter(ControlsInfo*data);
	void comboUintSetSetter(ControlsInfo * data);
	void editFloatSetSetter(ControlsInfo*data);
	void editHexVec4SetSetter(ControlsInfo * data);
	void (ToolBarParticles::*actions[ControllSetterTypes::MAX_ControllSetterTypes])(ControlsInfo*controlldata);




	
	virtual INT_PTR CALLBACK toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	XVector4 chooseColor(HWND editBox, COLORREF rgb);

	virtual void update(Object*object);
	void setInfo();
	virtual void destroy();
	virtual void create();
private:
	char szFile[100];
	OPENFILENAME ofnTexture;
	std::map<uint, ControlsInfo> controllsInfo;


	CHOOSECOLOR chooseStruct;

	COLORREF acrCustClr[16];
	DWORD rgbCurrent;
	ParticleInfo info;
};

