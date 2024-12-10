/// ==============================
/// ==  PeraRenderクラスの定義  ==
/// ==============================

#include "PeraRender.h"
#include "Utils/ConvertString.h"
#include "Engine/Core/DescriptorHeap/CbvSrvUavHeap.h"
#include "imgui.h"
#include "Engine/Core/DirectXCommand/DirectXCommand.h"
#include <cassert>
#include <numbers>

#include "Utils/Random.h"

#include "Utils/EngineInfo.h"

#include "Utils/SafePtr.h"

PeraRender::PeraRender():
	color(std::numeric_limits<uint32_t>::max())
{
	peraPipelineObject_.reset(new PeraPipeline{});
}

PeraRender::PeraRender(uint32_t width, uint32_t height):
	color(std::numeric_limits<uint32_t>::max())
{
	peraPipelineObject_.reset(new PeraPipeline{});

	peraPipelineObject_->SetSize(width, height);
}

PeraRender::~PeraRender() {
	peraPipelineObject_.reset();
}

void PeraRender::Initialize(const std::string& psFileName, std::initializer_list<DXGI_FORMAT> formtats) {
	peraPipelineObject_->Init("./Shaders/PostShader/Post.VS.hlsl", psFileName, formtats);
}

void PeraRender::Initialize(PeraPipeline* pipelineObject) {
	ResetPipelineObject(pipelineObject);
}

void PeraRender::PreDraw(const D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle) {
	peraPipelineObject_->GetRender().SetThisRenderTarget(depthHandle);
}

void PeraRender::Draw(
	Pipeline::Blend blend, 
	D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle
) {
	peraPipelineObject_->color = color;
	peraPipelineObject_->DataSet();

	// 各種描画コマンドを積む
	Lamb::SafePtr commandList = DirectXCommand::GetMainCommandlist()->GetCommandList();
	peraPipelineObject_->Use(blend, !!depthHandle);
	commandList->DrawInstanced(3, 1, 0, 0);
}

void PeraRender::ResetPipelineObject(PeraPipeline* pipelineObject) {
	if (!pipelineObject) {
		throw Lamb::Error::Code<PeraRender>("pipelineObject is nullptr", ErrorPlace);
	}
	else {
		peraPipelineObject_.reset(pipelineObject);
	}
}

RenderTarget& PeraRender::GetRender()
{
	return peraPipelineObject_->GetRender();
}

const RenderTarget& PeraRender::GetRender() const
{
	return peraPipelineObject_->GetRender();
}

void PeraRender::Debug([[maybe_unused]]const std::string& guiName) {
#ifdef USE_DEBUG_CODE
	ImGui::Begin(guiName.c_str());
	ImGui::ColorEdit4("color", peraPipelineObject_->color.m.data());

	color = Vector4ToUint(peraPipelineObject_->color);
	ImGui::End();
#endif // USE_DEBUG_CODE
}