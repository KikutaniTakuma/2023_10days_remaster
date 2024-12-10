#pragma once
#include <stdint.h>
#include <memory>

#include "Utils/SoLib/SoLib_Timer.h"
#include "Utils/SoLib/SoLib_Easing.h"

#include "Math/Vector2.h"
#include "Drawer/Texture2D/Texture2D.h"

class Fade {
	Fade() = default;
	Fade(const Fade &) = delete;
	Fade &operator=(const Fade &) = delete;
	~Fade() = default;
public:

	static Fade *const GetInstance() {
		static Fade instance{};
		return &instance;
	}

	static void StaticInit();

	void SetState(const Vector2 &pos, uint32_t alpha);

	void Start(const Vector2 &targetPos, uint32_t targetAlpha, int32_t goalTime);
	void Update();
	void Draw();

	void SetEaseFunc(float (*easeFunc)(float)) { easeFunc_ = easeFunc; }

	Tex2DInstance *const GetSprite() { return sprite_.get(); }
	const SoLib::Timer *const GetTimer() { return timer_.get(); }

private:

	Vector2 targetPos_{};	// 移動後の座標
	Vector2 originalPos_{};	// 移動前の座標

	uint32_t targetColor_{};	// 変更後の色
	uint32_t originalColor_{};	// 変更前の色

	const uint32_t kFadeColor_ = 0xed7b6800;

	/// @brief イージング関数の関数ポインタ
	float (*easeFunc_)(float) = SoLib::easeLinear;

	std::unique_ptr<SoLib::Timer> timer_ = nullptr;
	std::unique_ptr<Tex2DInstance> sprite_ = nullptr;
	std::unique_ptr<Mat4x4> cameraMat_ = nullptr;
};