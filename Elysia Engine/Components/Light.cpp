#include <stdafx.h>
#include "Light.h"

std::string Light::componentName = "Light";

Light::Light(Entity* entity) : ComponentBase(entity) {
	Log::Info("Catch");
}

void Light::Bind() {
	ComponentBase::initList[componentName] = &Parse;
}

void Light::Parse(Entity& entity, const rapidjson::Value& parm) {
	const XMFLOAT3 target{
		parm[0].GetFloat(),
		parm[1].GetFloat(),
		parm[2].GetFloat(),
	};

	entity.AppendComponent<Light>()
		.GetComponent<Light>()
		->SetTarget(target);
}

void Light::DrawEditorUI() {
	ImGui::Text(componentName.c_str());
	ImGui::SliderFloat("Power", &mLightPower, 0, 10);
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
	ImGui::ColorPicker3("##MyColor##6", &mLightColor.x, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
	ImGui::Spacing();
}


Light* Light::SetPosition(const XMFLOAT3& position) {
	mPosition = position;
	return this;
}

Light* Light::SetTarget(const XMFLOAT3& target) {
	mTarget = target;
	return this;
}

Entity& Light::ReturnParentEntity() {
	return *parentEntity;
}

XMFLOAT3 Light::GetPosition() {
	//XMVECTOR pos = parentEntity->translation.position;
	mPosition = parentEntity->translation.position;
	return mPosition;
}

XMFLOAT3 Light::GetDirection() {
	XMFLOAT3 direction{ mTarget.x - mPosition.x, mTarget.y - mPosition.y, mTarget.z - mPosition.z};
	return direction;
}

XMFLOAT3 Light::GetColor() {
	return mLightColor;
}

float Light::GetPower() {
	return mLightPower;
}