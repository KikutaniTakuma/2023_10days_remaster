#include "Particle.h"
#include <Utils/SoLib/SoLib_Easing.h>

void Particle::Init() {
}

void Particle::Reset() {
	lifeSpan_ = maxLifeSpan_;
	isAlive_ = true;
}

void Particle::Update(const float deltaTime) {
	if (isAlive_) {
		lifeSpan_ -= deltaTime;	// 寿命の減少
		// 規定値を下回ったら死
		if (lifeSpan_ <= 0.f) { isAlive_ = false; }

		velocity_ += acceleration_ * deltaTime;
		sprite_.pos += velocity_ * deltaTime;
		acceleration_ = Vector2{};

		float totalProgress = 1.f - (lifeSpan_ / maxLifeSpan_);
		// もし現在地が中間地点より前だったら
		if (totalProgress <= centorProgress_) {
			float diffProgress = totalProgress / centorProgress_;
			sprite_.color = SoLib::ColorLerp(colorStart_, colorMiddle_, SoLib::easeInOutQuad(diffProgress));
		}
		else {
			float diffProgress = (totalProgress - centorProgress_) / (1.f - centorProgress_);
			sprite_.color = SoLib::ColorLerp(colorMiddle_, colorEnd_, SoLib::easeInOutQuad(diffProgress));

		}
		//sprite_.color = SoLib::ColorLerp(colorStart_, colorEnd_, progress);

	}
}

void Particle::Draw(const Mat4x4 &cameraMat) {
	if (isAlive_) {
		sprite_.Draw(cameraMat);
	}
}

void Particle::SetSprite(const std::string &fileName) {
	sprite_.Load(fileName);
}

void Particle::SetSprite(const std::string &fileName, const Vector2 &spriteSize) {
	SetSprite(fileName);
	sprite_.scale = spriteSize;
}

void Particle::SetMaxLifeSpan(float lifeSpan) {
	maxLifeSpan_ = lifeSpan;
}

void Particle::SetUvOffset(const Vector2 &offset) {
	sprite_.uvPibot = offset;
}

void Particle::SetUvSize(const Vector2 &size) {
	sprite_.uvSize = size;
}
