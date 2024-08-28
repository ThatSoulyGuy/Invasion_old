#pragma once

#include "ECS/GameObjectManager.hpp"
#include "Entity/Entities/EntityPlayer.hpp"
#include "Render/Mesh.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"
#include "Render/TextureManager.hpp"
#include "Util/Typedefs.hpp"
#include "Util/XXMLParser.hpp"
#include "World/Chunk.hpp"
#include "World/TextureAtlas.hpp"

using namespace Invasion::Entity;
using namespace Invasion::Entity::Entities;
using namespace Invasion::Render;
using namespace Invasion::World;

namespace Invasion
{
	class InvasionGame
	{

	public:
		
		InvasionGame(const InvasionGame&) = delete;
		InvasionGame& operator=(const InvasionGame&) = delete;

		void PreInitialize()
		{
			Shared<XXMLParser> parser = XXMLParser::Create("EngineSettings.xxml", "engineSettings");

			if (parser->GetGlobalVariable<std::string>("ID") != "0x989374BC6A7EF9DB22D1")
				Logger_ThrowException("Invalid engine settings file.", true);

			String title = parser->GetNamespacedVariable<std::string>("Invasion_Default", "DefaultApplicationTitle");
			CommonVersionFormat version = parser->GetNamespacedVariable<CommonVersionFormat>("Invasion_Default", "DefaultApplicationVersion");
			Vector2i windowDimensions = parser->GetNamespacedVariable<Vector2i>("Invasion_Default", "DefaultWindowDimensions");

			Settings::GetInstance().Set("defaultApplicationTitle", title);
			Settings::GetInstance().Set("defaultApplicationVersion", version);
			Settings::GetInstance().Set("defaultWindowDimensions", windowDimensions);
			Settings::GetInstance().Set<WNDPROC>("defaultWindowProcedure", [](HWND handle, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
			{
				switch (message)
				{
					case WM_SIZE:
					{
						POINTS dimensions = MAKEPOINTS(lParam);

						Renderer::GetInstance().Resize({ dimensions.x, dimensions.y });
						break;
					}

					case WM_CLOSE:
						PostQuitMessage(0);
						break; 

					case WM_KEYDOWN:
						InputManager::GetInstance().SetKeyState(static_cast<KeyCode>(wParam), KeyState::PRESSED);
						return 0;

					case WM_KEYUP:
						InputManager::GetInstance().SetKeyState(static_cast<KeyCode>(wParam), KeyState::RELEASED);
						return 0;

					case WM_LBUTTONDOWN:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::LEFT, MouseState::PRESSED);
						return 0;

					case WM_LBUTTONUP:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::LEFT, MouseState::RELEASED);
						return 0;

					case WM_RBUTTONDOWN:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::RIGHT, MouseState::PRESSED);
						return 0;

					case WM_RBUTTONUP:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::RIGHT, MouseState::RELEASED);
						return 0;

					case WM_MBUTTONDOWN:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::MIDDLE, MouseState::PRESSED);
						return 0;

					case WM_MBUTTONUP:
						InputManager::GetInstance().SetMouseButtonState(MouseCode::MIDDLE, MouseState::RELEASED);
						return 0;

					case WM_MOUSEMOVE:
					{
						POINTS points = MAKEPOINTS(lParam);
						InputManager::GetInstance().SetMousePosition({ points.x, points.y });
					}
				}

				return DefWindowProc(handle, message, wParam, lParam);
			});
		}

		void Initialize()
		{
			Renderer::GetInstance().Initialize();

			ShaderManager::GetInstance().Register(Shader::Create("default", "Shader/Default"));

			D3D11_SAMPLER_DESC samplerDescription = {};

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

			Shared<TextureAtlas> texturePacker = TextureAtlas::Create();

			TextureManager::GetInstance().Register(Texture::Create("debug", "Texture/Debug.dds", samplerDescription));

			player = GameObjectManager::GetInstance().Register(GameObject::Create("Player"));

			player->AddComponent(IEntity::Create<EntityPlayer>());
			player->GetTransform()->Translate({ 0.0f, 0.0f, -5.0f });

			mesh = GameObjectManager::GetInstance().Register(GameObject::Create("Chunk"));
			
			mesh->AddComponent(ShaderManager::GetInstance().Get("default"));
			mesh->AddComponent(TextureManager::GetInstance().Get("debug"));

			mesh->AddComponent(Mesh::Create("square", ShaderManager::GetInstance().Get("default"), TextureManager::GetInstance().Get("debug"), {}, {}));

			mesh->AddComponent(Chunk::Create());

			mesh->GetComponent<Chunk>()->Generate();
		}

		void Update()
		{
			InputManager::GetInstance().Update();
			GameObjectManager::GetInstance().Update();
		}

		void Render()
		{
			Renderer::GetInstance().PreRender();

			GameObjectManager::GetInstance().Render(player->GetComponent<EntityPlayer>()->GetCamera());

			Renderer::GetInstance().PostRender();
		}

		void CleanUp()
		{
			GameObjectManager::GetInstance().CleanUp();
			ShaderManager::GetInstance().CleanUp();
			TextureManager::GetInstance().CleanUp();
			Renderer::GetInstance().CleanUp();
		}

		static InvasionGame& GetInstance()
		{
			static InvasionGame instance;

			return instance;
		}

	private:

		InvasionGame() = default;

		Shared<GameObject> player = nullptr;
		Shared<GameObject> mesh = nullptr;

	};
}