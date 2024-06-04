#pragma once
#include "Window.h"
#include "Caramel/Core/LayerStack.h"
#include "Caramel/Tools/Editor/EditorLayer.h"
#include "Caramel/ECS/SceneManager.h"

namespace Caramel
{
    class ImGuiRendererLayer;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnWindowEvent(Event& event);

        void TrackLayer(Layer* layer);
        void TrackOverlay(Layer* overlay);

        template<class T = EditorLayer>
        void EnableEditorLayer()
        {
            static_assert(std::is_base_of<EditorLayer, T>::value, "Application::EnableEditorLayer must be called with a type derived from EditorLayer");
            CL_CORE_ASSERT(m_editorLayer == nullptr, "Pushing multiple editor layers isn't supported");
            m_editorLayer = new T();
            m_editorLayer->OnAttach();
        }

        inline Window* GetWindow() { return m_window.get(); };
        static inline Application* Get() { return s_application; }
        static inline WindowRenderAPI GetRenderAPI() { return s_application->GetWindow()->GetRenderAPIType(); }


    private:
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;

        SceneManager m_sceneManager;

        ImGuiRendererLayer* m_imGuiRendererLayer;
        Layer* m_editorLayer = nullptr; // Updates and receives events first + draws last


        float m_lastFrameTime = 0.0f;

        bool m_bIsRunning;

    private:
        static Application* s_application;
    };

    Application* CreateApplication();

}
