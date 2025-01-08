#include "LineObject.h"
#include <cmath>
#include <algorithm>
#include "Utils/Math/Angle.h"
#include <Game/Map/Map.h>

int32_t LineObject::pages_ = 0;
float LineObject::animateTimer_ = 0.f;
const float LineObject::kTotalAnimateSpan_ = 2.f; // アニメーションにかかる時間

const Vector2 LineObject::kUvEvery_{ 1.f / kHorizontalDivisions_, 1.f / kMaxAnimateCount_ };	// 1.f ÷ 分割数

void LineObject::Init() {
	sprite_.Load("Resources/line.png");	// 読み込む画像データ
	sprite_.uvSize.y = kUvEvery_.y;	// SpriteのUVを1コマ分の高さに変更

	//emitter_.Init();
	//StartEmitter();
}

void LineObject::SetPos(const Vector2 &start, const Vector2 &end, const float tileSize) {
	const Vector2 lineDiff = end - start;

	// 中心座標の変更
	origin_ = Vector2{ std::lerp(start.x,end.x,0.5f), std::lerp(start.y,end.y,0.5f) };
	// Lineのタイルを基準とした半径。radius_ * tileSize で実数値を取得できる
	radius_ = ((lineDiff.Length() / tileSize)) * 0.5f;

	// 両端の座標を保存
	start_ = origin_ - lineDiff.Normalize() * radius_ * tileSize;
	end_ = origin_ + lineDiff.Normalize() * radius_ * tileSize;

	// SpriteのTransformの変更。
	sprite_.pos.z = -1.0f;
	sprite_.pos = origin_;
	sprite_.rotate.z = Angle::Vec2ToRad(lineDiff);
	sprite_.scale.y = tileSize;
	sprite_.scale.x = radius_ * 2.f * tileSize;

	// 画像のuvの横幅の変更
	sprite_.uvPibot.x = 0.5f - (kUvEvery_.x * radius_);
	sprite_.uvSize.x = kUvEvery_.x * radius_ * 2.f;

	/*emitter.spawnArea_.origin_ = start_;
	emitter.spawnArea_.diff_ = end_ - start_;*/
}

void LineObject::SetAnimateFlame(int32_t pages) {
	// アニメーションのフレーム変更
	sprite_.uvPibot.y = pages * kUvEvery_.y;
}

void LineObject::Update() {
	// アニメーションの更新処理
	SetAnimateFlame(pages_);
	// emitter.Update(deltaTime);
}

void LineObject::StartEmitter() {
	/*emitter.Reset();
	emitter.spawnArea_.origin_ = start_;
	emitter.spawnArea_.diff_ = end_ - start_;*/
}

void LineObject::StaticUpdate(const float deltaTime) {
	// 時間差分の計算
	animateTimer_ += deltaTime;
	// 時間差分をもとに、現在のコマ数を算出
	pages_ = static_cast<int32_t>((animateTimer_ / kTotalAnimateSpan_) * kMaxAnimateCount_) % kMaxAnimateCount_;
}

void LineObject::Draw(const Mat4x4 &viewProjection) {
	// 背景透過でDrawコール
	sprite_.Draw(viewProjection);
}
