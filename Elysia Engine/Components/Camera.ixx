#include <stdafx.h>
import ECS;

//Dependency
import Translation;

export module Camera;
namespace Component {
	export class Camera {
	public:
		DirectX::XMFLOAT3 pos{ 4,1,0 };
		std::weak_ptr<Entity> targetEntity;
		DirectX::XMFLOAT3 target;
		DirectX::XMFLOAT3 up;

		DirectX::XMMATRIX getViewMatrix() {

			// Build the view matrix.
			DirectX::XMVECTOR posV = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);

			targetEntity = Entity::GetEntity("Sphere");

			DirectX::XMVECTOR targetV{};
			if (!targetEntity.expired()) {
				targetV = DirectX::XMVectorSet(
					targetEntity.lock()->GetComponent<Translation>()->GetLocation().x,
					targetEntity.lock()->GetComponent<Translation>()->GetLocation().y,
					targetEntity.lock()->GetComponent<Translation>()->GetLocation().z,
					1
				);
			}
			else {
				targetV = DirectX::XMVectorSet(pos.x - 1, pos.y, pos.z, 1.0f);
			}

			DirectX::XMVECTOR upV = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			return DirectX::XMMatrixLookAtLH(posV, targetV, upV);

		}

		DirectX::XMFLOAT3 GetPosition() {
			return pos;
		}

		DirectX::XMFLOAT3 GetDirection() {
			DirectX::XMFLOAT3 direction;
			direction.x = target.x - pos.x;
			direction.y = target.y - pos.y;
			direction.z = target.z - pos.z;
			return direction;
		}

	};
}
