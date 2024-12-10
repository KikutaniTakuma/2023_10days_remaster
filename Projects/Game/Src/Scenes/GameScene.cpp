#include "GameScene.h"

#include "AudioManager/AudioManager.h"
#include "imgui.h"

#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"

#include "Game/Map/Map.h"
#include "Engine/Core/WindowFactory/WindowFactory.h"

#include "GameObject2D/Fade/Fade.h"
#include "GameObject2D/LevelResult.h"
#include "Game/LevelSelect/LevelSelect.h"

#include "TitleScene.h"
#include "LevelSelectScene.h"
#include <Utils/SoLib/SoLib_ImGui.h>

#include "Engine/Graphics/TextureManager/TextureManager.h"

#include "Game/MapManager/LevelManager.h"

#include <sstream>

const char *const GameScene::kDefaultFileName_ = "./Resources/StageData/stage1.csv";

GameScene::GameScene(const char *const fileName) {
	// ファイルネームを保存
	fileName_ = fileName;
}

GameScene::~GameScene() {

}

void GameScene::OnEnter() {

	levelResult_ = std::make_unique<LevelResult>();
	levelResult_->Init();

	actCamera_.Init();

	// マップの生成
	map_ = std::make_unique<Map>();
	LoadLevel(fileName_.c_str());


	// 背景
	background_.Load("Resources/background.png");
	background_.pos.z = 10.0f;
	background_.scale = WindowFactory::GetInstance()->GetWindowSize();

	levelNumber_ = std::make_unique<Tex2DInstance>();
	levelNumber_->Load("Resources/UI/stageLabel.png");
	levelNumber_->pos = Vector2{ -431.f,-284.f };
	levelNumber_->pos.z = 1.0f;
	levelNumber_->scale = Vector2{ 256.f,128 } *1.5f;
	levelNumber_->uvSize.x = 1.f / LevelSelect::kLevelCount_;

	levelNumber_->uvPibot.x = (1.f / LevelSelect::kLevelCount_) * LevelSelect::GetInstance()->GetTargetLevel();

	levelSelectButton_.Load("Resources/UI/goStageSelect.png");
	levelSelectButton_.scale = Vector2::kIdentity * 128.f;
	levelSelectButton_.pos = Vector2{ -547.f, 285.f };
	levelSelectButton_.pos.z = 1.0f;
	levelSelectButton_.uvSize.y = 0.5f;

	/*moveButton_.Initialize();
	moveButton_.Load("Resources/UI/moveKey.png");
	moveButton_.scale = Vector2::kIdentity * 128.f;
	moveButton_.pos = Vector2{ 305.f, 285.f };*/

	backButton_.Load("Resources/UI/B.png");
	backButton_.scale = Vector2::kIdentity * 128.f;
	backButton_.pos = Vector2{ 440.f, 285.f };
	backButton_.pos.z = 1.0f;
	backButton_.uvSize.y = 0.5f;

	retryButton_.Load("Resources/UI/retryKey.png");
	retryButton_.scale = Vector2::kIdentity * 128.f;
	retryButton_.pos = Vector2{ 565.f, 285.f };
	retryButton_.pos.z = 1.0f;
	retryButton_.uvSize.y = 0.5f;;

	// キーボード
	levelSelectKey_.Load("Resources/UI/ESC.png");
	levelSelectKey_.scale = Vector2::kIdentity * 128.f;
	levelSelectKey_.pos = Vector2{ -547.f, 285.f };
	levelSelectKey_.pos.z = 1.0f;
	levelSelectKey_.uvSize.y = 0.5f;


	/*moveKey_.Initialize();
	moveKey_.Load("Resources/UI/wasd.png");
	moveKey_.scale = Vector2::kIdentity * 128.f;
	moveKey_.pos = Vector2{ 305.f, 285.f };*/

	backKey_.Load("Resources/UI/Z.png");
	backKey_.scale = Vector2::kIdentity * 128.f;
	backKey_.pos = Vector2{ 440.f, 285.f };
	backKey_.pos.z = 1.0f;
	backKey_.uvSize.y = 0.5f;

	retryKey_.Load("Resources/UI/R.png");
	retryKey_.scale = Vector2::kIdentity * 128.f;
	retryKey_.pos = Vector2{ 565.f, 285.f };
	retryKey_.pos.z = 1.0f;
	retryKey_.uvSize.y = 0.5f;



	// キー移動
	for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
		keySprite_[i].Load("Resources/UI/wasd.png");
		keySprite_[i].scale = Vector2::kIdentity * 128.f;
		keySprite_[i].pos = Vector2{ 305.f, 285.f };
		keySprite_[i].pos.z = 1.0f;
		keySprite_[i].uvSize = Vector2{ 1.f / 4.f, 1.f / 2.f };
		keySprite_[i].uvPibot = Vector2{ i / 4.f,0.f, };
	}

	for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
		dpadSprite_[i].Load("Resources/UI/moveKey.png");
		dpadSprite_[i].scale = Vector2::kIdentity * 128.f;
		dpadSprite_[i].pos = Vector2{ 305.f, 285.f };
		dpadSprite_[i].pos.z = 1.0f;
		dpadSprite_[i].uvSize = Vector2{ 1.f / 4.f, 1.f / 2.f };
		dpadSprite_[i].uvPibot = Vector2{ i / 4.f,0.f, };
	}

	// 決定
	buttonSprite_.Load("Resources/UI/A.png");
	buttonSprite_.scale = Vector2::kIdentity * 128.f;
	buttonSprite_.pos = Vector2{ 384.f, 277.f };
	buttonSprite_.pos.z = 1.0f;
	buttonSprite_.uvSize.y = 0.5f;

	sapceSprite_.Load("Resources/UI/space.png");
	sapceSprite_.scale = Vector2::kIdentity * 160.f;
	sapceSprite_.pos = Vector2{ 384.f, 297.f };
	sapceSprite_.pos.z = 1.0f;
	sapceSprite_.uvSize.y = 0.5f;


	Fade::GetInstance()->Start(Vector2::kZero, 0x00000000, 10);
	Fade::GetInstance()->SetEaseFunc(SoLib::easeLinear);

	isMenuActive_ = false;
	isResultFinish_ = false;

	Camera cameraBuff{ Camera::Type::Othographic };
	cameraBuff.pos.z = -10.0f;
	cameraBuff.Update();
	freezeCameraMat_ = cameraBuff.GetViewOthographics();

	audioManager_->Load("./Resources/Sound/BGM/stage.mp3");
	bgm_ = audioManager_->Get("./Resources/Sound/BGM/stage.mp3");
	if (bgm_) {
		bgm_->Start(0.45f, true);
	}
	audioManager_->Load("./Resources/Sound/SE/clear_short.mp3");
	clearSe_ = audioManager_->Get("./Resources/Sound/SE/clear_short.mp3");
	audioManager_->Load("./Resources/Sound/SE/death.mp3");
	dieSe_ = audioManager_->Get("./Resources/Sound/SE/death.mp3");

	duraion_ = std::chrono::milliseconds(800);

	dieAnimation_.Init();
	dieAnimation_.SetActCamera(&actCamera_);

	goalEmitter_.Init();
	goalEmitter_.Reset();
	goalEmitter_.spawnArea_.origin_ = map_->GetGoalNode()->pos_ + Vector2{ -28.f,-18.f };
	goalEmitter_.spawnArea_.diff_ = Vector2{ 56.f,0.f };
	goalEmitter_.spawnArea_.radius_ = 15.f;

	goalEmitter_.SetSpawnLate(0.5f);

	goalEmitter_.SetTextureName("Resources/UI/heart.png");
	goalEmitter_.SetParticleAngle(90.f * Angle::Dig2Rad);
	goalEmitter_.SetUncertaintyAngle(60.f * Angle::Dig2Rad);
	goalEmitter_.SetParticleSpeed({ 1000.f,4000.f });

	goalEmitter_.SetParticleSize({ 5.f,30.f });
	goalEmitter_.SetParticleLifeSpan({ 1.f,2.f });

	goalEmitter_.SetSpawnCount({ 2,5 });

	goalEmitter_.colorStart_ = 0xFFFFFF00;
	goalEmitter_.colorMiddle_ = 0xFFFFFFFF;
	goalEmitter_.colorEnd_ = 0xFFFFFF00;
	goalEmitter_.centorProgress_ = 0.25f;

	playerEmitter_.spawnArea_.radius_ = 15.f;

	playerEmitter_.SetTextureName("Resources/whiteHeart.png");
	playerEmitter_.SetParticleAngle(0.f * Angle::Dig2Rad);
	playerEmitter_.SetUncertaintyAngle(180.f * Angle::Dig2Rad);
	playerEmitter_.SetParticleSpeed({ 1000.f,1500.f });

	playerEmitter_.SetParticleSize({ 15.f,30.f });
	playerEmitter_.SetParticleLifeSpan({ 0.5f,1.f });

	playerEmitter_.SetSpawnCount({ 2,2 });

	backEmitter_.Init();
	backEmitter_.Reset();

	backEmitter_.SetSpawnLate(1.f);

	backEmitter_.SetTextureName("Resources/whiteHeart.png");

	backEmitter_.SetParticleAngle(90.f * Angle::Dig2Rad);
	backEmitter_.SetUncertaintyAngle(20.f * Angle::Dig2Rad);
	backEmitter_.SetParticleSpeed({ 500.f, 1000.f });


	backEmitter_.spawnArea_.origin_ = Vector2{ -300.f, 0.f };
	backEmitter_.spawnArea_.diff_ = Vector2{ 600.f, 0.f };
	backEmitter_.spawnArea_.radius_ = 500.f;

	backEmitter_.SetSpawnCount({ 1, 3 });

	backEmitter_.SetParticleSize({ 20.f, 50.f });
	backEmitter_.SetParticleLifeSpan({ 2.f, 5.f });

	backEmitter_.colorStart_ = 0x961616C9;
	backEmitter_.colorEnd_ = 0xFF140000;
	backEmitter_.CalcCentor(0.f);

	goalExplodeEmitter_.spawnArea_.origin_ = map_->GetGoalNode()->pos_;

	goalExplodeEmitter_.SetTextureName("Resources/whiteHeart.png");
	goalExplodeEmitter_.SetParticleAngle(0.f * Angle::Dig2Rad);
	goalExplodeEmitter_.SetUncertaintyAngle(180.f * Angle::Dig2Rad);
	goalExplodeEmitter_.SetParticleSpeed({ 7000.f,7000.f });

	goalExplodeEmitter_.SetParticleSize({ 15.f,30.f });
	goalExplodeEmitter_.SetParticleLifeSpan({ 0.5f,1.f });

	goalExplodeEmitter_.colorStart_ = 0x50191500;
	goalExplodeEmitter_.colorMiddle_ = 0x501915FF;
	goalExplodeEmitter_.colorEnd_ = 0x50191500;
	goalExplodeEmitter_.centorProgress_ = 0.4f;

	goalExplodeEmitter_.SetSpawnCount({ 30,30 });

	levelResult_->drawCount_ = LevelResult::kButtonCount_ - (LevelSelect::kLevelCount_ == LevelSelect::GetInstance()->GetTargetLevel() + 1);

	//goalEmitter_.SetActice(false);

	deathDuration_ = std::chrono::milliseconds(3000);


	if (LevelSelect::GetInstance()->GetTargetLevel() == 0) {
		direction_ = std::make_unique<Tex2DInstance>();
	}

	if (direction_) {
		direction_->Load("./Resources/goalArrow.png");
		direction_->scale *= 64.0f;
		direction_->pos = { 96.0f, 128.0f };
	}

	direcitonEaseT_ = 0.0f;
	direcitonEaseSpd_ = 1.0f;

	directionStartPos_ = { 96.0f, 150.0f };
	directionEndPos_ = { 96.0f, 128.0f };

	auto textureManager = TextureManager::GetInstance();
	// このフレームで画像読み込みが発生していたらTextureをvramに送る
	textureManager->UploadTextureData();
	// dramから解放
	textureManager->ReleaseIntermediateResource();
}

