#pragma once
#include "Components/Translation.h"
#include <Interface/IComponent.h>
#include <any>
#include <unordered_map>

class Entity {
public:

	Entity(std::string name):name(name) {
		return;
	}
	~Entity() {
		return;
	}


	//Entity Manage Relate
	static Entity& New(std::string name) {
		instances[name] = std::make_shared<Entity>(name);
		return *instances[name];
	}
	static void Erase(std::string name) {
		instances.erase(name);
	}
	static auto begin() {
		return instances.begin();
	}
	static auto end() {
		return instances.end();
	}
	static std::shared_ptr<Entity> GetEntity(std::string name) {
		if (instances.contains(name)) {
			return instances[name];
		}
		else {
			return nullptr;
		}
	}

	//Translation Relate
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

	XMFLOAT3 GetLocation() {
		return translation.position;
	}

	//Component Relate
	template<typename T>
	Entity& AppendComponent() {
		components[T::componentName] = std::make_shared<IComponent>();
		components[T::componentName]->make<T>(this);
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

	template<typename T>
	void RemoveComponent() {
		if (components.contains(T::componentName)) {
			components.erase(T::componentName);
		}
	}

	Translation translation;
	std::vector<std::string> tags;
	std::unordered_map<std::string, std::shared_ptr<IComponent>> components;
	std::string name;

private:

	static std::unordered_map<std::string, std::shared_ptr<Entity>> instances;
};

