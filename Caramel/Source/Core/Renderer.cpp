#include "clpch.h"
#include "Renderer.h"
#include "Core/RenderAPI/RenderAPI.h"
#include "Core/RenderAPI/Buffer.h"


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

	void Renderer::EndScene()
	{

	}
}