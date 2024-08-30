#pragma once

#define NOMINMAX
#include <filesystem>
#include <DirectXTex/DirectXTex.h>
#include "Core/InputManager.hpp"
#include "ECS/Component.hpp"
#include "Math/Vector2.hpp"
#include "Render/Renderer.hpp"
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
		
        void Bind() const
        {
            Renderer::GetInstance().GetContext()->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
        }

        Array<Vector2f, 4> GetTextureCoordinates(const String& textureName) const
        {
            if (!lookupTable.Contains(textureName))
            {
                Logger_WriteConsole("Texture not found: '" + textureName + "', I_WARN", LogLevel::WARNING);
                return Array<Vector2f, 4>();
            }

            const SubTextureInfo& info = lookupTable[textureName];

            Array<Vector2f, 4> texCoords;

            texCoords[0] = info.position;
            texCoords[1] = Vector2f(info.position.x + info.dimensions.x, info.position.y);
            texCoords[2] = Vector2f(info.position.x + info.dimensions.x, info.position.y + info.dimensions.y);
            texCoords[3] = Vector2f(info.position.x, info.position.y + info.dimensions.y);

            return texCoords;
        }

        String GetName() const
        {
            return name;
        }

        ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const
        {
            return shaderResourceView;
        }

        ComPtr<ID3D11SamplerState> GetSamplerState() const
        {
            return samplerState;
        }

        void CleanUp_NoOverride()
        {
            atlas.Release();
            images.ForEach([](DirectX::ScratchImage image) { image.Release(); });
        }

        static Shared<TextureAtlas> Create(const String& name, const String& inputDirectory, const String& outputDirectory)
        {
            class Enabled : public TextureAtlas { };
            Shared<TextureAtlas> result = std::make_shared<Enabled>();

            result->name = name;
            result->inputDirectory = inputDirectory;
            result->outputDirectory = outputDirectory;

            result->Generate();

            return std::move(result);
        }

	private:

        TextureAtlas() = default;

        void Generate()
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
                        return;
                    }

                    images |= std::move(image);
                    filenames += entry.path().filename().string();
                }
            }

            atlas = PackTextures();
            SaveAtlasAndLookupTable(outputDirectory.operator std::string());

            auto device = Renderer::GetInstance().GetDevice();

            HRESULT result = DirectX::CreateTexture(device.Get(), atlas.GetImages(), atlas.GetImageCount(), atlas.GetMetadata(), &texture);

            if (FAILED(result))
                Logger_ThrowException("Failed to create texture from image", true);

            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription = {};

            shaderResourceViewDescription.Format = atlas.GetMetadata().format;
            shaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDescription.Texture2D.MipLevels = atlas.GetMetadata().mipLevels;

            result = device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDescription, &shaderResourceView);

            if (FAILED(result))
                Logger_ThrowException("Failed to create shader resource view for texture", true);

            samplerDescription = {};

            samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDescription.MipLODBias = 0.0f;
            samplerDescription.MaxAnisotropy = 1;
            samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDescription.BorderColor[0] = 0;
            samplerDescription.BorderColor[1] = 0;
            samplerDescription.BorderColor[2] = 0;
            samplerDescription.BorderColor[3] = 0;
            samplerDescription.MinLOD = 0;
            samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

            result = device->CreateSamplerState(&samplerDescription, &samplerState);
        }

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
                lookupTable[String(filenames[i]).FindAndReplace(".dds", "")] = info;

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

        String inputDirectory;
        String outputDirectory;

        Vector<DirectX::ScratchImage> images;
        Vector<fs::path> filenames;
        DirectX::ScratchImage atlas;
        UnorderedMap<String, SubTextureInfo> lookupTable;

        D3D11_SAMPLER_DESC samplerDescription = {};

        ComPtr<ID3D11Resource> texture;
        ComPtr<ID3D11ShaderResourceView> shaderResourceView;
        ComPtr<ID3D11SamplerState> samplerState;
	};
}