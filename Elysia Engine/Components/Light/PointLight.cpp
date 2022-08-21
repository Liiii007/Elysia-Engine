//Note:For dependent component, include X.h here, define "class X;" in T.h
#include "PointLight.h"
#include <World/Entity.h>
std::string PointLight::componentName = "PointLight";
PointLight::PointLight(Entity* entity) : ComponentBase(entity) {}

Entity& PointLight::ReturnParentEntity() {
	return *parentEntity;//Define in ComponentBase.h
}
