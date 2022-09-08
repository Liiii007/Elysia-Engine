#include <stdafx.h>
import ECS;
import Camera;
import Log;
import Profiler;
export module CameraSystem;

namespace CameraSystem {
	std::string profilerEvent{ "CameraSystem" };

	export std::weak_ptr<Component::Camera> activeCamera;
	int maxPriority{ INT_MIN };
	export bool Tick() {
		Profiler::Auto _(profilerEvent);

		maxPriority = INT_MIN;

		for (auto iter = Entity::begin(); iter != Entity::end(); iter++) {
			std::shared_ptr<Entity> entity = iter->second;
			if (entity->HasComponent<Component::Camera>()) {
				std::shared_ptr<Component::Camera> cam = entity->GetComponent<Component::Camera>();
				if (cam->priority >= maxPriority) {
					maxPriority = cam->priority;
					activeCamera = entity->GetComponent<Component::Camera>();
					activeCamera.lock()->isActive = true;
				}
				else {
					cam->isActive = false;
				}
			}
		}

		return true;
	}


}