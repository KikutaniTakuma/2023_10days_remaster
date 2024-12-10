#include "Player.h"
#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Utils/SoLib/SoLib.h"

Player::Player():
	tex_(),
	pos_(),
	moveVec_(),
	direction_(Direction::kUp),
	mapPos_(),
	posData_(),
	mapPosData_(),
	aniCount_(0llu),
	aniDuration_(300),
	uvPibot_(),
	uvPibotSpd_(0.0f),
	moveSe_(nullptr),
	isMove_(false),
	cannotSe_(nullptr),
	easeSpd_(0.0f),
	easeT_(0.0f),
	isCannotMove(false),
	isEaseStart_(false),
	isEase_(false),
	isStick_(false)
{}

void Player::Initialize(
	const Vector2& pos, 
	const std::string& fileName, 
	float size, 
	const Vector2& mapPos
) {
	pos_ = pos;
	tex_.Load(fileName);
	tex_.pos = pos_;

	tex_.scale *= size;

	uvPibot_ = Vector2::kZero;
	uvPibotSpd_ = 1.0f / 24.0f;
	tex_.uvSize.x = uvPibotSpd_;

	mapPos_ = mapPos;

	AudioManager::GetInstance()->Load("./Resources/Sound/SE/walk.mp3");
	AudioManager::GetInstance()->Load("./Resources/Sound/SE/dontMove.mp3");
	moveSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/walk.mp3");
	cannotSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/dontMove.mp3");

	easeT_ = 0.0f;
	easeSpd_ = 10.0f;

	startPos_ = pos_;
	moveToPos_ = pos_;
	isEase_ = false;
	isEaseStart_ = false;

	preMapPos_ = mapPos;
	prePos_ = pos_;

	isStick_ = false;
}

bool Player::Move() {
	isCannotMove = false;
	bool isMove = false;
	isMove_ = isMove;
	moveVec_ = Vector2::kZero;
	if (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).LengthSQ() == 0.0f) {
		isStick_ = false;
	}

	if (isEase_) {
		isEaseStart_ = false;
		return false;
	}
	if (!Gamepad::GetInstance()->GetButton(Gamepad::Button::B) && !Gamepad::GetInstance()->GetPreButton(Gamepad::Button::B)
		&& !KeyInput::GetInstance()->GetKey(DIK_Z) && !KeyInput::GetInstance()->GetPreKey(DIK_Z)) {
		if (Gamepad::GetInstance()->Pushed(Gamepad::Button::UP)
			|| KeyInput::GetInstance()->Pushed(DIK_UP) || KeyInput::GetInstance()->Pushed(DIK_W)
			|| (0.6f < Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y && !isStick_)
			) {
			moveVec_ = Vector2::kYIdentity;
			isMove = true;
			direction_ = Direction::kUp;
			isEase_ = true;
			isEaseStart_ = true;
			isStick_ = true;
		}
		else if (Gamepad::GetInstance()->Pushed(Gamepad::Button::DOWN)
			|| KeyInput::GetInstance()->Pushed(DIK_DOWN) || KeyInput::GetInstance()->Pushed(DIK_S)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).y < -0.6f && !isStick_)
			) {
			moveVec_ = -Vector2::kYIdentity;
			isMove = true;
			direction_ = Direction::kDown;
			isEase_ = true;
			isEaseStart_ = true;
			isStick_ = true;
		}
		else if (Gamepad::GetInstance()->Pushed(Gamepad::Button::RIGHT)
			|| KeyInput::GetInstance()->Pushed(DIK_RIGHT) || KeyInput::GetInstance()->Pushed(DIK_D)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x > 0.6f && !isStick_)
			) {
			moveVec_ = Vector2::kXIdentity;
			isMove = true;
			direction_ = Direction::kRight;
			isEase_ = true;
			isEaseStart_ = true;
			isStick_ = true;
		}
		else if (Gamepad::GetInstance()->Pushed(Gamepad::Button::LEFT)
			|| KeyInput::GetInstance()->Pushed(DIK_LEFT) || KeyInput::GetInstance()->Pushed(DIK_A)
			|| (Gamepad::GetInstance()->GetStick(Gamepad::Stick::LEFT).x < -0.6f && !isStick_)
			) {
			moveVec_ = -Vector2::kXIdentity;
			isMove = true;
			direction_ = Direction::kLeft;
			isEase_ = true;
			isEaseStart_ = true;
			isStick_ = true;
		}
	}


	isMove_ = isMove;
	return isMove;
}

