#pragma once

#include "Util/DateTime.hpp"
#include "Util/Formatter.hpp"
#include "Util/Typedefs.hpp"

#define Logger_WriteConsole(message, level) _Logger::WriteConsole(message, __FUNCTION__, level)
#define Logger_ThrowException(message, fatal) _Logger::ThrowException(message, __FUNCTION__, __LINE__, fatal)

using namespace Invasion::Util;

namespace Invasion::Core
{
    enum class LogLevel
    {
        INFORMATION,
        DEBUGGING,
        WARNING,
        ERROR,
        FATAL_ERROR
    };

    class _Logger
    {

    public:

        static void WriteConsole(const String& message, const String& function, const LogLevel& level)
        {
            static String name = GetFunctionName(function);
            name[0] = toupper(name[0]);

            switch (level)
            {

            case LogLevel::INFORMATION:
                std::cout << Formatter::Format(Formatter::ColorFormat("&2&l[{}] [Thread/INFORMATION] [{}]: {}"), DateTime::Get("%H:%S:%M"), name, message) << std::endl;
                break;

            case LogLevel::DEBUGGING:
                std::cout << Formatter::Format(Formatter::ColorFormat("&1&l[{}] [Thread/DEBUGGING] [{}]: {}"), DateTime::Get("%H:%S:%M"), name, message) << std::endl;
                break;

            case LogLevel::WARNING:
                std::cout << Formatter::Format(Formatter::ColorFormat("&6[{}] [Thread/WARNING] [{}]: {}"), DateTime::Get("%H:%S:%M"), name, message) << std::endl;
                break;

            case LogLevel::ERROR:
                std::cout << Formatter::Format(Formatter::ColorFormat("&c[{}] [Thread/ERROR] [{}]: {}"), DateTime::Get("%H:%S:%M"), name, message) << std::endl;
                break;

            case LogLevel::FATAL_ERROR:
                std::cout << Formatter::Format(Formatter::ColorFormat("&4[{}] [Thread/FATAL ERROR] [{}]: {}"), DateTime::Get("%H:%S:%M"), name, message) << std::endl;
                break;

            default:
                break;
            }
        }

        static void ThrowException(const String& message, const String& function, int line, bool fatal)
		{
			String name = GetFunctionName(function);

            if (fatal)
			{
				name[0] = toupper(name[0]);
				std::cerr << Formatter::Format(Formatter::ColorFormat("&4&l[{}] [Thread/FATAL ERROR] [{}]: {} [Line: {}]"), DateTime::Get("%H:%S:%M"), name, message, line) << std::endl;
				std::terminate();
			}
			else
			{
				name[0] = toupper(name[0]);
				std::cerr << Formatter::Format(Formatter::ColorFormat("&c&l[{}] [Thread/ERROR] [{}]: {} [Line: {}]"), DateTime::Get("%H:%S:%M"), name, message, line) << std::endl;
			}
		}

    private:

        static String GetFunctionName(String input)
        {
            Size position = input.Rfind("::");

            if (position != String::NullPosition)
                position += 2;
            else
                position = 0;

            return input.SubString(position);
        }

    };
}