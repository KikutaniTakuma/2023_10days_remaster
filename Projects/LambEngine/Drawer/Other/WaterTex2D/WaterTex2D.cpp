/// ==========================
/// ==  Water2Dクラスの定義 ==
/// ==========================

#include "WaterTex2D.h"
#include "Engine/Graphics/TextureManager/TextureManager.h"
#include "Engine/Graphics/RenderContextManager/RenderContextManager.h"
#include "Engine/Graphics/RenderingManager/RenderingManager.h"
#include "Utils/SafePtr.h"

#ifdef USE_DEBUG_CODE
#include "Utils/FileUtils.h"

#include "imgui.h"
#endif // USE_DEBUG_CODE


const LoadFileNames WaterTex2D::kFileNames_ =
LoadFileNames{
	.resourceFileName{"./Resources/Water/WaterSurface.glb"},
	.shaderName = ShaderFileNames{
		.vsFileName = "./Shaders/WaterTex2DShader/WaterTex2D.VS.hlsl",
		.psFileName = "./Shaders/WaterTex2DShader/WaterTex2D.PS.hlsl",
		.gsFileName = "./Shaders/WaterTex2DShader/WaterTex2D.GS.hlsl",
		.dsFileName = "./Shaders/WaterTex2DShader/WaterTex2D.DS.hlsl",
		.hsFileName = "./Shaders/WaterTex2DShader/WaterTex2D.HS.hlsl",
	}
};

WaterTex2D::WaterTex2D() :
	BaseDrawer()
{}

void WaterTex2D::Load()
{
	Lamb::SafePtr renderContextManager = RenderContextManager::GetInstance();

	renderContextManager->Load<WaterRenderContext>(kFileNames_, 4);

	renderSet = renderContextManager->Get(kFileNames_);

	Lamb::SafePtr textureManager = TextureManager::GetInstance();
	textureManager->LoadTexture("./Resources/Common/Water/caustics_01.bmp");
}

void WaterTex2D::Draw(
	const Mat4x4& worldMatrix,
	const Mat4x4& camera,
	Vector2 randomVec,
	float32_t density,
	uint32_t edgeDivision,
	uint32_t insideDivision,
	WaveData waveData,
	uint32_t color,
	BlendType blend
) {
	Lamb::SafePtr renderContext = renderSet->GetRenderContextDowncast<WaterRenderContext>(blend);

	Vector2 clientSize = Lamb::ClientSize();

	renderContext->SetShaderStruct(
		ShaderData{
			.randomVec = randomVec,
			.density = density,
			.edgeDivision = std::clamp(edgeDivision, 1u, 64u),
			.insideDivision = std::clamp(insideDivision, 1u, 64u),
			.cameraDirection = RenderingManager::GetInstance()->GetCameraDirection(),
			.viewportMatrix = float32_t4x4::MakeViewPort(0.0f, 0.0f, clientSize.x, clientSize.y, 0.0f, 1.0f),
			.waveData = waveData,
			.atomosphericParam = RenderingManager::GetInstance()->GetAtmosphericParams()
		}
	);

	BaseDrawer::Draw(worldMatrix, camera, color, blend);
}

void WaterTex2D::AllDraw(BlendType blend)
{
	Lamb::SafePtr renderContext = renderSet->GetRenderContext(blend);
	renderContext->Draw();
	renderContext->ResetDrawCount();
}
