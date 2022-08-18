#pragma once
#include "../Components/Translation.h"
#include "../Components/MeshRenderer/Mesh.h"
#include "../Components/MeshRenderer/Material.h"
#include "../Interface/IComponent.h"
#include <any>

class Entity
{
public:
	Entity() = default;
	~Entity() = default;

	bool loadMesh(std::string meshPath);

	Entity& setName(std::string name) {
		this->name = name;
		return *this;
	}

	Entity& setLocation(float x, float y, float z) {
		translation.position = XMVECTOR{ x,y,z };
		return *this;
	}

	Entity& setRotation(float x, float y, float z) {
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
	Entity& appendComponent() {
		auto component = std::make_unique<IComponent>();
		component->make<T>(this);
		components.insert(std::make_pair(T::componentName, std::move(component)));
		return *this;
	}

	template<typename T>
	std::shared_ptr<T> getComponent() {
		return components[T::componentName]->get<T>();
	}

	Translation translation;
	std::unordered_map<std::string, std::unique_ptr<IComponent> > components;
	std::string name;

};

