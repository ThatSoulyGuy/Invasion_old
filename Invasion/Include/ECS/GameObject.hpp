#pragma once

#include "ECS/Component.hpp"
#include "Math/Transform.hpp"

using namespace Invasion::Math;

namespace Invasion::ECS
{
    class GameObject : public std::enable_shared_from_this<GameObject>
    {

    public:

        void Update()
        {
            LockGuard<Mutex> lock(mutex);

            for (auto& [type, component] : components)
                component->Update();
        }

        void Render(Shared<Invasion::Render::Camera> camera)
        {
            LockGuard<Mutex> lock(mutex);

            for (auto& [type, component] : components)
                component->Render(camera);
        }

        void CleanUp()
        {
            LockGuard<Mutex> lock(mutex);

            for (auto& [type, component] : components)
                component->CleanUp();
         
            components.Clear();
        }

        template <typename T>
        Shared<T> AddComponent(Shared<T> component)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            LockGuard<Mutex> lock(mutex);

            component->gameObject = std::static_pointer_cast<GameObject>(shared_from_this());
            component->Initialize();

            components += { typeid(T), std::move(component) };

            return std::static_pointer_cast<T>(components[typeid(T)]);
        }

        template <typename T>
        Shared<T> GetComponent()
        {
            Shared<Component> component = nullptr;

            {
                if (components.Contains(typeid(T)))
                    component = components[typeid(T)];
            }

            if (component != nullptr)
                return std::static_pointer_cast<T>(component);
            else
                return nullptr;
        }

        template <typename T>
        bool HasComponent()
        {
            LockGuard<Mutex> lock(mutex);
            return components.Contains(typeid(T));
        }

        template <typename T>
        void RemoveComponent()
        {
            LockGuard<Mutex> lock(mutex);

            if (components.Contains(typeid(T)))
            {
                components[typeid(T)]->CleanUp();
                components -= typeid(T);
            }
        }

        void SetName(const String& name)
        {
            LockGuard<Mutex> lock(mutex);
            this->name = name;
        }

        String GetName() const
        {
            LockGuard<Mutex> lock(mutex);
            return name;
        }

        void SetParent(Shared<GameObject> parent)
        {
            assert(parent != nullptr && "Parent cannot be null");

            Shared<Transform> parentTransform;
            {
                LockGuard<Mutex> lock(mutex);
                this->parent = parent;
                parentTransform = parent->GetTransform();
            }

            GetTransform()->parent = parentTransform;
        }

        Shared<GameObject> GetParent()
        {
            LockGuard<Mutex> lock(mutex);
            return parent.lock();
        }

        void AddChild(Shared<GameObject> child)
        {
            if (child == nullptr)
                return;

            child->SetParent(std::static_pointer_cast<GameObject>(shared_from_this()));

            {
                children += { child->GetName(), child };
            }
        }

        void RemoveParent()
        {
            {
                LockGuard<Mutex> lock(mutex);
                parent.reset();
            }

            GetTransform()->parent = nullptr;
        }

        Shared<GameObject> GetChild(const String& name)
        {
            if (children.Contains(name))
                return children[name].lock();
            else
                return nullptr;
        }

        void RemoveChild(const String& name)
        {
            LockGuard<Mutex> lock(mutex);

            if (children.Contains(name))
            {
                Weak<GameObject> child = children[name];
                {
                    LockGuard<Mutex> childLock(child.lock()->mutex);
                    child.lock()->parent.reset();
                }
                children -= name;
            }
        }

        Shared<Transform> GetTransform()
        {
            return GetComponent<Transform>();
        }

        static Shared<GameObject> Create(const String& name)
        {
            Shared<GameObject> result = std::make_shared<GameObject>();

            result->name = name;
            result->AddComponent(Transform::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

            return std::move(result);
        }

    private:

        mutable Mutex mutex;

        String name;
        Weak<GameObject> parent;
        UnorderedMap<String, Weak<GameObject>> children;
        UnorderedMap<TypeIndex, Shared<Component>> components;

    };
}