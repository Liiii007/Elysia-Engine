#pragma once
#include "SystemBase.h"
#include "../Components/Light.h"

#include "../World/Entity.h"

class LightMoveSystem : public SystemBase {
public:
	virtual void Tick() {
		auto light = Entity::entities["eLight"];
		auto pos = light->translation.position;
		pos.x += moveSpeed;
		if (pos.x > 10 || pos.x < -10) {
			moveSpeed = -moveSpeed;
		}
		light->SetLocation(pos);
		return;
	}

	float moveSpeed = 0.1;
};

