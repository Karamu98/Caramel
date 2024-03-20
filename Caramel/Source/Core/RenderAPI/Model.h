#pragma once

namespace Caramel
{
	struct ModelParams
	{
		std::string& Path;
	};

	class Model
	{
	public:
		virtual ~Model() {} ;

		static std::shared_ptr<Model> Create(const ModelParams& params);
	};
}


