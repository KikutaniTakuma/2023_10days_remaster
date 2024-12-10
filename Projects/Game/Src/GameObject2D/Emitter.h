#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include <list>
#include <Utils/Math/ValueRange.h>

//#include "Utils/Math/Vector2.h"
#include "Utils/Math/Angle.h"
#include "GlobalVariables/VariantItem.h"

#include "Particle.h"
class GlobalVariables;

class Emitter {
public:
	struct Capsule {
		Vector2 origin_;
		Vector2 diff_;
		float radius_;

		Vector2 GetRandom() const;
		void ImGuiWidget();
	};

	Emitter() = default;
	~Emitter() = default;

	/// @brief 純粋な初期化
	void Init();
	/// @brief 小規模初期化
	void Reset();
	/// @brief 毎フレーム更新
	/// @param deltaTime デルタタイム
	void Update(const float deltaTime);
	/// @brief 描画
	void Draw(const Mat4x4 &cameraMat);

	float GetLifeSpan() const { return lifeSpan_; }

	void PopParticle();

public:

	/// @brief テクスチャファイル名の設定
	/// @param fileName 
	void SetTextureName(const std::string &fileName) { fileName_ = fileName; }

	/// @brief 最大寿命の設定
	/// @param lifeSpan 寿命の値(秒単位)
	void SetMaxLifeSpan(const float lifeSpan);

	/// @brief 生成レートの設定
	/// @param spawnLate 生成レート(秒単位)
	void SetSpawnLate(const float spawnLate);

	void SetActice(bool active) { isActive_ = active; }
	bool GetActive() const { return isActive_; }

	void SetParticleAngle(float angle) { vParticleAngle_ = angle; }
	float GetParticleAngle() const { return vParticleAngle_; }

	void SetUncertaintyAngle(float angle) { vUncertaintyAngle_ = angle; }
	float GetUncertaintyAngle() const { return vUncertaintyAngle_; }

	void SetParticleSpeed(const ValueRange<float> &speed) { vParticleSpeed_ = speed; }
	const ValueRange<float> &GetParticleSpeed() const { return vParticleSpeed_; }

	void SetSpawnCount(const ValueRange<int32_t> &count) { vSpawnCount_ = count; }
	const ValueRange<int32_t> &GetSpawnCount() const { return vSpawnCount_; }

	void SetParticleSize(const ValueRange<float> &size) { vParticleSize_ = size; }
	const ValueRange<float> &GetParticleSize() const { return vParticleSize_; }

	void SetParticleLifeSpan(const ValueRange<float> &lifeSpan) { vParticleLifeSpan_ = lifeSpan; }
	const ValueRange<float> &GetParticleLifeSpan() const { return vParticleLifeSpan_; }


public:

	/// @brief jsonからの読み込み
	/// @param groupName グループ名
	void ApplyVariables(const GlobalVariables &gVariable, const char *const groupName);

	/// @brief jsonへの紐づけ
	/// @param groupName グループ名
	void AddVariable(GlobalVariables &gVariable) const;

	/// @brief ImGuiへの表示
	void ImGuiWidget();

	std::string groupName_ = "";

	// 生成するエリア
	Capsule spawnArea_{ .origin_{100.f,0.f} };

	float centorProgress_ = 0.f;

	uint32_t colorStart_ = 0xFFFFFFFF;
	uint32_t colorMiddle_ = 0xFFFFFFFF;
	uint32_t colorEnd_ = 0xFFFFFF00;

	/// @brief 始点と終点をもとに、中間色を計算する
	/// @param t Lerpとしての係数
	void CalcCentor(float t);

private:

	[[nodiscard]] Particle *const SpawnParticle() const;

private:
	// 生存の有無
	bool isActive_ = false;

	// 最大寿命
	VariantItem<float> vMaxLifeSpan_{ "maxLifeSpan", 0.f };
	// 現在の寿命
	float lifeSpan_ = 0.f;
	// パーティクルの生成レート
	VariantItem<float> vSpawnLate_{ "spawnLate", 0.125f };

	// 発射する角度
	VariantItem<float> vParticleAngle_{ "particleAngle",0.f };
	// 発射の時のブレ							   
	VariantItem<float> vUncertaintyAngle_{ "uncertaintyAngle", 45.f * Angle::Dig2Rad };
	// 発射速度
	VariantItem<ValueRange<float>> vParticleSpeed_{ "particleSpeed", { 4750.0f,10000.f} };

	// 生成数
	VariantItem<ValueRange<int32_t>> vSpawnCount_{ "spawnCount",{1,1} };

	// 画像サイズの乱数幅
	VariantItem<ValueRange<float>> vParticleSize_{ "particleSize", {10.f,20.f} };

	ValueRange<Vector2> uvOffset_{ {0.f,0.f},{7.f,7.f} };
	VariantItem<ValueRange<float>> vParticleLifeSpan_{ "particleLifeSpan", {7.f,7.f} };

	// 画像のファイル名
	std::string fileName_ = "Resources/uvChecker.png";

	// 子のパーティクル
	std::list<std::unique_ptr<Particle>> particleList_;

};
