#pragma once

#include "Core/Window.hpp"
#include "ECS/GameObject.hpp"
#include "Math/Transform.hpp"

using namespace Invasion::Core;
using namespace Invasion::ECS;
using namespace Invasion::Math;

namespace Invasion::Render
{
	class Camera : public Component
	{

	public:

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		
		Matrix GetProjectionMatrix() const
		{
			return DirectX::XMMatrixPerspectiveFovLH(fieldOfView * 0.0174532925f, Window::GetInstance().GetAspectRatio(), nearPlane, farPlane);
		}

		Matrix GetViewMatrix() const
		{
			return DirectX::XMMatrixLookAtLH(GetGameObject()->GetTransform()->GetWorldPosition(), GetGameObject()->GetTransform()->GetWorldPosition() + GetGameObject()->GetTransform()->GetForward(), { 0.0f, 1.0f, 0.0f });
		}

		float GetFieldOfView() const
		{
			return fieldOfView;
		}

		float GetNearPlane() const
		{
			return nearPlane;
		}

		float GetFarPlane() const
		{
			return farPlane;
		}

		static Shared<Camera> Create(float fieldOfView, float nearPlane, float farPlane)
		{
			class Enabled : public Camera { };
			Shared<Camera> result = std::make_shared<Enabled>();

			result->fieldOfView = fieldOfView;
			result->nearPlane = nearPlane;
			result->farPlane = farPlane;

			return std::move(result);
		}

	private:

		Camera() = default;

		float fieldOfView = 0.0f;
		float nearPlane = 0.0f;
		float farPlane = 0.0f;

	};
}