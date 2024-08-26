#pragma once

#include <DirectXTex/DirectXTex.h>
#include "Core/Settings.hpp"
#include "ECS/Component.hpp"
#include "Render/Renderer.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Core;
using namespace Invasion::ECS;
using namespace Invasion::Util;

namespace Invasion
{
	namespace Render
	{
		class Texture : public Component
		{

		public:

			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;

			void Bind() const
			{
				Renderer::GetInstance().GetContext()->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
			}

			String GetName() const
			{
				return name;
			}

			String GetLocalPath() const
			{
				return localPath;
			}

			String GetDomain() const
			{
				return domain;
			}

			String GetPath() const
			{
				return path;
			}

			ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const
			{
				return shaderResourceView;
			}

			ComPtr<ID3D11SamplerState> GetSamplerState() const
			{
				return samplerState;
			}

			Vector2i GetDimensions() const
			{
				return dimensions;
			}

			int GetChannels() const
			{
				return channels;
			}

			void CleanUp_NoOverride()
			{
				shaderResourceView.Reset();
				texture.Reset();
				samplerState.Reset();
			}

			static Shared<Texture> Create(const String& name, const String& localPath, D3D11_SAMPLER_DESC samplerDescription, const String& domain = Settings::GetInstance().Get<String>("defaultDomain"))
			{
				class Enabled : public Texture { };
				Shared<Texture> result = std::make_shared<Enabled>();

				result->name = name;
				result->localPath = localPath;
				result->samplerDescription = samplerDescription;
				result->domain = domain;
				result->path = "Assets/" + domain + "/" + localPath;

				result->Generate();

				return std::move(result);
			}

		private:
			
			Texture() = default;

			void Generate()
			{
				ComPtr<ID3D11Device> device = Renderer::GetInstance().GetDevice();

				DirectX::ScratchImage image;

				HRESULT result = DirectX::LoadFromDDSFile(std::wstring(path.begin(), path.end()).c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);

				if (FAILED(result))
					Logger_ThrowException("Failed to load texture from file: " + path, true);
				
				result = DirectX::CreateTexture(device.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);

				if (FAILED(result))
					Logger_ThrowException("Failed to create texture from image", true);
				
				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription = {};

				shaderResourceViewDescription.Format = image.GetMetadata().format;
				shaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
				shaderResourceViewDescription.Texture2D.MipLevels = image.GetMetadata().mipLevels;

				result = device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDescription, &shaderResourceView);

				if (FAILED(result))
					Logger_ThrowException("Failed to create shader resource view for texture", true);
				
				dimensions = Vector2i{ static_cast<int>(image.GetMetadata().width), static_cast<int>(image.GetMetadata().height) };
				channels = DirectX::BitsPerPixel(image.GetMetadata().format) / 8;

				result = Renderer::GetInstance().GetDevice()->CreateSamplerState(&samplerDescription, &samplerState);
			}

			String name;
			String localPath;
			String domain;

			String path;

			Vector2i dimensions;
			int channels = 0;

			D3D11_SAMPLER_DESC samplerDescription;

			ComPtr<ID3D11Resource> texture;
			ComPtr<ID3D11ShaderResourceView> shaderResourceView;
			ComPtr<ID3D11SamplerState> samplerState;

		};
	}
}