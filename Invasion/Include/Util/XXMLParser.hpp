#pragma once

#include "Core/Logger.hpp"
#include "Core/Settings.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Util/CommonVersionFormat.hpp"
#include "Util/FileHelper.hpp"
#include "Util/Formatter.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Core;
using namespace Invasion::Math;
using namespace Invasion::Util;

namespace Invasion::Util
{
	using VariableValue = Variant<std::string, int, float, double, CommonVersionFormat, Vector2i, Vector2f, Vector2d, Vector3i, Vector3f, Vector3d, Vector4i, Vector4f, Vector4d>;

	class XXMLParser
	{

	public:

		XXMLParser(const XXMLParser&) = delete;
		XXMLParser& operator=(const XXMLParser&) = delete;

		const String& GetVersion() const
		{
			return version;
		}

		const UnorderedMap<std::string, VariableValue>& GetGlobalVariables() const
		{
			return globalVariables;
		}

		const UnorderedMap<std::string, UnorderedMap<std::string, VariableValue>>& GetNamespaces() const
		{
			return namespaces;
		}

		const Vector<std::string>& GetGlobalDefines() const
		{
			return globalDefines;
		}

		template <typename T>
		const T& GetGlobalVariable(const String& name) const
		{
			if (!globalVariables.Contains(name))
			{
				Logger_ThrowException(Formatter::Format("Global variable '{}' does not exist in XXML file '{}'", name, path), false);
				return T();
			}

			return std::get<T>(globalVariables[name]);
		}

		template <typename T>
		const T& GetNamespacedVariable(const String& header, const String& name) const
		{
			if (!namespaces.Contains(header))
			{
				Logger_ThrowException(Formatter::Format("Namespace '{}' does not exist in XXML file '{}'", header, path), false);
				return T();
			}

			if (!namespaces[header].Contains(name))
			{
				Logger_ThrowException(Formatter::Format("Variable '{}' does not exist in namespace '{}' in XXML file '{}'", name, header, path), false);
				return T();
			}

			return std::get<T>(namespaces[header][name]);
		}

		static Shared<XXMLParser> Create(const String& localPath, const String& name, const String& domain = Settings::GetInstance().Get<String>("defaultDomain"))
		{
			class Enabled : public XXMLParser { };
			Shared<XXMLParser> result = std::make_shared<Enabled>();

			result->name = name;
			result->localPath = localPath;
			result->domain = domain;
			result->path = Formatter::Format("Assets/{}/{}", domain, localPath);

			result->Generate();

			return result;
		}

	private:

		XXMLParser() = default;

		void Generate()
		{
			String data = FileHelper::ReadFile(path);

			InputStringStream stream(data.operator std::string());
			std::string line;

			Regex xxmlVersionRegex(R"(<\s*XXML\s*=\s*([\d\.]+)\s*>)");

			Regex namespaceStartRegex(R"(\[\s*<\s*([\w]+)\s*>)");
			Regex namespaceEndRegex(R"(^\s*\]\s*$)");

			Regex stringVariableRegex(R"((\w+)\s*=\s*"([^"]+)\")");

			Regex versionVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+)\s*\.\s*(\d+)\s*\.\s*(\d+)\s*\])");

			Regex intVariableRegex(R"((\w+)\s*=\s*(\d+))");
			Regex floatVariableRegex(R"((\w+)\s*=\s*(\d+\.?\d*)f)");
			Regex doubleVariableRegex(R"((\w+)\s*=\s*(\d+\.?\d*)d)");

