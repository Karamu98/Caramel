#pragma once

#define BIT(x) (1 << x)

enum class WindowRenderAPI
{
    OpenGL,
    Vulkan,
    DirectX11,
    DirectX12,
    Metal
};