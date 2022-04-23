#pragma once

#include "clpch.h"

#include "Render/Window.h"
#include "Core/Input.h"
#include "Core/Cursor.h"

namespace Caramel
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

	protected:
		friend class Caramel;

		// Client implementation
		virtual bool OnCreate() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void ImDraw() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;
	};
}