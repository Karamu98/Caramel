#pragma once

#include "Core/RenderAPI/RenderAPI.h"

namespace Caramel
{
	class VertexArray;
	class Model;

	class RenderCommand
	{
	public:
		inline static void Init(RenderAPI* api)
		{
			s_renderAPI = api;
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_renderAPI->DrawIndexed(vertexArray);
		}

		inline static void Clear()
		{
			s_renderAPI->Clear();
		}

		inline static void Present()
		{
			s_renderAPI->Present();
		}

		inline static void DrawArray(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_renderAPI->DrawArray(vertexArray);
		}
	private:
		static RenderAPI* s_renderAPI;
	};


	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();


		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void Submit(const std::shared_ptr<Model>& model);
	};
}



