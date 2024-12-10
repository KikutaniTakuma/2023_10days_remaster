#pragma once
#include "DescriptorHeap.h"
#include "Engine/Graphics/TextureManager/Texture/Texture.h"
#include "Engine/Graphics/RenderTarget/RenderTarget.h"
#include "Utils/ExecutionLog.h"

#include "Utils/SafePtr.h"

#include <list>

/// <summary>
/// ディスクリプタヒープ管理クラス
/// </summary>
class CbvSrvUavHeap final : public DescriptorHeap {
private:
	CbvSrvUavHeap(UINT numDescriptor, UINT maxTexture);

	CbvSrvUavHeap() = delete;
	CbvSrvUavHeap(const CbvSrvUavHeap& right) = delete;
	CbvSrvUavHeap(CbvSrvUavHeap&& right) noexcept = delete;
	CbvSrvUavHeap& operator=(const CbvSrvUavHeap& right) = delete;
	CbvSrvUavHeap& operator=(CbvSrvUavHeap&& right) noexcept = delete;
public:
	~CbvSrvUavHeap();

public:
	static void Initialize(UINT heapSize, UINT maxTexture = 128u);

	static void Finalize();

	static CbvSrvUavHeap* const GetInstance();

private:
	static Lamb::SafePtr<CbvSrvUavHeap> instance_;

private:
	void CreateDescriptorHeap(uint32_t heapSize) override;

	void CreateHeapHandles() override;

public:
	void Use(D3D12_GPU_DESCRIPTOR_HANDLE handle, UINT rootParmIndex);
	void Use(uint32_t handleIndex, UINT rootParmIndex);

public:
	/// <summary>
	/// View生成用関数
	/// </summary>
	/// <param name="buffer"></param>
	/// <returns></returns>
	uint32_t CreateView(class Descriptor& buffer);

	/// <summary>
	/// TextureのViewを作成する
	/// </summary>
	/// <param name="tex">テクスチャ</param>
	/// <returns>作ったヒープハンドル</returns>
	uint32_t CreateTextureView(class Descriptor& tex);

	/// <summary>
	/// TextureのViewを解放済みにする
	/// </summary>
	/// <param name="tex">テクスチャ</param>
	void ReleaseView(class Texture& tex);

	/// <summary>
	/// 他のViewを解放する
	/// </summary>
	/// <param name="viewHandle">ビューのハンドル</param>
	void ReleaseView(UINT viewHandle) override;

public:
	uint32_t GetMaxTexture() const;

private:
	uint32_t currentTextureHeapIndex_;
	std::list<uint32_t> releaseTextureHeapIndex_;
	const uint32_t kMaxTextureHadle_;
};