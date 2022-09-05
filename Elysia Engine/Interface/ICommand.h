#pragma once
#include <vector>
#include <Tools/Action.h>
#include <Tools/Logger.h>
#include <Tools/Singleton.h>
#include <unordered_map>

class InputSystem;

template<typename T>
class ICommand {
public:

	int keycode{};
	T value{};
	std::vector<int> a;
	
	std::unordered_map<std::string, Action<T>> OnValueChangeEvent{};
	std::unordered_map<std::string, Action<T>> OnValueActiveEvent{};

	ICommand(int keycode) : keycode(keycode) {
		
	}
	
	//Add event to listened list
	void Listen(std::string name, Action<T> event) {
		OnValueChangeEvent[name] = event;
	}
	void RemoveListen(std::string name) {
		if (OnValueChangeEvent.contains(name)) {
			OnValueChangeEvent.erase(name);
		}
	}

	void ListenActive(Action<T> event) {
		OnValueActiveEvent[name] = event;
	}

	//Invoke Once
	void OnValueChange(T value) {
		if (value != this->value) {
			for (auto event : OnValueChangeEvent) {
				event.second(value);
			}
		}
	}

	//Invoke Continue
	void OnValueUpdate(T value) {
		OnValueChange(value);
		this->value = value;
	}
};

