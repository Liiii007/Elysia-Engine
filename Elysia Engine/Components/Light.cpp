#include "Light.h"
#include "World/Entity.h"

std::string Light::componentName = "Light";

Light::Light(Entity* entity) : ComponentBase(entity) {

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