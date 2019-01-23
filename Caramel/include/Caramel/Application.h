#pragma once

#include "Core.h"

namespace Caramel
{
	class CARAMEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


