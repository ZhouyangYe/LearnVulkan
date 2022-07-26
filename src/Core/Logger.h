#pragma once
#define SPDLOG_FMT_EXTERNAL
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace LearnVulkan {
	class Logger {
	public:
		static std::shared_ptr<spdlog::logger> console;

		static void Init();
	};
}
