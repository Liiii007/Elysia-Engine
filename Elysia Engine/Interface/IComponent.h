#pragma once
#include <string>
#include <memory>
#include <any>
#include <Tools/Logger.h>
#include <Interface/ComponentBase.h>

class Entity;

class IComponent {

public:
	std::any component;
	std::shared_ptr<ComponentBase> basePtr;
	bool enabled{ true };
	std::string name;
	Entity* parentEntity;

	IComponent(); 

	template<typename T>
	void make(Entity* parentEntity) {
		this->parentEntity = parentEntity;
		std::shared_ptr<T> raw = std::make_shared<T>(parentEntity);
		std::shared_ptr<ComponentBase> base{ raw.get() };
		basePtr = std::move(base);
		component = std::move(raw);
	}

	template<typename T>
	std::shared_ptr<T> get() {
		std::shared_ptr<T> returnComponent{ nullptr };
		try {
			returnComponent = std::any_cast<std::shared_ptr<T>>(component);
		}
		catch (const std::bad_any_cast& e) {
			Log::Error("Not contain this component");
			return nullptr;
		}

		return returnComponent;
	}

	std::weak_ptr<ComponentBase> getBase() {
		return basePtr;
	}

	template<typename T>
	bool is() {
		try {
			std::any_cast<std::shared_ptr<T>>(component);
			return true;
		}
		catch (const std::bad_any_cast& e) {
			return false;
		}
	}
};
