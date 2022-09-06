#include <stdafx.h>
export module ECS;

#include "Components/Translation.h"
#include <typeinfo>

export class IComponent;

export class Entity {
public:

	Entity(std::string name) :name(name) {
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
		std::string tn = typeid(T).name();
		components[tn] = std::make_shared<IComponent>();
		components[tn]->make<T>(this);
		return *this;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent() {
		std::string tn = typeid(T).name();
		std::shared_ptr<T> r = components[tn]->get<T>();
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
		std::string tn = typeid(T).name();
		return components.contains(tn);
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

export std::unordered_map<std::string, std::shared_ptr<Entity>> Entity::instances;


export class ComponentBase {
public:
	ComponentBase(Entity* entity) {
		this->parentEntity = entity;
	}

	virtual void DrawEditorUI() {
		return;
		//Log::Error("Base");
	}

	Entity* parentEntity;
	static std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> initList;
};

std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> ComponentBase::initList{};

//Component
export class IComponent {

public:
	std::any component;
	std::shared_ptr<ComponentBase> basePtr;
	bool enabled{ true };
	std::string name;
	Entity* parentEntity;

	IComponent() {

	}

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



