#pragma once
#include <stdint.h>
#include <array>

#include "Drawer/Texture2D/Texture2D.h"

class Audio;

class LevelSelect {
	LevelSelect() { Init(); };
	LevelSelect(const LevelSelect &) = delete;
	LevelSelect &operator= (const LevelSelect &) = delete;
	~LevelSelect() = default;
public:

	static const int32_t kWidthCount_ = 4;	// ステージの横に並ぶ数
	static const int32_t kLevelCount_ = 12;	// ステージの総数

	float defaultIconScale_ = 128.f;

	Vector2 iconOrigin_ = { 0.f, -72.f };
	Vector2 iconRange_ = { 256.f, 160.f };

	static LevelSelect *const GetInstance() {
		static LevelSelect instance{};
		return &instance;
	}

	void Init();

	void Update();

	void Draw(const Mat4x4 &viewProjection);

	void AddTargetLevel(int32_t value);

	int32_t GetTargetLevel()const { return selectTarget_; }

private:

	int32_t selectTarget_ = 0u;

	Tex2DInstance levelTitle_;

	std::array<Vector2, kLevelCount_> levelAnchor_;

	std::array<Tex2DInstance, kLevelCount_> levelIcon_;

	std::array<Tex2DInstance, kLevelCount_> clearIcon_;

	Audio *moveSE_ = nullptr;

	Vector2 texMaxSize_;
	Vector2 texMinSize_;
	Vector2 currentTexSize;

	bool isMinas_;

	float easeT;
	float easeSpd;

	bool isStick_ = false;
};