#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Utils/SoLib/SoLib_Timer.h"
#include <Math/Vector2.h>

class SceneManager;
class Entity;

class AudioManager;

class IScene {
public:
	IScene() = default;
	virtual ~IScene() = default;

	virtual void OnEnter() = 0;	// シーン入室時に一度走る
	virtual void OnExit() = 0;	// シーン退室時に一度走る

	virtual void Update(const float deltaTime) = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理

	static SceneManager *sceneManager_;
	static AudioManager *audioManager_;

	static bool isPad_;

	/// 今後実装
	// void AddObject(Object*const object);
	// std::list<std::unique_ptr<Object>> objectList_;
};


class SceneManager {
private:

	SoLib::Timer transitionTimer_{};
	// 現在読み込んでいるシーン
	std::unique_ptr<IScene> currentScene_ = nullptr;
	// 次に遷移するシーン
	std::unique_ptr<IScene> nextScene_ = nullptr;


	SceneManager() = default;
	SceneManager(const SceneManager &) = delete;
	SceneManager operator=(const SceneManager &) = delete;
	~SceneManager() = default;

public:

	void Init();


	/// @brief インスタンスの取得
	/// @return インスタンスの実体
	inline static SceneManager *const GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	IScene *const GetCurrentScene() { return currentScene_.get(); }

	void Cancel();


	/// @brief シーン遷移
	/// @param name 遷移先のシーン
	void ChangeScene(IScene *const nextScene);


	/// @brief シーン遷移
	/// @param name 遷移先の名前キー
	/// @param transitionTime 必要とする時間
	void ChangeScene(IScene *const nextScene, const int &transitionTime);


	/// @brief シーン遷移
	/// @param name 遷移先の名前キー
	/// @param transitionTime 必要とする時間
	void ChangeScene(const Vector2 &targetPos, uint32_t targetColor, IScene *const nextScene, const int &transitionTime);


	/// @brief シーンの更新
	void Update(const float deltaTime);


	/// @brief シーンの描画
	void Draw();
};