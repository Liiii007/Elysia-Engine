#pragma once

#include <Tools/Singleton.h>

namespace Log {

	enum LogLevel {
		LOGERROR, LOGWARNING, LOGINFO,
	};

	class LogConfig {
	public:
		std::string logPath = "C:\\Logs\\logs.txt";
		LogLevel filter = LogLevel::LOGINFO;
		bool writeBit = false;
		bool consoleBit = true;
		std::vector<std::string> logs;
	};


	static void writeToDisk(std::string message) {
		if (Singleton<LogConfig>::Get()->writeBit) {
			return;
		}
	}

	static void writeToConsole(std::string message) {
		if (Singleton<LogConfig>::Get()->consoleBit) {

		}
	}

	static void saveToMemory(std::string message) {
		Singleton<LogConfig>::Get()->logs.emplace_back(message);
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

	static std::vector<std::string>* GetLogs() {
		return &(Singleton<LogConfig>::Get()->logs);
	}

	static void clearLogs() {
		Singleton<LogConfig>::Get()->logs.clear();
	}

	static void Error(std::string message) {
		LogEvent error{ message, LogLevel::LOGERROR };
	}
	static bool ShowError{ true };

	static void Warning(std::string message) {
		LogEvent warning{ message, LogLevel::LOGWARNING };
	}
	static bool ShowWarning{ true };

	static void Info(std::string message) {
		LogEvent info{ message, LogLevel::LOGINFO };
	}
	static bool ShowInfo{ true };
}