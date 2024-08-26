#pragma once

#include "Util/Typedefs.hpp"

using namespace Invasion::Util;

namespace Invasion::Core
{
	class Settings
	{

	public:

		Settings(const Settings&) = delete;
		void operator=(const Settings&) = delete;

		template <typename T>
		void Set(const String& key, const T& value)
		{
			settings[key] = std::make_any<T>(value);
		}

		template <typename T>
		T Get(const String& key)
		{
			return std::any_cast<T>(settings[key]);
		}

		template <typename T>
		T Get(const String& key, const T& defaultValue)
		{
			if (!settings.Contains(key))
				return defaultValue;

			return std::any_cast<T>(settings[key]);
		}

		template <typename T>
		bool Contains(const String& key)
		{
			return settings.Contains(key);
		}

		template <typename T>
		void Remove(const String& key)
		{
			settings -= key;
		}

		static Settings& GetInstance()
		{
			static Settings instance;

			return instance;
		}

	private:

		Settings() = default;

		OrderedMap<String, Any> settings;

	};
}