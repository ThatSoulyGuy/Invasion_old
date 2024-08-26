#pragma once

#include <array>
#include <initializer_list>

namespace Invasion::Util
{
	template <typename T, size_t length>
	class Array
	{

	public:

		Array() = default;

		Array(const std::array<T, length>& data) : data(data) { }

		Array(std::array<T, length>&& data) : data(std::move(data)) { }

		Array(const Array<T, length>& other) : data(other.data) { }

		Array(Array<T, length>&& other) : data(std::move(other.data)) { }

		Array(std::initializer_list<T> list)
		{
			if (list.size() != length)
				throw std::runtime_error("Invalid initializer list size.");

			size_t i = 0;
			for (const T& element : list)
				data[i++] = element;
		}

		Array<T, length>& operator=(const Array<T, length>& other)
		{
			data = other.data;
			return *this;
		}

		Array<T, length>& operator=(Array<T, length>&& other)
		{
			data = std::move(other.data);
			return *this;
		}

		Array<T, length>& operator=(const std::array<T, length>& other)
		{
			data = other;
			return *this;
		}

		Array<T, length>& operator=(std::array<T, length>&& other)
		{
			data = std::move(other);
			return *this;
		}

		bool operator==(const Array<T, length>& other) const
		{
			return data == other.data;
		}

		bool operator!=(const Array<T, length>& other) const
		{
			return data != other.data;
		}

		T& operator[](size_t index)
		{
			return data[index];
		}

		const T& operator[](size_t index) const
		{
			return data[index];
		}

		size_t Length() const
		{
			return length;
		}

		void Swap(Array<T, length>& other)
		{
			data.swap(other.data);
		}

		auto begin()
		{
			return data.begin();
		}

		auto end()
		{
			return data.end();
		}

		operator std::array<T, length>&()
		{
			return data;
		}

		operator T*()
		{
			return data.data();
		}

	private:

		std::array<T, length> data;

	};
}