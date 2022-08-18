#pragma once
#include <string>
#include <memory>
#include <any>

class IComponent
{
public:
	IComponent(); 

	template<typename T>
	void make() {
		component = std::make_any<T>();
	}

	template<typename T>
	T* get() {
		T* returnComponent{ nullptr };
		try {
			returnComponent = &std::any_cast<T>(component);
		}
		catch (const std::bad_any_cast& e) {
			Log::Error("Not contain this component");
			return nullptr;
		}

		return returnComponent;
	}

	//std::unique_ptr<std::any> component{ nullptr };
	std::any component;
	std::string name;
};

