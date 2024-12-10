#pragma once
#include "SceneManager.h"
#include "Drawer/Texture2D/Texture2D.h"
#include "Utils/Camera/Camera.h"

class Input;
class AudioManager;
class Audio;

class TitleScene : public IScene {
public:
	TitleScene();
	~TitleScene();

	void OnEnter() override;
	void OnExit() override;

	void Update(const float deltaTime) override;
	void Draw() override;
public:

private:
	Camera camera_{ Camera::Type::Othographic };

	Audio *bgm_ = nullptr;
	Audio *selectSE_ = nullptr;
	float animateValue_ = 0.f;
	const float kAnimateTime_ = 2.f;

	float kButtonAnimateCircle_ = 1.f;

	Tex2DInstance titleSprite_;
	Tex2DInstance buttonSprite_;
	Tex2DInstance sapceSprite_;

	Tex2DInstance escSprite_;
	Tex2DInstance backButtonSprite_;

	Mat4x4 cameraMat_;
};