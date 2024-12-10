#pragma once
#include <deque>
#include <vector>
#include "Drawer/Texture2D/Texture2D.h"
#include "Game/Player/Player.h"
#include "Node/Node.h"
#include "Drawer/Line/Line.h"
#include "GameObject2D/LineObject.h"

class Map {
public:
	enum class Type {
		kNone = 0, // 空白
		kHole = 16, // 穴(ノードを繋ぐ赤い紐のみ通す。動かせない)
		kWall, // 壁(動かせず何も通さない)

		// 動かせるNodeは
		// 1 ～ 15までの数字
		// 4ビットでノードのポートの有効化フラグを取る
		/*
			  1
		   8 口 2
			  4
		*/

		kPlayer = 19,

		// ここからゴール
		kGoalUp,
		kGoalRight,
		kGoalDown,
		kGoalLeft,

		// ヒント(ノードのヒント)
		kHint,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
public:
	Map() = default;
	Map(const Map &) = default;
	Map(Map &&) = default;
	~Map();

	/// <summary>
	/// 代入演算子
	/// </summary>
public:
	Map &operator=(const Map &) = default;
	Map &operator=(Map &&) = default;

	/// <summary>
	/// メンバ関数
	/// </summary>
public:
	/// <summary>
	/// csvファイルを読み込んで初期化
	/// </summary>
	/// <param name="fileName">csvファイルパス</param>
	/// <param name="isHint">ヒントありかなしか</param>
	void Initialize(const std::string &fileName, bool isHint = false);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cameraMat">カメラ行列</param>
	void Draw(const Mat4x4 &cameraMat);

	/// <summary>
	/// Undo
	/// </summary>
	/// <param name="isMove">プレイヤーが動いたか</param>
	void Undo(bool isMove);

	// マップのリセット
	void MapReset();

public:

	/// @brief Lineの取得関数
	/// @return Lineのリスト
	const auto &GetLines() const { return lines_; }

	/// @brief タイルサイズの取得
	/// @return タイルサイズ
	static float GetTileSize() { return kMapSize; }

	void EffectPlayerMove() { /* playerのスクリーン座標とか */ }

	void EffectPlayerDie() { /* playerのスクリーン座標とか */ }

	void EffectChainLine() {}	// 接続時に呼び出すとか

	void EffectCutLine() {}	// 切断時に呼び出すとか

	inline bool GetIsClear() const { return isClear_; }
	inline bool GetIsCanGoal() const { return isCanGoal_; }
	inline const auto &GetPlayer() const { return player_; }
	inline const auto &GetGoalNode() const { return goal_; }
	inline bool GetIsDeath() const { return isDie_; }
	// 死んだ瞬間
	inline bool GetOnIsDeath() const { return !isPreDie_ && isDie_; }

private:
	// 当たり判定
	void Collision();

	// ラインを引く処理
	void Lines();

	/// <summary>
	/// メンバ変数
	/// </summary>
private:
	std::deque<MapNode> nodes_;

	std::deque<MapNode> holes_;

	std::deque<MapNode> walls_;

	std::deque<Tex2DInstance> hints_;

	std::list<LineObject> lines_;

	std::list<std::pair<Vector2, Vector2>> linePos_;

	std::unique_ptr<MapNode> goal_;

	std::vector<std::vector<int32_t>> mapDatas_;
	std::vector<std::vector<int32_t>> startMapDatas_;

	std::unique_ptr<Player> player_;

	bool isCanGoal_;

	bool isClear_;

	bool isDie_;
	bool isPreDie_;

	bool isHints_;
	uint32_t hintMaxColor_;
	uint32_t hintMinColor_;
	float hintEaseT_;
	float hintEaseSpd_;

	//bool isGoalAnimationFinish_;

	size_t allPortCount_;
	size_t portCount_;

	Audio *lineConectSe_ = nullptr;
	Audio *lineDisconectSe_ = nullptr;

	bool isLineConect_ = false;
	bool isLineDisconect_ = false;

	std::chrono::steady_clock::time_point mapStart_;
	std::chrono::milliseconds canMoveTimeDuration_;


	/// <summary>
	/// 静的定数
	/// </summary>
private:
	/// <summary>
	/// マップタイル一つ分の大きさ
	/// </summary>
	static constexpr float kMapSize = 64.0f;
};