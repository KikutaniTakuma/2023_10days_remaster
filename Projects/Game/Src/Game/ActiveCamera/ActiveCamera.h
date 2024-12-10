#pragma once
#include <memory>
#include "Math/Vector3.h"
#include "Math/Matrix.h"

#include "Utils/SoLib/SoLib_Easing.h"

class Camera;
namespace SoLib { class Timer; }

class ActiveCamera {
public:
	ActiveCamera() = default;
	~ActiveCamera() = default;

	void Init();
	void Reset();

	void Update();

	void Start(const Vector3 &targetPos, float targetScale, uint32_t goalFlame);
	void Clear();

	void SetEaseFunc(float (*easeFunc)(float)) { easeFunc_ = easeFunc; }

	const auto &GetTimer() const { return timer_; }

	const Mat4x4 &GetViewOthographics() const;

	const std::unique_ptr<Camera>& GetCamera() const {
		return camera_;
	}

private:

	Vector3 targetPos_{};	// 移動後の座標
	Vector3 originalPos_{};	// 移動前の座標
	float targetScale_{};	// 変更後のズーム倍率
	float originalScale_{};	// 変更前のズーム倍率

	/// @brief イージング関数の関数ポインタ
	float (*easeFunc_)(float) = SoLib::easeLinear;

	std::unique_ptr<SoLib::Timer> timer_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;

};