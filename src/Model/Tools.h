#pragma once
#include "Core/Common.h"
#include "Events/Error.h"

namespace LearnVulkan {
	namespace ModelTools {
		struct Mesh {
			glm::vec3 position;
			glm::vec3 normal;
		};

		std::vector<Mesh> loadModel(std::string filename);
	}
}
