#pragma once
#include <Drawer/Texture2D/Texture2D.h>
#include "GameObject2D/Emitter.h"
#include <stdint.h>
#include <Utils/SoLib/SoLib_Timer.h>

class ActiveCamera;

class DieAnimation
{
public:
	DieAnimation() = default;
	~DieAnimation() = default;

	void Init();
	void Update(float deltaTime);

	void Start(const Vector2 &playerPos);

	void ImGuiWidget();
	void Draw(const Mat4x4 &cameraMat);

	void SetActCamera(ActiveCamera *const actCamera) { actCamera_ = actCamera; }

	const float kLifeTime_ = 1.f;
	const float kEmitterTime_ = 1.75f;

	const float kZoomTimer_ = 0.5f;

	const float kMenuPop_ = 2.f;

	float animateValue_;

	bool isAnimateComplete_ = false;
	bool isActive_ = false;

private:

	ActiveCamera *actCamera_ = nullptr;

	Tex2DInstance playerSprite_;
	Emitter emitter_;

};