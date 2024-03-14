#include "clpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>

#include <Core/Log.h>
#include <Core/Application.h>
#include <Core/Window.h>

#include <Core/RenderAPI/RenderAPI.h>
#include <Core/ImGui/ImGuiWindowImpl.h>
#include <Core/ImGui/ImGuiRendererImpl.h>

Caramel::ImGuiLayer::ImGuiLayer() : Layer("ImGui")
{
}

Caramel::ImGuiLayer::~ImGuiLayer()
{
}

void Caramel::ImGuiLayer::OnAttach()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |=ImGuiConfigFlages_ViewportsNoMerge;
     
    // Setup Dear Imgui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    m_imGuiWindowImpl = ImGuiWindowImpl::Create();
    m_imGuiRendererImpl = ImGuiRendererImpl::Create();

    CL_CORE_ASSERT(m_imGuiWindowImpl && m_imGuiRendererImpl);

    Window* targetWindow = Application::Get()->GetWindow();
    m_imGuiWindowImpl->Init(targetWindow);
    m_imGuiRendererImpl->Init(targetWindow->GetRenderer());
}

void Caramel::ImGuiLayer::OnDetach()
{
    m_imGuiRendererImpl->Shutdown();
    m_imGuiWindowImpl->Shutdown();
    ImGui::DestroyContext();
}

void Caramel::ImGuiLayer::OnImGuiRender()
{
    static bool showWindow = true;
    ImGui::ShowDemoWindow(&showWindow);
}

void Caramel::ImGuiLayer::Begin()
{
    m_imGuiRendererImpl->NewFrame();
    m_imGuiWindowImpl->NewFrame();
    ImGui::NewFrame();
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void Caramel::ImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();
    Application* app = Application::Get();
    io.DisplaySize = ImVec2(app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight());

    // Rendering
    ImGui::Render();
    m_imGuiRendererImpl->Render(ImGui::GetDrawData());
    m_imGuiWindowImpl->Render();
}
