#pragma once
#include <chrono>
#include "Math/Vector2.h"
#include <cstdint>

namespace Lamb {
	float DeltaTime();

	Vector2 ClientSize();

	Vector2 WindowSize();

	std::chrono::steady_clock::time_point ThisFrameTime();

	bool IsEngineFianlize();

	bool IsCanUseMeshShader();

	uint32_t GetGraphicBufferIndex();
}