#include "SceneManager.h"
#include "AudioManager/AudioManager.h"

#include "GameObject2D/Fade/Fade.h"

SceneManager *IScene::sceneManager_ = nullptr;
AudioManager *IScene::audioManager_ = nullptr;
bool IScene::isPad_ = true;


void SceneManager::Init() {
	IScene::sceneManager_ = this;
	IScene::audioManager_ = AudioManager::GetInstance();
}

void SceneManager::Cancel() {
	nextScene_.reset();
	transitionTimer_.Start();
}

void SceneManager::ChangeScene(IScene *const nextScene) {
	if (nextScene == nullptr) return;
	if (currentScene_) {
		// 遷移前のシーンの退室処理
		currentScene_->OnExit();
	}
	// 保持するシーンのキーとポインタを更新
	currentScene_.reset(nextScene);
	currentScene_->OnEnter();
}

void SceneManager::ChangeScene(IScene *const nextScene, const int &transitionTime) {
	// もし、次のシーンがあったらキャンセル
	if (nextScene_ != nullptr) {
		delete nextScene;
		return;
	}
	// 次のシーンのポインタを保存
	nextScene_.reset(nextScene);
	// 遷移タイマーを開始
	transitionTimer_.Start(transitionTime);
}

void SceneManager::ChangeScene(const Vector2 &targetPos, uint32_t targetColor, IScene *const nextScene, const int &transitionTime) {
	Fade::GetInstance()->Start(targetPos, targetColor, transitionTime);
	ChangeScene(nextScene, transitionTime);
}

void SceneManager::Update(const float deltaTime) {
	Fade::GetInstance()->Update();

	if (transitionTimer_.Update() && transitionTimer_.IsFinish()) {
		ChangeScene(nextScene_.release());
	}


	if (currentScene_) {
		currentScene_->Update(deltaTime);
	}
}

void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}