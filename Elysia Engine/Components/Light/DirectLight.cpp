//Note:For dependent component, include X.h here, define "class X;" in T.h
#include <stdafx.h>
#include "DirectLight.h"
#include <World/Entity.h>
std::string DirectLight::componentName = "DirectLight";
DirectLight::DirectLight(Entity* entity) : ComponentBase(entity) {}

Entity& DirectLight::ReturnParentEntity() {
	return *parentEntity;//Define in ComponentBase.h
}
