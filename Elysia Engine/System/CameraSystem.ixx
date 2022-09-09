#include <stdafx.h>
import ECS;
import Camera;
import Log;
import Profiler;
export module CameraSystem;

namespace CameraSystem {
	export bool Tick();

	export class Meta final {
	public:
		std::string profilerEvent{ "CameraSystem" };
		Meta() {
		}
		~Meta() {

		}

		void AutoInit() {

		}

	};

	export Meta meta;

	export std::weak_ptr<Component::Camera> activeCamera;
	int maxPriority{ INT_MIN };
	export bool Tick() {
		Profiler::Auto _(meta.profilerEvent);

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

namespace System {
	export class csys : public SystemBase {
	public:
		csys():SystemBase(typeid(this).name()) {
			
		}

		virtual void Start() {

		};

		virtual void Update() {

		};

		~csys() = default;
	};

	csys _csys;
}