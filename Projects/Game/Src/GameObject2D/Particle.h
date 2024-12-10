#pragma once

#include "Math/Vector2.h"
#include "Drawer/Texture2D/Texture2D.h"

class Particle {
public:
	Particle() = default;
	~Particle() = default;

	void Init();
	void Reset();
	void Update(const float deltaTime);
	void Draw(const Mat4x4 &cameraMat);

	bool GetIsAlive() const { return isAlive_; }

	void SetSprite(const std::string &fileName);
	void SetSprite(const std::string &fileName, const Vector2 &spriteSize);

	const Vector2 &GetVelocity() const { return velocity_; }

	void SetMaxLifeSpan(float lifeSpan);

	//

	/// @brief 加速度の追加
	/// @param vec ベクトル
	void AddAcceleration(const Vector2 &vec) { acceleration_ += vec; }

	const Tex2DInstance &GetSprite() const { return sprite_; }

	void SetUvOffset(const Vector2 &offset);
	void SetUvSize(const Vector2 &size);

	void SetPosition(const Vector2 &vec) { sprite_.pos = vec; }

	void SetColorStart(uint32_t color) { colorStart_ = color; }
	void SetColorMiddle(uint32_t color) { colorMiddle_ = color; }
	void SetColorEnd(uint32_t color) { colorEnd_ = color; }

	float centorProgress_;
private:

	bool isAlive_ = false;

	float maxLifeSpan_ = 0.f;
	float lifeSpan_ = 0.f;

	Tex2DInstance sprite_;
	Vector2 velocity_;
	Vector2 acceleration_;


	uint32_t colorStart_;
	uint32_t colorMiddle_;
	uint32_t colorEnd_;

};