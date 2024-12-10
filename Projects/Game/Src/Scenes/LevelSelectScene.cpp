#include "LevelSelectScene.h"
#include "AudioManager/AudioManager.h"


#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"

#include "GameObject2D/Fade/Fade.h"
#include "Engine/Core/WindowFactory/WindowFactory.h"

#include "Game/MapManager/LevelManager.h"
#include "Game/LevelSelect/LevelSelect.h"

#include "GameScene.h"
#include "TitleScene.h"

#include "Engine/Graphics/TextureManager/TextureManager.h"

LevelSelectScene::LevelSelectScene() {
	levelSelecter_ = LevelSelect::GetInstance();
}

LevelSelectScene::~LevelSelectScene() {
}

void LevelSelectScene::OnEnter() {
	// 背景
	background_.Load("Resources/background.png");
	background_.pos.z = 10.0f;
	background_.scale = WindowFactory::GetInstance()->GetWindowSize();


	Fade::GetInstance()->Start(Vector2::kZero, 0x00000000, 10);


	audioManager_->Load("Resources/Sound/SE/choice.mp3");
	selectSE_ = audioManager_->Get("Resources/Sound/SE/choice.mp3");

	audioManager_->Load("./Resources/Sound/BGM/title_stageSelect.mp3");
	bgm_ = audioManager_->Get("./Resources/Sound/BGM/title_stageSelect.mp3");
	if (bgm_) {
		bgm_->Start(0.3f, true);
	}

	// HUD読み込み
	// 移動
	/*moveButton_.Initialize();
	moveButton_.Load("Resources/UI/moveKey.png");
	moveButton_.scale = Vector2::kIdentity * 128.f;
	moveButton_.pos = Vector2{ 551.f, 285.f };*/

	/*moveKey_.Initialize();
	moveKey_.Load("Resources/UI/wasd.png");
	moveKey_.scale = Vector2::kIdentity * 128.f;
	moveKey_.pos = Vector2{ 551.f, 285.f };*/

	// キー移動
	for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
		keySprite_[i].Load("Resources/UI/wasd.png");
		keySprite_[i].scale = Vector2::kIdentity * 128.f;
		keySprite_[i].pos = Vector2{ 551.f, 285.f };
		keySprite_[i].uvSize = Vector2{ 1.f / 4.f, 1.f / 2.f };
		keySprite_[i].uvPibot = Vector2{ i / 4.f,0.f, };
	}

	for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
		dpadSprite_[i].Load("Resources/UI/moveKey.png");
		dpadSprite_[i].scale = Vector2::kIdentity * 128.f;
		dpadSprite_[i].pos = Vector2{ 551.f, 285.f };
		dpadSprite_[i].uvSize = Vector2{ 1.f / 4.f, 1.f / 2.f };
		dpadSprite_[i].uvPibot = Vector2{ i / 4.f,0.f, };
	}

	// 戻る
	backButton_.Load("Resources/UI/goTitle_Button.png");
	backButton_.scale = Vector2::kIdentity * 128.f;
	backButton_.pos = Vector2{ -547.f, 285.f };
	backButton_.uvSize.y = 0.5f;

	backKey_.Load("Resources/UI/goTitleESC.png");
	backKey_.scale = Vector2::kIdentity * 128.f;
	backKey_.pos = Vector2{ -547.f, 285.f };
	backKey_.uvSize.y = 0.5f;

	// 決定
	buttonSprite_.Load("Resources/UI/A.png");
	buttonSprite_.scale = Vector2::kIdentity * 128.f;
	buttonSprite_.pos = Vector2{ 384.f, 277.f };
	buttonSprite_.uvSize.y = 0.5f;

	sapceSprite_.Load("Resources/UI/space.png");
	sapceSprite_.scale = Vector2::kIdentity * 160.f;
	sapceSprite_.pos = Vector2{ 384.f, 297.f };
	sapceSprite_.uvSize.y = 0.5f;

	auto textureManager = TextureManager::GetInstance();
	// このフレームで画像読み込みが発生していたらTextureをvramに送る
	textureManager->UploadTextureData();
	// dramから解放
	textureManager->ReleaseIntermediateResource();

	camera_.pos.z = -10.0f;
}

void LevelSelectScene::OnExit() {
	if (bgm_) {
		bgm_->Stop();
	}
}

