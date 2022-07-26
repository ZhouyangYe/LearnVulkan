#pragma once

namespace LearnVulkan {
	struct Error {
		std::string desc;

		Error(std::string desc) : desc(desc) {}
	};
}
