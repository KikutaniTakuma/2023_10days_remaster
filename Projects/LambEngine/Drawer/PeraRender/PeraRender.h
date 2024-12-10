/// ==============================
/// ==  PeraRenderクラスの宣言  ==
/// ==============================

#pragma once
#include <string>
#include <array>
#include <initializer_list>

#include "Engine/Graphics/PipelineObject/PeraPipeline/PeraPipeline.h"

#include "Math/Vector4.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Matrix.h"

/// <summary>
/// ポストエフェクトの描画
/// </summary>
class PeraRender {
public:
	struct PeraVertexData {
		Vector3 position;
		Vector2 uv;
	};

public:
	PeraRender();
	PeraRender(uint32_t width, uint32_t height);
	~PeraRender();

	PeraRender(const PeraRender&) = delete;
	PeraRender(PeraRender&&) = delete;
	PeraRender& operator=(const PeraRender&) = delete;
	PeraRender& operator=(PeraRender&&) = delete;

public:
	void Initialize(const std::string& psFileName, std::initializer_list<DXGI_FORMAT> formtats);
	void Initialize(PeraPipeline* pipelineObject);

public:
	void PreDraw(const D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle);

	void Draw(
		Pipeline::Blend blend, 
		D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle
	);

	Texture* GetTex() const {
		return peraPipelineObject_->GetRender().GetTex();
	}

	void ChangeResourceState() {
		peraPipelineObject_->GetRender().ChangeResourceState();
	}

	void SetMainRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle) {
		peraPipelineObject_->GetRender().SetMainRenderTarget(depthHandle);
	}

	void Debug(const std::string& guiName);

	void ResetPipelineObject(PeraPipeline* pipelineObject);

	RenderTarget& GetRender();
	const RenderTarget& GetRender() const;

public:
	uint32_t color;

private:
	std::unique_ptr<PeraPipeline> peraPipelineObject_;
};