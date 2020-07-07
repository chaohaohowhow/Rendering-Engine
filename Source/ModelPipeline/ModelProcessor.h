#pragma once

#include "Model.h"
#include <memory>

struct aiNode;

namespace Library
{
	struct ModelData;
	class SceneNode;
}

namespace ModelPipeline
{
	class ModelProcessor final
	{
	public:
		ModelProcessor() = delete;

		static Library::Model LoadModel(const std::string& filename, bool flipUVs = false);
	};
}
