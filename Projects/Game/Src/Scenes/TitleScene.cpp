#include "TitleScene.h"
#include "AudioManager/AudioManager.h"

#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"

#include "GameObject2D/Fade/Fade.h"
#include "Utils/Math/Angle.h"

#include "GameScene.h"
#include "LevelSelectScene.h"

#include "Engine/Graphics/TextureManager/TextureManager.h"
#include <Utils/SoLib/SoLib_ImGui.h>

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::OnEnter() {
	titleSprite_.Load("Resources/UI/title.png");
	Vector2 texSize = titleSprite_.GetTexSize();
	titleSprite_.scale = { texSize.x / 4.f,texSize.y };
	titleSprite_.uvSize.x = 1.f / 4.f;


	buttonSprite_.Load("Resources/UI/A.png");
	buttonSprite_.scale = Vector2::kIdentity * 128.f;
	buttonSprite_.pos = Vector2{ 0.f, -272.f };
	buttonSprite_.uvSize.y = 0.5f;

	sapceSprite_.Load("Resources/UI/space.png");
	sapceSprite_.scale = Vector2::kIdentity * 160.f;
	sapceSprite_.pos = Vector2{ 0.f, -260.f };
	sapceSprite_.uvSize.y = 0.5f;

	escSprite_.Load("Resources/UI/endESC.png");
	escSprite_.scale = Vector2::kIdentity * 128.f;
	escSprite_.pos = Vector2{ -547.f, -266.f };

	backButtonSprite_.Load("Resources/UI/end.png");
	backButtonSprite_.scale = Vector2::kIdentity * 128.f;
	backButtonSprite_.pos = Vector2{ -547.f, -266.f };
	backButtonSprite_.uvSize.y = 0.5f;

	Fade::GetInstance()->Start(Vector2::kZero, 0x00000000, 10);



	auto cameraBuff = Camera{ Camera::Type::Othographic };
	cameraBuff.pos.z = -10.0f;
	cameraBuff.Update();

	cameraMat_ = cameraBuff.GetViewOthographics();

	audioManager_->Load("./Resources/Sound/BGM/title_stageSelect.mp3");
	bgm_ = audioManager_->Get("./Resources/Sound/BGM/title_stageSelect.mp3");
	if (bgm_) {
		bgm_->Start(0.3f, true);
	}

	audioManager_->Load("Resources/Sound/SE/choice.mp3");
	selectSE_ = audioManager_->Get("Resources/Sound/SE/choice.mp3");

	auto textureManager = TextureManager::GetInstance();
	// このフレームで画像読み込みが発生していたらTextureをvramに送る
	textureManager->UploadTextureData();
	// dramから解放
	textureManager->ReleaseIntermediateResource();
}

void TitleScene::OnExit() {
	if (bgm_) {
		bgm_->Stop();
	}
}

void TitleScene::Update(const float deltaTime) {
	if (Gamepad::GetInstance()->PushAnyKey()) {
		isPad_ = true;
	}
	else if (KeyInput::GetInstance()->PushAnyKey() || Mouse::GetInstance()->PushAnyKey()) {
		isPad_ = false;
	}

#ifdef _DEBUG
	//buttonSprite_.Debug("buttonSprite_");

	////// キーボード
	//ImGui::Begin("isPad");
	//ImGui::Checkbox("flg", &isPad_);
	//ImGui::End();
	
#endif // _DEBUG


	//if (KeyInput::Pushed(DIK_SPACE) || KeyInput::Pushed(DIK_RETURN)) {
	sapceSprite_.uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_SPACE) || KeyInput::GetInstance()->GetKey(DIK_RETURN));
	//}
	buttonSprite_.uvPibot.y = 0.5f * Gamepad::GetInstance()->GetButton(Gamepad::Button::A);

	if (Gamepad::GetInstance()->Pushed(Gamepad::Button::A)
		|| KeyInput::GetInstance()->Pushed(DIK_SPACE) || KeyInput::GetInstance()->Pushed(DIK_RETURN)) {
		selectSE_->Start(1.0f, false);
		sceneManager_->ChangeScene(Vector2::kZero, 0x000000FF, new LevelSelectScene, 20);
	}
	animateValue_ += deltaTime;

	SoLib::ImGuiWidget("buttonPos", &buttonSprite_.pos);

	buttonSprite_.color = 0xFFFFFF00 | static_cast<uint32_t>(0xFF * (std::sin(animateValue_ * Angle::PI2) * 0.5f + 0.5f));

	sapceSprite_.color = 0xFFFFFF00 | static_cast<uint32_t>(0xFF * (std::sin(animateValue_ * Angle::PI2) * 0.5f + 0.5f));

	titleSprite_.uvPibot.x = std::floor(animateValue_ * 4.f / kAnimateTime_) * 0.25f;
	//camera_.Update();
}

void TitleScene::Draw() {
	//const Mat4x4 &cametaMat = camera_.GetViewOthographics();
	titleSprite_.Draw(cameraMat_);
	if (isPad_) {
		buttonSprite_.Draw(cameraMat_);
		backButtonSprite_.Draw(cameraMat_);

	}
	else {
		escSprite_.Draw(cameraMat_);
		sapceSprite_.Draw(cameraMat_);
	}

	Fade::GetInstance()->Draw();
}
