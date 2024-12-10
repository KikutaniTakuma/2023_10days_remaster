#include "Easeing.h"
#include "Utils/EngineInfo.h"
#include "imgui.h"

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include <cmath>
#include <numbers>

const std::array<std::string, size_t(Easeing::Type::kNum)> Easeing::kEaseingTypeString_ = {
		"None",

		"InSine",
		"OutSine",
		"InOutSine",

		"InQuad",
		"OutQuad",
		"InOutQuad",

		"InCubic",
		"OutCubic",
		"InOutCubic",

		"InQuart",
		"OutQuart",
		"InOutQuart",

		"InQuint",
		"OutQuint",
		"InOutQuint",

		"InExpo",
		"OutExpo",
		"InOutExpo",

		"InCirc",
		"OutCirc",
		"InOutCirc",

		"InBack",
		"OutBack",
		"InOutBack",

		"InElastic",
		"OutElastic",
		"InOutElastic",

		"InBounce",
		"OutBounce",
		"InOutBounce"
};

Easeing::Easeing():
	isActive_(false),
	isLoop_(false),
	t_(0.0f),
	spdT_(1.0f),
	type_(Type::kNone)
{}

void Easeing::Update() {
	if (isActive_) {
		t_ += spdT_ * Lamb::DeltaTime();
		t_ = std::clamp(t_, 0.0f, 1.0f);

		if (t_ <= 0.0f || 1.0f <= t_) {
			if (isLoop_) {
				spdT_ *= -1.0f;
			}
			else {
				Stop();
			}
		}
	}
}

void Easeing::Start(
	bool isLoop, 
	float easeTime, 
	Type type
) {
	isActive_ = true;
	isLoop_ = isLoop;
	t_ = 0.0f;
	spdT_ = 1.0f / easeTime;

	type_ = type;
}
void Easeing::Pause() {
	isActive_ = false;
}

void Easeing::Restart() {
	isActive_ = true;
}
void Easeing::Stop() {
	isActive_ = false;
	isLoop_ = false;
}

