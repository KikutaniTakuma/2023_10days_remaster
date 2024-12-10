/// ==================================
/// ==  AnimationModelクラスの宣言  ==
/// ==================================

#pragma once
#include "../Model/Model.h"
#include "Engine/Graphics/Animator/Animator.h"
#include "Engine/Graphics/Skeleton.h"

#include <memory>

/// <summary>
/// スキンアニメーションの描画クラス
/// </summary>
class AnimationModel : public Model {

public:
	AnimationModel() = default;
	AnimationModel(const std::string& fileName);
	AnimationModel(const AnimationModel&) = default;
	AnimationModel(AnimationModel&&) = default;
	virtual ~AnimationModel() = default;

	AnimationModel& operator=(const AnimationModel&) = default;
	AnimationModel& operator=(AnimationModel&&) = default;

public:
	virtual void Load(const std::string& fileName) override;

	virtual void Update();

	virtual void Draw(
		const Mat4x4& worldMatrix,
		const Mat4x4& camera,
		uint32_t color,
		BlendType blend,
		bool isLighting
	) override;

public:
	inline Animator& GetAnimator() {
		return *animator_;
	}

private:
	std::unique_ptr<Animator> animator_;

	std::unique_ptr<Skeleton> skeleton_;
	std::unique_ptr<SkinCluster> skinCluster_;
};