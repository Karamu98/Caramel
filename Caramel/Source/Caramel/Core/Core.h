#pragma once

#include <memory>

#define BIT(x) (1 << x)

enum class WindowRenderAPI
{
    OpenGL,
    Vulkan,
    DirectX11,
    DirectX12,
    Metal
};

namespace ImGui
{
    template<typename T>
    inline void DrawWidget(T* obj)
    {

    }
}


namespace Caramel
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