void Easeing::Debug([[maybe_unused]]const std::string& debugName) {
#ifdef USE_DEBUG_CODE
	ImGui::Begin(debugName.c_str());
	if (ImGui::BeginCombo("BlendType", kEaseingTypeString_[static_cast<uint32_t>(type_)].c_str()))
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(Type::kNum); ++i)
		{
			bool isSelected = (type_ == static_cast<Type>(i));
			if (ImGui::Selectable(kEaseingTypeString_[i].c_str(), isSelected))
			{
				type_ = static_cast<Type>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Checkbox("isLoop", isLoop_.data());
	if (ImGui::Button("Start")) {
		isActive_ = true;
		t_ = 0.0f;
	}
	else if (ImGui::Button("Stop")) {
		Stop();
	}
	ImGui::End();

#endif // USE_DEBUG_CODE
}

void Easeing::DebugTreeNode([[maybe_unused]] const std::string& debugName) {
#ifdef USE_DEBUG_CODE
	if (ImGui::TreeNode(debugName.c_str())) {
		if (ImGui::BeginCombo("BlendType", kEaseingTypeString_[static_cast<uint32_t>(type_)].c_str()))
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(Type::kNum); ++i)
			{
				bool isSelected = (type_ == static_cast<Type>(i));
				if (ImGui::Selectable(kEaseingTypeString_[i].c_str(), isSelected))
				{
					type_ = static_cast<Type>(i);
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("isLoop", isLoop_.data());
		if (ImGui::Button("Start")) {
			isActive_ = true;
			t_ = 0.0f;
		}
		else if (ImGui::Button("Stop")) {
			Stop();
		}

		ImGui::TreePop();
	}

#endif // USE_DEBUG_CODE
}

void Easeing::SetType(Type type) {
	type_ = type;
}


float Easeing::GetTypeT() const {
	switch (type_)
	{
	case Easeing::Type::kNone:
		return t_;
	case Easeing::Type::kInSine:
		return InSine(t_);
	case Easeing::Type::kOutSine:
		return OutSine(t_);
	case Easeing::Type::kInOutSine:
		return InOutSine(t_);
	case Easeing::Type::kInQuad:
		return InQuad(t_);
	case Easeing::Type::kOutQuad:
		return OutQuad(t_);
	case Easeing::Type::kInOutQuad:
		return InOutQuad(t_);
	case Easeing::Type::kInCubic:
		return InCubic(t_);
	case Easeing::Type::kOutCubic:
		return OutCubic(t_);
	case Easeing::Type::kInOutCubic:
		return InOutCubic(t_);
	case Easeing::Type::kInQuart:
		return InQuart(t_);
	case Easeing::Type::kOutQuart:
		return OutQuart(t_);
	case Easeing::Type::kInOutQuart:
		return InOutQuart(t_);
	case Easeing::Type::kInQuint:
		return InQuint(t_);
	case Easeing::Type::kOutQuint:
		return OutQuint(t_);
	case Easeing::Type::kInOutQuint:
		return InOutQuint(t_);
	case Easeing::Type::kInExpo:
		return InExpo(t_);
	case Easeing::Type::kOutExpo:
		return OutExpo(t_);
	case Easeing::Type::kInOutExpo:
		return InOutExpo(t_);
	case Easeing::Type::kInCirc:
		return InCirc(t_);
	case Easeing::Type::kOutCirc:
		return OutCirc(t_);
	case Easeing::Type::kInOutCirc:
		return InOutCirc(t_);
	case Easeing::Type::kInBack:
		return InBack(t_);
	case Easeing::Type::kOutBack:
		return OutBack(t_);
	case Easeing::Type::kInOutBack:
		return InOutBack(t_);
	case Easeing::Type::kInElastic:
		return InElastic(t_);
	case Easeing::Type::kOutElastic:
		return OutElastic(t_);
	case Easeing::Type::kInOutElastic:
		return InOutElastic(t_);
	case Easeing::Type::kInBounce:
		return InBounce(t_);
	case Easeing::Type::kOutBounce:
		return OutBounce(t_);
	case Easeing::Type::kInOutBounce:
		return InOutBounce(t_);
	default:
		return t_;
	}
}

float Easeing::InSine(float t) {
	return 1.0f - std::cos((t * std::numbers::pi_v<float>) / 2.0f);
}
float Easeing::OutSine(float t) {
	return std::sin((t * std::numbers::pi_v<float>) / 2.0f);
}
float Easeing::InOutSine(float t) {
	return -(std::cos(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
}

float Easeing::InQuad(float t) {
	return t * t;
}
float Easeing::OutQuad(float t) {
	return 1.0f - (1.0f - t) * (1.0f - t);
}
float Easeing::InOutQuad(float t) {
	return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float Easeing::InCubic(float t) {
	return t * t * t;
}
float Easeing::OutCubic(float t) {
	return 1.0f - std::pow(1.0f - t, 3.0f);
}
float Easeing::InOutCubic(float t) {
	return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float Easeing::InQuart(float t) {
	return t * t * t * t;
}
float Easeing::OutQuart(float t) {
	return 1.0f - std::pow(1.0f - t, 4.0f);
}
float Easeing::InOutQuart(float t) {
	return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

float Easeing::InQuint(float t) {
	return t * t * t * t * t;
}
float Easeing::OutQuint(float t) {
	return 1.0f - std::pow(1.0f - t, 5.0f);
}
float Easeing::InOutQuint(float t) {
	return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float Easeing::InExpo(float t) {
	return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
}
float Easeing::OutExpo(float t) {
	return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}
float Easeing::InOutExpo(float t) {
	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
		: (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

float Easeing::InCirc(float t) {
	return 1.0f - std::sqrt(1.0f - std::pow(t, 2.0f));
}
float Easeing::OutCirc(float t) {
	return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}
float Easeing::InOutCirc(float t) {
	return t < 0.5f
		? (1.0f - std::sqrt(1 - std::pow(2.0f * t, 2.0f))) / 2.0f
		: (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float Easeing::InBack(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}
float Easeing::OutBack(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}
float Easeing::InOutBack(float t) {
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return t < 0.5f
		? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

float Easeing::InElastic(float t) {
	const float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;

	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}
float Easeing::OutElastic(float t) {
	const float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;

	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}
float Easeing::InOutElastic(float t) {
	const float c5 = (2.0f * std::numbers::pi_v<float>) / 4.5f;

	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: t < 0.5f
		? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
		: (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

float Easeing::InBounce(float t) {
	return 1.0f - OutBounce(1.0f - t);
}
float Easeing::OutBounce(float t) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {
		return n1 * t * t;
	}
	else if (t < 2.0f / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}
float Easeing::InOutBounce(float t) {
	return t < 0.5
		? (1.0f - OutBounce(1.0f - 2.0f * t)) / 2.0f
		: (1.0f + OutBounce(2.0f * t - 1.0f)) / 2.0f;
}