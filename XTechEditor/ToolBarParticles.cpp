#include "ToolBarParticles.h"





ToolBarParticles::ToolBarParticles(HINSTANCE instance, int nCMD, ToolBarsEventInterface* toolbarInterface) :ToolBarBase(instance, nCMD, toolbarInterface)
{

	actions[EDIT_FLOAT_SETTER]   = &ToolBarParticles::editFloatSetter;
	actions[EDIT_COLOR_SETTER]   = &ToolBarParticles::editColorSetter;
	actions[BUTTON_COLOR_SETTER] = &ToolBarParticles::buttonColorSetter;
	actions[EDIT_HEX_VEC4_SET_SETTER] = &ToolBarParticles::editHexVec4SetSetter;
	actions[EDIT_FLOAT_SET_SETTER] = &ToolBarParticles::editFloatSetSetter;
	actions[COMBO_UINT_SETTER] = &ToolBarParticles::comboUintSetter;
	actions[COMBO_UINT_SET_SETTER] = &ToolBarParticles::comboUintSetSetter;
	actions[CHECKBOX_SETTER] = &ToolBarParticles::checkboxSetter;
	actions[CHECKBOX_SET_SETTER] = &ToolBarParticles::checkboxSetSetter;

	ZeroMemory(&ofnTexture, sizeof(ofnTexture));
	ofnTexture.lStructSize = sizeof(ofnTexture);
	ofnTexture.hwndOwner = NULL;
	ofnTexture.lpstrFile = szFile;
	ofnTexture.lpstrFile[0] = '\0';
	ofnTexture.nMaxFile = sizeof(szFile);
	ofnTexture.lpstrFilter = TEXT("Texture png\0*.png\0");
	ofnTexture.nFilterIndex = 1;
	ofnTexture.lpstrFileTitle = NULL;
	ofnTexture.nMaxFileTitle = 0;
	ofnTexture.lpstrInitialDir = NULL;
	ofnTexture.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

}


ToolBarParticles::~ToolBarParticles(void)
{
}

void ToolBarParticles::checkboxSetter(ControlsInfo*data) {
	*data->boolval = IsDlgButtonChecked(toolbarHWND, data->idItem);
	_toolBarInterface->setParticleInfo(info);
}

void ToolBarParticles::checkboxSetSetter(ControlsInfo*data) {
	CheckDlgButton(toolbarHWND, data->idItem, *data->boolval);
}


void ToolBarParticles::comboUintSetter(ControlsInfo * data) {
	if (HIWORD(data->wParam) == CBN_SELCHANGE){
		int ItemIndex = SendMessage((HWND)data->lParam, (UINT)CB_GETCURSEL,(WPARAM)0, (LPARAM)0);

		TCHAR  ListItem[256];
		(TCHAR)SendMessage((HWND)data->lParam, (UINT)CB_GETLBTEXT,
			(WPARAM)ItemIndex, (LPARAM)ListItem);



		std::vector<UintNameStruct> *comboData = data->comboData;
		*data->uintval = (*comboData)[ItemIndex].id;
		_toolBarInterface->setParticleInfo(info);
	}
}

