#pragma once
#include <functional>
#include <string>
#include <vector>

using NullFunc = std::pair<std::string, std::function<void()>>;
class Delegate {
private:
	std::vector<NullFunc> events;
public:
	void appendEvent(NullFunc event) {
		events.push_back(event);
	}

	void removeEvent(NullFunc event) {
		for (const auto& _event : events) {
			if (_event.first == event.first) {
				//FIXED:É¾³ıÊÂ¼şËã·¨
				//events.erase(_event);
			}
		}
		
	}

	void Invoke() {
		for (auto& event : events) {
			event;
		}
	}

	
};

