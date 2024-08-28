#pragma once

#include <iostream>
#include <ostream>
#include <istream>
#include <string>
#include <string_view>
#include <memory>
#include <codecvt>
#include <filesystem>
#include <locale>
#include <algorithm>

namespace Invasion::Util
{
    template <typename T>
    class BasicString
    {
        static_assert(std::is_same_v<T, char> || std::is_same_v<T, wchar_t>);

    public:

        BasicString()
        {
            data = std::basic_string<T>();
        }

        BasicString(std::filesystem::path path) : data(path.string()) { }

        BasicString(size_t length, char character)
		{
			if constexpr (std::is_same_v<T, wchar_t>)
			{
				data = std::wstring(length, static_cast<wchar_t>(character));
			}
			else
				data = std::string(length, character);
		}

        BasicString(size_t length, wchar_t character)
		{
			if constexpr (std::is_same_v<T, char>)
				data = std::string(length, static_cast<char>(character));
			else
				data = std::wstring(length, character);
		}

        BasicString(char character)
		{
            if constexpr (std::is_same_v<T, wchar_t>)
            {
				data.clear();
                data.push_back(static_cast<wchar_t>(character));
            }
            else
            {
                data.clear();
                data.push_back(character);
            }
		}

        BasicString(wchar_t character)
        {
            if constexpr (std::is_same_v<T, char>)
				data = std::string(1, character);
			else
				data = std::wstring(1, character);
        }

