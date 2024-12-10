#include "LevelResult.h"
#include "Game/LevelSelect/LevelSelect.h"
#include <Utils/Camera/Camera.h>
#include "Scenes/SceneManager.h"

#include <Scenes/GameScene.h>
#include <Scenes/LevelSelectScene.h>
#include "Game/MapManager/LevelManager.h"

#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"
#include <Utils/SoLib/SoLib_ImGui.h>

#include "GameObject2D/Fade/Fade.h"

#include "AudioManager/AudioManager.h"

LevelResult::LevelResult() {
	levelSelecter_ = LevelSelect::GetInstance();
}

void LevelResult::Init() {

	Camera camera{ Camera::Type::Othographic };
	camera.pos.z = -10.0f;
	camera.Update();

	selectButton_ = static_cast<int32_t>(ButtonID::NextLevel);

	// ステージセレクト
	buttonFunc_[static_cast<int32_t>(ButtonID::LevelSelect)] = []() {SceneManager::GetInstance()->ChangeScene(Vector2::kZero, 0xFF, new LevelSelectScene, 30); };

	// リトライ
	buttonFunc_[static_cast<int32_t>(ButtonID::Retry)] = []() {SceneManager::GetInstance()->ChangeScene(Vector2::kZero, 0xFF, new GameScene{ LevelManager::GetInstance()->GetLevelName(LevelSelect::GetInstance()->GetTargetLevel()).c_str() }, 15); };
	// 次のステージへ
	buttonFunc_[static_cast<int32_t>(ButtonID::NextLevel)] = []()
		{
			LevelSelect::GetInstance()->AddTargetLevel(1u);
			SceneManager::GetInstance()->ChangeScene(Vector2::kZero, 0xFF, new GameScene{ LevelManager::GetInstance()->GetLevelName(LevelSelect::GetInstance()->GetTargetLevel()).c_str() }, 15);
		};

	cameraMat_ = camera.GetViewOthographics();

	background_.Load("Resources/UI/resultBuckground.png");
	background_.scale = Vector2{ 896.f,256.f };
	background_.pos = origin_;

	for (auto &button : buttonSprite_) {
		button = std::make_unique<Tex2DInstance>();
	}
	buttonSprite_[static_cast<int32_t>(ButtonID::LevelSelect)]->Load("Resources/UI/resultStageSelect.png");
	buttonSprite_[static_cast<int32_t>(ButtonID::Retry)]->Load("Resources/UI/resultRetry.png");
	buttonSprite_[static_cast<int32_t>(ButtonID::NextLevel)]->Load("Resources/UI/resultNext.png");

	for (int32_t i = 0; i < kButtonCount_; ++i) {
		buttonSprite_[i]->scale = buttonSprite_[i]->GetTexSize();
		buttonSprite_[i]->scale.x *= 0.5f;
		buttonSprite_[i]->uvSize.x = 0.5f;

		float offsetX = i - (kButtonCount_ - 1.f) / 2.f;
		buttonSprite_[i]->pos = origin_ + Vector2{ offsetX * posRadius_ ,0.f };
	}

	AudioManager::GetInstance()->Load("./Resources/Sound/SE/walk.mp3");
	AudioManager::GetInstance()->Load("./Resources/Sound/SE/choice.mp3");
	cursorMoveSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/walk.mp3");
	choiseSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/choice.mp3");
}

void LevelResult::Update() {

	SoLib::ImGuiWidget("origin", &origin_);
	SoLib::ImGuiWidget("buttonPos", &posRadius_);

	if (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT, 0.5f).LengthSQ() != 0.0f) {
		isStick_ = false;
	}

	if (Gamepad::GetInstance()->Pushed(Gamepad::Button::LEFT) || (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT, 0.5f).x < 0.0f && !isStick_)
		|| KeyInput::GetInstance()->Pushed(DIK_A) || KeyInput::GetInstance()->Pushed(DIK_LEFT)) {
		--selectButton_;
		cursorMoveSe_->Start(0.4f, false);
		isStick_ = true;
	}
	if (Gamepad::GetInstance()->Pushed(Gamepad::Button::RIGHT) || (0.0f < Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT, 0.5f).x && !isStick_)
		|| KeyInput::GetInstance()->Pushed(DIK_D) || KeyInput::GetInstance()->Pushed(DIK_RIGHT)) {
		++selectButton_;
		cursorMoveSe_->Start(0.4f, false);
		isStick_ = true;
	}
	selectButton_ = std::clamp(selectButton_, 0, drawCount_ - 1);


	background_.pos = origin_;

	for (int32_t i = 0; i < drawCount_; ++i) {

		float offsetX = (i - (drawCount_ - 1.f) / 2.f) * (static_cast<float>(kButtonCount_) / drawCount_);
		buttonSprite_[i]->pos = origin_ + Vector2{ offsetX * posRadius_ ,0.f };
		bool isSelect = selectButton_ == i;
		buttonSprite_[i]->uvPibot.x = 0.5f * isSelect;
	}

	if (!isChoice_ &&
		(Gamepad::GetInstance()->Pushed(Gamepad::Button::A)
		|| KeyInput::GetInstance()->Pushed(DIK_SPACE) || KeyInput::GetInstance()->Pushed(DIK_RETURN))) {
		Fade::GetInstance()->SetEaseFunc(SoLib::easeOutQuad);
		buttonFunc_[selectButton_]();
		choiseSe_->Start(1.0f, false);
		isChoice_ = true;
	}

}

void LevelResult::Draw() {
	background_.pos.z = 2.0f;
	background_.blend = BlendType::kUnenableDepthNormal;
	background_.Draw(cameraMat_);
	for (int32_t i = 0; i < drawCount_; ++i) {
		buttonSprite_[i]->pos.z = 3.0f;
		buttonSprite_[i]->blend = BlendType::kUnenableDepthNormal;
		buttonSprite_[i]->Draw(cameraMat_);
	}
}
