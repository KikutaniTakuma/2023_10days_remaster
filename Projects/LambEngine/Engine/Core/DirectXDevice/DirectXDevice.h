#pragma once
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")
#include "Engine/EngineUtils/LambPtr/LambPtr.h"
#include "Utils/SafePtr.h"

/// <summary>
/// デバイス等の管理クラス
/// </summary>
class DirectXDevice {
private:
	DirectXDevice();
	DirectXDevice(const DirectXDevice&) = delete;
	DirectXDevice(DirectXDevice&&) = delete;
	DirectXDevice operator=(const DirectXDevice&) = delete;
	DirectXDevice operator=(DirectXDevice&&) = delete;
public:
	~DirectXDevice();

public:
	static DirectXDevice* const GetInstance();

	static void Initialize();
	static void Finalize();

private:
	static Lamb::SafePtr<DirectXDevice> instance_;

private:
	void SettingAdapter();

	void CreateDevice();

#ifdef USE_DEBUG_CODE
	void InfoQueue() const;
#endif // USE_DEBUG_CODE

	void CreateHeapIncrements();


/// <summary>
/// リソース作成
/// </summary>
public:
	[[nodiscard]]
	ID3D12DescriptorHeap* CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderrVisible
	);

	[[nodiscard]]
	ID3D12Resource* CreateBufferResuorce(size_t sizeInBytes);

	[[nodiscard]]
	ID3D12Resource* CreateDepthStencilTextureResource(int32_t width, int32_t height);

	[[nodiscard]]
	ID3D12Resource* CreateDepthStencilTextureResource(const class Vector2& size);

/// <summary>
/// getter
/// </summary>
public:
	inline UINT GetIncrementSRVCBVUAVHeap() const {
		return incrementSRVCBVUAVHeap_;
	}

	inline UINT GetIncrementRTVHeap() const {
		return incrementRTVHeap_;
	}

	inline UINT GetIncrementDSVHeap() const {
		return incrementDSVHeap_;
	}

	inline UINT GetIncrementSAMPLER() const {
		return incrementSAMPLER_;
	}

	ID3D12Device8* const GetDevice() const {
		return device_.Get();
	}

	IDXGIFactory7* const GetDxgiFactory() const {
		return dxgiFactory_.Get();
	}

	bool GetIsCanUseMeshShader() const {
		return isCanUseMeshShader_;
	}

private:
	Lamb::LambPtr<ID3D12Device8> device_;
	Lamb::LambPtr<IDXGIFactory7> dxgiFactory_;
	Lamb::LambPtr<IDXGIAdapter4> useAdapter_;

	UINT incrementSRVCBVUAVHeap_;
	UINT incrementRTVHeap_;
	UINT incrementDSVHeap_;
	UINT incrementSAMPLER_;

	bool isCanUseMeshShader_ = false;
};