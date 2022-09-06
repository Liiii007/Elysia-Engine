#include <stdafx.h>
export module Log;

namespace Log {

	enum LogLevel {
		LOGERROR, LOGWARNING, LOGINFO,
	};
	std::string logPath = "C:\\Logs\\logs.txt";
	export LogLevel filter = LogLevel::LOGINFO;
	bool writeBit = false;
	bool consoleBit = true;
	std::vector<std::string> logs;

	void writeToDisk(std::string message) {
		if (writeBit) {
			return;
		}
	}

	void writeToConsole(std::string message) {
		if (consoleBit) {

		}
	}

	void saveToMemory(std::string message) {
		logs.emplace_back(message);
	}


	class LogEvent {

	public:
		LogLevel eventLevel;
		std::string _time;
		std::string _message;

		LogEvent(std::string message, LogLevel level) : _message(message), eventLevel(level) {
			//FIXED:Add time code
			_time = "00";

			std::string NEWmessage = toOneLine();

			writeToConsole(NEWmessage);
			writeToDisk(NEWmessage);
			saveToMemory(NEWmessage);
		};

		std::string toOneLine() {
			std::string message = _message;
			message.append("---");
			message.append(_time);
			return message;
		}

	};

	export std::vector<std::string>* GetLogs() {
		return &logs;
	}

	export void clearLogs() {
		logs.clear();
	}

	export void Error(std::string message) {
		LogEvent error{ message, LogLevel::LOGERROR };
	}
	bool ShowError{ true };

	export void Warning(std::string message) {
		LogEvent warning{ message, LogLevel::LOGWARNING };
	}
	bool ShowWarning{ true };

	export void Info(std::string message) {
		LogEvent info{ message, LogLevel::LOGINFO };
	}
	bool ShowInfo{ true };
}