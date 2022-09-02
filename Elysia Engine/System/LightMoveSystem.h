#pragma once
#include "SystemBase.h"

#include <World/Entity.h>
#include <Components/FullComponentHeader.h>


class LightMoveSystem : public SystemBase {
public:
	virtual void Tick() {
		auto light = Entity::GetEntity("eLight");
		auto pos = light->translation.position;
		pos.x += moveSpeed;
		if (pos.x > 10 || pos.x < -10) {
			moveSpeed = -moveSpeed;
		}
		//light->SetLocation(pos);
		return;
	}

	float moveSpeed = 0.1f;
};

