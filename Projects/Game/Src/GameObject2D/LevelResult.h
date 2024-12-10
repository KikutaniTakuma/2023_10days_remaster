#pragma once
#include <stdint.h>
#include <functional>
#include <array>
#include "Drawer/Texture2D/Texture2D.h"
#include "Math/Vector2.h"

class LevelSelect;
class Audio;

class LevelResult final {

	enum class ButtonID {
		LevelSelect,
		Retry,
		NextLevel,

		kCount
	};

public:
	static const uint32_t kButtonCount_ = static_cast<uint32_t>(ButtonID::kCount);

	LevelResult();
	~LevelResult() = default;

	void Init();
	void Update();

	void Draw();

	void SetButton(uint32_t index) { selectButton_ = index; }

	Vector2 origin_ = Vector2{ 0.f,-232.f };
	float posRadius_ = 256.f;

	int32_t drawCount_ = kButtonCount_;

private:

	std::array<std::function<void(void)>, kButtonCount_> buttonFunc_;
	int32_t selectButton_ = 2u;

	LevelSelect *levelSelecter_ = nullptr;

	Tex2DInstance background_;
	std::array<std::unique_ptr<Tex2DInstance>, kButtonCount_> buttonSprite_;

	Mat4x4 cameraMat_;

	Audio *cursorMoveSe_ = nullptr;
	Audio *choiseSe_ = nullptr;

	bool isStick_ = false;
	bool isChoice_ = false;
};