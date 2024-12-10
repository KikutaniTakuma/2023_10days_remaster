#pragma once
#include "Math/Vector2.h"
#include "Drawer/Texture2D/Texture2D.h"
#include <list>
#include <stdint.h>
#include "Emitter.h"

// 基本的に、今あるLineの値入れておけばどうにかなります。

class LineObject {
public:
	LineObject() = default;
	~LineObject() = default;

	/// @brief 初期化
	void Init();

	/// @brief 位置の設定
	/// @param start 始点
	/// @param end 終点
	/// @param tileSize タイルサイズ (この値の半分、線を内側に縮める) 
	void SetPos(const Vector2 &start, const Vector2 &end, const float tileSize);

	/// @brief アニメーションの更新
	/// @param deltaTime 時間差分
	void Update();

	void StartEmitter();

	static void StaticUpdate(const float deltaTime);

	/// @brief 描画
	/// @param viewProjection 4x4行列
	void Draw(const Mat4x4 &viewProjection);

	/// @brief 接続演出の実行
	void ChainEffect();

	/// @brief 切断演出の実行
	void CutEffect();

private:
	/// @brief アニメーションの更新
	/// @param pages 画像のフレーム数
	void SetAnimateFlame(int32_t pages);

	static int32_t pages_;

	// アニメーションの時間管理
	static float animateTimer_;
	// アニメーションの一周にかかる時間(秒)
	static const float kTotalAnimateSpan_;

	// アニメーションのコマ数 (縦方向の分割数)
	static const int32_t kMaxAnimateCount_ = 4;
	// 横方向の分割数
	static const int32_t kHorizontalDivisions_ = 8;

	static const Vector2 kUvEvery_; // 分割後の1個分の値

	Vector2 origin_{};	// 画像の中心座標
	float radius_{};	// 画像の半径(scale.x)

	Vector2 start_{};
	Vector2 end_{};


	Tex2DInstance sprite_;

	//Emitter emitter_;

};