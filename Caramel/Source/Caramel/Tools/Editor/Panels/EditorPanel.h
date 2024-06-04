#pragma once


namespace Caramel
{
	class EditorPanel
	{
	public:
		EditorPanel(const std::string& name) : m_isOpen(false), m_name(name) {}

		void Render();
		void RenderListOption();
		inline bool IsOpen() { return m_isOpen; }

	protected:
		virtual void InternalRender() = 0;

	private:
		bool m_isOpen;
		std::string m_name;
	};
}