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

		void Remove()
		{
			textureAtlases.ForEach([](Shared<TextureAtlas> image) { image->CleanUp_NoOverride(); });
		}

	private:

		UnorderedMap<String, Shared<TextureAtlas>> textureAtlases;

	};
}