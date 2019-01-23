#pragma once

#include "Core.h"

namespace Caramel
{
	class CARAMEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
	};

	Application* CreateApplication();
}


