#pragma once
#include "SceneManager.h"
#include "Drawer/Texture2D/Texture2D.h"
#include <Utils/Camera/Camera.h>
#include "Game/ActiveCamera/ActiveCamera.h"
#include <chrono>

#include "GameObject2D/Emitter.h"
#include <GameObject2D/DieAnimation.h>

class AudioManager;
class Map;
class LevelResult;
class Audio;

class GameScene : public IScene {
public:
	GameScene(const char *const fileName = kDefaultFileName_);
	~GameScene();

	void OnEnter() override;
	void OnExit() override;

	void Update(const float deltaTime) override;
	void Draw() override;

public:

	/// @brief カメラの移動開始
	/// @param targetPos 移動先の座標
	/// @param targetScale 変更先の倍率 ( 1.f / ズーム倍率 )って感じの計算
	/// @param needFlame 必要フレーム数
	void CameraStart(const Vector3 &targetPos, float targetScale, uint32_t needFlame) { 
		actCamera_.Start(targetPos, targetScale, needFlame); 
	}

	/// @brief レベル読み込み
	/// @param fileName " 読み込むレベル .csv "
	void LoadLevel(const char *const fileName = kDefaultFileName_);

private:

	const int32_t kZoomTime_ = 45;
	const int32_t kResultFadeTime_ = 90;

	bool isResultFinish_ = false;
	bool isMenuActive_ = false;

	SoLib::Timer resultActiveTimer_;

	std::unique_ptr<Tex2DInstance> levelNumber_ = nullptr;

	Tex2DInstance levelSelectButton_;

	Tex2DInstance backButton_;
	Tex2DInstance retryButton_;

	Tex2DInstance levelSelectKey_;

	Tex2DInstance backKey_;
	Tex2DInstance retryKey_;

	// 決定HUD
	Tex2DInstance buttonSprite_;
	Tex2DInstance sapceSprite_;

	Emitter goalEmitter_;
	Emitter goalExplodeEmitter_;
	Emitter playerEmitter_;

	Emitter backEmitter_;

	ActiveCamera actCamera_;
	std::unique_ptr<Map> map_ = nullptr;
	std::unique_ptr<LevelResult> levelResult_ = nullptr;
	Tex2DInstance background_;

	enum class KeyTexture {
		W,
		A,
		S,
		D,
		kCount
	};

	std::array<Tex2DInstance, static_cast<uint32_t>(KeyTexture::kCount)> keySprite_;
	std::array<Tex2DInstance, static_cast<uint32_t>(KeyTexture::kCount)> dpadSprite_;


	DieAnimation dieAnimation_;

	Mat4x4 freezeCameraMat_; // 不動のカメラ

	float zoomScale_ = 1.f / 1.f;

	std::string fileName_ = kDefaultFileName_;

	static const char *const kDefaultFileName_;

	// BGM
	Audio *bgm_ = nullptr;
	Audio* clearSe_ = nullptr;
	Audio* dieSe_ = nullptr;

	std::chrono::steady_clock::time_point finishTime_;
	std::chrono::milliseconds duraion_;

	std::chrono::steady_clock::time_point deathTime_;
	std::chrono::milliseconds deathDuration_;

	// 矢印
	std::unique_ptr<Tex2DInstance> direction_;
	Vector2 directionStartPos_;
	Vector2 directionEndPos_;
	float direcitonEaseT_;
	float direcitonEaseSpd_;
};