#pragma once
#include "Drawer/Texture2D/Texture2D.h"
#include <stack>
#include <chrono>
#include "AudioManager/AudioManager.h"

class Player {
	enum class Direction {
		kUp,
		kRight,
		kDown,
		kLeft
	};

public:
	Player();
	Player(const Player&) = default;
	Player(Player&&)noexcept = default;
	~Player() = default;

public:
	Player& operator=(const Player&) = default;
	Player& operator=(Player&&) = default;

public:
	void Initialize(
		const Vector2& pos, 
		const std::string& fileName, 
		float size,
		const Vector2& mapPos
	);

	bool Move();

	void Update();

	void Draw(const Mat4x4& cameraMat);

	inline const Vector2& GetMoveVec() const {
		return moveVec_;
	}

	inline void SetMove(const Vector2& moveVec) {
		moveVec_ = moveVec;
	}

	// Initializeで設定したポジションを現在のポジションに追加する
	void SetStartPos();

	// Undo用、呼び出した時のposがstackに格納される
	void SaveThisFramePos();

	// Undo用、呼び出したときにstackに格納されてるデータを代入する
	void SetPrePos();

	void Animation(/*bool isMove, bool isBlock*/);

	void CannotMove() {
		isCannotMove = true;
	}

	const Vector2& GetMoveToPos() const {
		return moveToPos_;
	}

	bool GetIsEase() const {
		return isEase_;
	}

	bool GetIsEaseStart() const {
		return isEaseStart_;
	}

public:
	Vector2 mapPos_;

	Vector2 pos_;

	bool isMove_;

private:
	std::stack<Vector2> posData_;

	std::stack<Vector2> mapPosData_;

	Tex2DInstance tex_;

	Vector2 moveVec_;

	Direction direction_;

	Vector2 prePos_;
	Vector2 preMapPos_;

	std::chrono::steady_clock::time_point aniStartTime_;
	std::chrono::milliseconds aniDuration_;
	float aniCount_;

	Vector2 uvPibot_;
	float uvPibotSpd_;

	//音
	Audio* moveSe_;

	Audio* cannotSe_;

	Vector2 startPos_;
	Vector2 moveToPos_;

	float easeT_;
	float easeSpd_;

	bool isEase_;

	bool isEaseStart_;

	bool isCannotMove;

	bool isStick_;
};