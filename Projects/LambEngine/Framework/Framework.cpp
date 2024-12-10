#include "Framework.h"
#include "Engine/Engine.h"
#include "Engine/Core/WindowFactory/WindowFactory.h"
#include "Engine/EngineUtils/FrameInfo/FrameInfo.h"
#include "Utils/ExecutionLog.h"
#include "Error/Error.h"

#include "Input/Input.h"

#include "Drawer/Line/Line.h"

#include "Engine/Graphics/RenderingManager/RenderingManager.h"
#include "Engine/Graphics/RenderContextManager/RenderContextManager.h"

void Framework::Initialize() {
	// ライブラリ初期化
	Engine::Initialize(initDesc_.windowName, initDesc_.windowSize, initDesc_.maxFps, initDesc_.isFullesceen);

	RenderingManager::Initialize();

	RenderingManager::GetInstance()->SetState(
		RenderingManager::State{
			.isLighting = false,
			.hsv = Vector3(0.0f, 0.0f, 1.0f),
			.luminanceThreshold = 5.0f,
			.isDrawSkyBox = false,
			.skyBoxEnvironmentCoefficient = 0.0f,
			.isDrawOutline = false,
		}
		);

	// 入力処理初期化
	Input::Initialize();

	Line::Initialize();
}

void Framework::Finalize() {
	// インデックスリソース解放
	Line::Finalize();

	// 入力関連解放
	Input::Finalize();

	RenderingManager::Finalize();

	// ライブラリ終了
	Engine::Finalize();
}

void Framework::Execution() {
	try {
		this->Initialize();

		FrameInfo* const frameInfo = FrameInfo::GetInstance();
		Input* const input = Input::GetInstance();
		WindowFactory* const window = WindowFactory::GetInstance();

		frameInfo->StartFrameInfo();

		while (window->WindowMassage()) {
			// 描画開始処理
			Engine::FrameStart();

			// fps
			frameInfo->Debug();

			// 入力処理
			input->InputStart();

			// フルスクリーン化
			window->Fullscreen();

			// 今のウィンドウのポジションを更新
			window->UpdateCurrentPos();

#ifdef USE_DEBUG_CODE		
			if (frameInfo->GetIsDebugStop() && frameInfo->GetIsOneFrameActive()) {
				this->Update();
				frameInfo->SetIsOneFrameActive(false);
			}
			else if (!frameInfo->GetIsDebugStop()) {
				// 更新処理
				this->Update();
			}
#else
			// 更新処理
			this->Update();
#endif
			// 終了リクエストが来たら終わり
			if (this->isEnd_) {
				RenderingManager::GetInstance()->FinalFrame();
				break;
			}

			// 描画処理
			this->Draw();

			// フレーム終了処理
			Engine::FrameEnd();
		}
	}
	catch (const Lamb::Error& err) {
		Lamb::ErrorLog(err);
	}
	catch (const std::exception& err) {
		Lamb::ErrorLog(err.what(), __func__);
	}
	this->Finalize();
}