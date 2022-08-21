#include "Light.h"
#include "World/Entity.h"

std::string Light::componentName = "Light";

Light::Light(Entity* entity) : ComponentBase(entity) {

}

Light* Light::SetPosition(XMFLOAT3 position) {
	mPosition = position;
	return this;
}

Light* Light::SetTarget(XMFLOAT3 target) {
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