#include "clpch.h"
#include "Renderer.h"
#include "Caramel/Renderer/Camera.h"
#include "Caramel/Renderer/API/RenderAPI.h"
#include "Caramel/Renderer/API/Buffer.h"
#include "Caramel/Renderer/API/Shader.h"
#include "Caramel/Renderer/Primitives/Model.h"
#include "Caramel/Renderer/Primitives/Material.h"


namespace Caramel
{
	RenderAPI* RenderCommand::s_renderAPI = nullptr;
	Camera* Renderer::s_ActiveCam;

	void Renderer::BeginScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		if (vertexArray->HasIndexBuffer())
		{
			RenderCommand::DrawIndexed(vertexArray);
		}
		else
		{
			RenderCommand::DrawArray(vertexArray);
		}
	}

	void Renderer::Submit(const std::shared_ptr<Model>& model, glm::mat4& transform)
	{
		if (model == nullptr)
		{
			return;
		}

		// TODO: Improve to use instanced
		for (const auto& kvp : model->MeshByMaterials)
		{
			kvp.first->Bind();
			s_ActiveCam->Bind(kvp.first->Shader); // Gross
			kvp.first->Shader->SetValue("u_ModelTransform", Caramel::ShaderDataType::Mat4, glm::value_ptr(transform)); // Gross
			for (const auto& mesh : kvp.second)
			{
				Renderer::Submit(mesh->Data);
			}
		}
	}

	void Renderer::Submit(Camera* active)
	{
		s_ActiveCam = active;
	}

	void Renderer::EndScene()
	{

	}
}