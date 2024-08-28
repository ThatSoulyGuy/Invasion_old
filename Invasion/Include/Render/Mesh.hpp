#pragma once

#include "ECS/GameObject.hpp"
#include "Math/Transform.hpp"
#include "Render/Renderer.hpp"
#include "Render/Shader.hpp"
#include "Render/Texture.hpp"	
#include "Render/Vertex.hpp"
#include "Util/Typedefs.hpp"
#include "World/TextureAtlas.hpp"

using namespace Invasion::ECS;
using namespace Invasion::Math;
using namespace Invasion::Util;
using namespace Invasion::World;

namespace Invasion::Render
{
	struct DefaultMatrixBuffer
	{
		Matrix projection;
		Matrix view;
		Matrix world;
	};

	class Mesh : public Component
	{

	public:
		
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void Generate()
		{
			D3D11_BUFFER_DESC vertexBufferDescription = {};

			vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDescription.ByteWidth = sizeof(Vertex) * vertices.Length();
			vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDescription.CPUAccessFlags = 0;
			vertexBufferDescription.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData = {};

			vertexBufferData.pSysMem = vertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;

			HRESULT result = Renderer::GetInstance().GetDevice()->CreateBuffer(&vertexBufferDescription, &vertexBufferData, &vertexBuffer);

			if (FAILED(result))
				Logger_ThrowException("Failed to create vertex buffer", true);


			D3D11_BUFFER_DESC indexBufferDescription = {};

			indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDescription.ByteWidth = sizeof(unsigned int) * indices.Length();
			indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA indexBufferData = {};

			indexBufferData.pSysMem = indices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;

			result = Renderer::GetInstance().GetDevice()->CreateBuffer(&indexBufferDescription, &indexBufferData, &indexBuffer);

			if (FAILED(result))
				Logger_ThrowException("Failed to create index buffer", true);

			if (FAILED(result))
				Logger_ThrowException("Failed to create sampler state", true);
		}

		void Render(Shared<Camera> camera) override
		{
			Shared<Shader> shader = GetGameObject()->GetComponent<Shader>();
			Shared<Texture> texture = GetGameObject()->GetComponent<Texture>();
			Shared<TextureAtlas> textureAtlas = GetGameObject()->GetComponent<TextureAtlas>();
			Shared<Transform> transform = GetGameObject()->GetComponent<Transform>();

			auto context = Renderer::GetInstance().GetContext();

			UINT stride = sizeof(Vertex);
			UINT offset = 0;

			context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
			context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			shader->Bind();

			if (textureAtlas)
				textureAtlas->Bind();
			else
				texture->Bind();

			shader->SetConstantBuffer(0, DefaultMatrixBuffer
			{ 
				DirectX::XMMatrixTranspose(camera->GetProjectionMatrix()),
				DirectX::XMMatrixTranspose(camera->GetViewMatrix()),
				DirectX::XMMatrixTranspose(transform->GetWorldMatrix()) 
			}, ShaderType::VERTEX);

			if (textureAtlas)
				shader->SetSamplerState(0, textureAtlas->GetSamplerState(), ShaderType::PIXEL);
			else
				shader->SetSamplerState(0, texture->GetSamplerState(), ShaderType::PIXEL);

			context->DrawIndexed(indices.Length(), 0, 0);
		}

		void SetVertices(const Vector<Vertex>& vertices)
		{
			this->vertices = vertices;
		}

		void SetIndices(const Vector<unsigned int>& indices)
		{
			this->indices = indices;
		}

		void CleanUp() override
		{
			vertexBuffer.Reset();
			indexBuffer.Reset();
		}

		static Shared<Mesh> Create(const String& name, Shared<Shader> shader, Shared<Texture> texture, const Vector<Vertex>& vertices, const Vector<unsigned int>& indices)
		{
			class Enabled : public Mesh { };
			Shared<Mesh> result = std::make_shared<Enabled>();

			result->name = name;
			result->vertices = vertices;
			result->indices = indices;

			return std::move(result);
		}

	private:

		Mesh() = default;

		String name;

		Vector<Vertex> vertices;
		Vector<unsigned int> indices;

		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;
	};
}