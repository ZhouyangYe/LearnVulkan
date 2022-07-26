#include "Logger.h"

namespace LearnVulkan {
	std::shared_ptr<spdlog::logger> Logger::console = spdlog::stdout_color_mt("console");

	void Logger::Init()
	{
#ifdef DEBUG
		spdlog::set_level(spdlog::level::info);
#else
		spdlog::set_level(spdlog::level::error);
#endif
	}
}
