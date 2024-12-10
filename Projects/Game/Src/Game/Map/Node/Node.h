#pragma once
#include "Drawer/Texture2D/Texture2D.h"
#include "Drawer/Line/Line.h"
#include <bitset>
#include <stack>
#include <chrono>

class MapNode {
public:
	enum class Type {
		kMove, // 動かせるボックス
		kHole, // 穴 
		kWall, // 壁
		kGoal
	};

public:
	MapNode() = default;
	MapNode(const MapNode&) = default;
	MapNode(MapNode&&) = default;
	~MapNode() = default;

public:
	MapNode& operator=(const MapNode&) = default;
	MapNode& operator=(MapNode&&) = default;

public:
	//　初期化
	void Initialize(
		const Vector2& mapNum,
		size_t isPort,
		const Vector2& pos,
		const std::string& fileName,
		const std::string& portFileName,
		const std::string& heartFileName,
		float size,
		Type type
	);

	//更新
	void Update();

	// 描画
	void Draw(const Mat4x4& cameraMat);

	// ノードのタイプ
	inline Type GetType() const {
		return type_;
	}

	// 当たり判定
	bool Collision(const Vector2& pos) const;

	// ポートのフラグ
	const std::bitset<4>& GetIsPort() const {
		return isPort_;
	}

	// Initializeで設定したポジションを現在のポジションに追加する
	void SetStartPos();

	// Undo用、呼び出した時のposがstackに格納される
	void SaveThisFramePos();

	// Undo用、呼び出したときにstackに格納されてるデータを代入する
	void SetPrePos();

	// マップチップでのナンバー
	inline const Vector2& GetMapNum()const {
		return mapPos_;
	}

	bool Animation();

	inline void AnimaionStart(float startUv) {
		if (!isGoalAni_) {
			tex_.AnimationStart(startUv);
			isGoalAni_ = true;
		}
	}

	void ConectAnimation();

	inline const Vector2& GetPrePos() const {
		return prePos_;
	}

	inline const Vector2& GetPreMapPos() const {
		return preMapPos_;
	}

	inline const Vector2& GetMoveToPos() const {
		return moveToPos_;
	}

public:
	Vector2 pos_;

	Vector2 moveVec_;

	bool isConect_;
	
	bool isCanGoal_;

private:
	Vector2 mapPos_;

	std::stack<Vector2> posData_;

	std::stack<Vector2> mapPosData_;

	float size_;

	std::bitset<4> isPort_;

	std::array<Tex2DInstance, 4> ports_;

	Tex2DInstance tex_;


	Tex2DInstance heart_;

	MapNode::Type type_;

	
	std::chrono::milliseconds aniDuration_;

	bool isGoalAni_;

	Vector2 conectHeartScale_;
	Vector2 conectHeartMaxScale_;

	uint32_t heartColor_;
	uint32_t heartMaxColor_;


	bool isAni_;
	float easeT_;
	float easeSpd_;

	bool isPreConect_;

	Vector2 prePos_;
	Vector2 preMapPos_;

	Vector2 startPos_;
	Vector2 moveToPos_;

	bool isEase_;
	float easeMoveT_;
	float easeMoveSpd_;
};