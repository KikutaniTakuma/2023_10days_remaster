#include "Camera.h"
#include "Engine/Engine.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"
#include "Input/Gamepad/Gamepad.h"
#include "imgui.h"
#include "Engine/Core/WindowFactory/WindowFactory.h"

#include "Utils/EngineInfo.h"
#include <numbers>
#include <cmath>

Camera::Camera() noexcept :
	type(Type::Projecction),
	isDebug(false),
	pos(),
	scale(Vector3::kIdentity),
	rotate(),
	drawScale(1.0f),
	moveVec(),
	moveSpd(1.65f),
	moveRotateSpd(std::numbers::pi_v<float> / 720.0f),
	gazePointRotate(),
	gazePointRotateSpd(std::numbers::pi_v<float> / 90.0f),
	farClip(1000.0f),
	fov(0.45f),
	view(),
	projection(),
	othograohics()
{}

Camera::Camera(Camera::Type mode) noexcept :
	type(mode),
	isDebug(false),
	pos(),
	scale(Vector3::kIdentity),
	rotate(),
	drawScale(1.0f),
	moveVec(),
	moveSpd(0.02f),
	moveRotateSpd(std::numbers::pi_v<float> / 720.0f),
	gazePointRotate(),
	gazePointRotateSpd(std::numbers::pi_v<float> / 9.0f),
	farClip(1000.0f),
	fov(0.45f),
	view(),
	projection(),
	othograohics()
{}

Camera::Camera(const Camera& right) noexcept
{
	*this = right;
}

Camera::Camera(Camera&& right) noexcept
{
	*this = std::move(right);
}

void Camera::Update(const Vector3& gazePoint) {
	if (isDebug) {
		moveVec = Vector3();

		switch (type)
		{
		case Camera::Type::Projecction:
		default:
			moveSpd = 1.65f;
			if (Mouse::GetInstance()->LongPush(Mouse::Button::Right) && (KeyInput::GetInstance()->LongPush(DIK_LSHIFT) || KeyInput::GetInstance()->LongPush(DIK_RSHIFT))) {
				auto moveRotate = Mouse::GetInstance()->GetVelocity().Normalize() * moveRotateSpd;
				moveRotate.x *= -1.0f;

				rotate.x -= std::atan(moveRotate.y);
				rotate.y -= std::atan(moveRotate.x);
			}
			else if (Mouse::GetInstance()->LongPush(Mouse::Button::Right)) {
				auto moveRotateBuf = Mouse::GetInstance()->GetVelocity().Normalize() * gazePointRotateSpd;
				moveRotateBuf.x *= -1.0f;
				gazePointRotate -= moveRotateBuf;
			}
			if (Mouse::GetInstance()->LongPush(Mouse::Button::Middle)) {
				moveVec = Mouse::GetInstance()->GetVelocity().Normalize() * moveSpd * ImGui::GetIO().DeltaTime;
				moveVec *= Mat4x4::MakeRotateX(rotate.x) * Mat4x4::MakeRotateY(rotate.y) * Mat4x4::MakeRotateZ(rotate.z);
				pos -= moveVec;
			}
			if (Mouse::GetInstance()->GetWheelVelocity() != 0.0f) {
				moveVec.z = Mouse::GetInstance()->GetWheelVelocity();
				moveSpd = 6.6f;
				moveVec = moveVec.Normalize() * moveSpd * ImGui::GetIO().DeltaTime;
				moveVec *= Mat4x4::MakeRotateX(rotate.x) * Mat4x4::MakeRotateY(rotate.y) * Mat4x4::MakeRotateZ(rotate.z);
				pos += moveVec;
			}
			break;
		case Camera::Type::Othographic:
			moveSpd = 15.0f;

			if (Mouse::GetInstance()->LongPush(Mouse::Button::Middle)) {
				moveVec = Mouse::GetInstance()->GetVelocity().Normalize() * moveSpd * ImGui::GetIO().DeltaTime;
				moveVec *= Mat4x4::MakeRotateX(rotate.x) * Mat4x4::MakeRotateY(rotate.y) * Mat4x4::MakeRotateZ(rotate.z);
				pos -= moveVec * drawScale;
			}
			if (Mouse::GetInstance()->GetWheelVelocity() != 0.0f) {
				moveVec.z = Mouse::GetInstance()->GetWheelVelocity();
				if (drawScale <= 1.0f) {
					moveVec = moveVec.Normalize() * (moveSpd * 0.00005f);
				}
				else {
					moveVec = moveVec.Normalize() * (moveSpd * 0.001f);
				}
				drawScale -= moveVec.z;
				drawScale = std::clamp(drawScale, 0.01f, 10.0f);
			}
			break;
		}

		/*auto posTmp = pos + gazePoint;
		posTmp *= */
		view = Mat4x4::MakeAffin(scale, rotate, pos);
		view = view * Mat4x4::MakeAffin(Vector3::kIdentity, Vector3(gazePointRotate.y, gazePointRotate.x, 0.0f), gazePoint);
		worldPos = { view[0][3],view[1][3], view[2][3] };
		view = view.Inverse();
	}
	else {
		view = Mat4x4::MakeAffin(scale, rotate, pos + gazePoint);
		view = view * Mat4x4::MakeAffin(Vector3::kIdentity, Vector3(gazePointRotate.y, gazePointRotate.x, 0.0f), pos + gazePoint);
		worldPos = { view[0][3],view[1][3], view[2][3] };
		view = view.Inverse();
	}

	const Vector2 clientSize = Lamb::ClientSize();
	const float aspect = clientSize.x / clientSize.y;

	const auto&& windowSize = WindowFactory::GetInstance()->GetWindowSize();

	switch (type)
	{
	case Camera::Type::Projecction:
	default:
		fov = std::clamp(fov, 0.0f, 1.0f);
		projection = Mat4x4::MakePerspectiveFov(fov, aspect, kNearClip, farClip);
		viewProjecction = view * projection;

		viewProjecctionVp = viewProjecction * Mat4x4::MakeViewPort(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);
		break;

	case Camera::Type::Othographic:
		othograohics = Mat4x4::MakeOrthographic(
			clientSize.x * drawScale, clientSize.y * drawScale,
			kNearClip, farClip);
		viewOthograohics = view * othograohics;


		viewOthograohicsVp = viewOthograohics * Mat4x4::MakeViewPort(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);
		break;
	}
}

void Camera::Update(const Mat4x4& worldMat) {
	view = Mat4x4::MakeAffin(scale, rotate, pos);
	view = view * worldMat;
	worldPos = { view[0][3],view[1][3], view[2][3] };
	view = view.Inverse();

	const Vector2 clientSize = Lamb::ClientSize();
	const float aspect = clientSize.x / clientSize.y;

	const auto&& windowSize = WindowFactory::GetInstance()->GetWindowSize();

	switch (type)
	{
	case Camera::Type::Projecction:
	default:
		fov = std::clamp(fov, 0.0f, 1.0f);
		projection = Mat4x4::MakePerspectiveFov(fov, aspect, kNearClip, farClip);
		viewProjecction = view * projection;

		viewProjecctionVp = viewProjecction * Mat4x4::MakeViewPort(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);
		break;

	case Camera::Type::Othographic:
		othograohics = Mat4x4::MakeOrthographic(
			clientSize.x * drawScale, clientSize.y * drawScale,
			kNearClip, farClip);
		viewOthograohics = view * othograohics;


		viewOthograohicsVp = viewOthograohics * Mat4x4::MakeViewPort(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);
		break;
	}
}