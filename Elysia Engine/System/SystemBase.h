#pragma once
#include <vector>
#include "../Tools/Logger.h"

class Entity;

class SystemBase {
public:
	SystemBase() {
		systems.push_back(this);
	}

	~SystemBase() {
		for (auto it = systems.begin(); it != systems.end(); it++) {
			if (*it == this) {
				systems.erase(it);
				return;
			}
		}

		Log::Error("Unable to clear up deleted System");
	}

	virtual void Tick() {
		Log::Warning("Default Tick");
	}

	static std::vector<SystemBase*> systems;

	static void SystemTick() {
		for (auto& system : systems) {
			system->Tick();
		}
	}
};

