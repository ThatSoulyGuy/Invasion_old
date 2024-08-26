#pragma once

#include "Util/Typedefs.hpp"

using namespace Invasion::Util;

namespace Invasion::Render
{
	class Camera;
}

namespace Invasion::ECS
{
	class GameObject;

	class Component
	{

	public:

		virtual void Initialize() { }

		virtual void Update() { }
		virtual void Render(Shared<Invasion::Render::Camera> camera) { }

		virtual void CleanUp() { }

		Shared<GameObject> GetGameObject() const { return gameObject; }

	private:

		friend class GameObject;

		Shared<GameObject> gameObject = nullptr;

	};
}