void LevelSelectScene::Update(const float) {
	if (Gamepad::GetInstance()->PushAnyKey()) {
		isPad_ = true;
	}
	else if (KeyInput::GetInstance()->PushAnyKey() || Mouse::GetInstance()->PushAnyKey()) {
		isPad_ = false;
	}
#ifdef  _DEBUG
	/*moveButton_.Debug("moveHud");
	moveKey_.Debug("movehud_Key");
	backButton_.Debug("backHud");
	backKey_.Debug("buckHud_Key");
	buttonSprite_.Debug("choiceHud");
	sapceSprite_.Debug("choiceHud_key");
	ImGui::Begin("isPad");
	ImGui::Checkbox("flg", &isPad_);
	ImGui::End();*/
#endif //  _DEBUG


	sapceSprite_.uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_SPACE) || KeyInput::GetInstance()->GetKey(DIK_RETURN));
	buttonSprite_.uvPibot.y = 0.5f * Gamepad::GetInstance()->GetButton(Gamepad::Button::A);
	backKey_.uvPibot.y = 0.5f * KeyInput::GetInstance()->GetKey(DIK_ESCAPE);
	backButton_.uvPibot.y = 0.5f * Gamepad::GetInstance()->GetButton(Gamepad::Button::BACK);

	keySprite_[static_cast<int32_t>(KeyTexture::W)].uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_W) || KeyInput::GetInstance()->GetKey(DIK_UP));
	keySprite_[static_cast<int32_t>(KeyTexture::S)].uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_S) || KeyInput::GetInstance()->GetKey(DIK_DOWN));
	keySprite_[static_cast<int32_t>(KeyTexture::A)].uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_A) || KeyInput::GetInstance()->GetKey(DIK_LEFT));
	keySprite_[static_cast<int32_t>(KeyTexture::D)].uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_D) || KeyInput::GetInstance()->GetKey(DIK_RIGHT));

	dpadSprite_[static_cast<int32_t>(KeyTexture::W)].uvPibot.y = 0.5f * (Gamepad::GetInstance()->GetButton(Gamepad::Button::UP)
		|| KeyInput::GetInstance()->GetKey(DIK_UP) || KeyInput::GetInstance()->GetKey(DIK_W)
		|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y > 0.6f));
	dpadSprite_[static_cast<int32_t>(KeyTexture::S)].uvPibot.y = 0.5f * (Gamepad::GetInstance()->GetButton(Gamepad::Button::DOWN)
		|| KeyInput::GetInstance()->GetKey(DIK_DOWN) || KeyInput::GetInstance()->GetKey(DIK_S)
		|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y < -0.6f));
	dpadSprite_[static_cast<int32_t>(KeyTexture::D)].uvPibot.y = 0.5f * (Gamepad::GetInstance()->GetButton(Gamepad::Button::RIGHT)
		|| KeyInput::GetInstance()->GetKey(DIK_RIGHT) || KeyInput::GetInstance()->GetKey(DIK_D)
		|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x > 0.6f));
	dpadSprite_[static_cast<int32_t>(KeyTexture::A)].uvPibot.y = 0.5f * (Gamepad::GetInstance()->GetButton(Gamepad::Button::LEFT)
		|| KeyInput::GetInstance()->GetKey(DIK_LEFT) || KeyInput::GetInstance()->GetKey(DIK_A)
		|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x < -0.6f));




	if (Gamepad::GetInstance()->Pushed(Gamepad::Button::BACK) || KeyInput::GetInstance()->Pushed(DIK_ESCAPE)) {
		sceneManager_->ChangeScene(Vector2::kZero, 0x000000FF, new TitleScene, 20);
	}

	auto *const levelManager = LevelManager::GetInstance();
	camera_.Update();

	levelSelecter_->Update();

	if (!Fade::GetInstance()->GetTimer()->IsActive()) {
		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::A) ||
			KeyInput::GetInstance()->Pushed(DIK_SPACE) || KeyInput::GetInstance()->Pushed(DIK_RETURN)) {
			selectSE_->Start(1.f, false);
			const int32_t targetLevel = levelSelecter_->GetTargetLevel();
			sceneManager_->ChangeScene(Vector2::kZero, 0x000000FF, new GameScene{ levelManager->GetLevelName(targetLevel).c_str() }, 20);
		}
	}
}

void LevelSelectScene::Draw() {
	const Mat4x4 &cameraMat = camera_.GetViewOthographics();

	background_.Draw(cameraMat);
	levelSelecter_->Draw(cameraMat);

	if (isPad_) {
		backButton_.Draw(cameraMat);
		buttonSprite_.Draw(cameraMat);

		for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
			dpadSprite_[i].Draw(cameraMat);
		}
	}
	else {
		backKey_.Draw(cameraMat);
		sapceSprite_.Draw(cameraMat);


		for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
			keySprite_[i].Draw(cameraMat);
		}
	}

	Fade::GetInstance()->Draw();
}
