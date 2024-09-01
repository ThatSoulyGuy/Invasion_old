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
			for (auto& [type, component] : components)
				component->Update();
		}

		void Render(Shared<Invasion::Render::Camera> camera)
		{
			for (auto& [type, component] : components)
				component->Render(camera);
		}

		void CleanUp()
		{
			for (auto& [type, component] : components)
				component->CleanUp();

			components.Clear();
		}

		template <typename T>
		Shared<T> AddComponent(Shared<T> component)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			component->gameObject = std::static_pointer_cast<GameObject>(shared_from_this());
			component->Initialize();

			return component;
		}

		template <typename T>
		Shared<T> GetComponent()
		{
			if (components.Contains(typeid(T)))
				return std::static_pointer_cast<T>(components[typeid(T)]);
			else
				return nullptr;
		}

		template <typename T>
		bool HasComponent()
		{
			return components.Contains(typeid(T));
		}

		template <typename T>
		void RemoveComponent()
		{
			components[typeid(T)]->CleanUp();

			if (components.Contains(typeid(T)))
				components -= typeid(T);
		}

		void SetName(const String& name)
		{
			this->name = name;
		}

		String GetName() const
		{
			return name;
		}

		void SetParent(Shared<GameObject> parent)
		{
			assert(parent != nullptr && "Parent cannot be null");

			this->parent = parent;
			GetTransform()->parent = parent->GetTransform();
		}

		Shared<GameObject> GetParent()
		{
			return parent;
		}

		void RemoveParent()
		{
			parent = nullptr;
			GetTransform()->parent = nullptr;
		}

		void AddChild(Shared<GameObject> child)
		{
			child->SetParent(std::static_pointer_cast<GameObject>(shared_from_this()));
			children += { child->GetName(), child };
		}

			if (children.Contains(name))
				return children[name];
			else
				return nullptr;
		}

		void RemoveChild(const String& name)
		{
			children[name]->RemoveParent();

			if (children.Contains(name))
				children -= name;
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

		String name;
		Shared<GameObject> parent;
		UnorderedMap<String, Shared<GameObject>> children;
		UnorderedMap<TypeIndex, Shared<Component>> components;

	};
}