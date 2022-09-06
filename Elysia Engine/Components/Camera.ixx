#include <stdafx.h>
export module Camera;

import ECS;
using namespace DirectX;
export class SphereCamera {
public:
	XMFLOAT3 pos{ 4,1,0 };
	std::weak_ptr<Entity> targetEntity;
	XMFLOAT3 target;
	XMFLOAT3 up;

	XMMATRIX getViewMatrix() {

		// Build the view matrix.
		XMVECTOR posV = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);

		targetEntity = Entity::GetEntity("Sphere");

		XMVECTOR targetV{};
		if (!targetEntity.expired()) {
			targetV = XMVectorSet(
				targetEntity.lock()->GetLocation().x,
				targetEntity.lock()->GetLocation().y,
				targetEntity.lock()->GetLocation().z,
				1
			);
		}
		else {
			targetV = XMVectorSet(pos.x - 1, pos.y, pos.z, 1.0f);
		}

		XMVECTOR upV = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return XMMatrixLookAtLH(posV, targetV, upV);

	}

	XMFLOAT3 GetPosition() {
		return pos;
	}

	XMFLOAT3 GetDirection() {
		XMFLOAT3 direction;
		direction.x = target.x - pos.x;
		direction.y = target.y - pos.y;
		direction.z = target.z - pos.z;
		return direction;
	}

};