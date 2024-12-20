#pragma once

#include <map>
#include <string>

#include "colorstream.hpp"

namespace utils {
	enum class LogLevel {
		INFO,
		WARNING,
		CRITICAL,
		DEBUG
	};

	namespace logging {
		void Log(const std::string& message, LogLevel level = LogLevel::INFO);
		void Info(const std::string& message);
		void Warning(const std::string& message);
		void Critical(const std::string& message);
		void Debug(const std::string& message);
		void ThrowError(const std::string& message);
	}

	namespace threads {
		void wait(int ms);
	}
}
