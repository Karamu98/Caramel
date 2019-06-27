#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace Caramel 
{
	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(unsigned int count, bool depthTest = true);

		static void ClearMagenta();

		static void Init();

		static void* Submit(RenderCommandFn fn, unsigned int size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
	};

}

#define CL_RENDER_PASTE2(a, b) a ## b
#define CL_RENDER_PASTE(a, b) CL_RENDER_PASTE2(a, b)
#define CL_RENDER_UNIQUE(x) CL_RENDER_PASTE(x, __LINE__)

#define CL_RENDER(code) \
    struct CL_RENDER_UNIQUE(HZRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::Caramel::Renderer::Submit(CL_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(CL_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) CL_RENDER_UNIQUE(HZRenderCommand)();\
	}\

#define CL_RENDER_1(arg0, code) \
	do {\
    struct CL_RENDER_UNIQUE(HZRenderCommand) \
    {\
		CL_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Caramel::Renderer::Submit(CL_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(CL_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) CL_RENDER_UNIQUE(HZRenderCommand)(arg0);\
	} } while(0)

#define CL_RENDER_2(arg0, arg1, code) \
    struct CL_RENDER_UNIQUE(HZRenderCommand) \
    {\
		CL_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Caramel::Renderer::Submit(CL_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(CL_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) CL_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1);\
	}\

#define CL_RENDER_3(arg0, arg1, arg2, code) \
    struct CL_RENDER_UNIQUE(HZRenderCommand) \
    {\
		CL_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Caramel::Renderer::Submit(CL_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(CL_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) CL_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2);\
	}\

#define CL_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct CL_RENDER_UNIQUE(HZRenderCommand) \
    {\
		CL_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((CL_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(CL_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(CL_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) CL_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define CL_RENDER_S(code) auto self = this;\
	CL_RENDER_1(self, code)

#define CL_RENDER_S1(arg0, code) auto self = this;\
	CL_RENDER_2(self, arg0, code)

#define CL_RENDER_S2(arg0, arg1, code) auto self = this;\
	CL_RENDER_3(self, arg0, arg1, code)

#define CL_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	CL_RENDER_4(self, arg0, arg1, arg2, code)