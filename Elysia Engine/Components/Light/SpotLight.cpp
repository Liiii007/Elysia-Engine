//Note:For dependent component, include X.h here, define "class X;" in T.h
#include "SpotLight.h"
#include <World/Entity.h>
std::string SpotLight::componentName = "SpotLight";
SpotLight::SpotLight(Entity* entity) : ComponentBase(entity) {}

Entity& SpotLight::ReturnParentEntity() {
	return *parentEntity;//Define in ComponentBase.h
}
