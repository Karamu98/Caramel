#include "TransformComponent.h"
#include "Entity.h"
#include "Gizmos.h"
#include "imgui.h"

//typedefs
typedef Component PARENT;

TransformComponent::TransformComponent(Entity* pOwner) : PARENT(pOwner)
{
	SetComponentType(TRANSFORM);

	SetCurrentPosition(glm::vec3(0, 0, 0));
	SetFacingDirection(glm::vec3(0, 0, 1));
	SetUpDirection(glm::vec3(0, 1, 0));
	SetRightDirection(glm::vec3(1, 0, 0));
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::OnGUI()
{
	Gizmos::addTransform(m_m4TransformMatrix, 1);

	// List Transfrom Component
	ImGui::TextColored(ImVec4(255, 255, 255, 1), "Transform Component");
	ImGui::Text("Name: ");
	ImGui::SameLine(50);
	ImGui::InputText("", (char*)pGetOwnerEntity()->GetName()->c_str(), 10);
	ImGui::Text("Position: ");
	ImGui::InputFloat(": X", &m_m4TransformMatrix[3].x);
	ImGui::InputFloat(": Y", &m_m4TransformMatrix[3].y);
	ImGui::InputFloat(": Z", &m_m4TransformMatrix[3].z);
	//ImGui::Text("Position X: %i Y: %i Z: %i", (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().x, (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().y, (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().z);
	if(ImGui::Button("Reset"))
	{
		SetCurrentPosition(glm::vec3(0, 0, 0));
		SetFacingDirection(glm::vec3(0, 0, 1));
		SetUpDirection(glm::vec3(0, 1, 0));
		SetRightDirection(glm::vec3(1, 0, 0));
	}
	ImGui::NewLine();
}

glm::vec3 TransformComponent::GetRightDirection()
{
	return GetEntityMatrixRow(RIGHT_VECTOR);
}

glm::vec3 TransformComponent::GetUpDirection()
{
	return GetEntityMatrixRow(UP_VECTOR);
}

glm::vec3 TransformComponent::GetFacingDirection()
{
	return GetEntityMatrixRow(FORWARD_VECTOR);
}

glm::vec3 TransformComponent::GetCurrentPosition()
{
	return GetEntityMatrixRow(POSITION_VECTOR);
}

void TransformComponent::SetRightDirection(glm::vec3 vRightDirection)
{
	SetEntityMatrixRow(RIGHT_VECTOR, vRightDirection);
}

void TransformComponent::SetUpDirection(glm::vec3 vUpDirection)
{
	SetEntityMatrixRow(UP_VECTOR, vUpDirection); 
}

void TransformComponent::SetFacingDirection(glm::vec3 vFacingDirection)
{
	SetEntityMatrixRow(FORWARD_VECTOR, vFacingDirection);
}

void TransformComponent::SetCurrentPosition(glm::vec3 vCurrentPosition)
{
	SetEntityMatrixRow(POSITION_VECTOR, vCurrentPosition);
}

void TransformComponent::SetScale(glm::vec3 a_newScale)
{
	SetEntityMatrixRow(UP_VECTOR, GetUpDirection() * a_newScale.y);
	SetEntityMatrixRow(RIGHT_VECTOR, GetRightDirection() * a_newScale.x);
	SetEntityMatrixRow(FORWARD_VECTOR, GetFacingDirection() * a_newScale.z);
}

glm::mat4* TransformComponent::pGetTransformMatrix()
{
	return &m_m4TransformMatrix;
}

void TransformComponent::SetEntityMatrixRow(MATRIX_ROW eRow, glm::vec3 vVec)
{
	m_m4TransformMatrix[eRow] = glm::vec4(vVec, (eRow == POSITION_VECTOR ? 1.f : 0.f));
}

glm::vec3 TransformComponent::GetEntityMatrixRow(MATRIX_ROW eRow)
{
	return m_m4TransformMatrix[eRow];
}
