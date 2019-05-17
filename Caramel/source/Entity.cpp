#include "Entity.h"
#include "Transform.h"
#include "imgui.h"
#include "Gizmos.h"
#include "Component.h"
#include <iostream>
#include <string>
#include "Log.h"
#include "Camera.h"
#include "Scene.h"
#include "ImGuizmo.h"
#include "Defines.h"

unsigned int Entity::s_uiEntityIDCount = 0;

Entity::Entity(Scene* a_scene)
{
	a_scene->Add(this);
	//Increment entity ID
	m_uiEntityID = s_uiEntityIDCount++;

	SetName(std::string("Default ") += std::to_string(GetEntityID()));
}

Entity::Entity(const Entity& a_other, Scene* a_scene)
{
	a_scene->Add(this);
	m_uiEntityID = s_uiEntityIDCount++;

	memcpy(cName, a_other.cName, NAME_BUF_SIZE);
	m_Transform = a_other.m_Transform;

	for (Component* comp : a_other.m_apComponentList)
	{
		Component* newComp = comp->Duplicate(this);
	}
}

Entity::~Entity()
{
	s_uiEntityIDCount--;
	for (Component* comp : m_apComponentList)
	{
		if (comp != nullptr)
		{
			delete comp;
		}
	}

}

void Entity::Update(float a_fDeltaTime)
{
	std::vector<Component*>::iterator xIter;
	for(xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent)
		{
			pComponent->Update(a_fDeltaTime);
		}
	}
}

void Entity::OnGUI()
{
	Gizmos::addTransform(*m_Transform.GetMatrix(), 1);

	glm::mat4* matrix = m_Transform.GetMatrix();
	auto positionRow = &((*matrix)[3]);

	static glm::vec3 scale = glm::vec3(m_Transform.GetRight().x, m_Transform.GetUp().y, m_Transform.GetForward().z);


	if (ImGui::IsKeyPressed(291)) // F2
	{
		ImGui::SetKeyboardFocusHere();
	}


	// List Transfrom Component
	ImGui::TextColored(ImVec4(255, 255, 255, 1), "Transform Component");
	ImGui::InputText("Name", cName, NAME_BUF_SIZE);

	ImGuizmo::Enable(true);

	float* mat = glm::value_ptr(*m_Transform.GetMatrix());

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(mat, matrixTranslation, matrixRotation, matrixScale);
	ImGui::DragFloat3("Position", matrixTranslation, 0.05f);
	ImGui::DragFloat3("Rotation", matrixRotation, 0.05f);
	ImGui::DragFloat3("Scale", matrixScale, 0.05f);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, mat);


	if (ImGui::Button("Reset"))
	{
		m_Transform.Reset();
	}
	ImGui::NewLine();


	std::vector<Component*>::iterator xIter;
	for (xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent)
		{
			pComponent->OnGUI();
		}
	}
}

void Entity::AddComponent(Component * a_pComponent)
{
	m_apComponentList.push_back(a_pComponent);
}

void Entity::DeleteComponent(Component* a_pComponent)
{
	ptrdiff_t old = std::find(m_apComponentList.begin(), m_apComponentList.end(), a_pComponent) - m_apComponentList.begin();

	m_apComponentList.erase(m_apComponentList.begin() + old);
	delete a_pComponent;
}

std::vector<Component*>* Entity::GetComponentList()
{
	return &m_apComponentList;
}

Transform* Entity::GetTransform()
{
	return &m_Transform;
}

std::string Entity::GetName()
{
	return std::string(cName);
}

void Entity::SetName(std::string a_newName)
{
	memcpy(cName, a_newName.c_str(), NAME_BUF_SIZE);
}

void Entity::Save(std::ostream* a_outStream)
{
	SaveToFile(a_outStream, m_uiEntityID);
	SaveToFile(a_outStream, cName);
	SaveToFile(a_outStream, m_Transform);

	FileHeader compHeader;
	compHeader.flag = Flags::COMPONENTLIST_START;
	compHeader.size = m_apComponentList.size();

	for (Component* comp : m_apComponentList)
	{
		comp->Save(a_outStream);
	}

}

void Entity::Load(std::ifstream* a_inStream)
{
	LoadFromFile(a_inStream, &m_uiEntityID);
	LoadFromFile(a_inStream, cName);
	

	FileHeader compHeader;
	LoadFromFile(a_inStream, &compHeader);

	if (compHeader.flag != Flags::COMPONENTLIST_START)
	{
		CL_CORE_ERROR("Save file mismatch, expected 'COMPONENTLIST_START'");
		return;
	}

	m_apComponentList.reserve(compHeader.size);

	FileHeader compTypeHeader;

	for (int i = 0; i < compHeader.size; ++i)
	{
		LoadFromFile(a_inStream, &compTypeHeader);


		//Component* newComp = CreateComponent(this, compTypeHeader.flag);
		//if (newComp)
		//{
		//	m_apComponentList.push_back(newComp);
		//}
		//else
		//{
		//	CL_CORE_FATAL("Unable to load entity.");
		//}
	}

}
