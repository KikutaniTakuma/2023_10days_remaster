#pragma once

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#undef max
#undef min

#include "Engine/EngineUtils/LambPtr/LambPtr.h"
#include "Engine/Core/DirectXSwapChain/DirectXSwapChain.h"

#include <array>

class DirectXCommand {
public:
	DirectXCommand();
	DirectXCommand(const DirectXCommand&) = delete;
	DirectXCommand(DirectXCommand&&) = delete;

	DirectXCommand& operator=(const DirectXCommand&) = delete;
	DirectXCommand& operator=(DirectXCommand&&) = delete;
public:
	~DirectXCommand();

public:
	static DirectXCommand* const GetMainCommandlist();


public:
	/// <summary>
	/// コマンドリストを確定させる
	/// </summary>
	void CloseCommandlist();

	/// <summary>
	/// コマンドリストを実行
	/// </summary>
	void ExecuteCommandList();

	/// <summary>
	/// コマンドリストのリセット(また使えるようにする)
	/// </summary>
	void ResetCommandlist();

	/// <summary>
	/// コマンドリストの実行を待つ
	/// </summary>
	void WaitForFinishCommnadlist();

private:
	void CreateCommandQueue();

	void CreateCommandAllocator();

	void CreateGraphicsCommandList();

	void CrateFence();

public:
	ID3D12GraphicsCommandList6* const GetCommandList() const;

	ID3D12CommandQueue* const GetCommandQueue() const {
		return commandQueue_.Get();
	}

	inline bool GetIsCloseCommandList() const {
		return isCommandListClose_;
	}

	void SetBufferIndex(uint32_t bufferIndex);

private:
	Lamb::LambPtr<ID3D12CommandQueue> commandQueue_;
	std::array<Lamb::LambPtr<ID3D12CommandAllocator>, DirectXSwapChain::kBackBufferNumber> commandAllocators_;
	std::array<Lamb::LambPtr<ID3D12GraphicsCommandList6>, DirectXSwapChain::kBackBufferNumber> commandLists_;
	bool isCommandListClose_;

	Lamb::LambPtr<ID3D12Fence> fence_;
	uint64_t fenceVal_;
	HANDLE fenceEvent_;

	uint32_t bufferIndex_ = 0;

public:
	/// <summary>
	/// バリア
	/// </summary>
	/// <param name="resource">リソースバリアを貼るリソース</param>
	/// <param name="before">今の状態</param>
	/// <param name="after">遷移後の状態</param>
	/// <param name="subResource">サブリソース</param>
	static void Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subResource = 0u);
	static void BarrierUAV(ID3D12Resource* resource);
	
	/// <summary>
	/// 複数のリソースバリアを貼る
	/// </summary>
	/// <param name="resources">リソースバリアを貼るリソース</param>
	/// <param name="before">今の状態</param>
	/// <param name="after">遷移後の状態</param>
	/// <param name="subResource">サブリソース</param>
	static void Barrier(std::initializer_list<ID3D12Resource*> resources, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subResource = 0u);
	static void Barrier(ID3D12Resource** resources, uint32_t numResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subResource = 0u);
	static void Barrier(std::vector<ID3D12Resource*> resources, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subResource = 0u);
};