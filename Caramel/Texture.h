#pragma once

namespace Caramel
{
	struct TextureParams
	{
		const std::string& Path;
		// Filtering mode
		// Layout
	};

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;


		static std::shared_ptr<Texture> Create();
	};
}


