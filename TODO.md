- Continue developing
	Lazyload AssetManager system

- Refactor and abstract:
	Texture
	Shader
	ModelResource (Model)
	Utilities
	Framebuffer

- Future ideas
	Thread asset manager loading
	First pass ECS system using entt
	Rendering System
	Material System
	ECS saving/loading to file
	EditorProject with ImGUI editor UI
	Rendering system threaded? If possible

Theory:
	Maybe I can boot up a thread to do all render calls, assets load into memory but upload to GPU through RenderSystem. Submit shader data to RenderSystem before calling RenderSystem::Rendering

	RenderSystem::StartFrame();
	for(auto pipeline : RenderSystem::m_pipelines)
	{
		pipeline::Render();
	}
	
	for(auto uiPipeline : RenderSystem::m_uiPipelines)
	{
		uiPipeline::Render();
	}
	RenderSystem::EndFrame();

	