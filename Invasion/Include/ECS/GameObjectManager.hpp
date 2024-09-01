#pragma once

#include "ECS/GameObject.hpp"

namespace Invasion::ECS
{
    class GameObjectManager
    {
    public:

        Shared<GameObject> Register(Shared<GameObject> gameObject)
        {
            LockGuard<Mutex> lock(mutex);

            String name = gameObject->GetName();

            gameObjects |= { name, gameObject };

            return gameObjects[name];
        }

        Shared<GameObject> Get(const String& name)
        {
            LockGuard<Mutex> lock(mutex); 

            if (gameObjects.Contains(name))
                return gameObjects[name];
            else
                return nullptr;
        }

        void Unregister(const String& name)
        {
            LockGuard<Mutex> lock(mutex);

            if (gameObjects.Contains(name)) 
            {
                gameObjects[name]->CleanUp();
                gameObjects[name].reset();
                gameObjects -= name;
            }
        }

        void Update()
        {
            LockGuard<Mutex> lock(mutex);

            gameObjects.ForEach([](String, Shared<GameObject> gameObject) { gameObject->Update(); });
        }

        void Render(Shared<Invasion::Render::Camera> camera)
        {
            LockGuard<Mutex> lock(mutex);

            gameObjects.ForEach([camera](String, Shared<GameObject> gameObject) { gameObject->Render(camera); });
        }

        void CleanUp()
        {
            LockGuard<Mutex> lock(mutex);

            gameObjects.ForEach([](String, Shared<GameObject> gameObject) { gameObject->CleanUp(); });
            gameObjects.Clear();
        }

        static GameObjectManager& GetInstance()
        {
            static GameObjectManager instance;
            return instance;
        }

    private:

        GameObjectManager() = default;

        Mutex mutex;
        OrderedMap<String, Shared<GameObject>> gameObjects;
    };
}