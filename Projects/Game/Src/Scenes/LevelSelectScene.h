#pragma once
#include "SceneManager.h"
#include "Drawer/Texture2D/Texture2D.h"
#include "Utils/Camera/Camera.h"

class Input;
class AudioManager;
class LevelSelect;
class Audio;

class LevelSelectScene : public IScene {
public:
	LevelSelectScene();
	~LevelSelectScene();

	void OnEnter() override;
	void OnExit() override;

	void Update(const float deltaTime) override;
	void Draw() override;
public:

private:
	Camera camera_{ Camera::Type::Othographic };
	Tex2DInstance background_;
	LevelSelect *levelSelecter_ = nullptr;

	Audio *bgm_ = nullptr;
	Audio *selectSE_ = nullptr;


	// 移動HUD
	/*Tex2DInstance moveButton_;
	Tex2DInstance moveKey_;*/

	enum class KeyTexture {
		W,
		A,
		S,
		D,
		kCount
	};

	std::array<Tex2DInstance, static_cast<uint32_t>(KeyTexture::kCount)> keySprite_;
	std::array<Tex2DInstance, static_cast<uint32_t>(KeyTexture::kCount)> dpadSprite_;

	Tex2DInstance upButton_;
	Tex2DInstance downButton_;
	Tex2DInstance leftButton_;
	Tex2DInstance rightButton_;

	// 戻るHUD
	Tex2DInstance backKey_;
	Tex2DInstance backButton_;

	// 決定HUD
	Tex2DInstance buttonSprite_;
	Tex2DInstance sapceSprite_;
};