#pragma once

#include "World/TextureAtlas.hpp"

namespace Invasion::World
{
	class TextureAtlasManager	
	{

	public:
		
		void Register(Shared<TextureAtlas> textureAtlas)
		{
			textureAtlases += { textureAtlas->GetName(), textureAtlas };
		}

		Shared<TextureAtlas> Get(const String& name)
		{
			return textureAtlases[name];
		}

		void Remove(const String& name)
		{
			textureAtlases[name]->CleanUp_NoOverride();

			textureAtlases -= name;
		}

		void CleanUp()
		{
			textureAtlases.ForEach([](Shared<TextureAtlas> image) { image->CleanUp(); });
		}

		static TextureAtlasManager& GetInstance()
		{
			static TextureAtlasManager instance;

			return instance;
		}

	private:

		UnorderedMap<String, Shared<TextureAtlas>> textureAtlases;

	};
}