#include "clpch.h"
#include "Renderer.h"
#include "Core/RenderAPI/RenderAPI.h"
#include "Core/RenderAPI/Buffer.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Camera.h"


namespace Caramel
{
	RenderAPI* RenderCommand::s_renderAPI = nullptr;

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

	void Renderer::Submit(const std::shared_ptr<Model>& model)
	{
		// Call draw indexed on all meshes with correct materials
		// TODO: IMPROVE
		for (const auto& mesh : *model)
		{
			// TODO: Bind material
			Renderer::Submit(mesh.Data);
		}
	}

	void Renderer::EndScene()
	{

	}
}