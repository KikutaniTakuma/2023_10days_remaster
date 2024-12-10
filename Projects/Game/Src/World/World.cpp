#include "World.h"
#include "Engine/Core/StringOutPutManager/StringOutPutManager.h"
#include "Input/Input.h"

#include "Engine/Graphics/RenderingManager/RenderingManager.h"

#include "GameObject2D/Fade/Fade.h"

#include "Game/MapManager/LevelManager.h"
#include <GameObject2D/Emitter.h>

#include "GlobalVariables/GlobalVariables.h"
#include <GameObject2D/LineObject.h>

#include <Scenes/SceneManager.h>
#include <Scenes/TitleScene.h>
#include <Scenes/GameScene.h>

void World::Initialize() {
	// ウィンドウ初期化オプション
	initDesc_ = Framework::InitDesc{
		.windowName = "想いを繋げ_恋心",
		.windowSize = {1280.0f, 720.0f},
		.maxFps = 60.0f,
		.isFullesceen = false
	};

	Framework::Initialize();


	//Cloud::Initialize();

	StringOutPutManager::GetInstance()->LoadFont("./Resources/Font/default.spritefont");


	Fade::StaticInit();
	Fade::GetInstance()->SetState(Vector2::kZero, 0x000000FF);
	Fade::GetInstance()->SetEaseFunc(SoLib::easeOutQuad);


	// レベル一覧を設定する
	LevelManager::GetInstance()->LoadAllLevel();

	auto* const sceneManager = SceneManager::GetInstance();
	sceneManager->Init();
	sceneManager->ChangeScene(new TitleScene{});

#ifndef USE_DEBUG_CODE
	Input::GetInstance()->GetMouse()->Show(false);
#endif // !USE_DEBUG_CODE

}

void World::Finalize() {
	Framework::Finalize();
}

void World::Update() {
	SceneManager::GetInstance()->Update(Lamb::DeltaTime());
}

void World::Draw() {
	const auto currentScene = dynamic_cast<TitleScene*>(SceneManager::GetInstance()->GetCurrentScene());
	bool isTitle = currentScene != nullptr;
	if ((KeyInput::GetInstance()->Pushed(DIK_ESCAPE) || Gamepad::GetInstance()->Pushed(Gamepad::Button::BACK)) && isTitle) {
		isEnd_ = true;
	}
	SceneManager::GetInstance()->Draw();

}