void ToolBarParticles::comboUintSetSetter(ControlsInfo * data) {
	int index = -1;
	std::vector<UintNameStruct> *comboData = data->comboData;
	for (uint i = 0; i < comboData->size();i++) {
		if ((*comboData)[i].id == *data->uintval) {
			index = i;
			break;
		}
	}
	if (index!= -1) {
		SendMessage(data->hwnd[0], CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
	}
}


void ToolBarParticles::buttonColorSetter(ControlsInfo*data) {
	COLORREF rgb = RGB(data->vecto4val->r * 255, data->vecto4val->g * 255, data->vecto4val->b * 255);
	XVector4 color = chooseColor(data->hwnd[1], rgb);
	data->vecto4val->a = 1.0;
	data->vecto4val->r = color.w;
	data->vecto4val->g = color.z;
	data->vecto4val->b = color.y;
	_toolBarInterface->setParticleInfo(info);
}

void ToolBarParticles::editFloatSetter(ControlsInfo*data) {
	if (HIWORD(data->wParam) == EN_KILLFOCUS) {
		TCHAR buff[9];
		GetWindowText(data->hwnd[0], buff, 9);
		*data->floatval = atof(buff);
		_toolBarInterface->setParticleInfo(info);
	}
}

void ToolBarParticles::editColorSetter(ControlsInfo*data) {
	if (HIWORD(data->wParam) == EN_KILLFOCUS) {
		TCHAR buff[20];
		GetWindowText(data->hwnd[0], buff, 20);
		unsigned long long colorref = (unsigned long long)strtoll(buff, NULL, 16);
		if (colorref) {
			XVector4 color = HEXCOLOR(colorref);
			data->vecto4val->a = color.a;
			data->vecto4val->r = color.r;
			data->vecto4val->g = color.g;
			data->vecto4val->b = color.b;
			_toolBarInterface->setParticleInfo(info);
		}
	}
}

void ToolBarParticles::editHexVec4SetSetter(ControlsInfo*data) {
	TCHAR buff[20];
	XVector4 color = data->vecto4val;
	sprintf(buff, "%X", COLORHEX(color));
	SetWindowText(data->hwnd[0], buff);
}

void ToolBarParticles::editFloatSetSetter(ControlsInfo*data) {
	TCHAR buff[20];
	float value = *data->floatval;
	sprintf(buff, "%f", value);
	SetWindowText(data->hwnd[0], buff);
}

INT_PTR CALLBACK ToolBarParticles::toolbarCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_DESTROY:
		return FALSE;
	case WM_COMMAND:
		if (controllsInfo.find(LOWORD(wParam)) != controllsInfo.end()) {
			controllsInfo[LOWORD(wParam)].lParam = lParam;
			controllsInfo[LOWORD(wParam)].wParam = wParam;
			(this->*controllsInfo[LOWORD(wParam)].action)(&controllsInfo[LOWORD(wParam)]);
		}
		else {
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_PICK_TEXTURE:
			{
				GetOpenFileName(&ofnTexture);
				std::string pathtexture = ofnTexture.lpstrFile;
				_toolBarInterface->setTexture(pathtexture);
				pathtexture = basename(pathtexture);
				SetWindowText(GetDlgItem(toolbarHWND, IDC_STATIC_TEXTURE_PATH), pathtexture.c_str());
			}
				break;
			default:
				break;
			}
		}


		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_CONTEXTHELP) {
			return FALSE;
		}
	}
	return FALSE;
}


XVector4 ToolBarParticles::chooseColor(HWND editBox, COLORREF rgb) {
	
	ZeroMemory(&chooseStruct, sizeof(CHOOSECOLOR));
	chooseStruct.lStructSize = sizeof(CHOOSECOLOR);
	chooseStruct.hwndOwner = toolbarHWND;
	chooseStruct.lpCustColors = (LPDWORD)acrCustClr;
	chooseStruct.rgbResult = rgb;

	ChooseColor(&chooseStruct);

	TCHAR buff[20];
	sprintf(buff, "FF%X", (int)chooseStruct.rgbResult);
	SetWindowText(editBox, buff);

	XVector4 color = HEXCOLOR(chooseStruct.rgbResult);
	return color;
}

#include "ParticleEmiter.hpp"
void ToolBarParticles::update(Object*object) {
	if (object->typeObject != PARTICLES_OBJECT) return;
	ParticleEmiter*emiter = (ParticleEmiter*)object;
	if (emiter == nullptr) return;

	info = emiter->getInfo();
	setInfo();
}

