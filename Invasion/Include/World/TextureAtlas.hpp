#pragma once

#define NOMINMAX
#include <filesystem>
#include <DirectXTex/DirectXTex.h>
#include "Core/InputManager.hpp"
#include "ECS/Component.hpp"
#include "Math/Vector2.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Core;
using namespace Invasion::Math;
using namespace Invasion::Util;

namespace fs = std::filesystem;

namespace Invasion::World
{
	struct SubTextureInfo 
	{
		Vector2f position;
		Vector2f dimensions;
	};

	class TextureAtlas : public Component
	{

	public:

        TextureAtlas(const TextureAtlas&) = delete;
        TextureAtlas& operator= (const TextureAtlas&) = delete;
		
        bool Initialize(const String& name, const String& inputDirectory, const String& outputDirectory) 
        {
            this->name = name;

            for (const auto& entry : fs::directory_iterator(inputDirectory.operator std::string())) 
            {
                if (entry.is_regular_file() && String(entry.path().extension()) == String(".dds"))
                {
                    DirectX::ScratchImage image;
                    HRESULT result = DirectX::LoadFromDDSFile(entry.path().c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);

                    if (FAILED(result)) 
                    {
                        Logger_ThrowException("Failed to load DDS file: '" + String(entry.path()) + "', " + std::to_string(result) + ", I_FAIL_W_REASON", false);
                        return false;
                    }

                    images |= std::move(image);
                    filenames += entry.path().filename().string();
                }
            }

            atlas = PackTextures();
            SaveAtlasAndLookupTable(outputDirectory.operator std::string());

            return true;
        }

        String GetName() const
        {
            return name;
        }

        void CleanUp_NoOverride()
        {
            images.ForEach([](DirectX::ScratchImage image) { image.Release(); });
        }

        static Shared<TextureAtlas> Create()
        {
            class Enabled : public TextureAtlas { };
            return std::move(std::make_shared<Enabled>());
        }

	private:

        TextureAtlas() = default;

        DirectX::ScratchImage PackTextures()
        {
            if (images.IsEmpty())
            {
                Logger_WriteConsole("No images to pack, I_WARN", LogLevel::WARNING);
                return DirectX::ScratchImage();
            }

            size_t totalArea = 0;

            for (const auto& image : images)
            {
                const auto& meta = image.GetMetadata();
                totalArea += meta.width * meta.height;
            }

            size_t atlasSize = static_cast<size_t>(std::ceil(std::sqrt(static_cast<double>(totalArea))));

            size_t xOffset = 0;
            size_t yOffset = 0;
            size_t currentMaxHeight = 0;

            DirectX::ScratchImage atlas;
            atlas.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, atlasSize, atlasSize, 1, 1);

            for (size_t i = 0; i < images.Length(); ++i)
            {
                const auto& image = images[i];
                const auto& meta = image.GetMetadata();

                if (xOffset + meta.width > atlasSize)
                {
                    xOffset = 0;
                    yOffset += currentMaxHeight;
                    currentMaxHeight = 0;
                }

                if (meta.height > currentMaxHeight)
                    currentMaxHeight = meta.height;

                DirectX::CopyRectangle(*image.GetImage(0, 0, 0), DirectX::Rect{ 0, 0, meta.width, meta.height }, *atlas.GetImage(0, 0, 0), DirectX::TEX_FILTER_DEFAULT, xOffset, yOffset);

                SubTextureInfo info;
                info.position = Vector2f(static_cast<float>(xOffset) / atlasSize, static_cast<float>(yOffset) / atlasSize);
                info.dimensions = Vector2f(static_cast<float>(meta.width) / atlasSize, static_cast<float>(meta.height) / atlasSize);
                lookupTable[filenames[i]] = info;

                xOffset += meta.width;
            }

            return atlas;
        }

        void SaveAtlasAndLookupTable(const fs::path& outputPath)
        {
            if (!fs::exists(outputPath.parent_path()))
            {
                try
                {
                    fs::create_directories(outputPath.parent_path());
                }
                catch (const std::exception& e)
                {
                    Logger_ThrowException("Failed to create output directory: '" + String(outputPath.parent_path()) + "', " + String(e.what()) + ", I_FAIL_W_REASON", false);
                    return;
                }
            }

            std::wstring fullFileName = outputPath.string() + "\\" + static_cast<char>(toupper(name[0])) + name.SubString(1, name.Length()) + "Atlas.dds";

            HRESULT result = DirectX::SaveToDDSFile(atlas.GetImages(), atlas.GetImageCount(), atlas.GetMetadata(), DirectX::DDS_FLAGS_NONE, fullFileName.c_str());

            if (FAILED(result))
                Logger_ThrowException("Failure to save texture atlas: '" + String(outputPath) + "', HRESULT: " + std::to_string(result) + ", I_FAIL_W_REASON", false);
            else
                Logger_WriteConsole("Successfully saved the texture atlas to: '" + String(outputPath) + "', I_SUCCESS", LogLevel::INFORMATION);
        }

        String name;

        Vector<DirectX::ScratchImage> images;
        Vector<fs::path> filenames;
        DirectX::ScratchImage atlas;
        UnorderedMap<String, SubTextureInfo> lookupTable;
	};
}