#include "Emitter.h"
#include <imgui.h>
#include <cmath>

#include "Particle.h"
#include "Utils/Math/Angle.h"
#include "Utils/SoLib/SoLib_ImGui.h"
#include "GlobalVariables/GlobalVariables.h"
#include <Utils/SoLib/SoLib_Easing.h>


void Emitter::Init() {
}

void Emitter::Reset() {
	lifeSpan_ = vMaxLifeSpan_;
	isActive_ = true;
}

void Emitter::Update(const float deltaTime) {
	// 生きていたら実行
	if (isActive_) {
		// 1フレ前の寿命
		const float beforeLifeSpan = lifeSpan_;
		lifeSpan_ -= deltaTime;	// 減算
		// 0を下回った場合無効化
		if (lifeSpan_ <= 0.f && vMaxLifeSpan_.GetItem() > 0.f) {
			// 生存の無効化
			isActive_ = false;
		}

		// もし、生成レートをまたいだ場合は実行
		if (std::floor(beforeLifeSpan / vSpawnLate_) - std::floor(lifeSpan_ / vSpawnLate_) >= 1.f) {
			PopParticle();
		}
	}
	particleList_.remove_if([](auto &particle) {
		return !particle->GetIsAlive();
		}
	);

	// 子として持つパーティクルを更新
	for (auto &particle : particleList_) {
		particle->Update(deltaTime);
	}
}

void Emitter::Draw(const Mat4x4 &cameraMat) {
	// 子として持つパーティクルを描画
	for (auto &particle : particleList_) {
		particle->Draw(cameraMat);
	}
}

void Emitter::PopParticle() {

	int32_t spawnValue = vSpawnCount_->Random();
	for (int32_t i = 0; i < spawnValue; ++i) {
		Particle *const newParticle = SpawnParticle();

		// リストへの追加
		newParticle->Reset();
		particleList_.emplace_back(newParticle);
	}
}

void Emitter::ApplyVariables(const GlobalVariables &gVariable, const char *const groupName) {
	groupName_ = groupName;
	auto &group = gVariable.GetGroup(groupName_);
	group >> vMaxLifeSpan_;
	group >> vSpawnLate_;

	group >> vParticleAngle_;
	group >> vUncertaintyAngle_;

	group >> vParticleSpeed_;
	group >> vParticleSize_;
	group >> vParticleLifeSpan_;
	group >> vSpawnCount_;
}

void Emitter::AddVariable(GlobalVariables &gVariable) const {
	gVariable.AddItem(groupName_, vMaxLifeSpan_);
	gVariable.AddItem(groupName_, vSpawnLate_);

	gVariable.AddItem(groupName_, vParticleAngle_);
	gVariable.AddItem(groupName_, vUncertaintyAngle_);

	gVariable.AddItem(groupName_, vParticleSpeed_);
	gVariable.AddItem(groupName_, vParticleSize_);
	gVariable.AddItem(groupName_, vParticleLifeSpan_);

	gVariable.AddItem(groupName_, vSpawnCount_);
}

void Emitter::ImGuiWidget() {

#ifdef _DEBUG
	static bool isPress = false;
	bool buttonPress = ImGui::Button("popParticle");
	if (buttonPress && !isPress) {
		PopParticle();
	}
	isPress = buttonPress;

	SoLib::ImGuiWidget("isActive", &isActive_);	// 生存の有無
	SoLib::ImGuiWidget(&vMaxLifeSpan_);			// 最大寿命
	SoLib::ImGuiWidget("lifeSpan", &lifeSpan_);	// 現在寿命
	SoLib::ImGuiWidget(&vSpawnLate_);			// 生成レート

	SoLib::ImGuiWidgetAngle(vParticleAngle_.GetKey().c_str(), &vParticleAngle_.GetItem(), -180.f, 180.f);	// パーティクル発射角度
	SoLib::ImGuiWidgetAngle(vUncertaintyAngle_.GetKey().c_str(), &vUncertaintyAngle_.GetItem(), 0.f, 180.f);	// パーティクルのブレ

	SoLib::ImGuiWidget(&vParticleSpeed_);		// パーティクルの速度
	SoLib::ImGuiWidget(&vParticleSize_);		// パーティクルのサイズ
	SoLib::ImGuiWidget("uvOffset", &uvOffset_);	// パーティクルのUVオフセット
	SoLib::ImGuiWidget(&vParticleLifeSpan_);	// パーティクルの寿命

	if (ImGui::TreeNode("spawnArea")) {
		spawnArea_.ImGuiWidget();
		ImGui::TreePop();
	}

	Vector4 startColor = UintToVector4(colorStart_);
	if (ImGui::ColorEdit4("colorStart", &startColor.color.r)) {
		colorStart_ = Vector4ToUint(startColor);
	}

	SoLib::ImGuiWidget("centorProgress", &centorProgress_);

	Vector4 middleColor = UintToVector4(colorMiddle_);
	if (ImGui::ColorEdit4("colorMiddle", &middleColor.color.r)) {
		colorMiddle_ = Vector4ToUint(middleColor);
	}

	Vector4 endColor = UintToVector4(colorEnd_);
	if (ImGui::ColorEdit4("colorEnd", &endColor.color.r)) {
		colorEnd_ = Vector4ToUint(endColor);
	}

	SoLib::ImGuiWidget(&vSpawnCount_);

#endif // _DEBUG

}

void Emitter::SetMaxLifeSpan(const float lifeSpan) {
	vMaxLifeSpan_ = lifeSpan;
}

void Emitter::SetSpawnLate(const float spawnLate) {
	vSpawnLate_ = spawnLate;
}

void Emitter::CalcCentor(float t) {
	colorMiddle_ = SoLib::ColorLerp(colorStart_, colorEnd_, t);
}

Particle *const Emitter::SpawnParticle() const {
	// 実体の生成
	Particle *const newParticle = new Particle{};
	newParticle->Init();

	// テクスチャの設定
	newParticle->SetSprite(fileName_, Vector2::kIdentity * vParticleSize_->Random());
	//newParticle->SetUvOffset(uvOffset_.Random<int32_t>() * 64.f / 512.f);
	//newParticle->SetUvSize(Vector2{ 64.f,64.f } / 512.f);

	// ベクトルなどの設定
	newParticle->SetPosition(spawnArea_.GetRandom());
	Vector2 acceleration = Angle::RadToVec2(vParticleAngle_ + Lamb::Random(-vUncertaintyAngle_, vUncertaintyAngle_.GetItem())) * vParticleSpeed_->Random();
	newParticle->AddAcceleration(acceleration);

	// 色の設定
	newParticle->SetColorStart(colorStart_);
	newParticle->SetColorMiddle(colorMiddle_);
	newParticle->SetColorEnd(colorEnd_);
	newParticle->centorProgress_ = centorProgress_;

	// 寿命の設定
	newParticle->SetMaxLifeSpan(vParticleLifeSpan_->Random());

	return newParticle;
}

Vector2 Emitter::Capsule::GetRandom() const {
	Vector2 result;
	// 極座標からベクタ型を生成
	float angle = Lamb::Random(-180.f, 180);
	result = Angle::RadToVec2(angle) * Lamb::Random(0.f, radius_);

	// diffの乱数を生成
	result += diff_ * Lamb::Random(0.f, 1.f);

	// 原点の分移動
	result += origin_;

	return result;
}

void Emitter::Capsule::ImGuiWidget() {
	SoLib::ImGuiWidget("Origin", &origin_);	// 現在位置
	SoLib::ImGuiWidget("Diff", &diff_);	// 差分
	SoLib::ImGuiWidget("Radius", &radius_);	// 差分
}
