#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <initializer_list>
#include <functional>

namespace Invasion::Util
{
	template <typename Key, typename Value, template <typename, typename> class T>
	class BasicMap
	{

	public:
		
		BasicMap() = default;

		BasicMap(const BasicMap& other) : data(other.data) {}

		BasicMap(BasicMap&& other) noexcept : data(std::move(other.data)) {}

		BasicMap(std::initializer_list<std::pair<const Key, Value>> list) : data(list) {}

		BasicMap(std::map<Key, Value> map) : data(map) {}

		BasicMap(std::unordered_map<Key, Value> map) : data(map) {}

		~BasicMap() = default;
		
		BasicMap& operator=(const BasicMap& other)
		{
			data = other.data;
			return *this;
		}
		
		BasicMap& operator=(BasicMap&& other) noexcept
		{
			data = std::move(other.data);
			return *this;
		}

		BasicMap& operator=(std::map<Key, Value> map)
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::map<Key, Value>>)
				data = std::move(map);
			else
			{
				data.clear();
				data.insert(map.begin(), map.end());
			}

			return *this;
		}

		BasicMap& operator=(std::unordered_map<Key, Value> map)
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::unordered_map<Key, Value>>)
				data = std::move(map);
			else
			{
				data.clear();
				data.insert(map.begin(), map.end());
			}

			return *this;
		}

		bool operator==(const BasicMap& other) const
		{
			return data == other.data;
		}

		bool operator==(const std::map<Key, Value>& map) const
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::map<Key, Value>>)
				return data == map;
			else
				return std::map<Key, Value>(data.begin(), data.end()) == map;
		}

		bool operator==(const std::unordered_map<Key, Value>& map) const
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::unordered_map<Key, Value>>)
				return data == map;
			else
				return std::unordered_map<Key, Value>(data.begin(), data.end()) == map;
		}

		bool operator!=(const BasicMap& other) const
		{
			return !(*this == other);
		}

		bool operator!=(const std::map<Key, Value>& map) const
		{
			return !(*this == map);
		}

		bool operator!=(const std::unordered_map<Key, Value>& map) const
		{
			return !(*this == map);
		}

		BasicMap& operator+=(const BasicMap& other)
		{
			data.insert(other.data.begin(), other.data.end());
			return *this;
		}

		BasicMap& operator+=(const std::map<Key, Value>& map)
		{
			data.insert(map.begin(), map.end());
			return *this;
		}

		BasicMap& operator+=(const std::unordered_map<Key, Value>& map)
		{
			data.insert(map.begin(), map.end());
			return *this;
		}

		BasicMap& operator+=(std::pair<Key, Value> pair)
		{
			data.insert(pair);
			return *this;
		}

		BasicMap& operator+=(std::initializer_list<std::pair<const Key, Value>> list)
		{
			data.insert(list);
			return *this;
		}

		BasicMap& operator+=(std::initializer_list<std::pair<Key, Value>> list)
		{
			data.insert(list.begin(), list.end());
			return *this;
		}

		BasicMap& operator-=(const BasicMap& other)
		{
			for (const auto& pair : other.data)
				data.erase(pair.first);

			return *this;
		}

		BasicMap& operator-=(const std::map<Key, Value>& map)
		{
			for (const auto& pair : map)
				data.erase(pair.first);

			return *this;
		}

		BasicMap& operator-=(const std::unordered_map<Key, Value>& map)
		{
			for (const auto& pair : map)
				data.erase(pair.first);

			return *this;
		}

		BasicMap& operator-=(const Key& key)
		{
			data.erase(key);
			return *this;
		}

		BasicMap& operator-=(std::initializer_list<Key> list)
		{
			for (const Key& key : list)
				data.erase(key);

			return *this;
		}

		BasicMap& operator-=(std::initializer_list<std::pair<const Key, Value>> list)
		{
			for (const auto& pair : list)
				data.erase(pair.first);

			return *this;
		}

		BasicMap& operator-=(std::initializer_list<std::pair<Key, Value>> list)
		{
			for (const auto& pair : list)
				data.erase(pair.first);

			return *this;
		}

		BasicMap operator+(const BasicMap& other) const
		{
			BasicMap result = *this;
			result += other;
			return result;
		}

		BasicMap operator+(const std::map<Key, Value>& map) const
		{
			BasicMap result = *this;
			result += map;
			return result;
		}

		BasicMap operator+(const std::unordered_map<Key, Value>& map) const
		{
			BasicMap result = *this;
			result += map;
			return result;
		}

		BasicMap operator-(const BasicMap& other) const
		{
			BasicMap result = *this;
			result -= other;
			return result;
		}

		BasicMap operator-(const std::map<Key, Value>& map) const
		{
			BasicMap result = *this;
			result -= map;
			return result;
		}

		BasicMap operator-(const std::unordered_map<Key, Value>& map) const
		{
			BasicMap result = *this;
			result -= map;
			return result;
		}

		Value& operator[](const Key& key)
		{
			return data[key];
		}

		bool Contains(const Key& key) const
		{
			return data.find(key) != data.end();
		}

		bool IsEmpty() const
		{
			return data.empty();
		}

		bool Find(const Key& key, Value& value) const
		{
			auto iterator = data.find(key);

			if (iterator != data.end())
			{
				value = iterator->second;
				return true;
			}

			return false;
		}
		
		void ForEach(const std::function<void(const Key&, Value&)>& function)
		{
			for (auto& pair : data)
				function(pair.first, pair.second);
		}

		void ForEach(const std::function<void(const Key&, const Value&)>& function) const
		{
			for (const auto& pair : data)
				function(pair.first, pair.second);
		}

		void ForEach(const std::function<void(Value&)>& function)
		{
			for (auto& pair : data)
				function(pair.second);
		}

		void ForEach(const std::function<void(const Value&)>& function) const
		{
			for (const auto& pair : data)
				function(pair.second);
		}

		void ForEach(const std::function<void(const Key&)>& function)
		{
			for (auto& pair : data)
				function(pair.first);
		}

		void ForEach(const std::function<void()>& function)
		{
			for (auto& pair : data)
				function();
		}

		auto begin()
		{
			return data.begin();
		}

		auto end()
		{
			return data.end();
		}

		void Clear()
		{
			data.clear();
		}

		size_t Count(Key key) const
		{
			return data.count(key);
		}

		size_t Length() const
		{
			return data.size();
		}

		operator std::map<Key, Value>() const
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::map<Key, Value>>)
				return data;
			else
				return std::map<Key, Value>(data.begin(), data.end());
		}

		operator std::unordered_map<Key, Value>() const
		{
			if constexpr (std::is_same_v<T<Key, Value>, std::unordered_map<Key, Value>>)
				return data;
			else
				return std::unordered_map<Key, Value>(data.begin(), data.end());
		}

	private:

		T<Key, Value> data;

	};
}