#pragma once

#include "Core/Asset.h"

class Shader;

namespace Caramel
{
	class MaterialResource : public Asset
	{
	public:
		std::shared_ptr<Shader> Program;

	};
}