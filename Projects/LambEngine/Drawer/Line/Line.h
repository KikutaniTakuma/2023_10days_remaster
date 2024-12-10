/// ========================
/// ==  Lineクラスの宣言  ==
/// ========================

#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix.h"
#include "Math/Vector4.h"
#include "Engine/Buffer/StructuredBuffer.h"
#include "Engine/Graphics/Shader/ShaderManager/ShaderManager.h"

#include "Utils/SafePtr.h"

#include <memory>

/// <summary>
/// 線の描画
/// </summary>
class Line {
private:
	struct VertxData {
		Mat4x4 wvp;
		Vector4 color;
	};

private:
	static constexpr uint16_t kVertexNum = 2u;
	static constexpr uint16_t kDrawMaxNumber_ = 4096u;
	static uint32_t depthDrawCount_;
	static uint32_t nodepthDrawCount_;

public:
	static void Initialize();

	static void Finalize();

	static void AllDraw(bool isDepth);

private:
	static Shader shader_;

	static Lamb::SafePtr<class Pipeline> depthPipeline_;
	static Lamb::SafePtr<class Pipeline> nodepthPipeline_;

	static std::array<std::unique_ptr<StructuredBuffer<VertxData>>, DirectXSwapChain::kBackBufferNumber> depthVertData_;
	static std::array<std::unique_ptr<StructuredBuffer<VertxData>>, DirectXSwapChain::kBackBufferNumber> nodepthVertData_;

public:
	Line();
	Line(const Line& right) = default;
	Line(Line&& right) noexcept = default;
	~Line() = default;

	Line& operator=(const Line& right) = default;
	Line& operator=(Line&& right)noexcept = default;

public:
	void Debug(const std::string& guiName);

	void Draw(const Mat4x4& viewProjection, bool isDepth = true);

	static void Draw(
		const Vector3& start, 
		const Vector3& end,
		const Mat4x4& viewProjection, 
		uint32_t color = 0xffffffff, 
		bool isDepth = true
	);

public:
	Vector3 start;
	Vector3 end;
	uint32_t color;
};