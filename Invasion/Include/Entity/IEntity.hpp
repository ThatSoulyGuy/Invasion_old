#pragma once

#include "ECS/GameObject.hpp"

namespace Invasion::Entity
{
	struct EntityRegistration
	{

	public:

		String registryName = "";

		float maxHealth = 0;

		float movementSpeed = 0;
		float runningSpeed = 0;

		bool canJump = false;
		bool canCrouch = false;

		EntityRegistration SetRegistryName(const String& registryName)
		{
			this->registryName = registryName;

			return *this;
		}

		EntityRegistration SetMaxHealth(float maxHealth)
		{
			this->maxHealth = maxHealth;

			return *this;
		}

		EntityRegistration SetMovementSpeed(float movementSpeed)
		{
			this->movementSpeed = movementSpeed;

			return *this;
		}

		EntityRegistration SetRunningSpeed(float runningSpeed)
		{
			this->runningSpeed = runningSpeed;

			return *this;
		}

		EntityRegistration SetCanJump(bool canJump)
		{
			this->canJump = canJump;

			return *this;
		}

		EntityRegistration SetCanCrouch(bool canCrouch)
		{
			this->canCrouch = canCrouch;

			return *this;
		}

		static EntityRegistration New()
		{
			return EntityRegistration();
		}

	private:

		EntityRegistration() = default;

	};

	class IEntity : public Component
	{

	public:

		IEntity(const IEntity&) = delete;
		IEntity& operator=(const IEntity&) = delete;

		String GetRegistryName() const
		{
			return registryName;
		}

		float GetMaxHealth() const
		{
			return maxHealth;
		}

		float GetCurrentHealth() const
		{
			return currentHealth;
		}

		float GetMovementSpeed() const
		{
			return movementSpeed;
		}

		float GetRunningSpeed() const
		{
			return runningSpeed;
		}

		bool CanJump() const
		{
			return canJump;
		}

		bool CanCrouch() const
		{
			return canCrouch;
		}

		template <typename T>
		static Shared<T> Create()
		{
			static_assert(std::is_base_of<IEntity, T>::value, "T must derive from IEntity.");

			class Enabled : public T { };
			Shared<T> result = std::make_shared<Enabled>();

			EntityRegistration registration = result->GetRegistration();

			result->registryName = registration.registryName;
			result->maxHealth = registration.maxHealth;
			result->currentHealth = registration.maxHealth;
			result->movementSpeed = registration.movementSpeed;
			result->runningSpeed = registration.runningSpeed;
			result->canJump = registration.canJump;
			result->canCrouch = registration.canCrouch;

			return std::move(result);
		}

	protected:

		IEntity() = default;

		virtual EntityRegistration GetRegistration() const = 0;

	private:

		String registryName = "";

		float maxHealth = 0;
		float currentHealth = 0;

		float movementSpeed = 0;
		float runningSpeed = 0;

		bool canJump = false;
		bool canCrouch = false;

	};
}