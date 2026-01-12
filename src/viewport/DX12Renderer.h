#pragma once

#ifdef _WIN32
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>
#include "MeshImporter.h"
#include "HairStrand.h"
#include "GuideCurve.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

namespace BrokenArrow {
namespace Viewport {

class DX12Renderer {
public:
    DX12Renderer();
    ~DX12Renderer();

    bool Initialize(void* windowHandle, int width, int height);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void RenderMesh(const IO::Mesh& mesh, const Math::Matrix4x4& transform);
    void RenderGuides(const std::vector<Hair::GuideCurve>& guides);
    void RenderStrands(const std::vector<Hair::HairStrand>& strands);
    
    void SetViewProjection(const Math::Matrix4x4& view, const Math::Matrix4x4& projection);
    
    void Resize(int width, int height);
    void Present();

private:
    bool CreateDevice();
    bool CreateCommandQueue();
    bool CreateSwapChain(void* windowHandle, int width, int height);
    bool CreateRenderTargets();
    bool CreateDepthStencil(int width, int height);
    bool CreateRootSignature();
    bool CreatePipelineState();
    
    void WaitForGPU();

    static const int FRAME_COUNT = 2;
    
    ComPtr<ID3D12Device> device_;
    ComPtr<ID3D12CommandQueue> commandQueue_;
    ComPtr<IDXGISwapChain3> swapChain_;
    ComPtr<ID3D12DescriptorHeap> rtvHeap_;
    ComPtr<ID3D12DescriptorHeap> dsvHeap_;
    ComPtr<ID3D12Resource> renderTargets_[FRAME_COUNT];
    ComPtr<ID3D12Resource> depthStencil_;
    ComPtr<ID3D12CommandAllocator> commandAllocator_;
    ComPtr<ID3D12GraphicsCommandList> commandList_;
    ComPtr<ID3D12RootSignature> rootSignature_;
    ComPtr<ID3D12PipelineState> pipelineState_;
    
    ComPtr<ID3D12Fence> fence_;
    UINT64 fenceValue_;
    HANDLE fenceEvent_;
    
    int width_;
    int height_;
    int frameIndex_;
    
    Math::Matrix4x4 viewProjection_;
};

} // namespace Viewport
} // namespace BrokenArrow

#endif // _WIN32
