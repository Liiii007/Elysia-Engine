#pragma once
#include "Components/Translation.h"
#include "Components/MeshRenderer/Mesh.h"
#include "Components/MeshRenderer/Material.h"
#include <Interface/IComponent.h>
#include <any>
#include <unordered_map>

class Entity
{
public:
	Entity(std::string name) {
		this->name = name;
		entities[name] = this;
	}
	~Entity() = default;

	static Entity& New(std::string name) {
		Entity* newEntity = new Entity(name);
		return *newEntity;
	}

	Entity& SetLocation(float x, float y, float z) {
		translation.position = XMFLOAT3{ x,y,z };
		return *this;
	}

	Entity& SetLocation(const XMFLOAT3& position) {
		translation.position = position;
		return *this;
	}

	Entity& SetRotation(float x, float y, float z) {
		translation.rotation = XMFLOAT3{ x,y,z };
		return *this;
	}

	Entity& SetRotation(const XMFLOAT3& rotation) {
		translation.rotation = rotation;
		return *this;
	}

	Entity& SetUniformScale(float scale) {
		translation.scale = XMFLOAT3{ scale,scale,scale };
		return *this;
	}

	Entity& SetScale(float x, float y, float z) {
		translation.scale = XMFLOAT3{ x,y,z };
		return *this;
	}

	Entity& SetScale(const XMFLOAT3& scale) {
		translation.scale = scale;
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

	template<typename T>
	bool HasComponent() {
		return components[T::componentName]->is<T>();
	}

	Translation translation;
	std::vector<std::string> tags;
	std::unordered_map<std::string, std::shared_ptr<IComponent>> components;
	std::string name;

	static std::unordered_map<std::string, Entity*> entities;
};

