#pragma once

#include <filesystem>
#include "Util/Typedefs.hpp"

namespace Invasion::Util
{
	class FileHelper
	{

	public:

		FileHelper(const FileHelper&) = delete;
		FileHelper& operator=(const FileHelper&) = delete;

		static String ReadFile(const String& path)
		{
			InputFileStream file(path.operator std::string());

			if (!file.is_open())
				return "";

			OutputStringStream buffer;

			buffer << file.rdbuf();

			file.close();

			return buffer.str();
		}

		static void CreateFile(const String& path, const String& content)
		{
			OutputFileStream file(path.operator std::string());

			if (!file.is_open())
				return;

			file << content;
		}

		static void AppendToFile(const String& path, const String& content)
		{
			OutputFileStream file(path.operator std::string(), std::ios::app);

			if (!file.is_open())
				return;

			file << content;
		}

		static void DeleteFile(const String& path)
		{
			std::remove(path.operator std::string().c_str());
		}

		static void CreateDirectory(const String& path)
		{
			std::filesystem::create_directory(path.operator std::string());
		}

		static void DeleteDirectory(const String& path)
		{
			std::filesystem::remove_all(path.operator std::string());
		}

		static void CopyFile(const String& source, const String& destination)
		{
			std::filesystem::copy_file(source.operator std::string(), destination.operator std::string());
		}

		static void MoveFile(const String& source, const String& destination)
		{
			std::filesystem::rename(source.operator std::string(), destination.operator std::string());
		}

		static void CopyDirectory(const String& source, const String& destination)
		{
			std::filesystem::copy(source.operator std::string(), destination.operator std::string(), std::filesystem::copy_options::recursive);
		}

		static void MoveDirectory(const String& source, const String& destination)
		{
			std::filesystem::rename(source.operator std::string(), destination.operator std::string());
		}

		static bool FileExists(const String& path)
		{
			return std::filesystem::exists(path.operator std::string());
		}

		static bool DirectoryExists(const String& path)
		{
			return std::filesystem::is_directory(path.operator std::string());
		}

		static bool IsEmpty(const String& path)
		{
			return std::filesystem::is_empty(path.operator std::string());
		}

		static void RenameFile(const String& path, const String& newName)
		{
			std::filesystem::rename(path.operator std::string(), newName.operator std::string());
		}

		static void RenameDirectory(const String& path, const String& newName)
		{
			std::filesystem::rename(path.operator std::string(), newName.operator std::string());
		}

		static void SetFilePermissions(const String& path, const std::filesystem::perms& permissions)
		{
			std::filesystem::permissions(path.operator std::string(), permissions);
		}

		static void SetDirectoryPermissions(const String& path, const std::filesystem::perms& permissions)
		{
			std::filesystem::permissions(path.operator std::string(), permissions);
		}

		static void SetFileLastWriteTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static void SetDirectoryLastWriteTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static std::filesystem::file_time_type GetFileLastWriteTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static std::filesystem::file_time_type GetDirectoryLastWriteTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static void SetFileLastAccessTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static void SetDirectoryLastAccessTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static std::filesystem::file_time_type GetFileLastAccessTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static std::filesystem::file_time_type GetDirectoryLastAccessTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static void SetFileCreationTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static void SetDirectoryCreationTime(const String& path, const std::filesystem::file_time_type& time)
		{
			std::filesystem::last_write_time(path.operator std::string(), time);
		}

		static std::filesystem::file_time_type GetFileCreationTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static std::filesystem::file_time_type GetDirectoryCreationTime(const String& path)
		{
			return std::filesystem::last_write_time(path.operator std::string());
		}

		static std::filesystem::file_type GetFileAttributes(const String& path)
		{
			std::filesystem::file_status status = std::filesystem::status(path.operator std::string());
			return status.type();
		}

		static std::filesystem::file_type GetDirectoryAttributes(const String& path)
		{
			std::filesystem::file_status status = std::filesystem::status(path.operator std::string());
			return status.type();
		}

		static void SetFileOwner(const String& path, const std::filesystem::file_status& owner)
		{
			std::filesystem::permissions(path.operator std::string(), owner.permissions());
		}

		static void SetDirectoryOwner(const String& path, const std::filesystem::file_status& owner)
		{
			std::filesystem::permissions(path.operator std::string(), owner.permissions());
		}

		static std::filesystem::file_status GetFileOwner(const String& path)
		{
			return std::filesystem::status(path.operator std::string());
		}

		static std::filesystem::file_status GetDirectoryOwner(const String& path)
		{
			return std::filesystem::status(path.operator std::string());
		}

		static void SetFileGroup(const String& path, const std::filesystem::file_status& group)
		{
			std::filesystem::permissions(path.operator std::string(), group.permissions());
		}

		static void SetDirectoryGroup(const String& path, const std::filesystem::file_status& group)
		{
			std::filesystem::permissions(path.operator std::string(), group.permissions());
		}

		static std::filesystem::file_status GetFileGroup(const String& path)
		{
			return std::filesystem::status(path.operator std::string());
		}

		static std::filesystem::file_status GetDirectoryGroup(const String& path)
		{
			return std::filesystem::status(path.operator std::string());
		}

		static std::filesystem::perms GetFilePermissions(const String& path)
		{
			return std::filesystem::status(path.operator std::string()).permissions();
		}

		static std::filesystem::perms GetDirectoryPermissions(const String& path)
		{
			return std::filesystem::status(path.operator std::string()).permissions();
		}

	private:

		FileHelper() = default;

	};
}