void ToolBarParticles::setInfo() {
	std::string pathForTexture = _toolBarInterface->getTexture();
	SetWindowText(GetDlgItem(toolbarHWND, IDC_STATIC_TEXTURE_PATH) , pathForTexture.c_str());


	controllsInfo[IDC_EDIT_START_COLOR].vecto4val            = &info.startColor;
	controllsInfo[IDC_EDIT_START_COLOR_VARIANS].vecto4val    = &info.startColorVariance;
	controllsInfo[IDC_EDIT_FINISH_COLOR].vecto4val           = &info.finishColor;
	controllsInfo[IDC_EDIT_FINISH_COLOR_VARIANS].vecto4val   = &info.finishColorVariance;
	controllsInfo[IDC_START_COLOR_VARIANS_BUTTON].vecto4val  = &info.startColorVariance;
	controllsInfo[IDC_START_COLOR_BUTTON].vecto4val          = &info.startColor;
	controllsInfo[IDC_FINISH_COLOR_BUTTON].vecto4val         = &info.finishColor;
	controllsInfo[IDC_FINISH_COLOR_VARIANS_BUTTON].vecto4val = &info.finishColorVariance;

	controllsInfo[IDC_EDIT_PITCH_VARIANS].floatval           = &info.pitchVariance;
	controllsInfo[IDC_EDIT_PITCH].floatval                   = &info.pitch;
	controllsInfo[IDC_EDIT_AZIMUTH_VARIANS].floatval         = &info.azimuthVariance;
	controllsInfo[IDC_EDIT_AZIMUTH].floatval                 = &info.azimuth;

	controllsInfo[IDC_EDIT_SPEED].floatval                   = &info.speed;
	controllsInfo[IDC_EDIT_SPEED_VARIANS].floatval           = &info.speedVariance;

	controllsInfo[IDC_EDIT_FORCEX].floatval                  = &info.force.x;
	controllsInfo[IDC_EDIT_FORCEY].floatval                  = &info.force.y;
	controllsInfo[IDC_EDIT_FORCEZ].floatval                  = &info.force.z;
	controllsInfo[IDC_EDIT_FORCEX_VARIANS].floatval          = &info.forceVariance.x;
	controllsInfo[IDC_EDIT_FORCEY_VARIANS].floatval          = &info.forceVariance.y;
	controllsInfo[IDC_EDIT_FORCEZ_VARIANS].floatval          = &info.forceVariance.z;

	controllsInfo[IDC_EDIT_SIZE].floatval                    = &info.particleSize;
	controllsInfo[IDC_EDIT_SIZE_VARIANS].floatval            = &info.particleSizeVariance;
	controllsInfo[IDC_EDIT_PER_SECOND].floatval              = &info.particlesEmittedPerSecond;
	controllsInfo[IDC_EDIT_PER_SECOND_VARIANS].floatval      = &info.particleEmitVariance;
	controllsInfo[IDC_EDIT_LIFE_SPAN].floatval               = &info.particleLifespan;
	controllsInfo[IDC_EDIT_LIFE_SPAN_VARIANS].floatval       = &info.particleLifespanVariance;

	controllsInfo[IDC_COMBO_SORT_TYPE].uintval = (uint*)&info.typeSort;

	controllsInfo[IDC_BLEND_ENABLE].boolval                  = &info.blend;
	controllsInfo[IDC_COMBO_BLEND_IN].uintval                = &info.blendIn;
	controllsInfo[IDC_COMBO_BLEND_OUT].uintval               = &info.blendOut;
	
	for (std::map<uint, ControlsInfo>::iterator it = controllsInfo.begin(); it != controllsInfo.end(); ++it)
	{
		if (it->second.actionSet != nullptr)
		(this->*it->second.actionSet)(&it->second);
	}
}




void ToolBarParticles::destroy() {
	DestroyWindow(toolbarHWND);
}

