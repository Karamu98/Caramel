#include "clpch.h"
#include "EditorCamera.h"

#include "Core/Input.h"
#include "Core/Core.h"

namespace Caramel
{
    EditorCamera::EditorCamera()
    {
        m_lastMousePos = Input::GetMousePosition();
    }

    void EditorCamera::ImGui()
    {
	    ImGui::DragFloat("Camera speed", &m_moveSpeed);
	    ImGui::DragFloat2("Look speed", glm::value_ptr(m_lookSpeed));

	    ImGui::DrawWidget<Caramel::Camera>(&Camera);
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        glm::vec2 curMousePos = Input::GetMousePosition();
        if (!Input::IsMouseButtonPressed(1))
        {
            m_lastMousePos = curMousePos;
            return;
        }

        glm::vec2 mouseDelta = m_lastMousePos - curMousePos;
        Camera.Rotation.y -= mouseDelta.x * m_lookSpeed.x * ts;
        Camera.Rotation.x -= mouseDelta.y * m_lookSpeed.y * ts;

        m_lastMousePos = curMousePos;

        glm::vec3 camMove{};
        if (Input::IsKeyPressed(KeyCode::W)) camMove -= glm::vec3{ 0.0f, 0.0f, 1.0f };
        if (Input::IsKeyPressed(KeyCode::S)) camMove -= glm::vec3{ 0.0f, 0.0f, -1.0f };
        if (Input::IsKeyPressed(KeyCode::A)) camMove -= glm::vec3{ 1.0f, 0.0f, 0.0f };
        if (Input::IsKeyPressed(KeyCode::D)) camMove -= glm::vec3{ -1.0f, 0.0f, 0.0f };
        if (Input::IsKeyPressed(KeyCode::E)) camMove -= glm::vec3{ 0.0f, -1.0f, 0.0f };
        if (Input::IsKeyPressed(KeyCode::Q)) camMove -= glm::vec3{ 0.0f, 1.0f, 0.0f };

        //glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Camera.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        //rotation = glm::rotate(rotation, glm::radians(Camera.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotation = Camera.GetRotationMatrix();

        // Rotate camMove
        camMove = glm::vec3(glm::inverse(rotation)* glm::vec4(camMove, 0.0f));

        Camera.Position += camMove * m_moveSpeed * ts.GetSeconds();
    }
}

