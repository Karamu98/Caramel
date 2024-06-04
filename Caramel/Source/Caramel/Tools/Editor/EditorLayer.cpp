#include "clpch.h"
#include "EditorLayer.h"
#include "Caramel/Events/ApplicationEvent.h"
#include "Caramel/Events/KeyEvents.h"
#include "Caramel/Core/Input.h"
#include "Caramel/ECS/SceneManager.h"
#include "Caramel/ECS/Scene.h"
#include "Caramel/ECS/Entity.h"
#include "Caramel/ECS/Components/COM_UUID.h"

namespace Caramel
{
	void EditorLayer::OnAttach()
	{

	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_AutoHideTabBar);
		RenderViewport(m_editorView);
		RenderViewport(m_gameView);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& closeEvent)
			{
				return false;
			});
		dispatcher.Dispatch<KeyPressEvent>([&](KeyPressEvent& keyEvent)
			{
				if (keyEvent.GetKeyCode() == (int)KeyCode::F)
				{
					PrintECS();
					return true;
				}
				else if (keyEvent.GetKeyCode() == (int)KeyCode::N)
				{
					for (const auto scene : *SceneManager::Get())
					{
						scene.second->CreateEntity();
					}
				}
			});
	}

	void EditorLayer::PrintECS()
	{
		CL_CORE_INFO("Printing...");
		for (const auto scene : *SceneManager::Get())
		{
			auto view = scene.second->GetAllEntitiesWith<COM_UUID>();
			for (const auto entity : view)
			{
				COM_UUID& uuid = view.get<COM_UUID>(entity);

				CL_INFO("Entity: {}", (uint64_t)uuid.Identifier);
				
			}
		}
	}

	void EditorLayer::RenderViewport(Viewport& target)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(target.Name);

		target.IsFocused = ImGui::IsWindowFocused();
		target.IsHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		target.ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// TODO: replace 0 with actual texture ID that scene has rendered to
		ImGui::Image(reinterpret_cast<void*>(target.TargetImage), ImVec2{ target.ViewportSize.x, target.ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
}