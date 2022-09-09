#include <stdafx.h>

import Log;

export module ECS;

export class IComponent;
export class MetaData;
export class SystemBase;

namespace System {
	export std::unordered_map<std::string, SystemBase*> systemList{};
}

//Entity Class
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
	static std::unordered_map<std::string, std::shared_ptr<Entity>>::iterator begin() {
		return instances.begin();
	}
	static std::unordered_map<std::string, std::shared_ptr<Entity>>::iterator end() {
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

	//Component Relate
	template<typename T>
	Entity& AppendComponent() {
		auto tn = typeid(T).name();
		components[tn] = std::make_shared<IComponent>();
		components[tn]->make<T>(this);
		return *this;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent() {
		return components[typeid(T).name()]->get<T>();
	}

	template<typename T>
	bool HasComponent() {
		return components.contains(typeid(T).name());
	}

	template<typename T>
	bool RemoveComponent() {
		auto it = components.find(typeid(T).name());
		if (it != components.end()) { 
			components.erase(it);
			return true;
		}
		else {
			return false;
		}
	}

	std::unordered_map<std::string, std::shared_ptr<IComponent>> components;
	std::string name;

private:

	static std::unordered_map<std::string, std::shared_ptr<Entity>> instances;
};

//Component Base Class
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

//Component Container
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

export class RAIR {
	RAIR(std::function<void(void)> start) {
		start();
	}
};

//System Base Class
export class SystemBase {
public:
	const std::string name;
	bool enabled{ true };
	virtual void Start()  {};
	virtual void Update() {};

	SystemBase(const char* name) : name(name) {
		System::systemList[name] = this;
	}

	~SystemBase() {
		System::systemList.erase(name);
	}
};

export class MetaData {
public:
	std::string name;
	MetaData() {};
	~MetaData() {};

	void Start() {};
	void Tick() {};
};





std::unordered_map<std::string, std::shared_ptr<Entity>> Entity::instances;
std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> ComponentBase::initList{};