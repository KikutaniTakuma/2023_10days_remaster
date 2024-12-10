#include "Fade.h"

#include "Engine/Core/WindowFactory/WindowFactory.h"
#include "Utils/Camera/Camera.h"

void Fade::StaticInit() {
	auto *const instance = GetInstance();
	instance->timer_ = std::make_unique<SoLib::Timer>();
	instance->timer_->Clear();

	instance->sprite_ = std::make_unique<Tex2DInstance>();
	instance->sprite_->Load("Resources/EngineResources/white2x2.png");
	instance->sprite_->scale = WindowFactory::GetInstance()->GetWindowSize();

	auto cameraBuff = Camera{ Camera::Type::Othographic };
	cameraBuff.Update();

	instance->cameraMat_ = std::make_unique<Mat4x4>(cameraBuff.GetViewOthographics());

}

void Fade::SetState(const Vector2 &pos, uint32_t color) {
	sprite_->pos = pos;
	sprite_->color = kFadeColor_ | (0xFF & color);
}

void Fade::Start(const Vector2 &targetPos, uint32_t targetAlpha, int32_t goalTime) {

	// 目的地と、現在地を設定
	targetPos_ = targetPos;
	originalPos_ = Vector2{ sprite_->pos.x,sprite_->pos.y };

	// 目的colorと、現在colorを設定
	targetColor_ = kFadeColor_ | (0xFF & targetAlpha);
	originalColor_ = kFadeColor_ | (0xFF & sprite_->color);

	// タイマーの実行
	timer_->Start(goalTime);
}

void Fade::Update() {
	if (timer_->Update()) {	// 更新中だけ走る
		const float t = easeFunc_(timer_->GetProgress());

		// 画像位置を変更
		sprite_->pos = Vector2{
			std::lerp(originalPos_.x, targetPos_.x, t),
			std::lerp(originalPos_.y, targetPos_.y, t)
		};

		// colorを変更
		sprite_->color = SoLib::ColorLerp(originalColor_, targetColor_, t);

		if (timer_->IsFinish()) {	// ゴール到達時にのみ走る
			sprite_->pos = targetPos_;			// 画像位置を設定
			sprite_->color = targetColor_;	// colorを変更
			timer_->Clear();	// タイマーの初期化
		}
	}
}

void Fade::Draw() {
	sprite_->Draw(*cameraMat_);
}
