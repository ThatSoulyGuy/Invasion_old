#pragma once

#include "Render/Texture.hpp"

namespace Invasion::Render
{
	class TextureManager
	{

	public:

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		void Register(Shared<Texture> texture)
		{
			registeredTextures += { texture->GetName(), texture };
		}

		Shared<Texture> Get(const String& name)
		{
			return registeredTextures[name];
		}

		void Unregister(const String& name)
		{
			registeredTextures -= name;
		}

		void CleanUp()
		{
			registeredTextures.ForEach([](const Shared<Texture>& texture) { texture->CleanUp_NoOverride(); });

			registeredTextures.Clear();
		}

		static TextureManager& GetInstance()
		{
			static TextureManager instance;
			return instance;
		}

	private:

		TextureManager() = default;

		UnorderedMap<String, Shared<Texture>> registeredTextures;

	};
}