/// ==================================
/// ==  AnimationModelクラスの定義  ==
/// ==================================

#include "AnimationModel.h"
#include "Engine/Graphics/RenderContextManager/RenderContextManager.h"

AnimationModel::AnimationModel(const std::string& fileName) :
	AnimationModel()
{
	Load(fileName);
}

void AnimationModel::Load(const std::string& fileName) {
	Lamb::SafePtr renderContextManager = RenderContextManager::GetInstance();

	renderContextManager->LoadSkinAnimationModel<uint32_t, 1>(
		LoadFileNames{
			.resourceFileName = fileName,
			.shaderName{
				.vsFileName = "./Shaders/ModelShader/ModelAnimation.VS.hlsl",
				.psFileName = "./Shaders/ModelShader/Model.PS.hlsl",
			}
		},
		2
	);

	renderSet = renderContextManager->Get(
		LoadFileNames{
			.resourceFileName = fileName,
			.shaderName{
				.vsFileName = "./Shaders/ModelShader/ModelAnimation.VS.hlsl",
				.psFileName = "./Shaders/ModelShader/Model.PS.hlsl",
			}
		}
	);


	animator_.reset();
	animator_ = std::make_unique<Animator>();
	animator_->Load(fileName);

	skeleton_ = std::make_unique<Skeleton>(Lamb::CreateSkeleton(renderSet->GetNode()));
	skinCluster_.reset(SkinCluster::CreateSkinCluster(*skeleton_, *renderSet->GetModelData()));
}

void AnimationModel::Update() {
	animator_->Debug("animation model");
	animator_->Update(*skeleton_);
	skeleton_->Update();
	skinCluster_->Update(*skeleton_);
}

void AnimationModel::Draw(
	const Mat4x4& worldMatrix,
	const Mat4x4& camera,
	uint32_t color,
	BlendType blend,
	bool isLighting
) {
	SkinRenderContext<uint32_t, 1>* renderContext = renderSet->GetRenderContextDowncast<SkinRenderContext<uint32_t, 1>>(blend);
	renderContext->SetSkinCluster(skinCluster_.get());
	renderContext->SetShaderStruct(static_cast<uint32_t>(isLighting));

	BaseDrawer::Draw(worldMatrix, camera, color, blend);
	skeleton_->Draw(worldMatrix, camera);
}
