#pragma once
#include <string>
#include <memory>
#include <any>

class IComponent {

public:
	std::any component;
	bool enabled;
	std::string name;

	IComponent(); 

	template<typename T>
	void make() {
		std::shared_ptr<T> raw = std::make_shared<T>();
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
};