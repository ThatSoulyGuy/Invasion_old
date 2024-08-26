#pragma once

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include "Core/Logger.hpp"
#include "Core/Window.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Core;
using namespace Invasion::Util;

namespace Invasion::Render
{
	class Renderer
	{

	public:
		
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void Initialize()
		{
			Vector2i windowDimensions = Window::GetInstance().GetDimensions();

			DXGI_SWAP_CHAIN_DESC1 swapChainDescription = {};

			swapChainDescription.Width = windowDimensions.x;
			swapChainDescription.Height = windowDimensions.y;
			swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDescription.Stereo = false;
			swapChainDescription.SampleDesc.Count = 1;
			swapChainDescription.SampleDesc.Quality = 0;
			swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDescription.BufferCount = 3;
			swapChainDescription.Scaling = DXGI_SCALING_NONE;
			swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapChainDescription.Flags = 0;

			ComPtr<ID3D11Device> deviceOut;
			ComPtr<ID3D11DeviceContext> contextOut;

			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

			HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &deviceOut, nullptr, &contextOut);

			if (FAILED(result))
				Logger_ThrowException("Failed to create D3D11 device", true);

			result = deviceOut.As(&device);

			if (FAILED(result))
				Logger_ThrowException("Failed to get ID3D11Device5 interface", true);

			result = contextOut.As(&context);

			if (FAILED(result))
				Logger_ThrowException("Failed to get ID3D11DeviceContext4 interface", true);

			ComPtr<IDXGIDevice> dxgiDevice;

			result = device.As(&dxgiDevice);

			if (FAILED(result))
				Logger_ThrowException("Failed to get IDXGIDevice interface", true);

			ComPtr<IDXGIAdapter> adapter;

			result = dxgiDevice->GetAdapter(&adapter);

			if (FAILED(result))
				Logger_ThrowException("Failed to get IDXGIAdapter interface", true);

			ComPtr<IDXGIFactory6> factory;

			result = adapter->GetParent(IID_PPV_ARGS(&factory));

			if (FAILED(result))
				Logger_ThrowException("Failed to get IDXGIFactory6 interface", true);

			ComPtr<IDXGISwapChain1> swapChainOut;

			result = factory->CreateSwapChainForHwnd(device.Get(), Window::GetInstance().GetHandle(), &swapChainDescription, nullptr, nullptr, &swapChainOut);

			if (FAILED(result))
				Logger_ThrowException("Failed to create IDXGISwapChain1 interface", true);

			result = swapChainOut.As(&swapChain);

			if (FAILED(result))
				Logger_ThrowException("Failed to get IDXGISwapChain4 interface", true);

			ComPtr<ID3D11Texture2D> backBuffer;

			result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

			if (FAILED(result))
				Logger_ThrowException("Failed to get back buffer", true);

			result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

			if (FAILED(result))
				Logger_ThrowException("Failed to create render target view", true);

			D3D11_TEXTURE2D_DESC depthStencilDescription = {};

			depthStencilDescription.Width = windowDimensions.x;
			depthStencilDescription.Height = windowDimensions.y;
			depthStencilDescription.MipLevels = 1;
			depthStencilDescription.ArraySize = 1;
			depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDescription.SampleDesc.Count = 1;
			depthStencilDescription.SampleDesc.Quality = 0;
			depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDescription.CPUAccessFlags = 0;
			depthStencilDescription.MiscFlags = 0;

			ComPtr<ID3D11Texture2D> depthStencilBuffer;

			result = device->CreateTexture2D(&depthStencilDescription, nullptr, &depthStencilBuffer);

			if (FAILED(result))
				Logger_ThrowException("Failed to create depth stencil buffer", true);

			result = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &depthStencilView);

			if (FAILED(result))
				Logger_ThrowException("Failed to create depth stencil view", true);

			context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

			D3D11_VIEWPORT viewport = {};

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<float>(windowDimensions.x);
			viewport.Height = static_cast<float>(windowDimensions.y);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			context->RSSetViewports(1, &viewport);
		}

		void Resize(const Vector2i& dimensions)
		{
			if (dimensions.x <= 0 || dimensions.y <= 0)
				return;
			
			if (!swapChain || !device || !context || !renderTargetView || !depthStencilView)
				return;

			renderTargetView.Reset();
			depthStencilView.Reset();

			HRESULT result = swapChain->ResizeBuffers(0, dimensions.x, dimensions.y, DXGI_FORMAT_UNKNOWN, 0);

			if (FAILED(result))
				Logger_ThrowException("Failed to resize swap chain buffers", true);

			ComPtr<ID3D11Texture2D> backBuffer;

			result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

			if (FAILED(result))
				Logger_ThrowException("Failed to get back buffer", true);

			result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

			if (FAILED(result))
				Logger_ThrowException("Failed to create render target view", true);

			D3D11_TEXTURE2D_DESC depthStencilDescription = {};

			depthStencilDescription.Width = dimensions.x;
			depthStencilDescription.Height = dimensions.y;
			depthStencilDescription.MipLevels = 1;
			depthStencilDescription.ArraySize = 1;
			depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDescription.SampleDesc.Count = 1;
			depthStencilDescription.SampleDesc.Quality = 0;
			depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDescription.CPUAccessFlags = 0;
			depthStencilDescription.MiscFlags = 0;

			ComPtr<ID3D11Texture2D> depthStencilBuffer;

			result = device->CreateTexture2D(&depthStencilDescription, nullptr, &depthStencilBuffer);

			if (FAILED(result))
				Logger_ThrowException("Failed to create depth stencil buffer", true);

			result = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &depthStencilView);

			if (FAILED(result))
				Logger_ThrowException("Failed to create depth stencil view", true);

			context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

			D3D11_VIEWPORT viewport = {};

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<float>(dimensions.x);
			viewport.Height = static_cast<float>(dimensions.y);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			context->RSSetViewports(1, &viewport);
		}

		void PreRender()
		{
			const float clearColor[] = { 0.0f, 0.45f, 0.75f, 1.0f };

			context->ClearRenderTargetView(renderTargetView.Get(), clearColor);
			context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

		void PostRender()
		{
			if (FAILED(swapChain->Present(1, 0)))
				Logger_ThrowException("Failed to present swap chain", true);

			context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
		}

		ComPtr<ID3D11Device5> GetDevice() const
		{
			return device;
		}

		ComPtr<ID3D11DeviceContext4> GetContext() const
		{
			return context;
		}

		ComPtr<IDXGISwapChain4> GetSwapChain() const
		{
			return swapChain;
		}

		ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const
		{
			return renderTargetView;
		}

		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const
		{
			return depthStencilView;
		}

		void CleanUp()
		{
			renderTargetView.Reset();
			depthStencilView.Reset();
			swapChain.Reset();
			context.Reset();
			device.Reset();
		}

		static Renderer& GetInstance()
		{
			static Renderer instance;
			return instance;
		}

	private:

		Renderer() = default;	

		ComPtr<ID3D11Device5> device;
		ComPtr<ID3D11DeviceContext4> context;
		ComPtr<IDXGISwapChain4> swapChain;
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
	};
}