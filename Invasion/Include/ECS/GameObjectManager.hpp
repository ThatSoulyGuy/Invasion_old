#pragma once

#include "ECS/GameObject.hpp"

namespace Invasion::ECS
{
	class GameObjectManager
	{

	public:

		Shared<GameObject> Register(Shared<GameObject> gameObject)
		{
			gameObjects += { gameObject->GetName(), gameObject };

			return gameObject;
		}

		Shared<GameObject> Get(const String& name)
		{
			if (gameObjects.Contains(name))
				return gameObjects[name];
			else
				return nullptr;
		}

		void Unregister(const String& name)
		{
			gameObjects -= name;
		}

		void Update()
		{
			gameObjects.ForEach([]( String, Shared<GameObject> gameObject) { gameObject->Update(); });
		}

		void Render(Shared<Invasion::Render::Camera> camera)
		{
			gameObjects.ForEach([camera](String, Shared<GameObject> gameObject) { gameObject->Render(camera); });
		}

		void CleanUp()
		{
			gameObjects.ForEach([](String, Shared<GameObject> gameObject) { gameObject->CleanUp(); });
		}

		static GameObjectManager& GetInstance()
		{
			static GameObjectManager instance;

			return instance;
		}

	private:
		
		OrderedMap<String, Shared<GameObject>> gameObjects;

	};
}