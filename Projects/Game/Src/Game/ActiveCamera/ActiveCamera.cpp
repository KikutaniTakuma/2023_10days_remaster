#include "ActiveCamera.h"
#include "Utils/Camera/Camera.h"
#include "Utils/SoLib/SoLib_Timer.h"

void ActiveCamera::Init() {
	camera_ = std::make_unique<Camera>(Camera::Type::Othographic);
	camera_->pos = Vector3::kZIdentity * -10.0f;
	timer_ = std::make_unique<SoLib::Timer>();
}

void ActiveCamera::Reset() {
	targetPos_ = {};
	targetScale_ = 1.f;

	camera_->pos = Vector3::kZIdentity * -10.0f;
	camera_->drawScale = 1.f;

	timer_->Clear();
}

void ActiveCamera::Update() {
	if (timer_->Update()) {	// 更新中だけ走る
		const float t = easeFunc_(timer_->GetProgress());

		// カメラ位置を変更
		camera_->pos = Vector3{
			std::lerp(originalPos_.x, targetPos_.x, t),
			std::lerp(originalPos_.y, targetPos_.y, t),
			originalPos_.z
		};

		// 倍率を変更
		camera_->drawScale = std::lerp(originalScale_, targetScale_, t);

		if (timer_->IsFinish()) {	// ゴール到達時にのみ走る
			camera_->pos = targetPos_;			// 画面位置を設定
			camera_->pos.z = -10.0f;			// zは固定
			camera_->drawScale = targetScale_;	// 倍率を変更
			timer_->Clear();	// タイマーの初期化
		}
	}
	camera_->Update();
}

void ActiveCamera::Start(const Vector3 &targetPos, float targetScale, uint32_t goalFlame) {

	// 目的地と、現在地を設定
	targetPos_ = targetPos;
	originalPos_ = camera_->pos;

	// 目的倍率と、現在倍率を設定
	targetScale_ = targetScale;
	originalScale_ = camera_->drawScale;

	// タイマーの実行
	timer_->Start(goalFlame);
}

void ActiveCamera::Clear() {

}

const Mat4x4 &ActiveCamera::GetViewOthographics() const {
	return camera_->GetViewOthographics();
}
