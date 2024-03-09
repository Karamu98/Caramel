#pragma once

namespace Caramel
{
	class ImGuiWindowImpl
	{
	public:
		virtual void Init(class Window* window) = 0;
		virtual void Shutdown() = 0;
		virtual void NewFrame() = 0;
		virtual void Render() = 0;

		static ImGuiWindowImpl* Create();
	};
}