        BasicString(const char* data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::string& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::string&& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::string_view& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data.data());
            }
            else
                this->data = data;
        }

        BasicString(const std::string_view&& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data.data());
            }
            else
                this->data = data;
        }

        BasicString(const wchar_t* data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::wstring& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::wstring&& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
        }

        BasicString(const std::wstring_view& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data.data());
            }
            else
                this->data = data;
        }

        BasicString(const std::wstring_view&& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data.data());
            }
            else
                this->data = data;
        }

        BasicString& operator=(const char* data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;

            return *this;
        }

        BasicString& operator=(const std::string& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::string&& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data);
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::string_view& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data.data());
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::string_view&& data)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.from_bytes(data.data());
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const wchar_t* data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::wstring& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::wstring&& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data);
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::wstring_view& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data.data());
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator=(const std::wstring_view&& data)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data = converter.to_bytes(data.data());
            }
            else
                this->data = data;
            return *this;
        }

        BasicString& operator+=(const BasicString& other)
        {
            this->data += other.data;
            return *this;
        }

        BasicString& operator+=(const BasicString&& other)
        {
            this->data += std::move(other.data);
            return *this;
        }

        BasicString& operator+=(const char* other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::string& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::string&& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::string_view& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator+=(const std::string_view&& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator+=(const wchar_t* other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::wstring& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::wstring&& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(const std::wstring_view& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator+=(const std::wstring_view&& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator+=(char other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                this->data += static_cast<wchar_t>(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator+=(wchar_t other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                this->data += static_cast<char>(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString operator+(const char* other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::string& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::string&& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::string_view& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::string_view&& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const wchar_t* other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::wstring& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::wstring&& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::wstring_view& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const std::wstring_view&& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(char other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(wchar_t other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const BasicString& other) const
        {
            BasicString result = *this;
            result += other;
            return result;
        }

        BasicString operator+(const BasicString&& other) const
        {
            BasicString result = *this;
            result += std::move(other);
            return result;
        }

        BasicString& operator<<(const BasicString& other)
        {
            this->data += other.data;
            return *this;
        }

        BasicString& operator<<(const BasicString&& other)
        {
            this->data += std::move(other.data);
            return *this;
        }

        BasicString& operator<<(const char* other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::string& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::string&& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::string_view& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator<<(const std::string_view&& other)
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.from_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator<<(const wchar_t* other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::wstring& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::wstring&& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other);
            }
            else
                this->data += other;
            return *this;
        }

        BasicString& operator<<(const std::wstring_view& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        BasicString& operator<<(const std::wstring_view&& other)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                this->data += converter.to_bytes(other.data());
            }
            else
                this->data += other.data();
            return *this;
        }

        std::ostream& operator<<(std::ostream& stream) const
		{
			stream << this->data;
			return stream;
		}

        std::istream& operator>>(std::istream& stream)
        {
            stream >> this->data;
            return stream;
        }

        bool operator==(const BasicString& other) const
        {
            return this->data == other.data;
        }

        bool operator==(const BasicString&& other) const
        {
            return this->data == std::move(other.data);
        }

        bool operator!=(const BasicString& other) const
        {
            return this->data != other.data;
        }

        bool operator!=(const BasicString&& other) const
        {
            return this->data != std::move(other.data);
        }

        bool operator<(const BasicString& other) const
        {
            return this->data < other.data;
        }

        bool operator<(const BasicString&& other) const
        {
            return this->data < std::move(other.data);
        }

        bool operator>(const BasicString& other) const
        {
            return this->data > other.data;
        }

        bool operator>(const BasicString&& other) const
        {
            return this->data > std::move(other.data);
        }

        bool operator<=(const BasicString& other) const
        {
            return this->data <= other.data;
        }

        bool operator<=(const BasicString&& other) const
        {
            return this->data <= std::move(other.data);
        }

        bool operator>=(const BasicString& other) const
        {
            return this->data >= other.data;
        }

        bool operator>=(const BasicString&& other) const
        {
            return this->data >= std::move(other.data);
        }

        T& operator[](size_t index)
        {
            return this->data[index];
        }

        const T& operator[](size_t index) const
        {
            return this->data[index];
        }

        size_t Length() const
        {
            return this->data.length();
        }

        BasicString<T>& FindAndReplace(const BasicString& find, const BasicString& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find.data, pos)) != std::string::npos)
            {
                this->data.replace(pos, find.data.length(), replace.data);
                pos += replace.data.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const BasicString&& find, const BasicString&& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find.data, pos)) != std::string::npos)
            {
                this->data.replace(pos, find.data.length(), replace.data);
                pos += replace.data.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const char* find, const char* replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find, pos)) != std::string::npos)
            {
                this->data.replace(pos, strlen(find), replace);
                pos += strlen(replace);
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::string& find, const std::string& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find, pos)) != std::string::npos)
            {
                this->data.replace(pos, find.length(), replace);
                pos += replace.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::string&& find, const std::string&& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find, pos)) != std::string::npos)
            {
                this->data.replace(pos, find.length(), replace);
                pos += replace.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::string_view& find, const std::string_view& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find.data(), pos)) != std::string::npos)
            {
                this->data.replace(pos, find.length(), replace.data());
                pos += replace.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::string_view&& find, const std::string_view&& replace)
        {
            size_t pos = 0;
            while ((pos = this->data.find(find.data(), pos)) != std::string::npos)
            {
                this->data.replace(pos, find.length(), replace.data());
                pos += replace.length();
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const wchar_t* find, const wchar_t* replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = 0;
                while ((pos = this->data.find(find_str, pos)) != std::string::npos)
                {
                    this->data.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
            else
            {
                size_t pos = 0;
                while ((pos = this->data.find(find, pos)) != std::wstring::npos)
                {
                    this->data.replace(pos, wcslen(find), replace);
                    pos += wcslen(replace);
                }
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::wstring& find, const std::wstring& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = 0;
                while ((pos = this->data.find(find_str, pos)) != std::string::npos)
                {
                    this->data.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
            else
            {
                size_t pos = 0;
                while ((pos = this->data.find(find, pos)) != std::wstring::npos)
                {
                    this->data.replace(pos, find.length(), replace);
                    pos += replace.length();
                }
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::wstring&& find, const std::wstring&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = 0;
                while ((pos = this->data.find(find_str, pos)) != std::string::npos)
                {
                    this->data.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
            else
            {
                size_t pos = 0;
                while ((pos = this->data.find(find, pos)) != std::wstring::npos)
                {
                    this->data.replace(pos, find.length(), replace);
                    pos += replace.length();
                }
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::wstring_view& find, const std::wstring_view& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(std::wstring(find));
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                size_t pos = 0;
                while ((pos = this->data.find(find_str, pos)) != std::string::npos)
                {
                    this->data.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
            else
            {
                size_t pos = 0;
                while ((pos = this->data.find(find.data(), pos)) != std::wstring::npos)
                {
                    this->data.replace(pos, find.length(), replace.data());
                    pos += replace.length();
                }
            }

            return *this;
        }

        BasicString<T>& FindAndReplace(const std::wstring_view&& find, const std::wstring_view&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(std::wstring(find));
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                size_t pos = 0;
                while ((pos = this->data.find(find_str, pos)) != std::string::npos)
                {
                    this->data.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
            else
            {
                size_t pos = 0;
                while ((pos = this->data.find(find.data(), pos)) != std::wstring::npos)
                {
                    this->data.replace(pos, find.length(), replace.data());
                    pos += replace.length();
                }
            }

            return *this;
        }

        BasicString SubString(size_t start, size_t length)
        {
            return BasicString(this->data.substr(start, length));
        }

        BasicString SubString(size_t start)
        {
            return BasicString(this->data.substr(start));
        }

        BasicString& ToUpper()
        {
            std::transform(this->data.begin(), this->data.end(), this->data.begin(), ::toupper);
            return *this;
        }

        BasicString& ToLower()
        {
            std::transform(this->data.begin(), this->data.end(), this->data.begin(), ::tolower);
            return *this;
        }

        BasicString& Trim()
        {
            this->data.erase(0, this->data.find_first_not_of(' '));
            this->data.erase(this->data.find_last_not_of(' ') + 1);
            return *this;
        }

        BasicString& Trim(const T& character)
        {
            this->data.erase(0, this->data.find_first_not_of(character));
            this->data.erase(this->data.find_last_not_of(character) + 1);
            return *this;
        }

        BasicString& TrimStart()
        {
            this->data.erase(0, this->data.find_first_not_of(' '));
            return *this;
        }

        BasicString& TrimStart(const T& character)
        {
            this->data.erase(0, this->data.find_first_not_of(character));
            return *this;
        }

        BasicString& TrimEnd()
        {
            this->data.erase(this->data.find_last_not_of(' ') + 1);
            return *this;
        }

        BasicString& TrimEnd(const T& character)
        {
            this->data.erase(this->data.find_last_not_of(character) + 1);
            return *this;
        }

        BasicString& Replace(const BasicString& find, const BasicString& replace)
        {
            size_t pos = this->data.find(find.data);
            if (pos != std::string::npos)
                this->data.replace(pos, find.data.length(), replace.data);
            return *this;
        }

        BasicString& Replace(const BasicString&& find, const BasicString&& replace)
        {
            size_t pos = this->data.find(find.data);
            if (pos != std::string::npos)
                this->data.replace(pos, find.data.length(), replace.data);
            return *this;
        }

        BasicString& Replace(const char* find, const char* replace)
        {
            size_t pos = this->data.find(find);
            if (pos != std::string::npos)
                this->data.replace(pos, strlen(find), replace);
            return *this;
        }

        BasicString& Replace(const std::string& find, const std::string& replace)
        {
            size_t pos = this->data.find(find);
            if (pos != std::string::npos)
                this->data.replace(pos, find.length(), replace);
            return *this;
        }

        BasicString& Replace(const std::string&& find, const std::string&& replace)
        {
            size_t pos = this->data.find(find);
            if (pos != std::string::npos)
                this->data.replace(pos, find.length(), replace);
            return *this;
        }

        BasicString& Replace(const std::string_view& find, const std::string_view& replace)
        {
            size_t pos = this->data.find(find.data());
            if (pos != std::string::npos)
                this->data.replace(pos, find.length(), replace.data());
            return *this;
        }

        BasicString& Replace(const std::string_view&& find, const std::string_view&& replace)
        {
            size_t pos = this->data.find(find.data());
            if (pos != std::string::npos)
                this->data.replace(pos, find.length(), replace.data());
            return *this;
        }

        BasicString& Replace(const wchar_t* find, const wchar_t* replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = this->data.find(find_str);
                if (pos != std::string::npos)
                    this->data.replace(pos, find_str.length(), replace_str);
                return *this;
            }
            else
            {
                size_t pos = this->data.find(find);
                if (pos != std::wstring::npos)
                    this->data.replace(pos, wcslen(find), replace);
                return *this;
            }
        }

        BasicString& Replace(const std::wstring& find, const std::wstring& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = this->data.find(find_str);
                if (pos != std::string::npos)
                    this->data.replace(pos, find_str.length(), replace_str);
                return *this;
            }
            else
            {
                size_t pos = this->data.find(find);
                if (pos != std::wstring::npos)
                    this->data.replace(pos, find.length(), replace);
                return *this;
            }
        }

        BasicString& Replace(const std::wstring&& find, const std::wstring&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(find);
                std::string replace_str = converter.to_bytes(replace);
                size_t pos = this->data.find(find_str);
                if (pos != std::string::npos)
                    this->data.replace(pos, find_str.length(), replace_str);
                return *this;
            }
            else
            {
                size_t pos = this->data.find(find);
                if (pos != std::wstring::npos)
                    this->data.replace(pos, find.length(), replace);
                return *this;
            }
        }

        BasicString& Replace(const std::wstring_view& find, const std::wstring_view& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(std::wstring(find));
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                size_t pos = this->data.find(find_str);
                if (pos != std::string::npos)
                    this->data.replace(pos, find_str.length(), replace_str);
                return *this;
            }
            else
            {
                size_t pos = this->data.find(find.data());
                if (pos != std::wstring::npos)
                    this->data.replace(pos, find.length(), replace.data());
                return *this;
            }
        }

        BasicString& Replace(const std::wstring_view&& find, const std::wstring_view&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string find_str = converter.to_bytes(std::wstring(find));
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                size_t pos = this->data.find(find_str);
                if (pos != std::string::npos)
                    this->data.replace(pos, find_str.length(), replace_str);
                return *this;
            }
            else
            {
                size_t pos = this->data.find(find.data());
                if (pos != std::wstring::npos)
                    this->data.replace(pos, find.length(), replace.data());
                return *this;
            }
        }

        BasicString& Replace(const T& find, const T& replace)
        {
            size_t pos = this->data.find(find);
            if (pos != std::string::npos)
                this->data.replace(pos, 1, 1, replace);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const BasicString& replace)
        {
            this->data.replace(start, length, replace.data);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const BasicString&& replace)
        {
            this->data.replace(start, length, replace.data);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const char* replace)
        {
            this->data.replace(start, length, replace);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::string& replace)
        {
            this->data.replace(start, length, replace);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::string&& replace)
        {
            this->data.replace(start, length, replace);
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::string_view& replace)
        {
            this->data.replace(start, length, replace.data());
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::string_view&& replace)
        {
            this->data.replace(start, length, replace.data());
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const wchar_t* replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string replace_str = converter.to_bytes(replace);
                this->data.replace(start, length, replace_str);
            }
            else
            {
                this->data.replace(start, length, replace);
            }
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::wstring& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string replace_str = converter.to_bytes(replace);
                this->data.replace(start, length, replace_str);
            }
            else
            {
                this->data.replace(start, length, replace);
            }
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::wstring&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string replace_str = converter.to_bytes(replace);
                this->data.replace(start, length, replace_str);
            }
            else
            {
                this->data.replace(start, length, replace);
            }
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::wstring_view& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                this->data.replace(start, length, replace_str);
            }
            else
            {
                this->data.replace(start, length, replace.data());
            }
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const std::wstring_view&& replace)
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string replace_str = converter.to_bytes(std::wstring(replace));
                this->data.replace(start, length, replace_str);
            }
            else
            {
                this->data.replace(start, length, replace.data());
            }
            return *this;
        }

        BasicString& Replace(size_t start, size_t length, const T& replace)
        {
            this->data.replace(start, length, 1, replace);
            return *this;
        }

        size_t Rfind(const BasicString& str) const
		{
			return this->data.rfind(str.data);
		}

        bool IsEmpty() const
		{
			return this->data.empty();
		}

        void Clear()
		{
			this->data.clear();
		}

        auto begin() const
        {
            return this->data.begin();
        }

        auto end() const
        {
            return this->data.end();
        }

        operator std::string() const
        {
            if constexpr (std::is_same_v<T, wchar_t>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                return converter.to_bytes(this->data);
            }
            else
                return this->data;
        }

        operator std::wstring() const
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                return converter.from_bytes(this->data);
            }
            else
                return this->data;
        }

        operator const char*() const
		{
			if constexpr (std::is_same_v<T, wchar_t>)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.to_bytes(this->data).c_str();
			}
			else
				return this->data.c_str();
		}

        operator const wchar_t* () const
        {
            if constexpr (std::is_same_v<T, char>)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                return converter.from_bytes(this->data).c_str();
            }
            else
                return this->data.c_str();
        }

        operator char* ()
		{
			if constexpr (std::is_same_v<T, wchar_t>)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return const_cast<char*>(converter.to_bytes(this->data).c_str());
			}
			else
				return const_cast<char*>(this->data.c_str());
		}

        operator wchar_t* ()
		{
			if constexpr (std::is_same_v<T, char>)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return const_cast<wchar_t*>(converter.from_bytes(this->data).c_str());
			}
			else
				return const_cast<wchar_t*>(this->data.c_str());
		}

        friend std::ostream& operator<<(std::ostream& os, const BasicString& str)
		{
			os << str.data;
			return os;
		}

        friend std::istream& operator>>(std::istream& is, BasicString& str)
        {
            is >> str.data;
			return is;
		}

        static const size_t NullPosition;

    private:

        std::basic_string<T> data;
    };

    const size_t BasicString<char>::NullPosition = std::basic_string<char>::npos;
    const size_t BasicString<wchar_t>::NullPosition = std::basic_string<wchar_t>::npos;

    bool operator==(const char* lhs, const BasicString<char>& rhs)
	{
		return lhs == rhs.operator std::string();
	}

    bool operator==(const std::string& lhs, const BasicString<char>& rhs)
    {
        return lhs == rhs.operator std::string();
    }

    bool operator==(const std::string_view& lhs, const BasicString<char>& rhs)
	{
		return lhs.data() == rhs.operator std::string();
	}

    bool operator==(const wchar_t* lhs, const BasicString<wchar_t>& rhs)
    {
        return lhs == rhs;
	}

    bool operator==(const std::wstring& lhs, const BasicString<wchar_t>& rhs)
	{
		return lhs == rhs;
	}

    bool operator==(const std::wstring_view& lhs, const BasicString<wchar_t>& rhs)
    {
        return lhs == rhs;
    }

    bool operator!=(const char* lhs, const BasicString<char>& rhs)
    {
        return lhs != rhs.operator std::string();
    }

    bool operator!=(const std::string& lhs, const BasicString<char>& rhs)
	{
		return lhs != rhs.operator std::string();
	}

    bool operator!=(const std::string_view& lhs, const BasicString<char>& rhs)
    {
        return lhs.data() != rhs.operator std::string();
	}

    bool operator!=(const wchar_t* lhs, const BasicString<wchar_t>& rhs)
	{
		return lhs != rhs;
    }

    bool operator!=(const std::wstring& lhs, const BasicString<wchar_t>& rhs)
    {
        return lhs != rhs;
	}

    bool operator!=(const std::wstring_view& lhs, const BasicString<wchar_t>& rhs)
	{
		return lhs != rhs;
	} 

    BasicString<char> operator+(const char* lhs, const BasicString<char>& rhs)
    {
        return BasicString<char>(lhs) + rhs;
    }

    BasicString<wchar_t> operator+(const wchar_t* lhs, const BasicString<wchar_t>& rhs)
    {
        return BasicString<wchar_t>(lhs) + rhs;
    }

    BasicString<char> operator+(const BasicString<char>& lhs, float rhs)
	{
		return lhs + BasicString<char>(std::to_string(rhs));
	}

    BasicString<char> operator+(const BasicString<char>& lhs, double rhs)
	{
		return lhs + BasicString<char>(std::to_string(rhs));
	}

    BasicString<char> operator+(float lhs, const BasicString<char>& rhs)
	{
		return BasicString<char>(std::to_string(lhs)) + rhs;
	}

    BasicString<char> operator+(double lhs, const BasicString<char>& rhs)
    {
        return BasicString<char>(std::to_string(lhs)) + rhs;
    }

    BasicString<char> operator+(const std::string_view& lhs, const BasicString<char>& rhs)
    {
        return BasicString<char>(lhs) + rhs;
    }

    BasicString<wchar_t> operator+(const std::wstring& lhs, const BasicString<wchar_t>& rhs)
    {
        return BasicString<wchar_t>(lhs) + rhs;
    }

    BasicString<wchar_t> operator+(const std::wstring_view& lhs, const BasicString<wchar_t>& rhs)
    {
        return BasicString<wchar_t>(lhs) + rhs;
    }

    BasicString<char> operator+(const BasicString<char>& lhs, const std::string& rhs)
    {
        return lhs + BasicString<char>(rhs);
    }

    BasicString<char> operator+(const BasicString<char>& lhs, const std::string_view& rhs)
    {
        return lhs + BasicString<char>(rhs);
    }

    BasicString<wchar_t> operator+(const BasicString<wchar_t>& lhs, const wchar_t* rhs)
    {
        return lhs + BasicString<wchar_t>(rhs);
    }

    BasicString<wchar_t> operator+(const BasicString<wchar_t>& lhs, const std::wstring& rhs)
    {
        return lhs + BasicString<wchar_t>(rhs);
    }

    BasicString<wchar_t> operator+(const BasicString<wchar_t>& lhs, const std::wstring_view& rhs)
    {
        return lhs + BasicString<wchar_t>(rhs);
    }

    BasicString<char> operator+(char lhs, const BasicString<char>& rhs)
    {
        return BasicString<char>(lhs) + rhs;
    }

    BasicString<wchar_t> operator+(wchar_t lhs, const BasicString<wchar_t>& rhs)
    {
        return BasicString<wchar_t>(lhs) + rhs;
    }
}

