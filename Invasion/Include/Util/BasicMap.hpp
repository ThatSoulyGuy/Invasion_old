#pragma once

#include <memory>
#include <map>
#include <mutex>
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

        BasicMap(const BasicMap& other)
        {
            std::lock_guard<std::mutex> lock(other.mutex);
            data = other.data;
        }

        BasicMap(BasicMap&& other) noexcept
        {
            std::lock_guard<std::mutex> lock(other.mutex);
            data = std::move(other.data);
        }

        BasicMap(std::initializer_list<std::pair<const Key, Value>> list) : data(list) {}

        BasicMap(std::map<Key, Value> map) : data(map) {}

        BasicMap(std::unordered_map<Key, Value> map) : data(map) {}

        ~BasicMap() = default;

        BasicMap& operator=(const BasicMap& other)
        {
            if (this != &other)
            {
                std::lock_guard<std::mutex> lock(mutex);
                std::lock_guard<std::mutex> other_lock(other.mutex);
                data = other.data;
            }
            return *this;
        }

        BasicMap& operator=(BasicMap&& other) noexcept
        {
            if (this != &other)
            {
                std::lock_guard<std::mutex> lock(mutex);
                std::lock_guard<std::mutex> other_lock(other.mutex);
                data = std::move(other.data);
            }
            return *this;
        }

        BasicMap& operator=(std::map<Key, Value> map)
        {
            std::lock_guard<std::mutex> lock(mutex);
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
            std::lock_guard<std::mutex> lock(mutex);
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
            std::lock_guard<std::mutex> lock(mutex);
            std::lock_guard<std::mutex> other_lock(other.mutex_);
            return data == other.data;
        }

        bool operator==(const std::map<Key, Value>& map) const
        {
            std::lock_guard<std::mutex> lock(mutex);
            if constexpr (std::is_same_v<T<Key, Value>, std::map<Key, Value>>)
                return data == map;
            else
                return std::map<Key, Value>(data.begin(), data.end()) == map;
        }

        bool operator==(const std::unordered_map<Key, Value>& map) const
        {
            std::lock_guard<std::mutex> lock(mutex);
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
            std::lock_guard<std::mutex> lock(mutex);
            std::lock_guard<std::mutex> other_lock(other.mutex_);
            data.insert(other.data.begin(), other.data.end());
            return *this;
        }

        BasicMap& operator+=(const std::map<Key, Value>& map)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(map.begin(), map.end());
            return *this;
        }

        BasicMap& operator+=(const std::unordered_map<Key, Value>& map)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(map.begin(), map.end());
            return *this;
        }

        BasicMap& operator+=(std::pair<Key, Value> pair)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(pair);
            return *this;
        }

        BasicMap& operator+=(std::initializer_list<std::pair<const Key, Value>> list)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(list);
            return *this;
        }

        BasicMap& operator|=(std::pair<Key, Value> other)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(std::move(other));
            return *this;
        }

        BasicMap& operator|=(std::initializer_list<std::pair<const Key, Value>> list)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.insert(std::move(list));
            return *this;
        }

        BasicMap& operator-=(const BasicMap& other)
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::lock_guard<std::mutex> other_lock(other.mutex_);
            for (const auto& pair : other.data)
                data.erase(pair.first);
            return *this;
        }

        BasicMap& operator-=(const std::map<Key, Value>& map)
        {
            std::lock_guard<std::mutex> lock(mutex);
            for (const auto& pair : map)
                data.erase(pair.first);
            return *this;
        }

        BasicMap& operator-=(const std::unordered_map<Key, Value>& map)
        {
            std::lock_guard<std::mutex> lock(mutex);
            for (const auto& pair : map)
                data.erase(pair.first);
            return *this;
        }

        BasicMap& operator-=(const Key& key)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.erase(key);
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

        Value& operator[](const Key& key)
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data[key];
        }

        const Value& operator[](const Key& key) const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.at(key);
        }

        bool Contains(const Key& key) const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.find(key) != data.end();
        }

        bool IsEmpty() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.empty();
        }

        bool Find(const Key& key, Value& value) const
        {
            std::lock_guard<std::mutex> lock(mutex);
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
            //std::lock_guard<std::mutex> lock(mutex);
            for (auto& pair : data)
                function(pair.first, pair.second);
        }

        void ForEach(const std::function<void(const Key&, const Value&)>& function) const
        {
            //std::lock_guard<std::mutex> lock(mutex);
            for (const auto& pair : data)
                function(pair.first, pair.second);
        }

        void ForEach(const std::function<void(const Key&)>& function)
		{
			//std::lock_guard<std::mutex> lock(mutex);
			for (auto& pair : data)
				function(pair.first);
		}

        void ForEach(const std::function<void(Key&)>& function) const
        {
            //std::lock_guard<std::mutex> lock(mutex);
            for (const auto& pair : data)
				function(pair.first);
        }

        void ForEach(const std::function<void(const Value&)>& function) const
		{
			//std::lock_guard<std::mutex> lock(mutex);
			for (const auto& pair : data)
				function(pair.second);
		}

        auto begin()
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.begin();
        }

        auto end()
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.end();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.clear();
        }

        size_t Count(Key key) const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.count(key);
        }

        size_t Length() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data.size();
        }

        operator std::map<Key, Value>() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            if constexpr (std::is_same_v<T<Key, Value>, std::map<Key, Value>>)
                return data;
            else
                return std::map<Key, Value>(data.begin(), data.end());
        }

        operator std::unordered_map<Key, Value>() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            if constexpr (std::is_same_v<T<Key, Value>, std::unordered_map<Key, Value>>)
                return data;
            else
                return std::unordered_map<Key, Value>(data.begin(), data.end());
        }

    private:

        mutable std::mutex mutex;
        T<Key, Value> data;

    };
}