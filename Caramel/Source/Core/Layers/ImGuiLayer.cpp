#include "clpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>

#include <Core/Log.h>
#include <Core/Application.h>

// Assume we're only using GLFW for now
#include <Platform/Window/GLFW_Window.h>
#include <backends/imgui_impl_glfw.h>

// Todo: these could be moved
// DX12
#include <Platform/ImGui/ImGuiImpl_DX12.h>

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

    // GLFW is assumed
    GLFW_Window* window = (GLFW_Window*)Application::Get<Application>()->GetWindow();
    GLFWwindow* glfwWindow = (GLFWwindow*)window->GetNativeWindow();

    // TODO: CLEAN
    switch (window->GetRenderAPIType())
    {
    case WindowRenderAPI::DirectX12:
    {
        ImGui_ImplGlfw_InitForOther(glfwWindow, true);

        m_imGuiImpl = new ImGuiImpl_DX12();
        m_imGuiImpl->Init(window->GetRenderer());
        break;
    }
    case WindowRenderAPI::OpenGL:
    {
        //ImGui_ImplGlfw_InitForOpenGL(window, true);
        //ImGui_ImplOpenGL3_Init("#version 410");
        break;
    }

    default:
    {
        CL_CORE_NOT_IMPLEMENTED;
    }
    }

    // Setup Platform/Renderer bindings

}

void Caramel::ImGuiLayer::OnDetach()
{
    m_imGuiImpl->Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Caramel::ImGuiLayer::OnImGuiRender()
{
    static bool showWindow = true;
    ImGui::ShowDemoWindow(&showWindow);
}

void Caramel::ImGuiLayer::Begin()
{
    {
        m_imGuiImpl->NewFrame(); //ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    ImGui::NewFrame();
}

void Caramel::ImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();
    Application* app = Application::Get<Application>();
    io.DisplaySize = ImVec2(app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight());

    // Rendering
    ImGui::Render();
    m_imGuiImpl->Render(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