void GameScene::OnExit() {
	if (bgm_) {
		bgm_->Stop();
	}
}

void GameScene::Update(const float deltaTime) {
	if (Gamepad::GetInstance()->PushAnyKey()) {
		isPad_ = true;
	}
	else if (KeyInput::GetInstance()->PushAnyKey() || Mouse::GetInstance()->PushAnyKey()) {
		isPad_ = false;
	}

#ifdef _DEBUG

	if (KeyInput::GetInstance()->Pushed(DIK_O)) {
		sceneManager_->ChangeScene(new GameScene{ LevelManager::GetInstance()->GetLevelName(LevelSelect::GetInstance()->GetTargetLevel()).c_str() }, 1);
	}
	backEmitter_.ImGuiWidget();

#endif // _DEBUG

#ifdef _DEBUG
	//retryButton_.Debug("retryButton_");
	//retryKey_.Debug("retryKey_");

	//// キーボード
	//ImGui::Begin("isPad");
	//ImGui::Checkbox("flg", &isPad_);
	//ImGui::End();
	//levelNumber_->Debug("hoge");
#endif // _DEBUG

	if (!map_->GetIsClear() && !map_->GetIsDeath()) {
		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::BACK) || KeyInput::GetInstance()->Pushed(DIK_ESCAPE)) {
			sceneManager_->ChangeScene(Vector2::kZero, 0x000000FF, new LevelSelectScene, 10);
		}

		// HUDセレクトへ戻る
		if (Gamepad::GetInstance()->GetButton(Gamepad::Button::BACK)) {
			levelSelectButton_.uvPibot.y = 0.5f;
		}
		else {
			levelSelectButton_.uvPibot.y = 0.0f;
		}
		if (KeyInput::GetInstance()->GetKey(DIK_ESCAPE)) {
			levelSelectKey_.uvPibot.y = 0.5f;
		}
		else {
			levelSelectKey_.uvPibot.y = 0.0f;
		}
	}

	LineObject::StaticUpdate(deltaTime);

	SoLib::ImGuiWidget("labelPos", &levelNumber_->pos);

	SoLib::ImGuiWidget("levelSelectButton", &levelSelectButton_.pos);
	//SoLib::ImGuiWidget("moveButton", &moveButton_.pos);
	SoLib::ImGuiWidget("backButton", &backButton_.pos);
	SoLib::ImGuiWidget("retryButton", &retryButton_.pos);


	actCamera_.Update();


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



	if (!map_->GetIsClear() && !map_->GetIsDeath()) {
		if (KeyInput::GetInstance()->Pushed(DIK_R) || Gamepad::GetInstance()->Pushed(Gamepad::Button::START)) {
			Fade::GetInstance()->SetEaseFunc(SoLib::easeOutQuad);
			sceneManager_->ChangeScene(Vector2::kZero, 0xFF, new GameScene{ LevelManager::GetInstance()->GetLevelName(LevelSelect::GetInstance()->GetTargetLevel()).c_str() }, 20);
		}
	}

	if (!Fade::GetInstance()->GetTimer()->IsActive()) {
		map_->Update();
		if ((Fade::GetInstance()->GetSprite()->color & 0xFF) != 0x00) {
			Fade::GetInstance()->Start(Vector2::kZero, 0x00, 20);
		}
	}
	goalEmitter_.SetActice(map_->GetIsCanGoal());

	if (!map_->GetIsClear() && !map_->GetIsDeath()) {
		// HUDリトライ
		if (Gamepad::GetInstance()->GetButton(Gamepad::Button::START)) {
			retryButton_.uvPibot.y = 0.5f;
		}
		else {
			retryButton_.uvPibot.y = 0.0f;
		}
		if (KeyInput::GetInstance()->GetKey(DIK_R)) {
			retryKey_.uvPibot.y = 0.5f;
		}
		else {
			retryKey_.uvPibot.y = 0.0f;
		}

		// 戻る
		if (Gamepad::GetInstance()->GetButton(Gamepad::Button::B)) {
			backButton_.uvPibot.y = 0.5f;
		}
		else {
			backButton_.uvPibot.y = 0.0f;
		}
		if (KeyInput::GetInstance()->GetKey(DIK_Z)) {
			backKey_.uvPibot.y = 0.5f;
		}
		else {
			backKey_.uvPibot.y = 0.0f;
		}
	}

	// ゴール可能になった瞬間、爆発のパーティクルを出す
	static bool isBeforeCanGoal = false;
	if (isBeforeCanGoal == false && map_->GetIsCanGoal()) {
		//goalExplodeEmitter_.spawnArea_.origin_ = map_->GetGoalNode()->pos_;
		goalExplodeEmitter_.PopParticle();
	}
	isBeforeCanGoal = map_->GetIsCanGoal();
	goalExplodeEmitter_.Update(deltaTime);


	if (map_->GetIsDeath()) {
		if (dieAnimation_.isActive_ == false) {
			levelResult_->SetButton(1u);
		}
		dieAnimation_.Start(map_->GetPlayer()->pos_);

		dieAnimation_.Update(deltaTime);
	}

	if (map_->GetIsClear() && !actCamera_.GetTimer()->IsActive()) {
		CameraStart(map_->GetGoalNode()->pos_ / 2.f, float{ 1.f / 3.f }, kZoomTime_);
	}

	if (map_->GetIsClear() && !resultActiveTimer_.IsActive() && !isResultFinish_) {

		// もし最終ステージなら
		if ((LevelSelect::kLevelCount_ == LevelSelect::GetInstance()->GetTargetLevel() + 1)) {

			levelResult_->SetButton(0u);
		}
		else {
			levelResult_->SetButton(2u);

		}
		resultActiveTimer_.Start(kResultFadeTime_);
		bgm_->Pause();
		clearSe_->Start(0.5f, false);
	}

	// 今の時間
	auto nowTime = std::chrono::steady_clock::now();

	// 死んだ瞬間に音がなる
	if (map_->GetOnIsDeath()) {
		bgm_->Pause();
		dieSe_->Start(1.0f, false);
		deathTime_ = std::chrono::steady_clock::now();
	}

	if (dieAnimation_.isAnimateComplete_ && !bgm_->IsStart() && dieSe_->IsStart()
		&& std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - deathTime_) >= deathDuration_) {
		bgm_->ReStart();
		dieSe_->Stop();
	}


	const auto &player = map_->GetPlayer();
	if (player->GetIsEaseStart() && player->GetMoveVec() != Vector2::kZero) {
		playerEmitter_.spawnArea_.origin_ = player->pos_ + Vector2{ 0.f, -8.f };
		playerEmitter_.PopParticle();
	}

	playerEmitter_.Update(deltaTime);
	goalEmitter_.Update(deltaTime);
	backEmitter_.Update(deltaTime);

	// タイマー終了時に実行
	if (resultActiveTimer_.Update() && resultActiveTimer_.IsFinish()) {
		isMenuActive_ = true;
		isResultFinish_ = true;
		finishTime_ = std::chrono::steady_clock::now();
		LevelManager::GetInstance()->GetLevelList()[LevelSelect::GetInstance()->GetTargetLevel()]->isClear_ = true;

	}

	if (map_->GetIsCanGoal()) {
		backEmitter_.colorStart_ = 0xDD161600;
		backEmitter_.colorMiddle_ = 0xDD1616C9;
		backEmitter_.centorProgress_ = 0.2f;
		backEmitter_.SetSpawnCount({ 3, 5 });
	}
	else {
		backEmitter_.colorStart_ = 0x96161600;
		backEmitter_.colorMiddle_ = 0x961616C9;
		backEmitter_.SetSpawnCount({ 1, 3 });
		backEmitter_.centorProgress_ = 0.2f;
	}


	if (isResultFinish_) {
		if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - finishTime_) >= duraion_) {
			bgm_->ReStart();
		}
	}

	if (dieAnimation_.isAnimateComplete_) {
		isMenuActive_ = true;
	}

	if (isMenuActive_) {
		for (auto &key : keySprite_) {
			key.pos = Vector2{ 551.f, 285.f };
		}
		for (auto &dpad : dpadSprite_) {
			dpad.pos = Vector2{ 551.f, 285.f };
		}
		levelResult_->Update();
	}

	sapceSprite_.uvPibot.y = 0.5f * (KeyInput::GetInstance()->GetKey(DIK_SPACE) || KeyInput::GetInstance()->GetKey(DIK_RETURN));
	buttonSprite_.uvPibot.y = 0.5f * Gamepad::GetInstance()->GetButton(Gamepad::Button::A);

	if (direction_) {
		/*direction_->Debug("direction_");*/
		direction_->pos = Vector2::Lerp(directionStartPos_, directionEndPos_, SoLib::easeInOutSine(direcitonEaseT_));
		direcitonEaseT_ += direcitonEaseSpd_ * Lamb::DeltaTime();
		if (direcitonEaseT_ >= 1.0f) {
			direcitonEaseT_ = 1.0f;
			direcitonEaseSpd_ *= -1.0f;
		}
		else if (direcitonEaseT_ <= 0.0f) {
			direcitonEaseT_ = 0.0f;
			direcitonEaseSpd_ *= -1.0f;
		}
	}
}