void ToolBarParticles::create() {
	toolbarHWND = CreateDialog(_instance, MAKEINTRESOURCE(IDD_TOOLBAR_PARTICLES), NULL, toolbarCallbackR);

	SetWindowLongPtr(toolbarHWND, GWLP_USERDATA, (long)this);

	HWND comboBlendIn  = GetDlgItem(toolbarHWND, IDC_COMBO_BLEND_IN);
	HWND comboBlendOut = GetDlgItem(toolbarHWND, IDC_COMBO_BLEND_OUT);
	HWND comboTypeSort = GetDlgItem(toolbarHWND, IDC_COMBO_SORT_TYPE);

	
	
    for (uint i = 0; i < ParticleSortTypes.size();i++) {
		SendMessage(comboTypeSort, (UINT)CB_ADDSTRING, (WPARAM)i, (LPARAM)ParticleSortTypes[i].name);
    }

	for (uint i = 0; i < BlendInfoStaticData.size();i++) {
		SendMessage(comboBlendIn,  (UINT)CB_ADDSTRING, (WPARAM)i, (LPARAM)BlendInfoStaticData[i].name);
		SendMessage(comboBlendOut, (UINT)CB_ADDSTRING, (WPARAM)i, (LPARAM)BlendInfoStaticData[i].name);
	};


	std::vector<uint> comboUints;
	comboUints.push_back(IDC_COMBO_BLEND_IN);
	comboUints.push_back(IDC_COMBO_BLEND_OUT);
	comboUints.push_back(IDC_COMBO_SORT_TYPE);

	for (uint i = 0; i < comboUints.size();i++) {
		uint id = comboUints[i];
		controllsInfo[id].hwnd[0] = GetDlgItem(toolbarHWND, id);
		controllsInfo[id].idItem = id;
		controllsInfo[id].comboData = id == IDC_COMBO_SORT_TYPE ? &ParticleSortTypes : &BlendInfoStaticData;
		controllsInfo[id].action = actions[COMBO_UINT_SETTER];
		controllsInfo[id].actionSet = actions[COMBO_UINT_SET_SETTER];
	}


	controllsInfo[IDC_BLEND_ENABLE].hwnd[0]   = GetDlgItem(toolbarHWND, IDC_BLEND_ENABLE);
	controllsInfo[IDC_BLEND_ENABLE].idItem    = IDC_BLEND_ENABLE;
	controllsInfo[IDC_BLEND_ENABLE].action    = actions[CHECKBOX_SETTER];
	controllsInfo[IDC_BLEND_ENABLE].actionSet = actions[CHECKBOX_SET_SETTER];



	std::vector<uint> colorEdits;
	colorEdits.push_back(IDC_EDIT_START_COLOR_VARIANS);
	colorEdits.push_back(IDC_EDIT_FINISH_COLOR_VARIANS);
	colorEdits.push_back(IDC_EDIT_START_COLOR);
	colorEdits.push_back(IDC_EDIT_FINISH_COLOR);

	std::vector<uint> colorButtons;
	colorButtons.push_back(IDC_START_COLOR_VARIANS_BUTTON);
	colorButtons.push_back(IDC_FINISH_COLOR_VARIANS_BUTTON);
	colorButtons.push_back(IDC_START_COLOR_BUTTON);
	colorButtons.push_back(IDC_FINISH_COLOR_BUTTON);

	for (uint i = 0; i < colorButtons.size();i++) {
		uint id = colorButtons[i];
		uint id2 = colorEdits[i];
		controllsInfo[id].hwnd[1]   = GetDlgItem(toolbarHWND, id2);
		controllsInfo[id].hwnd[0]   = GetDlgItem(toolbarHWND, id);
		controllsInfo[id].action    = actions[BUTTON_COLOR_SETTER];
		controllsInfo[id].actionSet = nullptr;
	}

	

	for (uint i = 0; i < colorEdits.size(); i++) {
		uint id = colorEdits[i];
		controllsInfo[id].hwnd[0]   = GetDlgItem(toolbarHWND, id);
		controllsInfo[id].action    = actions[EDIT_COLOR_SETTER];
		controllsInfo[id].actionSet = actions[EDIT_HEX_VEC4_SET_SETTER];
	}

	std::vector<uint> floatsEdits;
	floatsEdits.push_back(IDC_EDIT_PITCH);
	floatsEdits.push_back(IDC_EDIT_AZIMUTH);
	floatsEdits.push_back(IDC_EDIT_PITCH_VARIANS);
	floatsEdits.push_back(IDC_EDIT_AZIMUTH_VARIANS);
	floatsEdits.push_back(IDC_EDIT_SPEED);
	floatsEdits.push_back(IDC_EDIT_SPEED_VARIANS);
	floatsEdits.push_back(IDC_EDIT_FORCEX);
	floatsEdits.push_back(IDC_EDIT_FORCEY);
	floatsEdits.push_back(IDC_EDIT_FORCEZ);
	floatsEdits.push_back(IDC_EDIT_FORCEX_VARIANS);
	floatsEdits.push_back(IDC_EDIT_FORCEY_VARIANS);
	floatsEdits.push_back(IDC_EDIT_FORCEZ_VARIANS);
	floatsEdits.push_back(IDC_EDIT_SIZE);
	floatsEdits.push_back(IDC_EDIT_SIZE_VARIANS);
	floatsEdits.push_back(IDC_EDIT_PER_SECOND);
	floatsEdits.push_back(IDC_EDIT_PER_SECOND_VARIANS);
	floatsEdits.push_back(IDC_EDIT_LIFE_SPAN);
	floatsEdits.push_back(IDC_EDIT_LIFE_SPAN_VARIANS);

	for (uint i = 0; i < floatsEdits.size();i++) {
		uint id = floatsEdits[i];
		controllsInfo[id].hwnd[0]   = GetDlgItem(toolbarHWND, id);
		controllsInfo[id].action    = actions[EDIT_FLOAT_SETTER];
		controllsInfo[id].actionSet = actions[EDIT_FLOAT_SET_SETTER];
	}

	ShowWindow(toolbarHWND, _nCMD);
	UpdateWindow(toolbarHWND);
}
