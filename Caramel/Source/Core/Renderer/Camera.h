#pragma once

#include "Core/Core.h"

#define UNIFORM_NAME_CAMERA_VIEW "u_View"
#define UNIFORM_NAME_CAMERA_PROJECTION "u_Projection"
#define UNIFORM_NAME_VIEW_POSITION "u_ViewPos"

namespace Caramel
{
	class Window;
	class Shader;
	class Camera
	{
	public:
		Camera(Window* window = nullptr);

		float FOV = 90.0f;
		float NearPlane = 0.01f;
		float FarPlane = 500.0f;

		glm::vec3 Position;
		glm::vec3 Rotation;

		glm::mat4& GetProjection(bool calculate = true);
		glm::mat4& GetView(bool calculate = true);
		glm::mat4& GetRotationMatrix(bool calculate = true);

		glm::vec3 GetForward();

		void Bind(std::shared_ptr<Shader> target);

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_rotationMatrix;

		Window* m_window;
	};
}

namespace ImGui
{
	template<>
	inline void DrawWidget(Caramel::Camera* obj)
	{
		ImGui::BeginChild("Camera");
		ImGui::DragFloat("FOV", &obj->FOV, 0.1f);
		ImGui::DragFloat("Near plane", &obj->NearPlane, 0.01f);
		ImGui::DragFloat("Far plane", &obj->FarPlane, 0.01f);

		ImGui::DragFloat3("Position", glm::value_ptr(obj->Position), 0.01f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(obj->Rotation), 0.01f);
		ImGui::EndChild();
	}
}
