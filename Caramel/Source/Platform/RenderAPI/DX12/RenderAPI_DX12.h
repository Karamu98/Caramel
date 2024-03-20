#pragma once
#include <Core/RenderAPI/RenderAPI.h>

#include <d3d12.h>       // for D3D12 interface
#include <dxgi1_6.h>     // for DXGI
#include <wrl.h>         // for Microsoft::WRL::ComPtr

namespace Caramel
{
	class Window;

	class RenderAPI_DX12 : public RenderAPI
	{
	public:
		RenderAPI_DX12() {};
	protected:
		// Inherited via RenderAPI
		virtual void Initialise(Window* window, const struct WindowProperties* props) override;
		virtual void Shutdown() override;

		virtual void SetVSync(bool enabled) override;
		virtual void SetRefreshRate(unsigned int frameRate) override;

		virtual void Clear() override;
		virtual void Render() override;
		virtual void Present() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawArray(const std::shared_ptr<VertexArray>& vertexArray) override;

	private:
		HWND m_nativeWindow;

		HRESULT InitDirect3D(HWND hwnd, int width, int height);
		HRESULT ResizeRenderedBuffers(int width, int height);

		HRESULT GetHardwareAdpter(_In_ IDXGIFactory1* pFactory, _In_ BOOL bRequestHighPerformanceAdapter, _In_ BOOL(*AdapterSelectionCallback)(IDXGIAdapter1*), _Out_ IDXGIAdapter1** ppAdapter);
		HRESULT CreateCommandObjects();
		HRESULT CreateSwapChain(HWND hwnd, int width, int height, IDXGIFactory4* pDXGIFactory);
		HRESULT CreateRtvAndDsvDescriptorHeaps(int extraRtvCount, int extraDsvCount);
		void FlushCommandQueue();
		void FreeD3DResources();
		void DxTrace(const wchar_t* file, unsigned long line, HRESULT hr, const wchar_t* proc);

	public:
		Microsoft::WRL::ComPtr<ID3D12Device> m_pd3dDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> m_adapter;
		INT64 m_iFencePoint;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_pd3dFence;
		HANDLE m_hFenceEvent;

		UINT m_uRtvDescriptorSize;
		UINT m_uDsvDescriptorSize;
		UINT m_uCbvSrvUavDescriptorSize;

		ID3D12DescriptorHeap* m_pd3dSrvDescHeap = nullptr;

		// Command queue
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pd3dCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pd3dDirectCmdAlloc;

		// Swap chain
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		static constexpr DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		static constexpr UINT m_iSwapChainBufferCount = 2;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDSVDescriptorHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_pSwapChainBuffers[m_iSwapChainBufferCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pDepthStencilBuffer;

		UINT m_iCurrentFrameIndex = 0;

		D3D12_VIEWPORT m_ScreenViewport;
		D3D12_RECT m_ScreenScissorRect;
	};
}


