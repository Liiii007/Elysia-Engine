#pragma once
#include "../Components/Translation.h"
#include "../Components/MeshRenderer/Mesh.h"
#include "../Components/MeshRenderer/Material.h"
#include "../Interface/IComponent.h"
#include <any>

class Entity
{
public:
	Entity(std::string name);
	~Entity();

	bool loadMesh(std::string meshPath);


	void setLocation(float x, float y, float z) {
		translation.position = XMVECTOR{ x,y,z };
	}

	void setRotation(float x, float y, float z) {
		translation.rotation = XMVECTOR{ x,y,z };
	}

	void setScale(float scale) {
		translation.scale = XMVECTOR{ scale,scale,scale };
	}

	void setScale(float x, float y, float z) {
		translation.scale = XMVECTOR{ x,y,z };
	}

	template<typename T>
	void appendComponent() {
		auto component = std::make_unique<IComponent>();
		component->make<T>();
		components.insert(std::make_pair(T::componentName, std::move(component)));
	}

	template<typename T>
	std::shared_ptr<T> getComponent() {
		return components[T::componentName]->get<T>();
	}

	Translation translation;

	std::unordered_map<std::string, std::unique_ptr<IComponent>> components;
	
	std::string name;

};

