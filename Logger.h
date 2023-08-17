// Logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Logger {
	inline std::shared_ptr<spdlog::logger> console = spdlog::stdout_color_mt("console");
}

#endif // LOGGER_H
