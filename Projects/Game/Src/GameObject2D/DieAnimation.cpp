#include "DieAnimation.h"

#include "Game/Map/Map.h"
#include <imgui.h>
#include "Game/ActiveCamera/ActiveCamera.h"

void DieAnimation::Init() {
	playerSprite_.Load("Resources/dead.png");
	playerSprite_.scale = Vector2::kIdentity * Map::GetTileSize();

	emitter_.Init();
	emitter_.SetMaxLifeSpan(kEmitterTime_ + kZoomTimer_);

	emitter_.SetSpawnLate(0.125f);

	emitter_.SetTextureName("Resources/whiteHeart.png");
	emitter_.SetParticleAngle(90.f * Angle::Dig2Rad);
	emitter_.SetUncertaintyAngle(80.f * Angle::Dig2Rad);
	emitter_.SetParticleSpeed({ 2000.f,5000.f });

	emitter_.SetParticleSize({ 5.f,25.f });
	emitter_.SetParticleLifeSpan({ 1.f,2.f });

	emitter_.SetSpawnCount({ 3,5 });

	emitter_.colorStart_ = 0xffffffc9;
	emitter_.colorEnd_ = 0xffffff00;
}

void DieAnimation::Update(float deltaTime) {
	animateValue_ += deltaTime;

	emitter_.Update(deltaTime);
	if (animateValue_ > kMenuPop_) {
		isAnimateComplete_ = true;
	}
}

void DieAnimation::Start(const Vector2 &playerPos) {
	if (isActive_ == false) {
		isActive_ = true;
		emitter_.Reset();
		emitter_.spawnArea_.origin_ = playerPos + Vector2{ 0.f,-14.f };
		playerSprite_.pos = playerPos;

		const auto flameRate = ImGui::GetIO().Framerate;
		actCamera_->Start(playerPos / 2.f, 1.f / 2.f, static_cast<int32_t>(kZoomTimer_ * flameRate));
	}
}

void DieAnimation::ImGuiWidget() {

#ifdef _DEBUG

	if (ImGui::TreeNode("dieEmitter")) {
		emitter_.ImGuiWidget();
		ImGui::TreePop();
	}

#endif // _DEBUG

}

void DieAnimation::Draw(const Mat4x4 &cameraMat) {
	if (animateValue_ <= kLifeTime_ + kZoomTimer_) {
		playerSprite_.Draw(cameraMat);
	}
	emitter_.Draw(cameraMat);
}