namespace std
{
    template<>
    struct hash<Invasion::Util::BasicString<char>>
    {
        size_t operator()(const Invasion::Util::BasicString<char>& str) const noexcept
        {
            return hash<std::string>()(str.operator std::string());
        }
    };

    template<>
    struct hash<Invasion::Util::BasicString<wchar_t>>
	{
		size_t operator()(const Invasion::Util::BasicString<wchar_t>& str) const noexcept
		{
			return hash<std::wstring>()(str.operator std::wstring());
		}
	};

	template<>
	struct equal_to<Invasion::Util::BasicString<wchar_t>>
	{
		bool operator()(const Invasion::Util::BasicString<wchar_t>& lhs, const Invasion::Util::BasicString<wchar_t>& rhs) const noexcept
		{
			return lhs == rhs;
		}
	};

    template<>
    struct less<Invasion::Util::BasicString<char>>
    {
		bool operator()(const Invasion::Util::BasicString<char>& lhs, const Invasion::Util::BasicString<char>& rhs) const noexcept
		{
			return lhs < rhs;
		}
	};

    template<>
	struct less<Invasion::Util::BasicString<wchar_t>>
	{
		bool operator()(const Invasion::Util::BasicString<wchar_t>& lhs, const Invasion::Util::BasicString<wchar_t>& rhs) const noexcept
		{
			return lhs < rhs;
		}
    };

    template<>
    struct formatter<Invasion::Util::BasicString<char>> : formatter<std::string> 
    {
        template<typename FormatContext>
        auto format(const Invasion::Util::BasicString<char>& str, FormatContext& ctx) const
        {
            return formatter<std::string>::format(str.operator const char *(), ctx);
        }
    };

    template<>
    struct formatter<Invasion::Util::BasicString<wchar_t>> : formatter<std::wstring>
    {
        template<typename ParseContext>
        auto format(const Invasion::Util::BasicString<wchar_t>& str, ParseContext& ctx)
		{
			return formatter<std::basic_string<wchar_t>>::format(str.operator std::wstring(), ctx);
		}
    };
}