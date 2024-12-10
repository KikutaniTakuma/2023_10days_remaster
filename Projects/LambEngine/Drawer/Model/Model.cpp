/// =========================
/// ==  Modelクラスの定義  ==
/// =========================

#include "Model.h"
#include "Engine/Graphics/RenderContextManager/RenderContextManager.h"
#include "Engine/Graphics/RenderingManager/RenderingManager.h"
#include "Utils/SafePtr.h"
#include "../DrawerManager.h"

#include "Utils/EngineInfo.h"

Model::Model(const std::string& fileName):
	Model()
{
	Load(fileName);
}

void Model::Load(const std::string& fileName) {
	Lamb::SafePtr renderContextManager = RenderContextManager::GetInstance();

	LoadFileNames fileNames = {
		.resourceFileName = fileName,
		.shaderName{
			.vsFileName = "./Shaders/ModelShader/Model.VS.hlsl",
			.psFileName = "./Shaders/ModelShader/Model.PS.hlsl",
		}
	};

	MeshLoadFileNames meshFileNames = {
		.resourceFileName = fileName,
		.shaderName{
			.asFileName = "./Shaders/ModelShader/MeshModel.AS.hlsl",
			.msFileName = "./Shaders/ModelShader/MeshModel.MS.hlsl",
			.psFileName = "./Shaders/ModelShader/MeshModel.PS.hlsl"
		}
	};

	// メッシュシェーダーが読み込む
	if (Lamb::IsCanUseMeshShader()) {
		// リソースとメッシュシェーダー読み込み
		renderContextManager->LoadMesh<uint32_t, kMaxDrawCount>(meshFileNames, 4);
		
		meshRenderSet = renderContextManager->Get(meshFileNames);
	}

	// リソースとシェーダー読み込み
	renderContextManager->Load<RenderContext<uint32_t, kMaxDrawCount>>(fileNames, 4);

	renderSet = renderContextManager->Get(fileNames);
}

void Model::Draw(
	const Mat4x4& worldMatrix,
	const Mat4x4& camera, 
	uint32_t color, 
	BlendType blend, 
	bool isLighting
) {
#ifdef USE_DEBUG_CODE
	isUseMeshShader_ = RenderingManager::GetInstance()->GetIsUseMeshShader();
#endif // USE_DEBUG_CODE

	
	if (isUseMeshShader_ and meshRenderSet) {
		Lamb::SafePtr renderContext = meshRenderSet->GetRenderContextDowncast<MeshRenderContext<uint32_t, kMaxDrawCount>>(blend);
		if (blend == BlendType::kNone) {
			renderContext->SetShaderStruct(static_cast<uint32_t>(false));
		}
		else {
			renderContext->SetShaderStruct(static_cast<uint32_t>(isLighting));
		}
	}
	else {
		Lamb::SafePtr renderContext = renderSet->GetRenderContextDowncast<RenderContext<uint32_t, kMaxDrawCount>>(blend);
		if (blend == BlendType::kNone) {
			renderContext->SetShaderStruct(static_cast<uint32_t>(false));
		}
		else {
			renderContext->SetShaderStruct(static_cast<uint32_t>(isLighting));
		}
	}

	BaseDrawer::Draw(worldMatrix, camera, color, blend);
}

void Model::Draw(const Data& data) {
#ifdef USE_DEBUG_CODE
	isUseMeshShader_ = RenderingManager::GetInstance()->GetIsUseMeshShader();
#endif // USE_DEBUG_CODE

	if (isUseMeshShader_ and meshRenderSet) {
		Lamb::SafePtr renderContext = meshRenderSet->GetRenderContextDowncast<MeshRenderContext<uint32_t, kMaxDrawCount>>(data.blend);
		if (data.blend == BlendType::kNone) {
			renderContext->SetShaderStruct(static_cast<uint32_t>(false));
		}
		else {
			renderContext->SetShaderStruct(static_cast<uint32_t>(data.isLighting));
		}
	}
	else {
		Lamb::SafePtr renderContext = renderSet->GetRenderContextDowncast<RenderContext<uint32_t, kMaxDrawCount>>(data.blend);
		if (data.blend == BlendType::kNone) {
			renderContext->SetShaderStruct(static_cast<uint32_t>(false));
		}
		else {
			renderContext->SetShaderStruct(static_cast<uint32_t>(data.isLighting));
		}
	}

	BaseDrawer::Draw(data.worldMatrix, data.camera, data.color, data.blend);
}

const Node& Model::GetNode() const
{
	return renderSet->GetNode();
}

const ModelData& Model::GetModelData() const {
	return *renderSet->GetModelData();
}



/// 
/// ==================================================================================================================
/// 

void ModelInstance::Load(const std::string& fileName) {
	DrawerManager::GetInstance()->LoadModel(fileName);
	model_ = DrawerManager::GetInstance()->GetModel(fileName);
}

void ModelInstance::Update()
{
	worldMat_ = Mat4x4::MakeAffin(scale, rotate, pos);
	if (parent_.have()) {
		worldMat_ *= parent_->worldMat_;
	}
}

void ModelInstance::Draw(const Mat4x4& cameraMat)
{
	if (model_.have()) {
		model_->Draw(
			worldMat_,
			cameraMat,
			color,
			blend,
			isLighting
		);
	}
}

void ModelInstance::SetParent(ModelInstance* parent)
{
	parent_ = parent;
}

const Mat4x4& ModelInstance::GetWorldMatrix() const
{
	return worldMat_;
}
