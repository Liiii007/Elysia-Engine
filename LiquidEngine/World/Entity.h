#pragma once
#include "../Components/Translation.h"
#include "../Components/MeshRenderer/Mesh.h"
#include "../Components/MeshRenderer/Material.h"
#include "../Interface/IComponent.h"
#include <any>

class Entity
{
public:
	Entity()  = default;
	~Entity() = default;


	Entity& SetName(std::string name) {
		this->name = name;
		return *this;
	}

	Entity& SetLocation(float x, float y, float z) {
		translation.position = XMVECTOR{ x,y,z };
		return *this;
	}

	Entity& SetRotation(float x, float y, float z) {
		translation.rotation = XMVECTOR{ x,y,z };
		return *this;
	}

	Entity& setScale(float scale) {
		translation.scale = XMVECTOR{ scale,scale,scale };
		return *this;
	}

	Entity& setScale(float x, float y, float z) {
		translation.scale = XMVECTOR{ x,y,z };
		return *this;
	}

	template<typename T>
	Entity& AppendComponent() {
		auto component = std::make_unique<IComponent>();
		component->make<T>(this);
		components.insert(std::make_pair(T::componentName, std::move(component)));
		return *this;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent() {
		std::shared_ptr<T> r = components[T::componentName]->get<T>();
		if (r != nullptr) {
			return r;
		}
		else {
			Log::Error("Unable to get this component");
			throw std::exception("Unable to get this component");
			return nullptr;
		}
		
	}

	Translation translation;
	std::unordered_map<std::string, std::unique_ptr<IComponent> > components;
	//FIXED:±£´æËùÓÐentity
	std::string name;

};

