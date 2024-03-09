#include "clpch.h"
#include "ImGuiImpl_DX12.h"


#include <Platform/RenderAPI/DX12/RenderAPI_DX12.h>
#include <backends/imgui_impl_dx12.h>


void Caramel::ImGuiRendererImpl_DX12::Init(RenderAPI* renderAPI)
{
    m_dxAPI = (RenderAPI_DX12*)renderAPI;

    ID3D12Device* device = m_dxAPI->m_pd3dDevice.Get();
    int num_frames_in_flight = m_dxAPI->m_iSwapChainBufferCount;
    DXGI_FORMAT rtv_format = m_dxAPI->m_BackBufferFormat;
    ID3D12DescriptorHeap* cbv_srv_heap = m_dxAPI->m_pd3dSrvDescHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE font_srv_cpu_desc_handle = m_dxAPI->m_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE font_srv_gpu_desc_handle = m_dxAPI->m_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

    ImGui_ImplDX12_Init(device, num_frames_in_flight, rtv_format, cbv_srv_heap, font_srv_cpu_desc_handle, font_srv_gpu_desc_handle);
}

void Caramel::ImGuiRendererImpl_DX12::NewFrame()
{
    ImGui_ImplDX12_NewFrame();
}

void Caramel::ImGuiRendererImpl_DX12::Render(ImDrawData* drawData)
{
    ImGui_ImplDX12_RenderDrawData(drawData, m_dxAPI->m_pd3dCommandList.Get());
}

void Caramel::ImGuiRendererImpl_DX12::Shutdown()
{
    ImGui_ImplDX12_Shutdown();
}
