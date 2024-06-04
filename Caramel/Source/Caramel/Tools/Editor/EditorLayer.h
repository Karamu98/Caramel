#include "Caramel/Core/Layer.h"

namespace Caramel
{
	struct Viewport
	{
		const char* Name;
		uint32_t TargetImage;

		bool IsFocused;
		bool IsHovered;
		glm::vec2 ViewportSize;
	};

	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void PrintECS();
		void RenderViewport(Viewport& target);

	private:
		Viewport m_editorView{ "Editor", 0 };
		Viewport m_gameView{ "Game", 1 };
	};
}