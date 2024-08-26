#pragma once

#include "Util/Typedefs.hpp"

namespace Invasion::Util
{
	using DefaultVoidFunction = void(*)();

	class Assembly
	{

	public:

		Assembly(const Assembly&) = delete;
		Assembly& operator=(const Assembly&) = delete;

		template <typename T>
		T GetFunction(const String& functionName) const
		{
			return (T)GetProcAddress(handle, functionName);
		}
		
		void CleanUp()
		{
			FreeLibrary(handle);

			handle = nullptr;
		}

		static Shared<Assembly> Create(const String& name, const String& path)
		{
			class Enabled : public Assembly {};
			Shared<Assembly> result = std::make_shared<Enabled>();

			result->name = name;
			result->path = path;

			result->Generate();

			return result;
		}

	private:

		Assembly() = default;

		void Generate()
		{
			handle = LoadLibrary(path);
		}

		String name;
		String path;

		HMODULE handle = nullptr;

	};
}