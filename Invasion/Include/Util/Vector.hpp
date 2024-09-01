#pragma once

#include <initializer_list>
#include <functional>
#include <vector>

namespace Invasion::Util
{
	template <typename T>
	class Vector
	{

	public:

		Vector() = default;

		Vector(const Vector& other) : data(other.data) {}

		Vector(int t, int a) : data(t, a) {}

		Vector(Vector&& other) noexcept : data(std::move(other.data)) {}

		Vector(std::initializer_list<T> list) : data(list) {}

		~Vector() = default;

		Vector& operator=(const Vector& other)
		{
			data = other.data;
			return *this;
		}

		Vector& operator=(Vector&& other) noexcept
		{
			data = std::move(other.data);
			return *this;
		}

		Vector& operator=(std::vector<T> vector)
		{
			data = std::move(vector);
			return *this;
		}

		Vector& operator+=(const Vector& other)
		{
			data.insert(data.end(), other.data.begin(), other.data.end());
			return *this;
		}

		Vector& operator+=(const T& value)
		{
			data.push_back(value);
			return *this;
		}

		Vector& operator|=(T&& value)
		{
			data.push_back(std::move(value));

			return *this;
		}

		Vector operator+(const Vector& other) const
		{
			Vector result = *this;
			result += other;
			return result;
		}

		Vector operator+(const T& value) const
		{
			Vector result = *this;
			result += value;
			return result;
		}

		Vector& operator-=(const Vector& other)
		{
			for (const T& value : other.data)
			{
				auto iterator = std::find(data.begin(), data.end(), value);

				if (iterator != data.end())
					data.erase(iterator);
			}

			return *this;
		}

		Vector& operator-=(const T& value)
		{
			auto iterator = std::find(data.begin(), data.end(), value);

			if (iterator != data.end())
				data.erase(iterator);

			return *this;
		}

		Vector operator-(const Vector& other) const
		{
			Vector result = *this;
			result -= other;
			return result;
		}

		Vector operator-(const T& value) const
		{
			Vector result = *this;
			result -= value;
			return result;
		}

		bool operator==(const Vector& other) const
		{
			return data == other.data;
		}

		bool operator==(const std::vector<T>& vector) const
		{
			return data == vector;
		}

		T& operator[](size_t index)
		{
			return data[index];
		}

		const T& operator[](size_t index) const
		{
			return data[index];
		}

		void Clear()
		{
			data.clear();
		}

		size_t Length() const
		{
			return data.size();
		}

		bool IsEmpty() const
		{
			return data.empty();
		}

		T& Front()
		{
			return data.front();
		}

		const T& Front() const
		{
			return data.front();
		}

		T& Back()
		{
			return data.back();
		}

		const T& Back() const
		{
			return data.back();
		}

		void Resize(size_t size)
		{
			data.resize(size);
		}

		void Reserve(size_t size)
		{
			data.reserve(size);
		}

		void ShrinkToFit()
		{
			data.shrink_to_fit();
		}

		void Swap(Vector& other)
		{
			data.swap(other.data);
		}

		void ForEach(const std::function<void(T&&)>& function)
		{
			for (T& value : data)
				function(std::move(value));
		}

		void ForEach(const std::function<void(const T&)>& function) const
		{
			for (const T& value : data)
				function(value);
		}

		void ForEach(const std::function<void(T&)>& function)
		{
			for (T& value : data)
				function(value);
		}

		void ForEach(const std::function<void(const T&, size_t)>& function) const
		{
			size_t index = 0;

			for (const T& value : data)
				function(value, index++);
		}

		void ForEach(const std::function<void(T&, size_t)>& function)
		{
			size_t index = 0;

			for (T& value : data)
				function(value, index++);
		}

		auto begin() const
		{
			return data.begin();
		}

		auto end() const
		{
			return data.end();
		}

		auto begin()
		{
			return data.begin();
		}

		auto end()
		{
			return data.end();
		}

		operator std::vector<T>() const
		{
			return data;
		}

		operator T*()
		{
			return data.data();
		}

	private:

		std::vector<T> data;
	};
}