			Regex vector2iVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+)\s*,\s*(\d+)\s*\])");
			Regex vector2fVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)f\s*,\s*(\d+\.?\d*)f\s*\])");
			Regex vector2dVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)d\s*,\s*(\d+\.?\d*)d\s*\])");

			Regex vector3iVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+)\s*,\s*(\d+)\s*\,\s*(\d+)\s*\])");
			Regex vector3fVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)f\s*,\s*(\d+\.?\d*)f\s*\,\s*(\d+\.?\d*)f\s*\])");
			Regex vector3dVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)d\s*,\s*(\d+\.?\d*)d\s*\,\s*(\d+\.?\d*)d\s*\])");

			Regex vector4iVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+)\s*,\s*(\d+)\s*\,\s*(\d+)\s*\,\s*(\d+)\s*\])");
			Regex vector4fVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)f\s*,\s*(\d+\.?\d*)f\s*\,\s*(\d+\.?\d*)f\s*\,\s*(\d+\.?\d*)f\s*\])");
			Regex vector4dVariableRegex(R"((\w+)\s*=\s*\[\s*(\d+\.?\d*)d\s*,\s*(\d+\.?\d*)d\s*\,\s*(\d+\.?\d*)d\s*\,\s*(\d+\.?\d*)d\s*\])");

			Regex globalDefineRegex(R"(#\s*(\w+)\s*(\S+)\s*(\w+))");

			Match<std::string::const_iterator> match;
			String currentNamespace;

			while (std::getline(stream, line))
			{
				if (std::regex_search(line, match, xxmlVersionRegex))
				{
					version = match[1];

					if (std::stof(version.operator std::string()) > VERSION)
					{
						Logger_ThrowException(Formatter::Format("XXML version mismatch! Detected version: '{}', when expecting version: '{}'!, '{}'", std::stof(version.operator std::string()), VERSION, name), false);
						return;
					}
				}
				else if (std::regex_search(line, match, namespaceStartRegex))
				{
					currentNamespace = match[1];
					namespaces[currentNamespace].Clear();
				}
				else if (std::regex_search(line, match, namespaceEndRegex))
					currentNamespace.Clear();
				else if (std::regex_search(line, match, stringVariableRegex))
				{
					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = match[2];
					else
						namespaces[currentNamespace][match[1]] = match[2];
				}
				else if (std::regex_search(line, match, versionVariableRegex))
				{
					CommonVersionFormat version = CommonVersionFormat::Create(std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = version;
					else
						namespaces[currentNamespace][match[1]] = version;
				}
				else if (std::regex_search(line, match, intVariableRegex))
				{
					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = std::stoi(match[2]);
					else
						namespaces[currentNamespace][match[1]] = std::stoi(match[2]);
				}
				else if (std::regex_search(line, match, floatVariableRegex))
				{
					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = std::stof(match[2]);
					else
						namespaces[currentNamespace][match[1]] = std::stof(match[2]);
				}
				else if (std::regex_search(line, match, doubleVariableRegex))
				{
					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = std::stod(match[2]);
					else
						namespaces[currentNamespace][match[1]] = std::stod(match[2]);

					Logger_WriteConsole("Warning: double-precision is not fully supported!", LogLevel::WARNING);
				}
				else if (std::regex_search(line, match, vector2iVariableRegex))
				{
					Vector2i vector(std::stoi(match[2]), std::stoi(match[3]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector2fVariableRegex))
				{
					Vector2f vector(std::stof(match[2]), std::stof(match[3]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector2dVariableRegex))
				{
					Vector2d vector(std::stod(match[2]), std::stod(match[3]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;

					Logger_WriteConsole("Warning: double-precision is not fully supported!", LogLevel::WARNING);
				}
				else if (std::regex_search(line, match, vector3iVariableRegex))
				{
					Vector3i vector(std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector3fVariableRegex))
				{
					Vector3f vector(std::stof(match[2]), std::stof(match[3]), std::stof(match[4]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector3dVariableRegex))
				{
					Vector3d vector(std::stod(match[2]), std::stod(match[3]), std::stod(match[4]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;

					Logger_WriteConsole("Warning: double-precision is not fully supported!", LogLevel::WARNING);
				}
				else if (std::regex_search(line, match, vector4iVariableRegex))
				{
					Vector4i vector(std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4]), std::stoi(match[5]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector4fVariableRegex))
				{
					Vector4f vector(std::stof(match[2]), std::stof(match[3]), std::stof(match[4]), std::stof(match[5]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;
				}
				else if (std::regex_search(line, match, vector4dVariableRegex))
				{
					Vector4d vector(std::stod(match[2]), std::stod(match[3]), std::stod(match[4]), std::stod(match[5]));

					if (currentNamespace.IsEmpty())
						globalVariables[match[1]] = vector;
					else
						namespaces[currentNamespace][match[1]] = vector;

					Logger_WriteConsole("Warning: double-precision is not fully supported!", LogLevel::WARNING);
				}
				else if (std::regex_search(line, match, globalDefineRegex))
				{
					globalDefines += match[3];
				}
			}
		}

		String name;
		String localPath;
		String domain;
		String path; 
		String version;

		UnorderedMap<std::string, VariableValue> globalVariables;
		UnorderedMap<std::string, UnorderedMap<std::string, VariableValue>> namespaces;
		Vector<std::string> globalDefines;

		static const float VERSION;
	};

	const float XXMLParser::VERSION = 1.2f;
}