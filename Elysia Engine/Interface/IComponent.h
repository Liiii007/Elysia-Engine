#pragma once
#include <string>
#include <memory>
#include <any>
#include <Tools/Logger.h>

class Entity;

class IComponent {

public:
	std::any component;
	bool enabled{ true };
	std::string name;
	Entity* parentEntity;

	IComponent(); 

	template<typename T>
	void make(Entity* parentEntity) {
		this->parentEntity = parentEntity;
		std::shared_ptr<T> raw = std::make_shared<T>(parentEntity);
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

#ifndef FullComponentHeader
#define FullComponentHeader \
    #include <Component/Light.h> \

#endif // !FullComponentHeader

#ifndef FullComponentClass
#define FullComponentClass \
	class Light;\

#endif
