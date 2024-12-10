/// ========================
/// ==  Lineクラスの定義  ==
/// ========================

#include "Line.h"
#include <algorithm>
#include "Engine/Graphics/PipelineManager/PipelineManager.h"
#include "Engine/Core/DescriptorHeap/CbvSrvUavHeap.h"
#include "Engine/Core/DirectXCommand/DirectXCommand.h"
#include "Error/Error.h"
#include "imgui.h"
#include <numbers>

#include "Utils/EngineInfo.h"

uint32_t Line::nodepthDrawCount_ = 0u;
uint32_t Line::depthDrawCount_ = 0u;
Shader Line::shader_ = {};
Lamb::SafePtr<class Pipeline> Line::depthPipeline_ = nullptr;
Lamb::SafePtr<Pipeline> Line::nodepthPipeline_ = nullptr;
std::array<std::unique_ptr<StructuredBuffer<Line::VertxData>>, DirectXSwapChain::kBackBufferNumber> Line::nodepthVertData_;
std::array<std::unique_ptr<StructuredBuffer<Line::VertxData>>, DirectXSwapChain::kBackBufferNumber> Line::depthVertData_;

void Line::Initialize() {
	Lamb::SafePtr shaderManager = ShaderManager::GetInstance();
	shader_.vertex = shaderManager->LoadVertexShader("./Shaders/LineShader/Line.VS.hlsl");
	shader_.pixel = shaderManager->LoadPixelShader("./Shaders/LineShader/Line.PS.hlsl");

	D3D12_ROOT_PARAMETER paramater = {};
	paramater.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	paramater.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	paramater.Descriptor.ShaderRegister = 0;

	RootSignature::Desc desc;
	desc.rootParameter = &paramater;
	desc.rootParameterSize = 1;
	desc.samplerDeacs.clear();

	auto pipelineManager = PipelineManager::GetInstance();
	Pipeline::Desc pipelineDesc;
	pipelineDesc.rootSignature = pipelineManager->CreateRootSgnature(desc);
	pipelineDesc.vsInputData.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT });
	pipelineDesc.shader = shader_;
	pipelineDesc.blend[0] = Pipeline::None;
	pipelineDesc.solidState = Pipeline::SolidState::Solid;
	pipelineDesc.cullMode = Pipeline::CullMode::None;
	pipelineDesc.topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	pipelineDesc.numRenderTarget = 1;

	pipelineDesc.rtvFormtat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pipelineDesc.isDepth = true;
	pipelineManager->SetDesc(pipelineDesc);
	depthPipeline_ = pipelineManager->Create();

	pipelineDesc.rtvFormtat[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.isDepth = false;
	pipelineManager->SetDesc(pipelineDesc);
	nodepthPipeline_ = pipelineManager->Create();

	pipelineManager->StateReset();

	std::for_each(
		nodepthVertData_.begin(), 
		nodepthVertData_.end(), 
		[](auto& n) {
			n = std::make_unique<StructuredBuffer<VertxData>>(); 
			n->Create(Line::kDrawMaxNumber_);
		}
	);
	std::for_each(
		depthVertData_.begin(),
		depthVertData_.end(),
		[](auto& n) {
			n = std::make_unique<StructuredBuffer<VertxData>>();
			n->Create(Line::kDrawMaxNumber_);
		}
	);
}

void Line::Finalize() {
	std::for_each(
		nodepthVertData_.begin(),
		nodepthVertData_.end(),
		[](auto& n) {
			n.reset();
		}
	);
	std::for_each(
		depthVertData_.begin(),
		depthVertData_.end(),
		[](auto& n) {
			n.reset();
		}
	);
}

void Line::AllDraw(bool isDepth) {
	uint32_t& drawCount = isDepth ? depthDrawCount_ : nodepthDrawCount_;
	if (drawCount == 0u) {
		return;
	}

	auto& vertData = isDepth ? depthVertData_[Lamb::GetGraphicBufferIndex()] : nodepthVertData_[Lamb::GetGraphicBufferIndex()];
	auto& pipeline = isDepth ? depthPipeline_ : nodepthPipeline_;

	auto commandList = DirectXCommand::GetMainCommandlist()->GetCommandList();

	pipeline->Use();
	commandList->SetGraphicsRootShaderResourceView(0, vertData->GetGPUVtlAdrs());
	commandList->DrawInstanced(kVertexNum, drawCount, 0, 0);

	drawCount = 0;
}

Line::Line():
	start{},
	end{},
	color(std::numeric_limits<uint32_t>::max())
{}

void Line::Debug([[maybe_unused]]const std::string& guiName) {
#ifdef USE_DEBUG_CODE
	ImGui::Begin(guiName.c_str());
	ImGui::DragFloat3("start", start.data(), 0.01f);
	ImGui::DragFloat3("end", end.data(), 0.01f);
	ImGui::End();
#endif // USE_DEBUG_CODE
}

void Line::Draw(const Mat4x4& viewProjection, bool isDepth) {
	uint32_t& drawCount = isDepth ? depthDrawCount_ : nodepthDrawCount_;
	assert(drawCount < kDrawMaxNumber_);
	if (not (drawCount < kDrawMaxNumber_)) [[unlikely]] {
		Lamb::Error::Code<Line>("Over Draw index", ErrorPlace);
	}

	auto& vertData = isDepth ? depthVertData_[Lamb::GetGraphicBufferIndex()] : nodepthVertData_[Lamb::GetGraphicBufferIndex()];

	auto&& colorFloat = UintToVector4(color);

	vertData->Map();
	(*vertData)[drawCount].color = colorFloat;

	Vector3 scale;
	scale.x = (end - start).Length();
	Vector3 to = (end - start).Normalize();
	Vector3 translate = start;

	(*vertData)[drawCount].wvp = Mat4x4::MakeAffin(scale, Vector3::kXIdentity, to, translate) * viewProjection;
	vertData->Unmap();

	drawCount++;
}

void Line::Draw(
	const Vector3& start, 
	const Vector3& end, 
	const Mat4x4& viewProjection, 
	uint32_t color,
	bool isDepth
) {
	uint32_t& drawCount = isDepth ? depthDrawCount_ : nodepthDrawCount_;
	assert(drawCount < kDrawMaxNumber_);
	if (not (drawCount < kDrawMaxNumber_)) [[unlikely]] {
		Lamb::Error::Code<Line>("Over Draw index", ErrorPlace);
	}

	auto& vertData = isDepth ? depthVertData_[Lamb::GetGraphicBufferIndex()] : nodepthVertData_[Lamb::GetGraphicBufferIndex()];

	auto&& colorFloat = UintToVector4(color);

	vertData->Map();
	(*vertData)[drawCount].color = colorFloat;

	Vector3 scale;
	scale.x = (end - start).Length();
	Vector3 to = (end - start).Normalize();
	Vector3 translate = start;

	(*vertData)[drawCount].wvp = Mat4x4::MakeAffin(scale, Vector3::kXIdentity, to, translate) * viewProjection;
	vertData->Unmap();

	drawCount++;
}