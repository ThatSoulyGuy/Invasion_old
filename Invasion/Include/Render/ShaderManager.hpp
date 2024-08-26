#pragma once

#include "Render/Shader.hpp"

namespace Invasion::Render
{
	class ShaderManager
	{

	public:

		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
		
		void Register(Shared<Shader> shader)
		{
			registeredShaders += { shader->GetName(), shader };
		}

		Shared<Shader> Get(const String& name)
		{
			return registeredShaders[name];
		}

		void Unregister(const String& name)
		{
			registeredShaders -= name;
		}

		void CleanUp()
		{
			registeredShaders.ForEach([](const Shared<Shader>& shader) { shader->CleanUp_NoOverride(); });

			registeredShaders.Clear();
		}

		static ShaderManager& GetInstance()
		{
			static ShaderManager instance;
			return instance;
		}

	private:

		ShaderManager() = default;

		UnorderedMap<String, Shared<Shader>> registeredShaders;

	};
}