void GameScene::Draw() {
	const auto &cameraMat = actCamera_.GetViewOthographics();
	background_.Draw(freezeCameraMat_);

	backEmitter_.Draw(cameraMat);

	map_->Draw(cameraMat);

	if (direction_ && map_->GetIsCanGoal() && !map_->GetIsClear()) {
		direction_->Draw(cameraMat);
	}

	playerEmitter_.Draw(cameraMat);
	if (map_->GetIsDeath()) {
		dieAnimation_.Draw(cameraMat);
	}
	goalEmitter_.Draw(cameraMat);
	goalExplodeEmitter_.Draw(cameraMat);

	levelNumber_->Draw(cameraMat);

	if (isPad_) {
		if (isMenuActive_) {
			buttonSprite_.blend = BlendType::kUnenableDepthNormal;
			buttonSprite_.Draw(freezeCameraMat_);
		}
		else {
			levelSelectButton_.blend = BlendType::kUnenableDepthNormal;
			levelSelectButton_.Draw(freezeCameraMat_);
			//moveButton_.Draw(freezeCameraMat_);
			backButton_.blend = BlendType::kUnenableDepthNormal;
			backButton_.Draw(freezeCameraMat_);
			retryButton_.blend = BlendType::kUnenableDepthNormal;
			retryButton_.Draw(freezeCameraMat_);

		}

		for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
			dpadSprite_[i].blend = BlendType::kUnenableDepthNormal;
			dpadSprite_[i].Draw(freezeCameraMat_);
		}
	}
	else {
		if (isMenuActive_) {
			sapceSprite_.blend = BlendType::kUnenableDepthNormal;
			sapceSprite_.Draw(freezeCameraMat_);
		}
		else {
			levelSelectKey_.blend = BlendType::kUnenableDepthNormal;
			levelSelectKey_.Draw(freezeCameraMat_);
			//moveKey_.Draw(freezeCameraMat_);
			backKey_.blend = BlendType::kUnenableDepthNormal;
			backKey_.Draw(freezeCameraMat_);
			retryKey_.blend = BlendType::kUnenableDepthNormal;
			retryKey_.Draw(freezeCameraMat_);

		}

		for (int32_t i = 0; i < static_cast<int32_t>(KeyTexture::kCount); ++i) {
			keySprite_[i].blend = BlendType::kUnenableDepthNormal;
			keySprite_[i].Draw(freezeCameraMat_);
		}
	}


	if (isMenuActive_) {
		levelResult_->Draw();
	}
	Fade::GetInstance()->Draw();
}

void GameScene::LoadLevel(const char *const fileName) {
	/*for (auto& i : fileName_) {
		if (i == '1') {
			map_->Initialize(fileName, true);
			return;
		}
	}*/

	map_->Initialize(fileName);
}
