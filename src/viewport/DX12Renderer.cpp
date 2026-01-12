#include "DX12Renderer.h"

#ifdef _WIN32
#include <stdexcept>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace BrokenArrow {
namespace Viewport {

DX12Renderer::DX12Renderer() 
    : width_(0), height_(0), frameIndex_(0), fenceValue_(0), fenceEvent_(nullptr) {
}

DX12Renderer::~DX12Renderer() {
    Shutdown();
}

bool DX12Renderer::Initialize(void* windowHandle, int width, int height) {
    width_ = width;
    height_ = height;

    if (!CreateDevice()) return false;
    if (!CreateCommandQueue()) return false;
    if (!CreateSwapChain(windowHandle, width, height)) return false;
    if (!CreateRenderTargets()) return false;
    if (!CreateDepthStencil(width, height)) return false;
    if (!CreateRootSignature()) return false;
    if (!CreatePipelineState()) return false;

    if (FAILED(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                IID_PPV_ARGS(&commandAllocator_)))) {
        return false;
    }

    if (FAILED(device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                           commandAllocator_.Get(), nullptr,
                                           IID_PPV_ARGS(&commandList_)))) {
        return false;
    }

    commandList_->Close();

    if (FAILED(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)))) {
        return false;
    }

    fenceEvent_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent_ == nullptr) {
        return false;
    }

    return true;
}

void DX12Renderer::Shutdown() {
    WaitForGPU();
    
    if (fenceEvent_) {
        CloseHandle(fenceEvent_);
        fenceEvent_ = nullptr;
    }
}

bool DX12Renderer::CreateDevice() {
    UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)))) {
        return false;
    }

    ComPtr<IDXGIAdapter1> hardwareAdapter;
    for (UINT adapterIndex = 0;
         SUCCEEDED(factory->EnumAdapters1(adapterIndex, &hardwareAdapter));
         ++adapterIndex) {
        
        DXGI_ADAPTER_DESC1 desc;
        hardwareAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        if (SUCCEEDED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0,
                                         IID_PPV_ARGS(&device_)))) {
            break;
        }
    }

    return device_ != nullptr;
}

bool DX12Renderer::CreateCommandQueue() {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    return SUCCEEDED(device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)));
}

bool DX12Renderer::CreateSwapChain(void* windowHandle, int width, int height) {
    ComPtr<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        return false;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    if (FAILED(factory->CreateSwapChainForHwnd(commandQueue_.Get(),
                                                static_cast<HWND>(windowHandle),
                                                &swapChainDesc, nullptr, nullptr,
                                                &swapChain))) {
        return false;
    }

    return SUCCEEDED(swapChain.As(&swapChain_));
}

bool DX12Renderer::CreateRenderTargets() {
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if (FAILED(device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap_)))) {
        return false;
    }

    UINT rtvDescriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < FRAME_COUNT; i++) {
        if (FAILED(swapChain_->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i])))) {
            return false;
        }
        device_->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }

    return true;
}

bool DX12Renderer::CreateDepthStencil(int width, int height) {
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if (FAILED(device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_)))) {
        return false;
    }

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC depthStencilDesc = {};
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0f;

    if (FAILED(device_->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
                                                  &depthStencilDesc,
                                                  D3D12_RESOURCE_STATE_DEPTH_WRITE,
                                                  &clearValue, IID_PPV_ARGS(&depthStencil_)))) {
        return false;
    }

    device_->CreateDepthStencilView(depthStencil_.Get(), nullptr,
                                     dsvHeap_->GetCPUDescriptorHandleForHeapStart());

    return true;
}

bool DX12Renderer::CreateRootSignature() {
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;

    if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                                            &signature, &error))) {
        return false;
    }

    return SUCCEEDED(device_->CreateRootSignature(0, signature->GetBufferPointer(),
                                                    signature->GetBufferSize(),
                                                    IID_PPV_ARGS(&rootSignature_)));
}

bool DX12Renderer::CreatePipelineState() {
    return true;
}

void DX12Renderer::WaitForGPU() {
    if (fence_ && commandQueue_) {
        const UINT64 fenceVal = fenceValue_;
        commandQueue_->Signal(fence_.Get(), fenceVal);
        fenceValue_++;

        if (fence_->GetCompletedValue() < fenceVal) {
            fence_->SetEventOnCompletion(fenceVal, fenceEvent_);
            WaitForSingleObject(fenceEvent_, INFINITE);
        }
    }
}

void DX12Renderer::BeginFrame() {
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), pipelineState_.Get());

    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = renderTargets_[frameIndex_].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    commandList_->ResourceBarrier(1, &barrier);

    UINT rtvDescriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += frameIndex_ * rtvDescriptorSize;

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();

    commandList_->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

    const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DX12Renderer::EndFrame() {
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = renderTargets_[frameIndex_].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    commandList_->ResourceBarrier(1, &barrier);
    commandList_->Close();

    ID3D12CommandList* commandLists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(_countof(commandLists), commandLists);
}

void DX12Renderer::Present() {
    swapChain_->Present(1, 0);
    WaitForGPU();
}

void DX12Renderer::RenderMesh(const IO::Mesh& mesh, const Math::Matrix4x4& transform) {
}

void DX12Renderer::RenderGuides(const std::vector<Hair::GuideCurve>& guides) {
}

void DX12Renderer::RenderStrands(const std::vector<Hair::HairStrand>& strands) {
}

void DX12Renderer::SetViewProjection(const Math::Matrix4x4& view, const Math::Matrix4x4& projection) {
    viewProjection_ = view * projection;
}

void DX12Renderer::Resize(int width, int height) {
    width_ = width;
    height_ = height;
}

} // namespace Viewport
} // namespace BrokenArrow

#endif // _WIN32
