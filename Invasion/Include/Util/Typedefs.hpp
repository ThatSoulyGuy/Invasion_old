#pragma once

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <Windows.h> 
#undef ERROR 
#undef WriteConsole

#include <any>
#include <iostream>
#include <fstream>
#include <ostream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <chrono>
#include <format>
#include <regex>
#include <variant>
#include <functional>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <wrl.h>
#include <DirectXMath.h>
#include "Util/Array.hpp"
#include "Util/BasicMap.hpp"
#include "Util/BasicString.hpp"	
#include "Util/Vector.hpp"

namespace Invasion::Util
{
	using byte = char;

	using Size = std::size_t;

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
	
	using Thread = std::thread;
	using Mutex = std::mutex;
	using ConditionVariable = std::condition_variable;

	using Time = std::time_t;
	using TimeInformation = std::tm;

	using InputFileStream = std::ifstream;
	using OutputFileStream = std::ofstream;

	using OutputStringStream = std::ostringstream;
	using InputStringStream = std::istringstream;

	using Any = std::any;

	using Regex = std::regex;

	using SystemClock = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Duration = std::chrono::duration<float>;

	using Matrix4x4 = DirectX::XMFLOAT4X4;
	using Matrix3x3 = DirectX::XMFLOAT3X3;
	using Matrix = DirectX::XMMATRIX;

	using TypeIndex = std::type_index;
	using TypeInformation = std::type_info;

	template <typename... Args>
	using Variant = std::variant<Args...>;

	template <typename Key, typename Value>
	using UnorderedMap = BasicMap<Key, Value, std::unordered_map>;

	template <typename Key, typename Value>
	using OrderedMap = BasicMap<Key, Value, std::map>;

	template <typename T>
	using Match = std::match_results<T>;

	template <typename T>
	using Function = std::function<T>;

	template <typename T>
	using Unique = std::unique_ptr<T>;

	template <typename T, typename A>
	using Pair = std::pair<T, A>;

	template <typename T>
	using Shared = std::shared_ptr<T>;

	template <typename T>
	using Weak = std::weak_ptr<T>;

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	template <typename T>
	using Future = std::future<T>;

	template <typename T>
	using Atomic = std::atomic<T>;

	template <typename T>
	using Promise = std::promise<T>;
}