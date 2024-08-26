#pragma once

#include "Core/InputManager.hpp"
#include "ECS/GameObjectManager.hpp"
#include "Entity/IEntity.hpp"
#include "Render/Camera.hpp"
#include "Render/Renderer.hpp"

using namespace Invasion::Core;
using namespace Invasion::Entity;
using namespace Invasion::Render;

namespace Invasion::Entity::Entities
{
	class EntityPlayer : public IEntity
	{

	public:

		virtual void Initialize() override
		{
			InputManager::GetInstance().SetCursorState(CursorState::LOCKED);

			GetGameObject()->AddChild(GameObjectManager::GetInstance().Register(GameObject::Create("Camera")));
			GetGameObject()->GetChild("Camera")->AddComponent(Camera::Create(45.0f, 0.01f, 1000.0f));
		}

		virtual void Update() override
		{
			UpdateMouseLook();
			UpdateMovement();
		}

		Shared<Camera> GetCamera() const
		{
			return GetGameObject()->GetChild("Camera")->GetComponent<Camera>();
		}

	protected:

		friend class IEntity;

		virtual EntityRegistration GetRegistration() const override
		{
			return EntityRegistration::New()
				.SetRegistryName("entity_player")
				.SetMaxHealth(100.0f)
				.SetMovementSpeed(10.0f)
				.SetRunningSpeed(20.0f)
				.SetCanJump(true)
				.SetCanCrouch(true);
		}

	private:

		void UpdateMouseLook()
		{
			Vector2i mouseDelta = InputManager::GetInstance().GetMouseDelta();
			const float sensitivity = 0.0035f;

			float yaw = static_cast<float>(mouseDelta.x) * sensitivity;
			float pitch = static_cast<float>(mouseDelta.y) * sensitivity;

			auto transform = GetCamera()->GetGameObject()->GetComponent<Transform>();

			Vector3f eulerRotation = transform->GetLocalRotation();

			eulerRotation.y += yaw;
			eulerRotation.x += pitch;

			if (eulerRotation.x > 89.0f)
				eulerRotation.x = 89.0f;

			if (eulerRotation.x < -89.0f)
				eulerRotation.x = -89.0f;

			transform->SetLocalRotation(eulerRotation);
		}

		void UpdateMovement()
		{
			Shared<Transform> cameraTransform = GetCamera()->GetGameObject()->GetComponent<Transform>();
			Vector3f movement = { 0.0f, 0.0f, 0.0f };

			float deltaTime = Window::GetInstance().GetDeltaTime();

			if (InputManager::GetInstance().GetKeyState(KeyCode::W, KeyState::HELD))
				movement += cameraTransform->GetForward();

			if (InputManager::GetInstance().GetKeyState(KeyCode::S, KeyState::HELD))
				movement -= cameraTransform->GetForward();

			if (InputManager::GetInstance().GetKeyState(KeyCode::A, KeyState::HELD))
				movement -= cameraTransform->GetRight();

			if (InputManager::GetInstance().GetKeyState(KeyCode::D, KeyState::HELD))
				movement += cameraTransform->GetRight();

			if (movement.Length() > 0)
			{
				movement = movement.Normalized();

				movement *= GetMovementSpeed() * deltaTime;

				GetGameObject()->GetComponent<Transform>()->Translate(movement);
			}
		}
	};
}