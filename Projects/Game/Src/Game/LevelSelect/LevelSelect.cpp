#include "LevelSelect.h"

#include <algorithm>

#include "GameObject2D/Fade/Fade.h"

#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"

#include "Utils/SoLib/SoLib_ImGui.h"

#include "AudioManager/AudioManager.h"

#include "Game/MapManager/LevelManager.h"

void LevelSelect::Init() {

	levelTitle_.Load("Resources/UI/stageLabel.png");
	levelTitle_.pos = Vector2{ 0.f,260.f };
	levelTitle_.scale = Vector2{ 256.f,128 } *1.5f;

	levelTitle_.uvSize.x = 1.f / kLevelCount_;


	for (uint32_t i = 0u; i < kLevelCount_; ++i) {
		levelIcon_[i].Load("Resources/UI/stageIcon.png");
		levelIcon_[i].uvPibot.x = 1.f / kLevelCount_ * i;
		levelIcon_[i].uvSize = Vector2{ 1.f / kLevelCount_, 0.5f };
		levelIcon_[i].scale = Vector2::kIdentity * defaultIconScale_;
	}

	auto *const audioManager = AudioManager::GetInstance();
	audioManager->Load("Resources/Sound/SE/walk.wav");
	moveSE_ = audioManager->Get("Resources/Sound/SE/walk.wav");

	texMaxSize_ = Vector2::kIdentity * defaultIconScale_ * 1.05f;
	texMinSize_ = Vector2::kIdentity * defaultIconScale_ * 1.0f;

	isMinas_ = false;
	easeT = 0.0f;
	easeSpd = 5.0f;

	for (size_t i = 0; i < clearIcon_.size(); i++) {
		clearIcon_[i].Load("Resources/UI/heart.png");
		clearIcon_[i].scale = Vector2::kIdentity * defaultIconScale_ * 0.6f;
	}
}

void LevelSelect::Update() {

	SoLib::ImGuiWidget("iconOrigin", &iconOrigin_);
	SoLib::ImGuiWidget("iconRange", &iconRange_);
	SoLib::ImGuiWidget("levelTitle", &levelTitle_.pos);

	// フェードが動いていない場合は実行
	if (!Fade::GetInstance()->GetTimer()->IsActive()) {

#pragma region カーソル移動

		if (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT, 0.5f).LengthSQ() != 0.0f
			&& Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT, 0.5f).LengthSQ() != 0.0f) {
			isStick_ = false;
		}

		bool isMoveing = false;

		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::UP)
			|| KeyInput::GetInstance()->Pushed(DIK_UP) || KeyInput::GetInstance()->Pushed(DIK_W)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y > 0.0f && !isStick_)
			) {
			selectTarget_ -= kWidthCount_;
			isMoveing = true;
			isStick_ = true;
		}
		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::DOWN)
			|| KeyInput::GetInstance()->Pushed(DIK_DOWN) || KeyInput::GetInstance()->Pushed(DIK_S)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y < -0.0f && !isStick_)
			) {
			selectTarget_ += kWidthCount_;
			isMoveing = true;
			isStick_ = true;
		}

		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::LEFT)
			|| KeyInput::GetInstance()->Pushed(DIK_LEFT) || KeyInput::GetInstance()->Pushed(DIK_A)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x < 0.0f && !isStick_)
			) {
			selectTarget_ += -1;
			isMoveing = true;
			isStick_ = true;
		}
		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::RIGHT)
			|| KeyInput::GetInstance()->Pushed(DIK_RIGHT) || KeyInput::GetInstance()->Pushed(DIK_D)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x > 0.0f && !isStick_)
			) {
			selectTarget_ += +1;
			isMoveing = true;
			isStick_ = true;
		}

		selectTarget_ = std::clamp(selectTarget_, 0, kLevelCount_ - 1);

		if (isMoveing) {
			moveSE_->Start(1.f, false);
		}

#pragma endregion

	}

	// 座標の計算
	const float centorX = (kWidthCount_ - 1.f) / 2.f;
	const float centorY = (std::ceil(1.f * kLevelCount_ / kWidthCount_) - 1.f) / 2.f;
	for (uint32_t i = 0u; i < kLevelCount_; ++i) {
		levelAnchor_[i].x = ((i % kWidthCount_) - centorX) * iconRange_.x;
		levelAnchor_[i].y = (std::floor(i * 1.f / kWidthCount_) - centorY) * -iconRange_.y;
	}

	for (int32_t i = 0; i < kLevelCount_; ++i) {
		bool isSelected = (i == selectTarget_);
		levelIcon_[i].uvPibot.y = isSelected * 0.5f;
	}

	levelTitle_.uvPibot.x = selectTarget_ * 1.f / kLevelCount_;

	for (uint32_t i = 0u; i < kLevelCount_; ++i) {
		levelIcon_[i].pos = levelAnchor_[i] + iconOrigin_;
	}


	// 演出
	currentTexSize = Vector2::Lerp(texMinSize_, texMaxSize_, SoLib::easeInExpo(easeT));
	if (!isMinas_) {
		easeT += easeSpd * ImGui::GetIO().DeltaTime;
	}
	else {
		easeT -= easeSpd * ImGui::GetIO().DeltaTime;
	}

	if (easeT >= 1.0f) {
		easeT = 1.0f;
		isMinas_ = true;
	}
	else if(easeT <= 0.0f) {
		easeT = 0.0f;
		isMinas_ = false;
	}

	for (auto& i : levelIcon_) {
		i.scale = currentTexSize;
	}

	for (size_t i = 0; i < clearIcon_.size(); i++) {
		clearIcon_[i].pos = levelAnchor_[i] + (texMinSize_ * 0.4f) + iconOrigin_;
		clearIcon_[i].scale = currentTexSize * 0.6f;
	}
}

void LevelSelect::Draw(const Mat4x4 &viewProjection) {

	for (auto &icon : levelIcon_) {
		icon.Draw(viewProjection);
	}
	levelTitle_.Draw(viewProjection);

	for (size_t i = 0; i < clearIcon_.size(); i++) {
		if (LevelManager::GetInstance()->GetLevelList()[i]->isClear_) {
			clearIcon_[i].Draw(viewProjection);
		}
	}
}

void LevelSelect::AddTargetLevel(int32_t value) {
	selectTarget_ = std::clamp(selectTarget_ + value, 0, kLevelCount_ - 1);
}
