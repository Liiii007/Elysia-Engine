#include <stdafx.h>
export module Profiler;
import Timer;

export std::string FloatToString(float Num) {
	std::ostringstream oss;
	oss << Num;
	return oss.str();
}

namespace Profiler {
	export std::map<std::string, float> tickTime;
	export void RecordStart(const std::string& name) {
		Timer::startRecord(name);
	}

	export float RecordStop(const std::string& name) {
		float singleTickTime = Timer::stopRecord(name);
		if (singleTickTime > 0) {
			tickTime[name] = singleTickTime;
		}
		return singleTickTime;
	}

	export void NewTick() {
		//tickTime.clear();
	}

	export class Auto final {
	public:
		Auto(const Auto&) = delete;

		Auto(const std::string& name) : name(name) {
			RecordStart(name);
		}
		~Auto() {
			RecordStop(name);
		}
	private:
		const std::string& name;
	};
}