void Player::Update() {

	if (isMove_ && isEaseStart_) {
		moveToPos_ = pos_ + moveVec_;
		startPos_ = pos_;
		easeT_ = 0.0f;
		isEase_ = true;
	}

	if (isEase_ && !isCannotMove) {
		if (easeT_ == 0.0f) {
			preMapPos_ = mapPos_;
			prePos_ = pos_;
			this->SaveThisFramePos();
			mapPos_.x += moveVec_.Normalize().x;
			mapPos_.y -= moveVec_.Normalize().y;
		}
		pos_ = Vector2::Lerp(startPos_, moveToPos_, SoLib::easeOutExpo(easeT_));
		easeT_ += easeSpd_ * Lamb::DeltaTime();
		if (easeT_ >= 1.0f) {
			easeT_ = 0.0f;
			pos_ = moveToPos_;
			isEase_ = false;
			startPos_ = moveToPos_;
		}
	}
	//else {
	//	easeT_ = 0.0f;
	//	//pos_ = moveToPos_;
	//	isEase_ = false;
	//	startPos_ = moveToPos_;
	//}

	tex_.pos = pos_;
}

void Player::Draw(const Mat4x4& cameraMat) {
	tex_.Draw(cameraMat);

	if (isMove_ && moveSe_ && moveVec_ != Vector2::kZero && isEase_ && isEaseStart_) {
		moveSe_->Start(1.0f, false);
	}
	else if (isMove_ && cannotSe_ && isEaseStart_ && moveVec_ == Vector2::kZero) {
		cannotSe_->Start(1.0f, false);
	}
}
void Player::SetStartPos() {
	while (!posData_.empty())
	{
		if (posData_.size() == 1) {
			pos_ = posData_.top();
			posData_.pop();
		}
		else {
			posData_.pop();
		}
	}
	while (!mapPosData_.empty())
	{
		if (mapPosData_.size() == 1) {
			mapPos_ = mapPosData_.top();
			mapPosData_.pop();
		}
		else {
			mapPosData_.pop();
		}
	}

	prePos_ = pos_;
	preMapPos_ = mapPos_;
	startPos_ = pos_;
	moveToPos_ = pos_;
}

void  Player::SaveThisFramePos() {
	if (moveVec_ != Vector2::kZero) {
		// stackにデータを格納
		posData_.push(prePos_);

		mapPosData_.push(preMapPos_);
	}
}

void  Player::SetPrePos() {
	if (!isEase_) {
		prePos_ = pos_;
		preMapPos_ = mapPos_;
		if (!posData_.empty()) {
			// stackの一番上のデータを取得
			pos_ = posData_.top();
			// stackの一番上のデータを削除
			posData_.pop();
		}
		if (!mapPosData_.empty()) {
			// stackの一番上のデータを取得
			mapPos_ = mapPosData_.top();
			// stackの一番上のデータを削除
			mapPosData_.pop();
		}

		startPos_ = pos_;
		moveToPos_ = pos_;
	}
}

void Player::Animation(/*bool isMove, bool isBlock*/) {
	auto nowTime = std::chrono::steady_clock::now();
	auto duraion = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - aniStartTime_);
	
	switch (direction_)
	{
	case Player::Direction::kUp:
	case Player::Direction::kDown:
		if (duraion >= aniDuration_) {
			aniStartTime_ = nowTime;
			aniCount_++;

			if (aniCount_ >= 2.0f) {
				aniCount_ = 0.0f;
			}
		}
		uvPibot_.x = 0.0f;
		uvPibot_.x += uvPibotSpd_ * aniCount_;

		break;
	case Player::Direction::kRight:
	case Player::Direction::kLeft:
		if (duraion >= aniDuration_) {
			aniStartTime_ = nowTime;

			aniCount_++;

			if (aniCount_ >= 2.0f) {
				aniCount_ = 0.0f;
			}
		}
		uvPibot_.x = uvPibotSpd_ * 12.0f;
		uvPibot_.x += uvPibotSpd_ * aniCount_;
		break;
	}

	tex_.uvPibot = uvPibot_;
}